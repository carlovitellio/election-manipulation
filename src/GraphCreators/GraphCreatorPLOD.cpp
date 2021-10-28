#include "GraphCreators/GraphCreatorPLOD.hpp"
#include <boost/graph/plod_generator.hpp>

namespace ElectionManipulation::GraphCreator{
  using Graph = EMTraits::Graph;

  std::unique_ptr<GraphCreatorBase> GraphCreatorPLOD::clone() const
  {
    return std::unique_ptr<GraphCreatorBase>(new GraphCreatorPLOD(*this));
  }

  Graph GraphCreatorPLOD::create()
  {
    using SFGen = boost::plod_iterator<RandomGenerator, Graph>;
    std::clog << "Implementig a scale-free graph with " << N << " vertices "
              << "using the Power Law Out Degree (PLOD) algorithm.\n"
              << "The parameters used are:\n"
              << "alpha = " << alpha << "\n"
              << "beta = " << beta << std::endl;
    Graph g(SFGen(gen, N, alpha, beta), SFGen(), N);

    return g;
  }

  void GraphCreatorPLOD::read_params(GetPot GPfile)
  {
    N = GPfile("Graph_option/N", 100);
    alpha = GPfile("Graph_option/PLOD/alpha", 2.72);
    beta = GPfile("Graph_option/PLOD/beta", 1000);
  }

} // end namespace ElectionManipulation::GraphCreator
