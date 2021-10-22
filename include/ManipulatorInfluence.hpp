#ifndef MANIPULATORINFLUENCE_HPP
#define MANIPULATORINFLUENCE_HPP

#include "EMTraits.hpp"

namespace ElectionManipulation{

  class ManipulatorInfluence{

  public:
    using Graph = EMTraits::Graph;
    using Vertex = Graph::vertex_descriptor;

    ManipulatorInfluence(Graph& my_graph_, std::size_t steps_):
              my_graph{my_graph_}, steps{steps_} {}

    /*!
      This function computes the utility of the seed with respect to the expected max_utility
      within its neighours of order lower or equal to the steps member.
      For example, if steps=2 the utility will be computed considering the seed's neighbours
      and their adjacent nodes.
      @param seed The node whose utility has to be computed
    */
    void compute_utility(Vertex seed);

    /*!
      This function iterates through all nodes of the graph and call the
      compute_utility method on each of them and pick the best
    */
    Vertex max_utility_vertex();

    /*!

    */
    void influence();

    /*!

    */
    void update_estimated_prob(Vertex v, bool accepted);

  private:
    Graph& my_graph;
    //! Only vertices at distance lower or equal to this parameter
    //! will be considered for computing the utility of each vertex
    std::size_t steps;
  };

} // end namespace ElectionManipulation


#endif // MANIPULATORINFLUENCE_HPP
