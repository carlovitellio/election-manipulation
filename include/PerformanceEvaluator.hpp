#ifndef PERFORMANCEEVALUATOR_HPP
#define PERFORMANCEEVALUATOR_HPP

#include "EMTraits.hpp"

namespace ElectionManipulation{

  class PerformanceEvaluator{
  public:
    using Graph = EMTraits::Graph;

    PerformanceEvaluator(Graph& graph_): my_graph{graph_} {}

    //! The function returns the Lp distance between the real and the estimated
    //! probabilities of voting of all the voters in the network
    double error_estimation_prob(std::size_t p);

  private:
    Graph& my_graph;
  };

} // end namespace ElectionManipulation


#endif // PERFORMANCEEVALUATOR_HPP
