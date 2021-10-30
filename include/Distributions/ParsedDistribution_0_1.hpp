#ifndef PARSEDDISTRIBUTION_0_1_HPP
#define PARSEDDISTRIBUTION_0_1_HPP

#include "Distributions/ProbabilityDistribution.hpp"
#include <vector>
#include <algorithm>


namespace ElectionManipulation::Distributions{
  //! Helper function that emulates the Matlab function
  //! It generates N samples lineary spaced in [a,b]
  template<class RealType=double>
  std::vector<RealType> linspace(RealType a, RealType b, std::size_t N)
  {
    static_assert(std::is_floating_point_v<RealType>,
          "A floating point type is needed in linspace");
    RealType h = (b-a) / static_cast<RealType>(N-1);
    std::vector<RealType> v(N);
    std::generate(v.begin(), v.end(), [n=0, &h]() mutable {return n++*h;});

    return v;
  }
  //! @brief Class for a probability distribution on [0,1]
  /*! This class enables to read from a .txt file a probability mass function
      on [0,1] and to simulate from it.

      In order to correctly parse the .txt file it takes advantage of the MuParser
      library. For further information about how it is parsed, read the
      documentation at https://beltoforion.de/en/muparser/

      In order to extract a new sample, Rejection sampling is used.
      The algorithm proceeds as follows:
      1. Sample a point on the x-axis from the proposal distribution.
      2. Draw a vertical line at this x-position, up to the maximum y-value of
         the probability density function of the proposal distribution.
      3. Sample uniformly along this line from 0 to the maximum of the probability
         density function. If the sampled value is greater than the value of the
         desired distribution at this vertical line, reject the x-value and return
         to step 1; else the x-value is a sample from the desired distribution.

      @note the parsed distribution has to be a proper Probability Distribution
            up to a moltiplicative factor.
            No checks are performed about it, so the user has to be cautious

      \tparam Generator Random Engine used to extract a number in [0,1]
      \tparam ResT the result type has to be of floating type since the distribution
              is on [0,1]

  */
  template <class Generator, class ResT=double>
  class ParsedDistribution_0_1 final: public ProbabilityDistribution<Generator, ResT>
  {
    static_assert(std::is_floating_point_v<ResT>,
          "A floating point type has to be the result type of a drawn from [0,1]");

  public:

    std::unique_ptr<ProbabilityDistribution<Generator, ResT>> clone() const override
    { return std::unique_ptr<ProbabilityDistribution<Generator, ResT>>(new ParsedDistribution_0_1(*this));}

    std::string name() const override {return "Parsed";}

    void set_gen(const Generator& gen_) override {gen=gen_;}

    void read_params(GetPot) override;
    //! Method used to find an estimate of the maximum of pdf in [0,1]
    double find_pdf_maximum();

    ResT extract() override;

  private:
    Generator gen;        //!< The random engine used to extract new samples from the Distribution
    std::string my_pdf;   //!< The actual string read from the txt file
    mu::Parser parser;    //!< The parser provided with the MuParser library
    double my_x{0.};      //!< The variable attached to the Parser
    double maximum;       //!< The maximum of pdf that will be used as scaling factor for Rejection sampling
  };

  template <class Generator, class ResT>
  void ParsedDistribution_0_1<Generator, ResT>::read_params(GetPot GPfile)
  {
    std::string filename = GPfile("Person_option/ProbabilityVoting/filename", "");
    std::filesystem::path filepath(filename);
    if (!std::filesystem::exists(filepath))
    {
      std::cerr << "Input file " << filename << " does not exists\n";
      std::exit(1);
    }
    std::ifstream file(filename);
    std::getline(file, my_pdf);
    file.close();

    try
    {
      parser.DefineVar("x", &my_x);
      parser.SetExpr(my_pdf);
    }
    catch (mu::Parser::exception_type &e)
    {
      std::cerr << e.GetMsg() << std::endl;
    }

    maximum = find_pdf_maximum();
  }

  template <class Generator, class ResT>
  double ParsedDistribution_0_1<Generator, ResT>::find_pdf_maximum()
  {
    auto eval = [&,this](ResT& x){this->my_x = x; x = this->parser.Eval();};
    std::vector<ResT> v = linspace(0., 1., 1001);
    std::for_each(v.begin(), v.end(), eval);

    return *std::max_element(v.cbegin(), v.cend());
  }

  template <class Generator, class ResT>
  ResT ParsedDistribution_0_1<Generator, ResT>::extract()
  {
    //! Implementing rejection sampling
    //! A uniform proposal function is used. In particular the value of the
    //! proposal function is equal to maximum computed with find_pdf_maximum()
    std::uniform_real_distribution<double> unif_distribution(0.0,1.0);
    double my_y;

    while(true)
    {
      //! Sample a point on [0,1] from the proposal distribution (uniform in this case)
      my_x = unif_distribution(gen);
      //! Sample uniformly from 0 to the maximum of the pdf of the proposed
      //! distribution (equal to the maximum found above).
      my_y = maximum * unif_distribution(gen);
      //! If the sampled value is less (or equal) than the value of the desidered
      //! distribution at this vertical line, accept the sampled point
      if(my_y <= parser.Eval()) // if true the sample is accepted
        return my_x;
    }

  }


} // end of namespace ElectionManipulation::Distributions


#endif // PARSEDDISTRIBUTION_0_1_HPP
