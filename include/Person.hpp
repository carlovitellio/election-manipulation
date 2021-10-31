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
      This class represents the properties attached to each vertex in the Social Network.
      In fact, it is used as a Bundled Property attached to the Boost Graph.
      Being the vertex property, it should be default constructable!
      It is characterized by a private and a public interface. In particular,
      private data members are intended to be modified only inside the class itself
      and by the friend class PersonCreator. Also the class PerformanceEvaluator
      is declared friend in order to correctly evaluate the influence process.

  */
  class Person{
  public:
    using DefRandEngine=std::default_random_engine;

  private:
    std::string name;           //!< The name of the person
    double prob_voting_c0{0.};  //!< Probability of voting for the candidate analyzed
    DefRandEngine engine;       //!< Source of randomness during casting a ballot

  public:
    // Accessible also to the manipulator
    std::size_t resistance{10};             //!< The resistance in changing the probability of voting
    double manipulator_estim_prob{0.5};     //!< The manipulator's estimate of prob_voting_c0
    double manipulator_prob_activ{0.};      //!< The manipulator's belief in activation during an information cascade
    double manipulator_utility{0.};         //!< The manipulator's gain in influencing the node
    double manipulator_marginal_utility{0.};//!< The manipulator's expected utility in conditioning other vertices

    //! Helper function if the stream operator is used to print out the
    //! the social network (contained in <boost/graph/adjacency_list_io.hpp>)
    friend std::ostream & operator << (std::ostream &, Person const &);
    //! Forward declaration of the friend class used to generate the Person's attributes
    template<class Generator> friend class PersonCreator;
    //! Forward declaration of the friend class used evaluate the influence process
    friend class PerformanceEvaluator;

    //! @brief The Person casts a ballot according to her probability of voting
    //! A sample from a Bernoulli distribution with parameter equal to
    //! prob_voting_c0 is sampled
    //! \return true if she votes for the candidate in consideration, false otherwise
    bool cast_a_ballot();

    //! @brief The Person receives a message and decides whether accepting it or not
    //! She accepts the message with the same probability she casts a ballot for the candidate in consideration.
    //! If accepted, a message changes the prob_voting_c0 and update_prob() is called
    //! \return true if the message has been accepted, false otherwise
    bool receive_message();

    //! The Person has been influenced and changes her probability of voting accordingly
    /*! In particular, her probability of voting the candidate c0 increases
        according to the following rule
        \f[
            p_{t+1} = \frac{p_t * w +1}{w+1}
        \f]
    */
    void update_prob();

    //! Function used by the manipulator to update her marginal utility
    //! in conditioning the current node
    /*! The manipulator marginal utility is determined by the possible
        shift in probability obtained if the Person accepts her message
        \f[
            p_{t+1} - p_t = \frac{1 - p_t}{w+1}
        \f]
    */
    void update_marginal_utility();

  };


  //! #brief Helper function for constructing a dynamic property associated with a graph
  /*!
      It is based on the class Person defined above
      It is used to read and write graphs from and to files with graphml and graphviz
      \param with_prop asks if it requested to include all properties or just the
             node_it useful to read a graph with no properties attached
  */
  template <class Graph>
  boost::dynamic_properties create_dynamicProperties(Graph& g, bool with_prop=true)
  {
    boost::dynamic_properties dp(boost::ignore_other_properties);
    dp.property("node_id",                 get(&Person::name, g));
    if(with_prop)
    {
      dp.property("Probability_voting",    get(&Person::prob_voting_c0, g));
      dp.property("Resistance",            get(&Person::resistance, g));
      dp.property("Estimated_probability", get(&Person::manipulator_estim_prob, g));
    }

    return dp;
  }

} // end of namespace ElectionManipulation

#endif // PERSON_HPP
