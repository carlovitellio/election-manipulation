#include <iostream>
#include <chrono>
#include "ExternalUtilities/GetPot"
#include "EMUtilities.hpp"
#include "EMTraits.hpp"
#include "GraphCreators/GCTraits.hpp"
#include "Distributions/DistributionsTraits.hpp"
#include "ReadInformation.hpp"
#include "PersonCreator.hpp"
#include "SocialNetworkCreator.hpp"
#include "ManipulatorInfluence.hpp"
#include "PerformanceEvaluator.hpp"

int main(int argc, char** argv)
{
  using namespace ElectionManipulation;
  using namespace ElectionManipulation::EMTraits;

  GetPot cl(argc,argv);

  std::random_device rd;
  ElectionManipulation::EMTraits::RandomGenerator gen{rd()};

  //! Helper class used to handle the input parameters
  ReadInfoGetPot reader(cl, gen);
  //! Check whether it is asked the help of the executable
  reader.check_help();
  //! Look for the file to be read
  reader.read_input_file();

  // ---------------------------------------------------
  // ------------- SOCIAL NETWORK CREATION -------------
  // ---------------------------------------------------

  using GCHandler = GraphCreator::GCHandler; // alias to apsc::PointerWrapper<GraphCreatorBase>;

  //! The graph type is deduced run-time reading from an input file
  //! Read from input file which GraphCreator to be used and retrieve it from the factory
  GCHandler gc_ptr = reader.instantiateGraphCreator();


  using ResHandler = Distributions::ResDistHandler<RandomGenerator> ;
  using VotHandler = Distributions::VotDistHandler<RandomGenerator> ;

  //! Deduce the probability distributions to be used for generating each Person
  //! if requested from the chosen GraphCreator
  ResHandler resistance_distr_ptr = reader.readInfoResistanceDist();
  VotHandler voting_distr_ptr = reader.readInfoVotingDist();
  //! Class used to generate each Person, if requested from the chosen GraphCreator
  PersonCreator<RandomGenerator> pc(gen, resistance_distr_ptr, voting_distr_ptr);
  //! Class that unifies all the concepts defined above and that enables to
  //! create a proper Social Network
  SocialNetworkCreator<RandomGenerator> snc(gc_ptr, pc);

  Graph my_graph{snc.apply()};


  // -------------------------------------------------
  // ------------- ELECTION MANIPULATION -------------
  // -------------------------------------------------

  //! rounds represents how many times to influence the network
  auto [rounds, complete] = reader.readInfluenceOption();

  //! Class where the performance metrics are implemented
  PerformanceEvaluator pe(my_graph);

  //! Class where the Manipulator defines her methods
  ManipulatorInfluence mi(my_graph, complete);

  std::string name = "../out/results_" + gc_ptr->name() + "_v" + std::to_string(num_vertices(my_graph))
              + "_e" + std::to_string(num_edges(my_graph)) + "_compl" + std::to_string(complete) + ".dat";

  //! Checks if it is requested to output the performance metrics in a file
  bool output_results = reader.readInfoOutputResults();
  std::size_t parziale = rounds/20;
  int j{0};

  auto t1 = std::chrono::steady_clock::now();

  if(output_results) {
    std::ofstream file (name);
    file << "Round prob_MSE expected_votes tot_sol" << '\n';
    for(std::size_t i=0; i<=rounds; i++)
    {
      pe.compute();
      file << i << " " << pe.metrics() << '\n';
      mi.influence();
      if(i%parziale==0){
        std::clog << j++*5 << "%" << std::endl;
      }
    }
    file.close();
  } else {
    for(std::size_t i=0; i<=rounds; i++)
    {
      mi.influence();
    }
  }

  auto t2 = std::chrono::steady_clock::now();

  std::clog << "Elapsed time for influence: " << std::chrono::duration<double>(t2-t1).count() << std::endl;
  //! Checks if it is requested to output the graph in a file
  if(reader.readInfoPrintGraph()) output_graph(my_graph);


  return 0;

} // end main
