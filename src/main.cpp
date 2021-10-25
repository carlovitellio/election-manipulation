#include <iostream>
#include <sstream>
#include <filesystem>
#include "Utilities/GetPot"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-override"
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/adjacency_list_io.hpp>
#include <boost/graph/graphviz.hpp>
#pragma GCC diagnostic pop

#include "EMUtilities.hpp"
#include "EMTraits.hpp"
#include "GraphCreatorFactory.hpp"
#include "PersonCreator.hpp"
#include "SocialNetworkCreator.hpp"
#include "ManipulatorInfluence.hpp"
#include "PerformanceEvaluator.hpp"


static_assert(std::is_default_constructible_v<ElectionManipulation::Person>,
                  "The Boost Graph Library requires default-constructible vertex properties");


void printHelp(){

}


int main(int argc, char** argv)
{
  using namespace ElectionManipulation;
  using namespace ElectionManipulation::EMTraits;


  GetPot cl(argc,argv);
  if (cl.search(2, "--help", "-h")){
    printHelp();
    return 0;
  }


  std::string inputFile = cl("InputFile","../input/graph_input.getpot");
  std::filesystem::path filepath(inputFile);
  if (!std::filesystem::exists(filepath))
    {
      std::cerr<<"Input file "<<inputFile<<" does not exists\n";
      return 1;
    }

  GetPot GPfile(inputFile.c_str());

  //! Read the graph configuration parameters


  //! Instantiating the Factory collecting the graph creator methods
  using GCHandler = GraphCreator::GCHandler; // alias to a apsc::PointerWrapper<GCBase>
  using GraphFactory = GraphCreator::GraphCreatorFactory;

  const GraphFactory & MyFactory = GraphCreator::MyFactory;

  if (cl.search(2, "--list", "-l"))
  {
    printRegistered(MyFactory);
    return 0;
  }

  std::string GCmethod = GPfile("Graph_option/graph_type", "");
  if(GCmethod == "?"){
    printRegistered(MyFactory);
    return 0;
  } else if (GCmethod == "") {
    std::cerr << "Invalid graph type requested" << std::endl;
    printRegistered(MyFactory);
    return 1;
  }

  std::random_device rd ;
  RandomGenerator reng{rd()};

  GCHandler gc_ptr;

  try {
    gc_ptr = MyFactory.create(GCmethod);
    gc_ptr->set_gen(reng);
    gc_ptr->read_params(GPfile);
    std::clog << "Implementing the graph with " << GCmethod << " creator" << std::endl;
  }
  catch (std::invalid_argument & e)
  {
    std::cerr << e.what() << std::endl;
    std::cerr << "Registered methods are " << std::endl;
    printRegistered(MyFactory);
    return 1;
  }


  double lambda = GPfile("Person_option/Resistance/poisson/lambda", -1.);
  double alpha = GPfile("Person_option/ProbabilityVoting/beta/alpha", -1.);
  double beta = GPfile("Person_option/ProbabilityVoting/beta/beta", -1.);


  // Person
  DistributionResistance distribution(lambda);
  DistributionProbability distrib2(alpha, beta);

  PersonCreator pc(reng, distribution, distrib2);

  SocialNetworkCreator snc(*gc_ptr, pc);

  Graph my_graph{snc.apply()};


  std::ofstream dotfile;

  dotfile.open("out/test.dot");
  write_graphviz(dotfile, my_graph);
  dotfile.close();


//  print_graph(my_graph, std::cout);

//  std::cout << boost::write(my_graph);


  std::size_t steps  = GPfile("InfluenceOption/Estimation/steps", 1);
  std::size_t rounds = GPfile("InfluenceOption/rounds", 10);

  PerformanceEvaluator pe(my_graph);

  ManipulatorInfluence mi(my_graph, steps);

  std::ostringstream tmp;

  tmp << "out/" << GCmethod << "_N" << num_vertices(my_graph)
      << "_E" << num_edges(my_graph) << "_lambda" << lambda << ".dat";
  std::ofstream file (tmp.str());

  file << 0 << " " << pe.error_estimation_prob(2) << '\n';

  for(std::size_t i{1}; i<=rounds; i++)
  {
    mi.influence();
    file << i << " " << pe.error_estimation_prob(2) << '\n';
  }

  file.close();


  boost::dynamic_properties dp = create_dynamicProperties_writing(my_graph);

  dotfile.open("out/test_w_property.dot");
  write_graphviz_dp(dotfile, my_graph, dp);
  dotfile.close();


  return 0;

} // end main
