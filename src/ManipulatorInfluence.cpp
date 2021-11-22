#include "ManipulatorInfluence.hpp"
#include <unordered_set>
#include <queue>
#include <utility>
#include <cmath>
#include "Person.hpp"

namespace ElectionManipulation{
  using Graph = EMTraits::Graph;
  using Vertex = Graph::vertex_descriptor;

  void ManipulatorInfluence::compute_utility(Vertex seed, std::false_type)
  {
    std::size_t steps{1};
    using MyQueue = std::queue<std::pair<Vertex, std::size_t>>;

    std::unordered_set<Vertex> visited;
    visited.emplace(seed);
    MyQueue vertex_queue;
    vertex_queue.emplace(std::make_pair(seed, steps));

    double utility{my_graph[seed].manipulator_marginal_utility};
    //! If the node is picked as seed node for the cascade, it is assumed it will activate
    my_graph[seed].manipulator_prob_activ = 1.;

    while(!vertex_queue.empty())
    {
      auto [u, step] = vertex_queue.front();
      vertex_queue.pop();

      if(step == 0) continue;

      for (auto out : boost::make_iterator_range(out_edges(u, my_graph)))
      {
        Vertex v = target(out, my_graph);

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



  void ManipulatorInfluence::compute_utility(Vertex seed, std::true_type)
  {
    using MyQueue = std::queue<Vertex>;

    std::unordered_set<Vertex> visited;
    visited.emplace(seed);
    MyQueue vertex_queue;
    vertex_queue.emplace(seed);

    double utility{my_graph[seed].manipulator_marginal_utility};

    while(!vertex_queue.empty())
    {
      auto u = vertex_queue.front();
      vertex_queue.pop();

      for (auto out : boost::make_iterator_range(out_edges(u, my_graph)))
      {
        Vertex v = target(out, my_graph);

        if(visited.find(v) != visited.end())
        {
          continue;
        } else { // The vertex has never been reached before
          std::bernoulli_distribution d(my_graph[v].manipulator_estim_prob);
          if(d(engine))
          {
            visited.emplace(v);
            vertex_queue.emplace(v);
            utility += my_graph[v].manipulator_marginal_utility;
          }
        }

      }
    }
    my_graph[seed].manipulator_utility = utility;
  }

  Vertex ManipulatorInfluence::max_utility_vertex()
  {
    double max_utility{-1.};
    Vertex best_vertex;
    //! For each vertex is computed its utility and saved the maximum
    for(auto i : boost::make_iterator_range(vertices(my_graph)))
    {
      if(complete) {
        compute_utility(i, std::integral_constant<bool, true>());
      } else {
        compute_utility(i, std::integral_constant<bool, false>());
      }

      if(my_graph[i].manipulator_utility > max_utility)
      {
        max_utility = my_graph[i].manipulator_utility;
        best_vertex = i;
      }
    }

    return best_vertex;
  }


  void ManipulatorInfluence::influence()
  {
    //! First, the vertex with maximum utility is found
    Vertex seed = max_utility_vertex();
    //! Since it is taken as seed, she is assumed to accept the message and
    //! to update her probability of voting
    my_graph[seed].node_bought();
    update_estimated_prob(seed, true);

    std::unordered_set<Vertex> visited;
    visited.emplace(seed);
    //! Create a queue of vertices to be explored and firstly add the seed
    std::queue<Vertex> vertex_queue;
    vertex_queue.emplace(seed);

    //! For each node in the queue, explore her neighbourhood and try to influence
    //! them, if they haven't already been reached by the message
    while(!vertex_queue.empty())
    {
      Vertex u = vertex_queue.front();
      vertex_queue.pop();

      for (auto out : boost::make_iterator_range(out_edges(u, my_graph)))
      {
        Vertex v = target(out, my_graph);

        if(visited.find(v) != visited.end())
        {
          continue;
        } else { // The vertex has never been reached before
          visited.emplace(v);

          //! If the node is reached for the first time, she is asked if she
          //! accepts it or not
          bool accepted = my_graph[v].receive_message();
          update_estimated_prob(v, accepted);

          //! If the Person assiocated to the vertex accepts the message, it will
          //! resent to her neighbourhood
          if(accepted)
            vertex_queue.emplace(v);
        }
      }
    }
  }

  void ManipulatorInfluence::update_estimated_prob(Vertex v, bool accepted)
  {
    double& p = my_graph[v].manipulator_estim_prob;
    std::size_t& resist_0 = my_graph[v].init_resistance;
    std::size_t& n_solic = my_graph[v].n_solicited;
    if(estim_method == "Standard")
      p = (p *(n_solic + 1.) + int(accepted)*(1. + resist_0/n_solic))/
              (n_solic + 1. + int(accepted)*(1.) + resist_0/n_solic);
    else if (estim_method == "Power")
      p = (p *(n_solic + 1.) + int(accepted)*(1. + pow(resist_0, 1.3)/n_solic))/
              (n_solic + 1. + int(accepted)*(1.) + pow(resist_0, 1.3)/n_solic);
    else {
      std::cerr << "No estimation method called" << estim_method;
      std::exit(1);
    }

    update_marginal_utility(v);
  }

  void ManipulatorInfluence::update_marginal_utility(Vertex v)
  {
    my_graph[v].manipulator_marginal_utility = (1 - my_graph[v].manipulator_estim_prob)/(my_graph[v].resistance + 1);
  }

} // end namespace ElectionManipulation
