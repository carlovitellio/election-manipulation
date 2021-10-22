#ifndef ELECTIONMANIPULATIONTRAITS_HPP
#define ELECTIONMANIPULATIONTRAITS_HPP

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-override"
#include <boost/graph/adjacency_list.hpp>
#pragma GCC diagnostic pop

#include <random>
#include <boost/random/beta_distribution.hpp>
#include "Person.hpp"
#include "Utilities/CloningUtilities.hpp"
#include "Utilities/Factory.hpp"

namespace ElectionManipulation{
  using Person = ElectionManipulation::Person;

  namespace EMTraits{
    using RandomGenerator = std::knuth_b;

    using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
    Person>;

    using DistributionResistance = std::poisson_distribution<std::size_t>;
    using DistributionProbability = boost::random::beta_distribution<>;
  } // end namespace ElectionManipulation::EMTraits

  namespace GraphCreator{
    //! Forward declaration
    class GraphCreatorBase;

    using GCHandler = apsc::PointerWrapper<GraphCreatorBase>;

  } // end namespace ElectionManipulation::GraphCreator


} // end namespace ElectionManipulation


#endif //ELECTIONMANIPULATIONTRAITS_HPP
