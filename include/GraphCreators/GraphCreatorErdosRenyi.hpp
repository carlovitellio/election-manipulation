#ifndef GRAPHCREATORERDOSRENYI_HPP
#define GRAPHCREATORERDOSRENYI_HPP

#include "EMTraits.hpp"
#include "GraphCreators/GraphCreatorBase.hpp"

namespace ElectionManipulation::GraphCreator{

  //! Class for generating a Erdös-Rényi graph
  /*!
      It exploits the erdos_renyi_generator implemented in the Boost Graph Library
      within the common interface of GraphCreatorBase.
      A Erdös-Rényi graph is a random graph chosen uniformly at random from the
      collection of all graphs with N vertices and E edges.
      Erdös-Renyi graphs typically exhibit very little structure.
      No self-loops is allowed within this implementation.
  */
class GraphCreatorErdosRenyi final: public GraphCreatorBase
  {
  public:
    //! The class is default constructable
    GraphCreatorErdosRenyi()=default;
    //! Another constructor is provided especially for test purposes
    GraphCreatorErdosRenyi(RandomGenerator& gen_, Graph::vertices_size_type N_,
                           Graph::edges_size_type E_):
                           gen{gen_}, N{N_}, E{E_} {};

    std::unique_ptr<GraphCreatorBase> clone() const override;

    std::string name() const override {return "Erdos-Rényi";}
    //! Method to set the Random Generator (source of randomness for creating the graph)
    void set_gen(const RandomGenerator& gen_) override {gen=gen_;}

    void read_params(GetPot GPFile) override;
    //! Within this GraphCreator the Graph is not read from a file and so neither each vertex attributes are read
    bool get_read_attributes() const override {return false;}

    Graph create() override;

  private:
    RandomGenerator gen;         //!< Source of randomness
    Graph::vertices_size_type N; //!< Number of vertices in the graph
    Graph::edges_size_type E;    //!< Number of edges in the graph

  };

} // end namespace ElectionManipulation::GraphCreator


#endif // GRAPHCREATORERDOSRENYI_HPP
