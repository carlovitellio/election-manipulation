#ifndef GRAPHCREATORRMAT_HPP
#define GRAPHCREATORRMAT_HPP

#include "EMTraits.hpp"
#include "GraphCreators/GraphCreatorBase.hpp"

namespace ElectionManipulation::GraphCreator{

  //! Makes available the RMAT graph generator provided by BGL
  /*!

  */

  class GraphCreatorRMAT final: public GraphCreatorBase
  {
  public:

    GraphCreatorRMAT()=default;

    GraphCreatorRMAT(RandomGenerator& gen_, Graph::vertices_size_type N_,
                     Graph::edges_size_type E_,
                     double a_, double b_, double c_, double d_):
                 gen{gen_}, N{N_}, E{E_}, a{a_}, b{b_}, c{c_}, d{d_}
                 {check_params_consistency();}

    std::unique_ptr<GraphCreatorBase> clone() const override;

    Graph create() override;

    void set_gen(const RandomGenerator& gen_) override {gen=gen_;}
    void read_params(GetPot GPFile) override;
    bool get_read_attributes() const override {return false;}

    void check_params_consistency();

    std::string name() const override {return "R-MAT";}

  private:
    RandomGenerator gen;
    Graph::vertices_size_type N; //!< Number of vertices in the graph
    Graph::edges_size_type E;    //!< Number of edges in the graph
    double a, b, c, d; /*!< a, b, c, and d represent the probability that
                                  a generated edge is placed of each of the 4
                                  quadrants of the partitioned adjacency matrix */

  };


} // end namespace ElectionManipulation::GraphCreator


#endif // GRAPHCREATORSRMAT_HPP
