#include <iostream>
#include <filesystem>
#include <boost/graph/adjacency_list.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/graph/graph_utility.hpp> // print_graph
#include "GetPot"
// #include "ruleFactory.hpp"
#include "person.hpp"
#include "GraphCreatorSmallWorld.hpp"
#include "GraphCreatorRMAT.hpp"
#include "GraphCreatorErdosRenyi.hpp"


using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                            ElectionManipulation::Person>;
// using ERGen = boost::erdos_renyi_iterator<boost::minstd_rand, Graph>;


void printHelp(){

}

/*
//! Helper function
template <class Factory>
void printList(Factory const & rulesFactory)
{
  auto rules = rulesFactory.registered();
  std::cout << "The following rules are registered" << std::endl;
  for (auto i:rules)
    std::cout << i << std::endl;
}
*/

int main(int argc, char** argv)
{
  using namespace election_manipulation::GraphCreator;

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

  boost::minstd_rand gen;
  unsigned k = 6;
  double p = 0.03;
  unsigned E = 50;
  double a{0.6};
  double b{0.2};
  double c{0.15};
  double d{0.05};
  // Create graph with N nodes

  GraphCreatorSmallWorld<boost::minstd_rand, boost::vecS, boost::vecS, boost::undirectedS,
                              ElectionManipulation::Person> gc(gen, N, k, p);

  GraphCreatorRMAT<boost::minstd_rand, boost::vecS, boost::vecS, boost::undirectedS,
                              ElectionManipulation::Person> gc1(gen, N, E, a, b, c, d);

  GraphCreatorErdosRenyi<boost::minstd_rand, boost::vecS, boost::vecS, boost::undirectedS,
                              ElectionManipulation::Person> gc2(gen, N, E);

  Graph g{gc.create()};
  Graph h{gc1.create()};
  Graph i{gc2.create()};

  print_graph(g, std::cout);
  print_graph(h, std::cout);
  print_graph(i, std::cout);


  return 0;

} // end main
