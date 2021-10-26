#ifndef GRAPHCREATORPLOD_HPP
#define GRAPHCREATORPLOD_HPP

#include "EMTraits.hpp"
#include "GraphCreatorBase.hpp"

namespace ElectionManipulation::GraphCreator{


  class GraphCreatorPLOD final: public GraphCreatorBase
  {
  public:

    GraphCreatorPLOD()=default;

    GraphCreatorPLOD(RandomGenerator& gen_, Graph::vertices_size_type N_,
                     double alpha_, double beta_):
                     gen{gen_}, N{N_}, alpha{alpha_}, beta{beta_}
    {};

    std::unique_ptr<GraphCreatorBase> clone() const override;

    Graph create() override;

    void set_gen(const RandomGenerator& gen_) override {gen=gen_;}
    void read_params(GetPot) override;

    //! a string that identify the general type of Graph Creator
    std::string name() const override {return "PLOD";}

  private:
    RandomGenerator gen;
    Graph::vertices_size_type N;  //!< Number of vertices in the graph
    double alpha;
    double beta;

  };

} // end namespace ElectionManipulation::GraphCreator


#endif // GRAPHCREATORPLOD_HPP
