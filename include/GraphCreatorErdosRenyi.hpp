#ifndef GRAPHCREATORERDOSRENYI_HPP
#define GRAPHCREATORERDOSRENYI_HPP

#include "EMTraits.hpp"
#include "GraphCreatorBase.hpp"

namespace ElectionManipulation::GraphCreator{

  //! Makes available the Erdos Rényi graph generator using the R-MAT generator
  /*!

  */


  class GraphCreatorErdosRenyi final: public GraphCreatorBase
  {
  public:

    GraphCreatorErdosRenyi()=default;

    GraphCreatorErdosRenyi(RandomGenerator& gen_, std::size_t N_, std::size_t E_):
                           gen{gen_}, N{N_}, E{E_}
    {};

    std::unique_ptr<GraphCreatorBase> clone() const override;


    Graph create() override;

    void set_gen(const RandomGenerator& gen_) override {gen=gen_;}
    void read_params(GetPot GPFile) override;

    //! a string that identify the general type of Graph Creator
    std::string name() const override {return "Erdos-Rényi";}

  private:
    RandomGenerator gen;
    std::size_t N; //!< Number of vertices in the graph
    std::size_t E; //!< Number of edges in the graph

  };

} // end namespace ElectionManipulation::GraphCreator


#endif // GRAPHCREATORERDOSRENYI_HPP
