#include <iostream>
#include <filesystem>
#include <random>
#include <boost/random/beta_distribution.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/adjacency_list_io.hpp>
#include "GetPot"
#include "Person.hpp"
#include "PersonCreator.hpp"
#include "GraphCreatorBase.hpp"
#include "GraphCreatorSmallWorld.hpp"
#include "GraphCreatorRMAT.hpp"
#include "GraphCreatorErdosRenyi.hpp"
#include "SocialNetworkCreator.hpp"
#include "ManipulatorInfluence.hpp"

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                            ElectionManipulation::Person>;


void printHelp(){

}


int main(int argc, char** argv)
{
  using namespace ElectionManipulation;
  using namespace ElectionManipulation::GraphCreator;

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

  unsigned int N = GPfile("Graph_option/N", 0);
  if(N==0){
    std::cerr << "Invalid number of vertices inserted" << std::endl;
    std::cerr << "Using 100 vertices" << std::endl;
    N = 100;
  }

  unsigned E = GPfile("Graph_option/E", 0);

  unsigned k = GPfile("Graph_option/small_world_generator/k", 0);
  double p = GPfile("Graph_option/small_world_generator/p", -1.);

  double a = GPfile("Graph_option/R-MAT/a", -1.);
  double b = GPfile("Graph_option/R-MAT/b", -1.);
  double c = GPfile("Graph_option/R-MAT/c", -1.);
  double d = GPfile("Graph_option/R-MAT/d", -1.);

  double lambda = GPfile("Person_option/Resistance/poisson/lambda", -1.);
  double alpha = GPfile("Person_option/ProbabilityVoting/beta/alpha", -1.);
  double beta = GPfile("Person_option/ProbabilityVoting/beta/beta", -1.);

  // Create graph with N nodes
  using RandomGenerator = std::knuth_b;

  std::random_device rd ;
  RandomGenerator reng{rd()};

  GraphCreatorSmallWorld<RandomGenerator, Graph> gc(reng, N, k, p);
  GraphCreatorRMAT<RandomGenerator, Graph> gc1(reng, N, E, a, b, c, d);
  GraphCreatorErdosRenyi<RandomGenerator, Graph> gc2(reng, N, E);

  // Person
  using DistributionResistance = std::poisson_distribution<unsigned>;
  using DistributionProbability = boost::random::beta_distribution<>;

  DistributionResistance distribution(lambda);
  DistributionProbability distrib2(alpha, beta);

  PersonCreator pc(reng, distribution, distrib2);

  // SocialNetwork

  SocialNetworkCreator<Graph,
                       GraphCreatorBase<RandomGenerator, Graph>,
                       PersonCreator<RandomGenerator, DistributionResistance, DistributionProbability>
                       > snc(gc1, pc);

  Graph my_graph{snc.apply()};

//  std::cout << boost::write(my_graph);
//  print_graph(my_graph, std::cout);

  unsigned steps = GPfile("InfluenceOption/Estimation/steps", 1);

  ManipulatorInfluence mi(my_graph, steps);

  mi.influence();


  return 0;

} // end main
