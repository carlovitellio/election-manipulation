#ifndef POISSONDISTRIBUTION_HPP
#define POISSONDISTRIBUTION_HPP

#include "Distributions/ProbabilityDistribution.hpp"
#include "EMTraits.hpp"
#include <random>
#include <string>
#include <stdexcept>

namespace ElectionManipulation::Distributions{

  template <class Generator, class ResT=int>
  class PoissonDistribution final: public ProbabilityDistribution<Generator, ResT>
  {
    static_assert(std::is_unsigned_v<ResT>,
          "An unsigned type has to be the result type of a drawn from a Poisson distribution");
  public:
    using Distribution = std::poisson_distribution<ResT>;

    std::unique_ptr<ProbabilityDistribution<Generator, ResT>> clone() const override
    { return std::unique_ptr<ProbabilityDistribution<Generator, ResT>>(new PoissonDistribution(*this));}

    std::string name() const override {return "Poisson";}

    void set_gen(const Generator& gen_) override {gen=gen_;}

    void read_params(GetPot) override;

    ResT extract() override {return d(gen);}

  private:
    Generator gen;  //!< The random engine used to extract new samples from the Distribution
    Distribution d; //!< The actual distribution
  };

  template <class Generator, class ResT>
  void PoissonDistribution<Generator, ResT>::read_params(GetPot GPfile)
  {
    using param_t = typename Distribution::param_type;
    //! Read the distribution parameter from GetPot
    double lambda = GPfile("Person_option/Resistance/poisson/lambda", 50.);
    //! Checking consistance of the parameter just read
    if(lambda <= 0.)
    {
      std::string excep = "Wrong input parameters for the Resistance distribution.\n\
                The parameter of the Poisson distribution has to be greater than 0\n";
      throw std::invalid_argument(excep);
    }
    //! Set the distribution parameter to the one read from GetPot
    d.param(param_t(lambda));
  }

} // end of namespace ElectionManipulation::Distributions


#endif // POISSONDISTRIBUTION_HPP
