#include "PerformanceEvaluator.hpp"
#include <boost/math/tools/norms.hpp>
#include "Person.hpp"
#include "EMTraits.hpp"

namespace ElectionManipulation{
  using EMTraits::RealType;

  std::ostream& operator<<(std::ostream& os, const eval_metrics& metrics)
  {
    os << metrics.error_estimation_prob << " "
       << metrics.expected_n_votes << "\n";

    return os;
  }


  void PerformanceEvaluator::compute()
  {
    using vertices_size_type = Graph::vertices_size_type;

    my_metrics.expected_n_votes = 0.;
    vertices_size_type N{num_vertices(my_graph)};
    std::vector<RealType> real(N), estimated(N);

    for(auto v : boost::make_iterator_range(vertices(my_graph)))
    {
      my_metrics.expected_n_votes += my_graph[v].prob_voting_c0;
      real.push_back(my_graph[v].prob_voting_c0);
      estimated.push_back(my_graph[v].manipulator_estim_prob);
    }

    using boost::math::tools::lp_distance;
    my_metrics.error_estimation_prob = lp_distance(real, estimated, 2);
  }

} // end namespace ElectionManipulation
