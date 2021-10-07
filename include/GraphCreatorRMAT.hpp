#ifndef HH_GRAPHCREATORRMAT_HH
#define HH_GRAPHCREATORRMAT_HH

#include "GraphCreatorBase.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/rmat_graph_generator.hpp>

namespace election_manipulation::GraphCreator{

  //! Makes available the RMAT graph generator provided by BGL
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
  class GraphCreatorRMAT final: public GraphCreatorBase<RandomGenerator,
                          OutEdgeListS, VertexListS, DirectedS, VertexProperty,
                          EdgeProperty, GraphProperty, EdgeListS>
  {
  public:
    using GCBase = GraphCreatorBase<RandomGenerator, OutEdgeListS, VertexListS,
              DirectedS, VertexProperty, EdgeProperty, GraphProperty, EdgeListS>;
    using Graph = boost::adjacency_list<OutEdgeListS, VertexListS, DirectedS,
                        VertexProperty, EdgeProperty, GraphProperty, EdgeListS>;

    GraphCreatorRMAT(RandomGenerator& gen_, unsigned int N_, unsigned int E_,
                     double a_, double b_, double c_, double d_):
    gen{gen_}, N{N_}, E{E_}, a{a_}, b{b_}, c{c_}, d{d_}
    {};

    std::unique_ptr<GCBase> clone() const override
    {
      return std::unique_ptr<GCBase>(new GraphCreatorRMAT(*this));
    }


    Graph create() override
    {
      using RMATGen = boost::rmat_iterator<RandomGenerator, Graph>;
      Graph g(RMATGen(gen, N, E, a, b, c, d), RMATGen(), N);

      return g;
    }

    //! a string that identify the general type of Graph Creator
    std::string name() const override {return "R-MAT";}

  private:
    RandomGenerator& gen;
    const unsigned int N; //!< Number of vertices in the graph
    const unsigned int E; //!< Number of edges in the graph
    const double a, b, c, d;

  };

} // end namespace election_manipulation::GraphCreator


#endif // HH_GRAPHCREATORSRMAT_HH
