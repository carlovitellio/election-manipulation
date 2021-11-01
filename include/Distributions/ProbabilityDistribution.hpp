#ifndef PROBABILITYDISTRIBUTIONS_HPP
#define PROBABILITYDISTRIBUTIONS_HPP

#include <memory>
#include <string>
#include "ExternalUtilities/GetPot"

namespace ElectionManipulation::Distributions{

  //! @brief The basis class for a Probability Distribution
  /*!
      This pure virtual class offers a common interface for implementing a
      Probability Distribution.
      The class is clonable for helping with compositions.
      A new sample can be sampled with the extract() method.

      \tparam Generator Random Engine used as source of randomness
      \tparam ResT the result type of the ProbabilityDistribution
  */
  template<class Generator, class ResultType>
  class ProbabilityDistribution
  {
  public:
    //! The class is clonable
    virtual std::unique_ptr<ProbabilityDistribution> clone() const=0;

    virtual ~ProbabilityDistribution()=default;
    //! String that identifies the current Distribution in use
    virtual std::string name() const=0;
    //! Set the random engine used to extract new samples from the Distribution
    virtual void set_gen(const Generator&) {}
    //! Method used to read the distribution parameters from GetPot
    virtual void read_params(GetPot)=0;
    //! Method used to extract a new sample from the distribution
    virtual ResultType extract()=0;
  };

} // end namespace ElectionManipulation::GraphCreator

#endif // PROBABILITYDISTRIBUTIONS_HPP
