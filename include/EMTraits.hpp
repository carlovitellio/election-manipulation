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
    using RealType = double;
    using UnsignedType  = unsigned int;

    using RandomGenerator = std::knuth_b;

    static_assert(std::is_default_constructible_v<ElectionManipulation::Person>,
                      "The Boost Graph Library requires default-constructible vertex properties");

    using Graph = boost::adjacency_list<boost::vecS, boost::vecS,
                                        boost::undirectedS, Person>;
  } // end namespace ElectionManipulation::EMTraits

} // end namespace ElectionManipulation


#endif //EMTRAITS_HPP
