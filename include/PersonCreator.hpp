#ifndef PERSONCREATOR_HPP
#define PERSONCREATOR_HPP

#include "Person.hpp"
#include "Distributions/DistributionsTraits.hpp"

namespace ElectionManipulation{

  template<class Generator>
  class PersonCreator{
  public:
    using ResDist = ResistanceDistribution<Generator>;
    using VotDist = VotingDistribution<Generator>;
    using ResDistHandler = Distributions::ResDistHandler<Generator>;
    using VotDistHandler = Distributions::VotDistHandler<Generator>;

    PersonCreator(const Generator& re, const ResDist& dr,
                  const VotDist& dp, std::string name_ = ""):
      engine{re}, dis_res{dr.clone()}, dis_prob{dp.clone()}, name{name_} {}


    void apply(Person& p);

  private:
    Generator engine;
    ResDistHandler dis_res;
    VotDistHandler dis_prob;
  public:
    std::string name;

  };

  template<class Generator>
  void PersonCreator<Generator>::apply(Person& p)
  {
//    p.resistance = dis_res(engine);
    p.resistance = dis_res->extract();
//    p.prob_voting_c0 = dis_prob(engine);
    p.prob_voting_c0 = dis_prob->extract();
    p.name = name;
    std::size_t seed = engine();
    p.engine = std::default_random_engine(seed);
    p.update_marginal_utility();
  }
} // end namespace ElectionManipulation

#endif // PERSONCREATOR_HPP
