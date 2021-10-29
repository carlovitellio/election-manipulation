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
    double prob_voting_c0{0.};
    DefRandEngine engine;

  public:
    // Accessible also to the manipulator
    std::size_t resistance{10};    //!< The resistance in changing the probability of voting
    double manipulator_estim_prob{0.5};
    double manipulator_prob_activ{0.};
    double manipulator_utility{0.};
    double manipulator_marginal_utility{0.};

    //! Being the vertex property, it should be default constructable

    friend std::ostream & operator << (std::ostream &, Person const &);

    //! Forward declaration
    template<class RandGen> friend class PersonCreator;

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
  boost::dynamic_properties create_dynamicProperties(Graph& g, bool with_prop=true)
  {
    boost::dynamic_properties dp(boost::ignore_other_properties);
    dp.property("node_id",               get(&Person::name, g));
    if(with_prop)
    {
      dp.property("Probability_voting", get(&Person::prob_voting_c0, g));
      dp.property("Resistance",            get(&Person::resistance, g));
      dp.property("Estimated_probability", get(&Person::manipulator_estim_prob, g));
    }

    return dp;
  }
/*

  template <class Graph>
  boost::dynamic_properties create_dProperties_no_read(Graph& g)
  {
    boost::dynamic_properties dp(boost::ignore_other_properties);
    dp.property("node_id",               get(&Person::name, g));

    return dp;
  }
*/
} // end of namespace ElectionManipulation

#endif // PERSON_HPP
