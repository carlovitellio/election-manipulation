#ifndef PERSON_HPP
#define PERSON_HPP

#include <iostream>
#include <random>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-override"
#include <boost/property_map/dynamic_property_map.hpp> // for the dynamic properties helper function
#include <boost/graph/adjacency_list.hpp>
#pragma GCC diagnostic pop

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


  //! Helper function for constructing a dynamic property associated with a graph
  //! It is based on the class Person defined above
  template <class Graph>
  boost::dynamic_properties create_dynamicProperties_reading(Graph& g)
  {
    boost::dynamic_properties dp(boost::ignore_other_properties);
    // dp.property("node_id",               get(boost::vertex_index,g));
    dp.property("Name",                  get(&Person::name, g));
    dp.property("Probability of voting", get(&Person::prob_voting_c0, g));
    dp.property("Resistance",            get(&Person::resistance, g));
    dp.property("Estimated probability", get(&Person::manipulator_estim_prob, g));

    return dp;
  }

  template <class Graph>
  boost::dynamic_properties create_dynamicProperties_writing(Graph& g)
  {
    boost::dynamic_properties dp{create_dynamicProperties_reading(g)};
    dp.property("node_id",               get(boost::vertex_index,g));
    return dp;
  }

} // end of namespace ElectionManipulation

#endif // PERSON_HPP
