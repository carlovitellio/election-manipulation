#ifndef HH_GRAPHCREATORSMALLWORLD_HH
#define HH_GRAPHCREATORSMALLWORLD_HH

#include "GraphCreatorBase.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/small_world_generator.hpp>

namespace election_manipulation::GraphCreator{

  //! Makes available the Small World graph generator provided by BGL
  /*!

  */

  template< class RandomGenerator,
            class OutEdgeListS = boost::vecS, // a Sequence or an AssociativeContainer
            class VertexListS = boost::vecS, // a Sequence or a RandomAccessContainer
            class DirectedS = boost::undirectedS, // difference with the BGL since the default there is directed
            class VertexProperty = boost::no_property,
            class EdgeProperty = boost::no_property,
            class GraphProperty = boost::no_property,
            class EdgeListS = boost::listS>
  class GraphCreatorSmallWorld final: public GraphCreatorBase<RandomGenerator,
                          OutEdgeListS, VertexListS, DirectedS, VertexProperty,
                          EdgeProperty, GraphProperty, EdgeListS>
  {
  public:
    using GCBase = GraphCreatorBase<RandomGenerator, OutEdgeListS, VertexListS,
              DirectedS, VertexProperty, EdgeProperty, GraphProperty, EdgeListS>;
    using Graph = boost::adjacency_list<OutEdgeListS, VertexListS, DirectedS,
                        VertexProperty, EdgeProperty, GraphProperty, EdgeListS>;

    GraphCreatorSmallWorld(RandomGenerator& gen_, unsigned int N_,
                           unsigned int k_, double p_):
    gen{gen_}, N{N_}, k{k_}, p{p_}
    {};

    std::unique_ptr<GCBase> clone() const override
    {
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

} // end namespace election_manipulation::GraphCreator


#endif // HH_GRAPHCREATORSMALLWORLD_HH
