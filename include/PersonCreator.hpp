#ifndef PERSONCREATOR_HPP
#define PERSONCREATOR_HPP

#include "EMTraits.hpp"
#include "Person.hpp"

namespace ElectionManipulation{

  class PersonCreator{
  public:
    using RandomGenerator = EMTraits::RandomGenerator;
    using DistributionResistance = EMTraits::DistributionResistance;
    using DistributionProbability = EMTraits::DistributionProbability;

    PersonCreator(RandomGenerator re, DistributionResistance dr, DistributionProbability dp,
                  std::string name_ = ""):
      engine{re}, dis_res{dr}, dis_prob{dp}, name{name_} {}

    void apply(Person& p)
    {
      p.resistance = (std::size_t) dis_res(engine);
      p.prob_voting_c0 = (double) dis_prob(engine);
      p.name = name;
      std::size_t seed = engine();
      p.engine = std::default_random_engine(seed);
      p.update_marginal_utility();
    }

  private:
    RandomGenerator engine;
    DistributionResistance dis_res;
    DistributionProbability dis_prob;
  public:
    std::string name;

  };

} // end namespace ElectionManipulation
#endif // PERSONCREATOR_HPP
