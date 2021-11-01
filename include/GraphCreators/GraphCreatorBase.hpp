#ifndef GRAPHCREATORBASE_HPP
#define GRAPHCREATORBASE_HPP

#include "EMTraits.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <memory>
//#include <functional>
#include <string>
#include "ExternalUtilities/GetPot"

namespace ElectionManipulation::GraphCreator{

  //! The basis class for all graph creators
  /*!
      This class represents the common interface for all Graph Creators.
      The class is clonable in order to write copy constructors and assignment
      operators for classes that aggregate object of the GraphCreator hierarchy
      by composition.

      The main method is create() that generate the Graph with the corresponding
      attributes.
  */
  class GraphCreatorBase
  {
  public:
    using Graph = EMTraits::Graph;
    using RandomGenerator = EMTraits::RandomGenerator;

    //! The class is clonable
    virtual std::unique_ptr<GraphCreatorBase> clone() const=0;

    virtual ~GraphCreatorBase()=default;
    //! String that identifies the type of Graph Creator
    virtual std::string name() const=0;
    //! Method to set the Random Generator (source of randomness for creating the graph)
    virtual void set_gen(const RandomGenerator&) {}
    //! Method to read the parameters needed by each GraphCreator. It needs a GetPot parameter where every information may be found
    virtual void read_params(GetPot)=0;
    //! Method that returns a boolean indicating whether the attributes of each vertex should be read from a file or whether they should be randomly generated
    virtual bool get_read_attributes() const=0;
    //! The main method used to actually create the Graph
    virtual Graph create()=0;
  };

} // end namespace ElectionManipulation::GraphCreator

#endif // GRAPHCREATORBASE_HPP
