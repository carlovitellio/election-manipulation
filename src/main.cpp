#include <iostream>
#include <sstream>
#include <filesystem>
#include <random>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-override"
#include <boost/random/beta_distribution.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/adjacency_list_io.hpp>
#pragma GCC diagnostic pop

#include "GetPot"
#include "ElectionManipulationTraits.hpp"
#include "Person.hpp"
#include "PersonCreator.hpp"
#include "GraphCreatorBase.hpp"
#include "SocialNetworkCreator.hpp"
#include "ManipulatorInfluence.hpp"
#include "PerformanceEvaluator.hpp"
#include "Utilities/Factory.hpp"
#include "LoadFactory.hpp"


static_assert(std::is_default_constructible_v<ElectionManipulation::Person>,
                  "The Boost Graph Library requires default-constructible vertex properties");
using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                            ElectionManipulation::Person>;


void printHelp(){

}


int main(int argc, char** argv)
{
  using namespace ElectionManipulation;

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
  using GCBase = EMTraits<Graph>::GCBase;
  using GraphFactory = EMTraits<Graph>::GraphFactory;
  using GCHandler = EMTraits<Graph>::GCHandler; // alias to a apsc::PointerWrapper<GCBase>

  GraphFactory & MyFactory = GraphFactory::Factory::Instance();
  loadFactory<Graph>();

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

  bool found{true};
  GCHandler gc_ptr;

  try {
    gc_ptr = MyFactory.create(GCmethod);
    gc_ptr->set_gen(reng);
    gc_ptr->read_params(GPfile);
  }  catch (std::invalid_argument &) {
    found = false;
  }

  if (!found)
  {
    std::cout << "Graph creator method " << GCmethod << " does not exist" << std::endl;
    std::cout << "Registered methods are " << std::endl;
    printRegistered(MyFactory);
    return 2;
  }


  double lambda = GPfile("Person_option/Resistance/poisson/lambda", -1.);
  double alpha = GPfile("Person_option/ProbabilityVoting/beta/alpha", -1.);
  double beta = GPfile("Person_option/ProbabilityVoting/beta/beta", -1.);


  // Person
  DistributionResistance distribution(lambda);
  DistributionProbability distrib2(alpha, beta);

  PersonCreator pc(reng, distribution, distrib2);

  SocialNetworkCreator<Graph,
  GCBase,
  PersonCreator<RandomGenerator, DistributionResistance, DistributionProbability>
  > snc(*gc_ptr, pc);

  Graph my_graph{snc.apply()};

//  print_graph(my_graph, std::cout);
//  std::cout << boost::write(my_graph);


  std::size_t steps  = GPfile("InfluenceOption/Estimation/steps", 1);
  std::size_t rounds = GPfile("InfluenceOption/rounds", 10);

  PerformanceEvaluator pe(my_graph);

  ManipulatorInfluence mi(my_graph, steps);

  std::ostringstream tmp;

  tmp << "out/Ex_N" << num_vertices(my_graph)
      << "_E" << num_edges(my_graph) << "_rounds" << rounds << ".dat";
  std::ofstream file (tmp.str());

  file << 0 << " " << pe.error_estimation_prob(2) << '\n';

  for(std::size_t i{1}; i<=rounds; i++)
  {
    mi.influence();
    file << i << " " << pe.error_estimation_prob(2) << '\n';
  }

  file.close();




  return 0;

} // end main
