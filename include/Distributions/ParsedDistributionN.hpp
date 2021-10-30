#ifndef PARSEDDISTRIBUTIONN_HPP
#define PARSEDDISTRIBUTIONN_HPP

#include "Distributions/ProbabilityDistribution.hpp"
#include "muParser.h"
#include <string>
#include <fstream>
#include <random>
#include <boost/math/special_functions/factorials.hpp>


namespace ElectionManipulation::Distributions{

  double my_factorial(double n)
  {
    //! @note (from Boost documentation) the template argument must be a
    //! real-valued type not an integer type - that would overflow far too easily
    return boost::math::factorial<double>(n);
  }

  //! @brief Class for a probability distribution on the set of natural numbers
  /*! This class enables to read from a .txt file a probability mass function
      on the set of natural numbers and to simulate from it.

      In order to correctly parse the .txt file it takes advantage of the MuParser
      library. For convenience, a factorial function has been registered (simply
      write factorial(x) in the txt file). For further information about how it
      is parsed, read the documentation at https://beltoforion.de/en/muparser/

      In order to extract a new sample from the parsed probability mass function
      a number is randomly extracted in [0,1] and it is compared with the cumulative
      distribution function. In particular, the [0,1] interval represents the
      image of the cumulative distribution function. [0,1] is subdivided in
      intervals according to the cdf's jumps and so the new sample is extracted
      depending on the interval where it belongs

      \tparam Generator Random Engine used to extract a number in [0,1]
      \tparam ResT the result type has to be of unsigned type since the distribution
              is on the natural numbers
  */
  template <class Generator, class ResT=unsigned>
  class ParsedDistributionN: public ProbabilityDistribution<Generator, ResT>
  {
    static_assert(std::is_unsigned_v<ResT>,
    "An unsigned type has to be the result type of a drawn \
    from a probability distribution on N (set of natural numbers)");

  public:
    ParsedDistributionN()=default;

  //  ~ParsedDistributionN();

    std::unique_ptr<ProbabilityDistribution<Generator, ResT>> clone() const override
    { return std::unique_ptr<ProbabilityDistribution<Generator, ResT>>
                                    (new ParsedDistributionN(*this));}

    std::string name() const override {return "Parsed";}

    void set_gen(const Generator& gen_) override {gen=gen_;}

    void read_params(GetPot) override;

    ResT extract() override;
    //! Helper function for computing the cumulative density function till x
    double cdf(ResT x);

  private:
    Generator gen;        //!< The random engine used to extract new samples from [0,1]
    std::string my_pdf;   //!< The actual string read from the txt file
    double my_x{0.};
    mu::Parser parser;

  };


  template <class Generator, class ResT>
  void ParsedDistributionN<Generator, ResT>::read_params(GetPot GPfile)
  {
    std::string filename = GPfile("Person_option/Resistance/filename", "");
    std::ifstream file(filename);
    std::getline(file, my_pdf);
    file.close();

    try
  	{
      parser.DefineVar("x", &my_x);
      parser.DefineFun("factorial", my_factorial);
      parser.SetExpr(my_pdf);
  	}
  	catch (mu::Parser::exception_type &e)
  	{
  		std::cerr << e.GetMsg() << std::endl;
  	}

  }

  template <class Generator, class ResT>
  double ParsedDistributionN<Generator, ResT>::cdf(ResT x)
  {
    double sum{0.};
    for(ResT i=0; i<=x; i++)
    {
      my_x = i;
      sum += parser.Eval();
    }
    return sum;
  }

  template <class Generator, class ResT>
  ResT ParsedDistributionN<Generator, ResT>::extract()
  {
    std::uniform_real_distribution<double> unif_distribution(0.0,1.0);
    double goal_cdf = unif_distribution(gen);
    ResT res{0};
    while(true)
    {
      if(cdf(res) > goal_cdf){
        return res;
      }
      res++;
    }

  }

} // end of namespace ElectionManipulation::Distributions


#endif // PARSEDDISTRIBUTIONN_HPP
