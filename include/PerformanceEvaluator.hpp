#ifndef PERFORMANCEEVALUATOR_HPP
#define PERFORMANCEEVALUATOR_HPP

#include <boost/math/tools/norms.hpp>
#include "Person.hpp"

namespace ElectionManipulation{

  template<class Graph>
  class PerformanceEvaluator{
  private:
    Graph& my_graph;

  public:
    PerformanceEvaluator(Graph& graph_): my_graph{graph_} {}

    //! The function returns the Lp distance between the real and the estimated
    //! probabilities of voting of all the voters in the network
    double error_estimation_prob(std::size_t p);


  };

  template<class Graph>
  double PerformanceEvaluator<Graph>::error_estimation_prob(std::size_t p)
  {
    using vertices_size_type = typename Graph::vertices_size_type;

    vertices_size_type N{num_vertices(my_graph)};
    std::vector<double> real(N), estimated(N);

    for(auto v : boost::make_iterator_range(vertices(my_graph)))
    {
      real.push_back(my_graph[v].prob_voting_c0);
      estimated.push_back(my_graph[v].manipulator_estim_prob);
    }

    return boost::math::tools::lp_distance(real, estimated, p);
  }

} // end namespace ElectionManipulation


#endif // PERFORMANCEEVALUATOR_HPP
