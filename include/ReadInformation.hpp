#ifndef READINFORMATION_HPP
#define READINFORMATION_HPP

#include "ExternalUtilities/GetPot"
#include "GraphCreators/GraphCreatorBase.hpp"
#include "GraphCreators/GraphCreatorFactory.hpp"
#include "Distributions/DistributionsTraits.hpp"
#include "Distributions/DistributionFactories.hpp"
#include "EMUtilities.hpp"
#include <dlfcn.h>
#include <filesystem>
#include <utility>
#include <boost/lexical_cast.hpp>

namespace ElectionManipulation{

  using GraphCreatorBase = GraphCreator::GraphCreatorBase;
  using namespace Distributions;
  //! Helper class for reading the information needed from the command line and the input file
  /*!
      In this class many methods are implemented for retrieving information from
      command line and input file. Some method takes care of returning the proper
      pointer to the method chosen by the user at run-time and loaded from a factory.

      For the GraphCreator is used a Factory using the Singleton design pattern
      as proposed by Prof. Formaggia during his classes at Politecnico di Milano.

      For the ProbabilityDistribution is used a Factory implemented as a map.

      @tparam The RandomGenerator class that will be used in the creation of the graph
      (both the structure and the vertex properties)
  */
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
    //! \return a string related to the method to use
    std::string readInfoGraphCreator();

    //! Read the plugin libraries to be loaded
    void readPluginLibraries();

    //! Try to instantiate the requested method
    //! \return Returns a unique pointer to the GraphCreator base class
    std::unique_ptr<GraphCreatorBase> instantiateGraphCreator();

    //! Look for the ProbabilityDistribution needed for the generating the
    //! Resistance attribute of the Person in the social network.
    //! \return Returns a unique pointer to the ResistanceDistribution class
    std::unique_ptr<ResistanceDistribution<Generator>> readInfoResistanceDist();

    //! Look for the ProbabilityDistribution needed for the generating the
    //! probability of voting of each Person in the social network.
    //! \return Returns a unique pointer to the VotingDistribution class
    std::unique_ptr<VotingDistribution<Generator>> readInfoVotingDist();

    //! Read parameters useful for the influence process
    //! \return A pair containing the number of influence rounds and a boolean true if the utility estimation has to be estimated properly
    std::pair<std::size_t, bool> readInfluenceOption();

    //! Check whether the user wants the results (in term of metrics)
    //! printed in a file in the out folder
    bool readInfoOutputResults();

    //! Check whether the user wants the graph printed in a file in the out folder
    bool readInfoPrintGraph();
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
  std::string ReadInfoGetPot<Generator>::readInfoGraphCreator()
  {
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

    return GCmethod;
  }

  template<class Generator>
  void ReadInfoGetPot<Generator>::readPluginLibraries()
  {
    auto nlibs = GPfile.vector_variable_size("Graph_option/library");
    for(std::size_t i=0; i<nlibs; i++){
      std::string library = GPfile("Graph_option/library", i, "");
      if(library == "") {
        std::cerr << "File wrongly parsed. Unable to read the library" << std::endl;
        std::exit(1);
      } else {
        void* plugin_lib = dlopen(library.c_str(), RTLD_NOW);
        if (plugin_lib == nullptr) {
          std::cerr << "Unable to find the library " << library << std::endl;
          std::cerr << dlerror() << std::endl;
          std::exit(1);
        }
      }

    }
  }

  template<class Generator>
  std::unique_ptr<GraphCreatorBase>
  ReadInfoGetPot<Generator>::instantiateGraphCreator()
  {
    using GraphFactory = GraphCreator::GraphCreatorFactory;
    //! Instantiating the Factory collecting the graph creator methods
    const GraphFactory & MyFactory = GraphCreator::MyFactory;

    std::string GCmethod = readInfoGraphCreator();
    readPluginLibraries();

    std::unique_ptr<GraphCreatorBase> gc_ptr;

    try {
      gc_ptr = MyFactory.create(GCmethod);
      gc_ptr->set_gen(gen);
      gc_ptr->read_params(GPfile);
      std::clog << "---------------------------------------" << std::endl;
      std::clog << "Implementing the graph with " << GCmethod << " creator" << std::endl;
      std::clog << "---------------------------------------" << std::endl;
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


  template<class Generator>
  std::pair<std::size_t, bool>
  ReadInfoGetPot<Generator>::readInfluenceOption()
  {
    std::size_t rounds = GPfile("InfluenceOption/rounds", 10);
    std::string complete = GPfile("InfluenceOption/Estimation/complete", "1");
    return std::make_pair(rounds, boost::lexical_cast<bool>(complete));
  }


  template<class Generator>
  bool ReadInfoGetPot<Generator>::readInfoOutputResults(){
    std::string output_results = GPfile("Output/output_results", "1");
    return boost::lexical_cast<bool>(output_results);
  }


  template<class Generator>
  bool ReadInfoGetPot<Generator>::readInfoPrintGraph(){
    std::string print_graph = GPfile("Output/print_graph", "1");
    return boost::lexical_cast<bool>(print_graph);
  }

} // end namespace ElectionManipulation

#endif // READINFORMATION_HPP
