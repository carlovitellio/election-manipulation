#ifndef EMTRAITS_HPP
#define EMTRAITS_HPP

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-override"
#include <boost/graph/adjacency_list.hpp>
#pragma GCC diagnostic pop
#include <random>
#include "Person.hpp"

namespace ElectionManipulation{
  using Person = ElectionManipulation::Person;

  namespace EMTraits{
    //! The real type used in the following implementation
    using RealType = double;
    static_assert(std::is_floating_point_v<RealType>,
          "A floating point type is requested");
    //! The unsigned type used in the following implementation
    using UnsignedType  = unsigned int;
    static_assert(std::is_unsigned_v<UnsignedType>,
          "An unsigned type is requested");
    //! The Generator used as source of randomness
    using RandomGenerator = std::knuth_b;

    static_assert(std::is_default_constructible_v<ElectionManipulation::Person>,
                      "The Boost Graph Library requires default-constructible vertex properties");

    //! The Graph type implementation requested
    using Graph = boost::adjacency_list<boost::vecS, boost::vecS,
                                        boost::undirectedS, Person>;
  } // end namespace ElectionManipulation::EMTraits

} // end namespace ElectionManipulation


#endif //EMTRAITS_HPP
