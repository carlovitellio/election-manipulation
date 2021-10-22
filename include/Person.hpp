#ifndef PERSON_HPP
#define PERSON_HPP

#include <iostream>
#include <random>

namespace ElectionManipulation{
  //! Class that identifies a person in the network
  /*!

  */
  class Person{
  public:
    using DefRandEngine=std::default_random_engine;

  private:
    std::string name;           //!< The name of the person
    double prob_voting_c0;
    DefRandEngine engine;

  public:
    // Accessible also to the manipulator
    std::size_t resistance;    //!< The resistance in changing the probability of voting
    double manipulator_estim_prob{0.5};
    double manipulator_prob_activ;
    double manipulator_utility;
    double manipulator_marginal_utility;

    //! Being the vertex property, it should be default constructable

    friend std::ostream & operator << (std::ostream &, Person const &);

    friend class PersonCreator;

    friend class PerformanceEvaluator;


    //! The Person casts a ballot according to her probability of voting
    bool cast_a_ballot();

    //! The Person receives a message and decides whether accepting it or not
    bool receive_message();

    //! The Person has accepted the message and changes her probability of voting accordingly
    /*! In particular, her probability of voting the candidate c0 increases
        according to the following rule
        \f[
            p_{t+1} = \frac{p_t * w +1}{w+1}
        \f]
    */
    void update_prob();

    //! Function used to update the marginal utility for the manipulator
    /*! The manipulator marginal utility is determined by the possible
        shift in probability obtained if the Person accepts her message
        \f[
            p_{t+1} - p_t = \frac{1 - p_t}{w+1}
        \f]
    */
    void update_marginal_utility();

  };

} // end of namespace ElectionManipulation

#endif // PERSON_HPP
