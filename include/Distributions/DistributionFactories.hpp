#ifndef DISTRIBUTIONFACTORIES_HPP
#define DISTRIBUTIONFACTORIES_HPP

#include "Distributions/DistributionsTraits.hpp"
#include "Distributions/BetaDistribution.hpp"
#include "Distributions/PoissonDistribution.hpp"
/*!
    @note If it is not intended to use MuParser for reading a generic Distribution,
    the below lines have to be commented. Furthermore, in the loadFactory function
    (both of the overloaded functions) the corresponding lines have to be commented
*/
#include "Distributions/ParsedDistribution_N.hpp"
#include "Distributions/ParsedDistribution_0_1.hpp"

namespace ElectionManipulation::Distributions{

  /*!
      Function to load in the ResistanceDistFactory the corresponding Distributions.
      If a new Distribution is implemented and it is intended to be added in the
      current Factory, it must be added in this function as new value in the map
  */
  template<class Generator>
  void loadFactory(ResistanceDistFactory<Generator>& f){
    f["Poisson"]=[](){return std::make_unique<PoissonDistribution<Generator, UnsignedType>>();};
    //! If it is not intended to use MuParser for reading a generic Distribution,
    //! the below line has to be commented
    f["Parsed"]=[](){return std::make_unique<ParsedDistribution_N<Generator, UnsignedType>>();};
  }

  /*!
      Function to load in the VotingDistFactory the corresponding Distributions.
      If a new Distribution is implemented and it is intended to be added in the
      current Factory, it must be added in this function as new value in the map
  */
  template<class Generator>
  void loadFactory(VotingDistFactory<Generator>& f){
    f["Beta"]=[](){return std::make_unique<BetaDistribution<Generator,RealType>>();};
    //! If it is not intended to use MuParser for reading a generic Distribution,
    //! the below line has to be commented
    f["Parsed"]=[](){return std::make_unique<ParsedDistribution_0_1<Generator,RealType>>();};
  }

  /*!
      Helper function for returning a unique pointer to the Distribution requested
      with the string parameter, if the Distribution is in the corresponding Factory
  */
  template <class Generator, class Factory>
  auto find_in_factory(const Generator& gen, GetPot GPfile,
                          std::string distr, Factory& factory)
  {
    loadFactory(factory);
    auto it = factory.find(distr);
    if(it != factory.end()){
      auto dis_ptr = it->second();
      dis_ptr->set_gen(gen);
      dis_ptr->read_params(GPfile);

      return dis_ptr;

    } else {
      std::cerr << "Currently the following distributions are available" << std::endl;
      for(auto it_=factory.begin(); it_!=factory.end(); it_++)
        std::cerr << it_->first << std::endl;

      std::string excep = "Resistance distribution " + distr + " not available";
      throw std::invalid_argument(excep);
    }
  }

} // end namespace ElectionManipulation::Distributions


#endif // DISTRIBUTIONFACTORIES_HPP
