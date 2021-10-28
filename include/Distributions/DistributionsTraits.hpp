#ifndef DISTRIBUTIONSTRAITS_HPP
#define DISTRIBUTIONSTRAITS_HPP

#include <memory>
#include <string>
#include <map>
#include <functional>
#include "ExternalUtilities/CloningUtilities.hpp"
#include "Distributions/ProbabilityDistribution.hpp"

namespace ElectionManipulation::Distributions{
  //! Forward declaration
//  template<class Generator, class ResultType> class ProbabilityDistribution;

  template<class Generator, class ResT>
  using DistributionBuilder = std::function<std::unique_ptr<ProbabilityDistribution<Generator, ResT>>()>;
  using DistributionID = std::string;


  //------------------------ Resistance section -----------------------
  using UnsignedType = EMTraits::UnsignedType;

  //! Specification of a ResistanceDistribution
  template<class Generator>
  using ResistanceDistribution = ProbabilityDistribution<Generator, UnsignedType>;
  //! unique_ptr wrapper for a ResistanceDistribution
  template<class Generator>
  using ResDistHandler = apsc::PointerWrapper<ResistanceDistribution<Generator>>;
  //! The factory associated to the resistance attribute of a Person
  template<class Generator>
  using ResistanceDistFactory = std::map<DistributionID,
                        DistributionBuilder<Generator, UnsignedType>>;
  //---------------------- end Resistance section ---------------------

  //------------------ Probability of voting section ------------------
  using RealType = EMTraits::RealType;

  //! Specification of a VotingDistribution
  template<class Generator>
  using VotingDistribution = ProbabilityDistribution<Generator, RealType>;
  //! unique_ptr wrapper for a VotingDistribution
  template<class Generator>
  using VotDistHandler = apsc::PointerWrapper<VotingDistribution<Generator>>;
  //! The factory associated to the probability of voting attribute of a Person
  template<class Generator>
  using VotingDistFactory = std::map<DistributionID,
                        DistributionBuilder<Generator, RealType>>;
  //---------------- end Probability of voting section ----------------

} // end namespace ElectionManipulation::Distributions

#endif // DISTRIBUTIONSTRAITS_HPP
