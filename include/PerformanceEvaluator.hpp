#ifndef PERFORMANCEEVALUATOR_HPP
#define PERFORMANCEEVALUATOR_HPP

#include <iostream>
#include "EMTraits.hpp"

namespace ElectionManipulation{
  using EMTraits::RealType;
  using EMTraits::UnsignedType;
  //! Struct to encapsulate the metrics used to evaluate the process' performance
  struct eval_metrics{
    RealType error_estimation_prob{0.}; //!< L2 norm between the real and the estimated probabilities of voting of all the voters in the network
    RealType expected_n_votes{0.};      //!< Expected number of votes
    UnsignedType tot_sol_person{0};

  };

  std::ostream& operator<<(std::ostream&, const eval_metrics&);

  //! Class used to generate metrics about the influence process
  /*!
      The class contains a reference to the graph and evaluates the metrics
      on it.

      At this moment, two metrics are implemented:
        - the MSE of the estimated probabilities
        - the number of expected votes for the candidate in consideration
  */
  class PerformanceEvaluator{
  public:
    using Graph = EMTraits::Graph;

    PerformanceEvaluator(const Graph& graph_): my_graph{graph_} {}
    //! Function to compute the metrics
    void compute();
    //! Helper function to return the struct with the metrics
    eval_metrics metrics() const {return my_metrics;}

  private:
    const Graph& my_graph;    //!< The manipulated graph
    eval_metrics my_metrics;  //!< Struct containing the metrics computed
  };

} // end namespace ElectionManipulation


#endif // PERFORMANCEEVALUATOR_HPP
