#include "GraphCreatorErdosRenyi.hpp"
#include "GraphCreatorRMAT.hpp"

namespace ElectionManipulation::GraphCreator{
  using Graph = EMTraits::Graph;

  std::unique_ptr<GraphCreatorBase> GraphCreatorErdosRenyi::clone() const
  {
    return std::unique_ptr<GraphCreatorBase>(new GraphCreatorErdosRenyi(*this));
  }

  Graph GraphCreatorErdosRenyi::create()
  {
    GraphCreatorRMAT gc(gen, N, E, 0.25, 0.25, 0.25, 0.25);
    return gc.create();
  }

  void GraphCreatorErdosRenyi::read_params(GetPot GPfile)
  {
    N = GPfile("Graph_option/N", 100);
    E = GPfile("Graph_option/E", 200);
  }


} // end namespace ElectionManipulation::GraphCreator
