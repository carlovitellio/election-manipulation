#ifndef PERSONCREATOR_HPP
#define PERSONCREATOR_HPP

#include "Person.hpp"
#include "Distributions/DistributionsTraits.hpp"

namespace ElectionManipulation{

  //! Class to populate a single node in the Graph
  /*!
      This class is used to characterise a Person in the Graph. In particular,
      the Person is populated w.r.t. her probability of voting the candidate in
      consideration, her resistance to change her belief and her name.
      Her resistance attribute is drawn from the ResistanceDistribution passed
      via the constructor.
      Her probability of voting is drawn from the VotingDistribution passed
      via the constructor.

      Two constructors are provided:
        - the first one takes the distributions as objects and exploits the fact
          that the base class ProbabilityDistribution is clonable
        - the second one takes the distributions as unique pointers storing them
      Both constructors take also the Generator passed to the Person (used as
      source of randomness in her lifetime) and, possibly, the Person's name.

      The class stores both the ResistanceDistribution and the VotingDistribution
      in a wrapper around a unique pointer. The implementation of this wrapper
      is due to Prof. Luca Formaggia.

      \tparam Generator random engine passed to the Person created and the oned
              by the Resistance and Voting distributions
  */
  template<class Generator>
  class PersonCreator{
  public:
    using ResDistHandler = Distributions::ResDistHandler<Generator>;
    using VotDistHandler = Distributions::VotDistHandler<Generator>;
    //! First constructor taking the distributions as objects
    PersonCreator(const Generator& re, const ResistanceDistribution<Generator>& dr,
                  const VotingDistribution<Generator>& dp, const std::string& name_ = ""):
      engine{re}, dis_res{dr.clone()}, dis_prob{dp.clone()}, name{name_} {}
    //! Second constructor taking the distributions as unique pointers
    template<class ResDist, class VotDist>
    PersonCreator(const Generator& re, ResDist&& dr,
                  VotDist&& dp, const std::string& name_ = ""):
              engine{re}, dis_res{std::forward<ResDist>(dr)},
              dis_prob{std::forward<VotDist>(dp)}, name{name_} {}

    //! The actual method called to populate the Person
    void apply(Person& p);

  private:
    Generator engine;            //!< random engine passed to the Person as her source of randomness
    ResDistHandler dis_res;      //!< probability distribution used to draw the resistance of the Person
    VotDistHandler dis_prob;     //!< probability distribution used to draw the probability of voting the candidate
  public:
    std::string name;            //!< name to give to the Person created
  };

  template<class Generator>
  void PersonCreator<Generator>::apply(Person& p)
  {
    p.init_resistance = dis_res->extract();
    p.resistance = p.init_resistance;
    p.prob_voting_c0 = dis_prob->extract();
    p.name = name;
    std::size_t seed = engine();
    p.engine = std::default_random_engine(seed);
  }
} // end namespace ElectionManipulation

#endif // PERSONCREATOR_HPP
