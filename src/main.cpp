#include <iostream>
#include <sstream>
#include <stdexcept>
#include "ExternalUtilities/GetPot"
//#include "EMUtilities.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-override"
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/adjacency_list_io.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/graphml.hpp>
#pragma GCC diagnostic pop

#include "EMTraits.hpp"
#include "GraphCreators/GCTraits.hpp"
#include "Distributions/DistributionsTraits.hpp"
#include "ReadInformation.hpp"
#include "PersonCreator.hpp"
#include "SocialNetworkCreator.hpp"
#include "ManipulatorInfluence.hpp"
#include "PerformanceEvaluator.hpp"


int main(int argc, char** argv)
{
  using namespace ElectionManipulation;
  using namespace ElectionManipulation::EMTraits;

  GetPot cl(argc,argv);

  std::random_device rd ;
  ElectionManipulation::EMTraits::RandomGenerator gen{rd()};

  ReadInfoGetPot reader(cl, gen);

  reader.check_help();
  reader.read_input_file();


  using GCHandler = GraphCreator::GCHandler; // alias to a apsc::PointerWrapper

  GCHandler gc_ptr = reader.readInfoGraphCreator();


  using ResHandler = Distributions::ResDistHandler<RandomGenerator> ;
  using VotHandler = Distributions::VotDistHandler<RandomGenerator> ;

  ResHandler resistance_distr_ptr = reader.readInfoResistanceDist();
  VotHandler voting_distr_ptr = reader.readInfoVotingDist();


  PersonCreator<RandomGenerator> pc(gen, *resistance_distr_ptr, *voting_distr_ptr);

  SocialNetworkCreator<RandomGenerator> snc(*gc_ptr, pc);

  Graph my_graph{snc.apply()};

  std::cout << boost::write(my_graph);

  std::ofstream dotfile, xmlfile;

  dotfile.open("out/test.dot");
  write_graphviz(dotfile, my_graph);
  dotfile.close();




//  print_graph(my_graph, std::cout);

//  std::cout << boost::write(my_graph);


//  std::size_t steps  = GPfile("InfluenceOption/Estimation/steps", 1);
//  std::size_t rounds = GPfile("InfluenceOption/rounds", 10);

  PerformanceEvaluator pe(my_graph);

//  ManipulatorInfluence mi(my_graph, steps);
/*
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
*/

  boost::dynamic_properties dp = create_dynamicProperties(my_graph);

  dotfile.open("out/test_w_property.dot");
  write_graphviz_dp(dotfile, my_graph, dp);
  dotfile.close();

  xmlfile.open("out/test_w_property.xml");
  write_graphml(xmlfile, my_graph, dp);
  xmlfile.close();

  xmlfile.open("out/test.xml");
  write_graphml(xmlfile, my_graph, create_dynamicProperties(my_graph, false));
  xmlfile.close();

  return 0;

} // end main
