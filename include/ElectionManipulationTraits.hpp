#ifndef ELECTIONMANIPULATIONTRAITS_HPP
#define ELECTIONMANIPULATIONTRAITS_HPP

#include <random>
#include "Utilities/CloningUtilities.hpp"
#include "Utilities/Factory.hpp"
#include "GraphCreatorBase.hpp"
#include "GraphCreatorSmallWorld.hpp"
#include "GraphCreatorRMAT.hpp"
#include "GraphCreatorErdosRenyi.hpp"

namespace ElectionManipulation{
  using namespace ElectionManipulation::GraphCreator;

  using RandomGenerator = std::knuth_b;

  using DistributionResistance = std::poisson_distribution<std::size_t>;
  using DistributionProbability = boost::random::beta_distribution<>;

  template<class Graph>
  struct EMTraits{

    using GCBase = GraphCreatorBase<RandomGenerator, Graph>;
    using GCHandler = apsc::PointerWrapper<GCBase>;

    using GCSmallWorld = GraphCreatorSmallWorld<RandomGenerator, Graph>;
    using GCRMAT = GraphCreatorRMAT<RandomGenerator, Graph>;
    using GCErdosRenyi = GraphCreatorErdosRenyi<RandomGenerator, Graph>;

    using Builder = std::function<std::unique_ptr<GCBase>()>;

    using GraphFactory = GenericFactory::Factory<
                                GCBase,
                                std::string
                                >;

  };

} // end namespace ElectionManipulation


#endif //ELECTIONMANIPULATIONTRAITS_HPP
