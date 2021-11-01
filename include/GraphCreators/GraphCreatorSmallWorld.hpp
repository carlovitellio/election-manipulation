#ifndef GRAPHCREATORSMALLWORLD_HPP
#define GRAPHCREATORSMALLWORLD_HPP

#include "EMTraits.hpp"
#include "GraphCreators/GraphCreatorBase.hpp"

namespace ElectionManipulation::GraphCreator{

  //! Class for generating a Small World graph
  /*!
      It exploits the small_world_generator implemented in the Boost Graph Library
      within the common interface of GraphCreatorBase.
      A small-world graph consists of a ring graph (i.e. each vertex is connected
      to its k nearest neighbours). Edges in the graph are randomly rewired to
      different vertices with a probability p.
  */
  class GraphCreatorSmallWorld final: public GraphCreatorBase
  {
  public:
    //! The class is default constructable
    GraphCreatorSmallWorld()=default;
    //! Another constructor is provided especially for test purposes
    GraphCreatorSmallWorld(RandomGenerator& gen_, Graph::vertices_size_type N_,
                           Graph::vertices_size_type k_, double p_):
    gen{gen_}, N{N_}, k{k_}, p{p_}
    {};

    std::unique_ptr<GraphCreatorBase> clone() const override;

    std::string name() const override {return "Small-World";}
    //! Method to set the Random Generator (source of randomness for creating the graph)
    void set_gen(const RandomGenerator& gen_) override {gen=gen_;}

    void read_params(GetPot) override;
    //! Within this GraphCreator the Graph is not read from a file and so neither each vertex attributes are read
    bool get_read_attributes() const override {return false;}

    Graph create() override;

  private:
    RandomGenerator gen;          //!< Source of randomness for rewiring
    Graph::vertices_size_type N;  //!< Number of vertices in the graph
    Graph::vertices_size_type k;  //!< Number of neighbours connected
    double p;                     //!< Probability of rewiring

  };

} // end namespace ElectionManipulation::GraphCreator


#endif // GRAPHCREATORSMALLWORLD_HPP
