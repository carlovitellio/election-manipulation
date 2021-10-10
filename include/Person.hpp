#ifndef PERSON_HPP
#define PERSON_HPP

#include <iostream>

namespace ElectionManipulation{
  //! Class that identifies a person in the network
  /*!

  */
  class Person{

  private:
    std::string name;           //!< The name of the person
    unsigned int resistance;    //!< The resistance in changing the probability of voting
    double prob_voting_c0;

  public:
//    void receive_message();
//    void update_prob();

    friend std::ostream & operator << (std::ostream &, Person const &);

     template<class RandomEngine, class DistributionResistance,
              class DistributionProbability> friend class PersonCreator;
  };

} // end of namespace ElectionManipulation

#endif // PERSON_HPP
