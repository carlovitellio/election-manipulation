#ifndef PROBABILITYDISTRIBUTIONS_HPP
#define PROBABILITYDISTRIBUTIONS_HPP

#include <memory>
#include <string>
#include "ExternalUtilities/GetPot"

namespace ElectionManipulation::Distributions{

  //! The basis class for all graph creators
  template<class Generator, class ResultType>
  class ProbabilityDistribution
  {
  public:
    //! The class is clonable.
    /*!
      Having a clonable class makes it possible to write copy constructors
      and assignment operators for classes that aggregate object of the
      GraphCreator hierarchy by composition.
    */
    virtual std::unique_ptr<ProbabilityDistribution> clone() const=0;

    virtual ~ProbabilityDistribution()=default;
    //! a string that identify the general type of Graph Creator
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
