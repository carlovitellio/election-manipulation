#include "PerformanceEvaluator.hpp"
#include <boost/math/tools/norms.hpp>
#include "Person.hpp"
#include "EMTraits.hpp"
//#include <fstream>
namespace ElectionManipulation{
  using EMTraits::RealType;

  std::ostream& operator<<(std::ostream& os, const eval_metrics& metrics)
  {
    os << metrics.error_estimation_prob << " "
       << metrics.expected_n_votes << " "
       << metrics.tot_sol_person << " "
       << metrics.avg_solicited << " "
       << metrics.n_estimated_1 << "\n";

    return os;
  }


  void PerformanceEvaluator::compute()
  {
    using vertices_size_type = Graph::vertices_size_type;
    my_metrics.expected_n_votes = 0.;
    my_metrics.tot_sol_person = 0;
    my_metrics.n_estimated_1 = 0;
    my_metrics.avg_solicited = 0;
    vertices_size_type N{num_vertices(my_graph)};
    std::vector<RealType> real(N), estimated(N);

    for(auto v : boost::make_iterator_range(vertices(my_graph)))
    {
//      std::ofstream file("../out/nodes/node" + std::to_string(v) + ".dat", std::ios_base::app);
      my_metrics.expected_n_votes += my_graph[v].prob_voting_c0;
      if(my_graph[v].solicited) my_metrics.tot_sol_person++;
      if(my_graph[v].manipulator_estim_prob == 1.) my_metrics.n_estimated_1++;
      real.push_back(my_graph[v].prob_voting_c0);
      estimated.push_back(my_graph[v].manipulator_estim_prob);
      my_metrics.avg_solicited += my_graph[v].n_solicited;
//      file << my_graph[v].prob_voting_c0 << " " << my_graph[v].manipulator_estim_prob << "\n";
//      file.close();
    }

    using boost::math::tools::lp_distance;
    my_metrics.error_estimation_prob = lp_distance(real, estimated, 2);
    my_metrics.avg_solicited = my_metrics.avg_solicited/N;
  }

} // end namespace ElectionManipulation
