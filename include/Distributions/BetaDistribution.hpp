#ifndef BETADISTRIBUTION_HPP
#define BETADISTRIBUTION_HPP

#include "Distributions/ProbabilityDistribution.hpp"
#include <boost/random/beta_distribution.hpp>
#include <stdexcept>

namespace ElectionManipulation::Distributions{

  template <class Generator, class ResT=double>
  class BetaDistribution final: public ProbabilityDistribution<Generator, ResT>
  {
    static_assert(std::is_floating_point_v<ResT>,
          "A floating point type has to be the result type of a drawn from a Beta distribution");

  public:
    //! @note in the Beta distribution implemented in the Boost Library
    //! both result_type and input_type are equal to the template parameter RealType
    using Distribution = boost::random::beta_distribution<ResT>;

    std::unique_ptr<ProbabilityDistribution<Generator, ResT>> clone() const override
    { return std::unique_ptr<ProbabilityDistribution<Generator, ResT>>(new BetaDistribution(*this));}

    std::string name() const override {return "Beta";}

    void set_gen(const Generator& gen_) override {gen=gen_;}

    void read_params(GetPot) override;

    ResT extract() override {return d(gen);}

  private:
    Generator gen;  //!< The random engine used to extract new samples from the Distribution
    Distribution d; //!< The actual distribution
  };

  template <class Generator, class ResT>
  void BetaDistribution<Generator, ResT>::read_params(GetPot GPfile)
  {
    //! Read the distribution parameters from GetPot
    ResT alpha = GPfile("Person_option/ProbabilityVoting/beta/alpha", 1.);
    ResT beta = GPfile("Person_option/ProbabilityVoting/beta/beta", 1.);
    //! Checking consistance of the parameters just read
    if((alpha <= 0.) || (beta <= 0.))
    {
      std::string excep = "Wrong input parameters for the Probability of \
                          Voting distribution.\n The parameters of the \
                          Beta distribution have to be greater than 0\n";
      throw std::invalid_argument(excep);
    }

    //! Set the distribution parameters to the ones read from GetPot
    using param_t = typename Distribution::param_type;
    d.param(param_t(alpha, beta));
  }

} // end of namespace ElectionManipulation::Distributions


#endif // BETADISTRIBUTION_HPP
