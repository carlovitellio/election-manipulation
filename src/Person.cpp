#include "Person.hpp"

namespace ElectionManipulation{

  std::ostream & operator << (std::ostream & str, Person const & p)
  {
    str << "Name: " << p.name << ", "
        << "Resistance: " << p.resistance << ", "
        << "Probability_voting: " << p.prob_voting_c0 << ".\n";

    return str;
  }

  //! The Person casts a ballot according to her probability of voting
  bool Person::cast_a_ballot() const
  {
    std::bernoulli_distribution d(prob_voting_c0);
    return d(engine);
  }

  bool Person::receive_message()
  {
    solicited = true;
    bool accepted = cast_a_ballot();

    if(accepted) update_prob();

    return accepted;

  }

  void Person::update_prob()
  {
    prob_voting_c0 = (prob_voting_c0 * resistance + 1)/(resistance + 1);
    resistance++;
  }

} // end of namespace ElectionManipulation
