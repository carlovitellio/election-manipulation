#include "GraphCreatorErdosRenyi.hpp"
#include <boost/graph/erdos_renyi_generator.hpp>

namespace ElectionManipulation::GraphCreator{
  using Graph = EMTraits::Graph;

  std::unique_ptr<GraphCreatorBase> GraphCreatorErdosRenyi::clone() const
  {
    return std::unique_ptr<GraphCreatorBase>(new GraphCreatorErdosRenyi(*this));
  }

  Graph GraphCreatorErdosRenyi::create()
  {
    using ERGen = boost::erdos_renyi_iterator<RandomGenerator, Graph>;
    std::clog << "Implementig a Erdos-Renyi graph with " << N << " vertices and " << E
              << " edges. \nEdges are seleceted randomly" << std::endl;
    Graph g(ERGen(gen, N, E), ERGen(), N);

    return g;
  }

  void GraphCreatorErdosRenyi::read_params(GetPot GPfile)
  {
    N = GPfile("Graph_option/N", 100);
    E = GPfile("Graph_option/E", 200);
  }

} // end namespace ElectionManipulation::GraphCreator
