#include <iostream>
#include <filesystem>
#include <random>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/adjacency_list_io.hpp>
#include "GetPot"
#include "Person.hpp"
#include "PersonCreator.hpp"
#include "GraphCreatorSmallWorld.hpp"
#include "GraphCreatorRMAT.hpp"
#include "GraphCreatorErdosRenyi.hpp"


using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                            ElectionManipulation::Person>;
// using ERGen = boost::erdos_renyi_iterator<boost::minstd_rand, Graph>;


void printHelp(){

}


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

  unsigned int E = GPfile("Graph_option/E", 0);

  unsigned k = GPfile("Graph_option/small_world_generator/k", 0);
  double p = GPfile("Graph_option/small_world_generator/p", -1.);

  double a = GPfile("Graph_option/R-MAT/a", -1.);
  double b = GPfile("Graph_option/R-MAT/b", -1.);
  double c = GPfile("Graph_option/R-MAT/c", -1.);
  double d = GPfile("Graph_option/R-MAT/d", -1.);

  // Create graph with N nodes

  std::random_device rd ;
  std::knuth_b reng{rd()};

  GraphCreatorSmallWorld<std::knuth_b, boost::vecS, boost::vecS, boost::undirectedS,
                              ElectionManipulation::Person> gc(reng, N, k, p);

  GraphCreatorRMAT<std::knuth_b, boost::vecS, boost::vecS, boost::undirectedS,
                              ElectionManipulation::Person> gc1(reng, N, E, a, b, c, d);

  GraphCreatorErdosRenyi<std::knuth_b, boost::vecS, boost::vecS, boost::undirectedS,
                              ElectionManipulation::Person> gc2(reng, N, E);

  Graph g{gc.create()};
  Graph h{gc1.create()};
  Graph i{gc2.create()};

//  print_graph(g, std::cout);
//  print_graph(h, std::cout);
//  print_graph(i, std::cout);
//  std::cout << boost::write(i);



  return 0;

} // end main
