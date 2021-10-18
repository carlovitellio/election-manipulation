#include "Person.hpp"

namespace ElectionManipulation{

  std::ostream & operator << (std::ostream & str, Person const & p)
  {
    str << "Name: " << p.name << ", "
        << "resistance: " << p.resistance << ", "
        << "probability of voting c_0: " << p.prob_voting_c0 << ".\n";

    return str;
  }

  bool Person::receive_message()
  {
    // It should be reinstatiated since the probability changes during time
    std::bernoulli_distribution d(prob_voting_c0);
    bool accepted = d(engine);

    if(accepted) update_prob();

    return accepted;

  }

  void Person::update_prob()
  {
    /*! The Person's probability of voting the candidate c0 increases
        according to the following rule
        \f[
            p_{t+1} = \dfrac{p_t * w +1}{w+1}
        \f]
    */
    prob_voting_c0 = (prob_voting_c0 * resistance + 1)/(resistance + 1);
    resistance++;
    update_marginal_utility();

  }

  void Person::update_marginal_utility()
  {
    /*! The manipulator marginal utility is determined by the possible
        shift in probability obtained if the Person accepts her message
        \f[
            p_{t+1} - p_t = \dfrac{1 - p_t}{w+1}
        \f]
    */
    manipulator_marginal_utility = (1 - prob_voting_c0)/(resistance + 1);
  }

} // end of namespace ElectionManipulation
