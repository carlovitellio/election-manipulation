#ifndef DISTRIBUTIONFACTORIES_HPP
#define DISTRIBUTIONFACTORIES_HPP

#include "Distributions/DistributionsTraits.hpp"
#include "Distributions/BetaDistribution.hpp"
#include "Distributions/PoissonDistribution.hpp"
#include "Distributions/ParsedDistributionN.hpp"

namespace ElectionManipulation::Distributions{
/*
  template<class Generator>
  void loadResistanceFactory(ResistanceDistFactory<Generator>& f){
    f["Poisson"]=[](){return std::make_unique<PoissonDistribution<Generator, UnsignedType>>();};
  }

  template<class Generator>
  void loadVotingFactory(VotingDistFactory<Generator>& f){
    f["Beta"]=[](){return std::make_unique<BetaDistribution<Generator,RealType>>();};
  }
*/
  template<class Generator>
  void loadFactory(ResistanceDistFactory<Generator>& f){
    f["Poisson"]=[](){return std::make_unique<PoissonDistribution<Generator, UnsignedType>>();};
    f["Parsed"]=[](){return std::make_unique<ParsedDistributionN<Generator, UnsignedType>>();};
  }

  template<class Generator>
  void loadFactory(VotingDistFactory<Generator>& f){
    f["Beta"]=[](){return std::make_unique<BetaDistribution<Generator,RealType>>();};
  }

  template <class Generator, class Factory>
  auto find_in_factory(const Generator& gen, GetPot GPfile, std::string distr, Factory& factory)
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
