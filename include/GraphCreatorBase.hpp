#ifndef GRAPHCREATORBASE_HPP
#define GRAPHCREATORBASE_HPP

#include "EMTraits.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <memory>
#include <functional>
#include <string>
#include "Utilities/GetPot"

namespace ElectionManipulation::GraphCreator{

  //! The basis class for all graph creators
  class GraphCreatorBase
  {
  public:
    using Graph = EMTraits::Graph;
    using RandomGenerator = EMTraits::RandomGenerator;

    //! The class is clonable.
    /*!
      Having a clonable class makes it possible to write copy constructors
      and assignment operators for classes that aggregate object of the
      GraphCreator hierarchy by composition.
    */
    virtual std::unique_ptr<GraphCreatorBase> clone() const=0;

    virtual ~GraphCreatorBase()=default;

    //! a string that identify the general type of Graph Creator
    virtual std::string name() const=0;

    virtual void set_gen(const RandomGenerator&)=0;

    virtual void read_params(GetPot)=0;

    virtual Graph create()=0;
  };

} // end namespace ElectionManipulation::GraphCreator

#endif // GRAPHCREATORBASE_HPP
