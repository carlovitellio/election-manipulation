#ifndef HH_GRAPHCREATORERDOSRENYI_HH
#define HH_GRAPHCREATORERDOSRENYI_HH

#include "GraphCreatorBase.hpp"
#include "GraphCreatorRMAT.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/rmat_graph_generator.hpp>

namespace election_manipulation::GraphCreator{

  //! Makes available the Erdos Rényi graph generator using the R-MAT generator
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
  class GraphCreatorErdosRenyi final: public GraphCreatorBase<RandomGenerator,
                          OutEdgeListS, VertexListS, DirectedS, VertexProperty,
                          EdgeProperty, GraphProperty, EdgeListS>
  {
  public:
    using GCBase = GraphCreatorBase<RandomGenerator, OutEdgeListS, VertexListS,
              DirectedS, VertexProperty, EdgeProperty, GraphProperty, EdgeListS>;
    using Graph = boost::adjacency_list<OutEdgeListS, VertexListS, DirectedS,
                        VertexProperty, EdgeProperty, GraphProperty, EdgeListS>;

    GraphCreatorErdosRenyi(RandomGenerator& gen_, unsigned int N_, unsigned int E_):
                           gen{gen_}, N{N_}, E{E_}
    {};

    std::unique_ptr<GCBase> clone() const override
    {
      return std::unique_ptr<GCBase>(new GraphCreatorErdosRenyi(*this));
    }


    Graph create() override
    {
      GraphCreatorRMAT<RandomGenerator, OutEdgeListS, VertexListS,
                DirectedS, VertexProperty, EdgeProperty, GraphProperty, EdgeListS> gc(gen, N, E, 0.25, 0.25, 0.25, 0.25);

      return gc.create();
    }

    //! a string that identify the general type of Graph Creator
    std::string name() const override {return "Erdos-Rényi";}

  private:
    RandomGenerator& gen;
    const unsigned int N; //!< Number of vertices in the graph
    const unsigned int E; //!< Number of edges in the graph

  };

} // end namespace election_manipulation::GraphCreator


#endif // HH_GRAPHCREATORERDOSRENYI_HH
