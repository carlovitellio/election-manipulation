#ifndef READINFORMATION_HPP
#define READINFORMATION_HPP

#include "ExternalUtilities/GetPot"
#include "GraphCreators/GraphCreatorBase.hpp"
#include "GraphCreators/GraphCreatorFactory.hpp"
#include "Distributions/DistributionsTraits.hpp"
#include "Distributions/DistributionFactories.hpp"
#include "EMUtilities.hpp"
#include <filesystem>

namespace ElectionManipulation{

  using GraphCreatorBase = GraphCreator::GraphCreatorBase;
  using namespace Distributions;

  template<class Generator>
  class ReadInfoGetPot{
  private:
    GetPot cl;       //!< The GetPot member related to the command line
    GetPot GPfile;   //!< The GetPot member related to the input file to be read
    Generator gen;   //!< The RandomGenerator passed to the Creators
  public:
    ReadInfoGetPot(const GetPot& cl_, const Generator& gen_): cl{cl_}, gen{gen_} {}
    //! Check whether the user is asking for the help of the executable
    void check_help();
    //! Look for the input file where to search every information needed for
    //! the manipulation process
    void read_input_file();
    //! Look for the information needed for constructing the GraphCreator requested
    //! \return Returns a unique pointer to the GraphCreator base class
    std::unique_ptr<GraphCreatorBase> readInfoGraphCreator();
    //! Look for the ProbabilityDistribution needed for the generating the
    //! Resistance attribute of the Person in the social network.
    //! \return Returns a unique pointer to the ResistanceDistribution class
    std::unique_ptr<ResistanceDistribution<Generator>> readInfoResistanceDist();
    //! Look for the ProbabilityDistribution needed for the generating the
    //! probability of voting of each Person in the social network.
    //! \return Returns a unique pointer to the VotingDistribution class
    std::unique_ptr<VotingDistribution<Generator>> readInfoVotingDist();

  };

  template<class Generator>
  void ReadInfoGetPot<Generator>::check_help()
  {
    if (cl.search(2, "--help", "-h")){
      printHelp();
      std::exit(0);
    }
  }


  template<class Generator>
  void ReadInfoGetPot<Generator>::read_input_file()
  {
    std::string inputFile = cl("InputFile","../input/graph_input.getpot");
    std::filesystem::path filepath(inputFile);
    if (!std::filesystem::exists(filepath))
      {
        std::cerr << "Input file " << inputFile << " does not exists\n";
        std::exit(1);
      }

    GPfile = GetPot(inputFile.c_str());
  }


  template<class Generator>
  std::unique_ptr<GraphCreatorBase>
  ReadInfoGetPot<Generator>::readInfoGraphCreator()
  {
    std::unique_ptr<GraphCreatorBase> gc_ptr;

    using GraphFactory = GraphCreator::GraphCreatorFactory;
    //! Instantiating the Factory collecting the graph creator methods
    const GraphFactory & MyFactory = GraphCreator::MyFactory;

    //! If the user used -l or --list option, the Graph Creator methods available are printed
    if (cl.search(2, "--list", "-l"))
    {
      printRegistered(MyFactory);
      std::exit(0);
    }

    //! The graph type is deduced run-time reading from an input file
    std::string GCmethod = GPfile("Graph_option/graph_type", "");
    if(GCmethod == "?"){
      printRegistered(MyFactory);
      std::exit(0);
    }

    try {
      gc_ptr = MyFactory.create(GCmethod);
      gc_ptr->set_gen(gen);
      gc_ptr->read_params(GPfile);
      std::clog << "Implementing the graph with " << GCmethod << " creator" << std::endl;
    }
    catch (std::invalid_argument & e)
    {
      std::cerr << e.what() << std::endl;
      std::cerr << "Registered methods are " << std::endl;
      printRegistered(MyFactory);
      std::exit(1);
    }

    return gc_ptr;
  }



  template<class Generator>
  std::unique_ptr<ResistanceDistribution<Generator>>
  ReadInfoGetPot<Generator>::readInfoResistanceDist()
  {
    std::string distr = GPfile("Person_option/Resistance/distribution", "");
    ResistanceDistFactory<Generator> factory;

    return find_in_factory(gen, GPfile, distr, factory);
  }



  template<class Generator>
  std::unique_ptr<VotingDistribution<Generator>>
  ReadInfoGetPot<Generator>::readInfoVotingDist(){

    VotingDistFactory<Generator> factory;
    std::string distr = GPfile("Person_option/ProbabilityVoting/distribution", "");

    return find_in_factory(gen, GPfile, distr, factory);
  }


} // end namespace ElectionManipulation

#endif // READINFORMATION_HPP
