#ifndef PERSONCREATOR_HPP
#define PERSONCREATOR_HPP

#include "Person.hpp"

namespace ElectionManipulation{

  template<class RandomGenerator, class DistributionResistance, class DistributionProbability>
  class PersonCreator{

  public:
    PersonCreator(RandomGenerator re, DistributionResistance dr, DistributionProbability dp,
                  std::string name_ = ""):
      engine{re}, dis_res{dr}, dis_prob{dp}, name{name_} {}

    void apply(Person& p)
    {
      p.resistance = (unsigned) dis_res(engine);
      p.prob_voting_c0 = (double) dis_prob(engine);
      p.name = name;
      unsigned int seed = engine();
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
