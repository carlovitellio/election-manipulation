#ifndef GRAPHCREATORSMALLWORLD_HPP
#define GRAPHCREATORSMALLWORLD_HPP

#include "EMTraits.hpp"
#include "GraphCreators/GraphCreatorBase.hpp"

namespace ElectionManipulation::GraphCreator{

  //! Makes available the Small World graph generator provided by BGL
  /*!

  */

  class GraphCreatorSmallWorld final: public GraphCreatorBase
  {
  public:

    GraphCreatorSmallWorld()=default;

    GraphCreatorSmallWorld(RandomGenerator& gen_, Graph::vertices_size_type N_,
                           Graph::vertices_size_type k_, double p_):
    gen{gen_}, N{N_}, k{k_}, p{p_}
    {};

    std::unique_ptr<GraphCreatorBase> clone() const override;

    Graph create() override;

    void set_gen(const RandomGenerator& gen_) override {gen=gen_;}
    void read_params(GetPot) override;
    bool get_read_attributes() const override {return false;}

    //! a string that identify the general type of Graph Creator
    std::string name() const override {return "Small-World";}

  private:
    RandomGenerator gen;
    Graph::vertices_size_type N;  //!< Number of vertices in the graph
    Graph::vertices_size_type k;  //!< Number of neighbours connected
    double p;       //!< Probability of reconnection

  };

} // end namespace ElectionManipulation::GraphCreator


#endif // GRAPHCREATORSMALLWORLD_HPP
