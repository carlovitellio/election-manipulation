#ifndef MANIPULATORINFLUENCE_HPP
#define MANIPULATORINFLUENCE_HPP

#include <unordered_set>
#include <queue>
#include <utility>
#include "Person.hpp"

namespace ElectionManipulation{

  template<class Graph>
  class ManipulatorInfluence{
  private:
    Graph& my_graph;
    //! Only vertices at distance lower or equal to this parameter
    //! will be considered for computing the utility of each vertex
    unsigned steps;

  public:
    using Vertex = typename Graph::vertex_descriptor;

    ManipulatorInfluence(Graph& my_graph_, unsigned steps_):
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

  };

  template<class Graph>
  void ManipulatorInfluence<Graph>::compute_utility(typename Graph::vertex_descriptor seed)
  {
    using Vertex = typename Graph::vertex_descriptor;
    using MyQueue = std::queue<std::pair<Vertex, unsigned>>;

    std::unordered_set<Vertex> visited;
    visited.emplace(seed);
    MyQueue vertex_queue;
    vertex_queue.emplace(std::make_pair(seed, steps));

    double utility{my_graph[seed].manipulator_marginal_utility};
    // If the node is picked as seed node for the cascade, it is assumed it will activate
    my_graph[seed].manipulator_prob_activ = 1.;

    while(!vertex_queue.empty())
    {
      auto [u, step] = vertex_queue.front();
      vertex_queue.pop();

      if(step == 0) continue;

      for (auto out : boost::make_iterator_range(out_edges(u, my_graph)))
      {
        auto v = target(out, my_graph);

        if(visited.find(v) != visited.end())
        {
          continue;
        } else { // The vertex has never been reached before
          visited.emplace(v);
          vertex_queue.emplace(std::make_pair(v, step-1));
          my_graph[v].manipulator_prob_activ = my_graph[u].manipulator_prob_activ \
                                              * my_graph[v].manipulator_estim_prob;

          utility += my_graph[v].manipulator_prob_activ * my_graph[v].manipulator_marginal_utility;
        }

      }
    }
    my_graph[seed].manipulator_utility = utility;
  }


  template<class Graph>
  typename Graph::vertex_descriptor ManipulatorInfluence<Graph>::max_utility_vertex()
  {
    using Vertex = typename Graph::vertex_descriptor;

    double max_utility{-1.};
    Vertex best_vertex;

    for(auto i : boost::make_iterator_range(vertices(my_graph)))
    {
      compute_utility(i);

      if(my_graph[i].manipulator_utility > max_utility)
      {
        max_utility = my_graph[i].manipulator_utility;
        best_vertex = i;
      }
    }

    return best_vertex;
  }

  template<class Graph>
  void ManipulatorInfluence<Graph>::influence()
  {
    using Vertex = typename Graph::vertex_descriptor;

    Vertex seed = max_utility_vertex();
    my_graph[seed].update_prob();
    update_estimated_prob(seed, true);

    std::unordered_set<Vertex> visited;
    visited.emplace(seed);

    std::queue<Vertex> vertex_queue;
    vertex_queue.emplace(seed);

    while(!vertex_queue.empty())
    {
      auto u = vertex_queue.front();
      vertex_queue.pop();

      for (auto out : boost::make_iterator_range(out_edges(u, my_graph)))
      {
        auto v = target(out, my_graph);

        if(visited.find(v) != visited.end())
        {
          continue;
        } else { // The vertex has never been reached before
          visited.emplace(v);

          bool accepted = my_graph[v].receive_message();
          update_estimated_prob(v, accepted);

          // If the Person assiocated to the vertex accepts the message, it will
          // resent to her neighbourhood
          if(accepted)
            vertex_queue.emplace(v);
        }
      }
    }
  }

  template<class Graph>
  void ManipulatorInfluence<Graph>::update_estimated_prob(typename Graph::vertex_descriptor v, bool accepted)
  {
    if(accepted)
    {
      my_graph[v].manipulator_estim_prob = (my_graph[v].manipulator_estim_prob * my_graph[v].resistance + 2.)\
                                    /(my_graph[v].resistance + 1.);
      if(my_graph[v].manipulator_estim_prob > 1.)
        my_graph[v].manipulator_estim_prob = 1.;
    } else {
      my_graph[v].manipulator_estim_prob = (my_graph[v].manipulator_estim_prob * my_graph[v].resistance - 1.)\
                                  /(my_graph[v].resistance + 1.);
      if(my_graph[v].manipulator_estim_prob < 0.)
        my_graph[v].manipulator_estim_prob = 0.;
    }

  }

} // end namespace ElectionManipulation


#endif // MANIPULATORINFLUENCE_HPP
