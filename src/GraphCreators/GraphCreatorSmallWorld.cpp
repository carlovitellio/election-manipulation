#include "GraphCreators/GraphCreatorSmallWorld.hpp"
#include <boost/graph/small_world_generator.hpp>

namespace ElectionManipulation::GraphCreator{
  using Graph = EMTraits::Graph;

  std::unique_ptr<GraphCreatorBase> GraphCreatorSmallWorld::clone() const
  {
    return std::unique_ptr<GraphCreatorBase>(new GraphCreatorSmallWorld(*this));
  }

  Graph GraphCreatorSmallWorld::create()
  {
    using SWGen = boost::small_world_iterator<RandomGenerator, Graph>;
    std::clog << "Implementig a Small-World graph with " << N
              << " vertices and each vertex is connected to its " << k
              << " neighbours. \nEdges are rewired with probability " << p << std::endl;
    Graph g(SWGen(gen, N, k, p), SWGen(), N);

    return g;
  }

  void GraphCreatorSmallWorld::read_params(GetPot GPfile)
  {
    N = GPfile("Graph_option/N", 100);
    k = GPfile("Graph_option/small_world_generator/k", 4);
    p = GPfile("Graph_option/small_world_generator/p", 0.05);
  }

} // end namespace ElectionManipulation::GraphCreator
