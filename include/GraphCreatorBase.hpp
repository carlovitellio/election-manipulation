#ifndef GRAPHCREATORBASE_HPP
#define GRAPHCREATORBASE_HPP

#include <boost/graph/adjacency_list.hpp>
#include <memory>
#include <functional>
#include <string>

namespace election_manipulation::GraphCreator{

  //! The basis class for all graph creators
  template< class RandomGenerator,
            class OutEdgeListS = boost::vecS, // a Sequence or an AssociativeContainer
            class VertexListS = boost::vecS, // a Sequence or a RandomAccessContainer
            class DirectedS = boost::undirectedS, // difference with the BGL since the default there is directed
            class VertexProperty = boost::no_property,
            class EdgeProperty = boost::no_property,
            class GraphProperty = boost::no_property,
            class EdgeListS = boost::listS>
  class GraphCreatorBase
  {
  public:

    using GCBase = GraphCreatorBase<RandomGenerator, OutEdgeListS, VertexListS,
                          DirectedS, VertexProperty, EdgeProperty, GraphProperty, EdgeListS>;
    using Graph = boost::adjacency_list<OutEdgeListS, VertexListS, DirectedS,
                                VertexProperty, EdgeProperty, GraphProperty, EdgeListS>;

    //! The class is clonable.
    /*!
      Having a clonable class makes it possible to write copy constructors
      and assignment operators for classes that aggregate object of the
      GraphCreator hierarchy by composition.
    */
    virtual std::unique_ptr<GCBase> clone() const =0;


    virtual Graph create()=0;
    virtual ~GraphCreatorBase()=default;

    //! a string that identify the general type of Graph Creator
    virtual std::string name() const=0;
  };

} // end namespace election_manipulation::GraphCreator

#endif // GRAPHCREATORBASE_HPP
