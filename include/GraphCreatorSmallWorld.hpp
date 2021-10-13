#ifndef GRAPHCREATORSMALLWORLD_HPP
#define GRAPHCREATORSMALLWORLD_HPP

#include "GraphCreatorBase.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/small_world_generator.hpp>

namespace ElectionManipulation::GraphCreator{

  //! Makes available the Small World graph generator provided by BGL
  /*!

  */

  template< class RandomGenerator,
            class Graph>
  class GraphCreatorSmallWorld final: public GraphCreatorBase<RandomGenerator, Graph>
  {
  public:
    using GCBase = GraphCreatorBase<RandomGenerator, Graph>;

    GraphCreatorSmallWorld(RandomGenerator& gen_, unsigned int N_,
                           unsigned int k_, double p_):
    gen{gen_}, N{N_}, k{k_}, p{p_}
    {};

    std::unique_ptr<GCBase> clone() const override
    {
      // return std::unique_ptr<GCBase>(*this);
      return std::unique_ptr<GCBase>(new GraphCreatorSmallWorld(*this));
    }


    Graph create() override
    {
      using SWGen = boost::small_world_iterator<RandomGenerator, Graph>;
      Graph g(SWGen(gen, N, k, p), SWGen(), N);

      return g;
    }

    //! a string that identify the general type of Graph Creator
    std::string name() const override {return "Small World";}

  private:
    RandomGenerator& gen;
    const unsigned int N; //!< Number of vertices in the graph
    const unsigned int k; //!< Number of neighbours connected
    const double p;       //!< Probability of reconnection

  };

} // end namespace ElectionManipulation::GraphCreator


#endif // GRAPHCREATORSMALLWORLD_HPP
