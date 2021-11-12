#ifndef MANIPULATORINFLUENCE_HPP
#define MANIPULATORINFLUENCE_HPP

#include "EMTraits.hpp"
#include <type_traits>

namespace ElectionManipulation{

  //! Class to influence the Social Network
  /*!
      The class encapsulates a reference to the social newtork and it tries to
      manipulate it.

      The manipulator assigns an utility to each vertex and initiate an
      information cascade for conditioning the network. The utility represents
      the expected change in number of votes.

      In particular, the vertex chosen as seed accepts the message received and
      updates her probability of voting accordingly. Then, she sends the same
      message to her neighbourhood. If a neighour accepts the message, she will
      continue the information cascade. A node cannot be influenced twice by the
      same message. The process stops when either no node accepts the message or
      every node has already been reached.

      The constructor requests the graph to be manipulated and the number of
      steps to be used in order to evaluate the utility of a single vertex. It
      represents the maximum distance that will be reached in order to estimate
      the possible advantage influencing that node.

      Whether or not a user accepts a message, the manipulator improves her
      estimate on the probability of voting of that node.
  */
  class ManipulatorInfluence{

  public:
    using DefRandEngine=std::default_random_engine;
    using Graph = EMTraits::Graph;
    using Vertex = Graph::vertex_descriptor;

    ManipulatorInfluence(Graph& my_graph_, bool complete_):
                  my_graph{my_graph_}, complete{complete_} {
                    for(auto i : boost::make_iterator_range(vertices(my_graph)))
                      update_marginal_utility(i);
                  }

    /*!
      This function computes the utility of the seed with respect to the expected max_utility
      within its neighours of order lower or equal to the steps member.
      For example, if steps=2 the utility will be computed considering the seed's neighbours
      and their adjacent nodes.
      @param seed The node whose utility has to be computed
    */
    void compute_utility(Vertex seed, std::true_type);
    void compute_utility(Vertex seed, std::false_type);

    /*!
      This function iterates through all nodes of the graph and call the
      compute_utility method on each of them and pick the one with maximum utility
    */
    Vertex max_utility_vertex();

    /*!
      Function to influence the social network
    */
    void influence();

    /*!
      The manipulator updates her estimate on the probability of voting of
      each Person reached in the information cascade.
      If the message his accepted,
      \f[
      \hat{prob}_{t+1} = \min{(1, \frac{\hat{prob}_{t} * resistance + 2}{resistance + 1})}
      \f]
      If the message is refused,
      \f[
      \hat{prob}_{t+1} = \max{(0, \frac{\hat{prob}_{t} * resistance - 1}{resistance + 1})}
      \f]
    */
    void update_estimated_prob(Vertex v, bool accepted);

    //! Function used by the manipulator to update her marginal utility
    //! in conditioning the current node
    /*! The manipulator marginal utility is determined by the possible
        shift in probability obtained if the Person accepts her message
        \f[
            p_{t+1} - p_t = \frac{1 - p_t}{w+1}
        \f]
    */
    void update_marginal_utility(Vertex);

  private:
    Graph& my_graph;   //!< The actual social network to influence
    bool complete;     //!< true estimate properly the vertex's utility, false approximate it with just its neighbours
    mutable DefRandEngine engine;       //!< Source of randomness
  };

} // end namespace ElectionManipulation


#endif // MANIPULATORINFLUENCE_HPP
