#include "PerformanceEvaluator.hpp"
#include <boost/math/tools/norms.hpp>
#include "Person.hpp"

namespace ElectionManipulation{

  double PerformanceEvaluator::error_estimation_prob(std::size_t p)
  {
    using vertices_size_type = Graph::vertices_size_type;

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
