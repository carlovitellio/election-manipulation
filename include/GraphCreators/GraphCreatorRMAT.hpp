#ifndef GRAPHCREATORRMAT_HPP
#define GRAPHCREATORRMAT_HPP

#include "EMTraits.hpp"
#include "GraphCreators/GraphCreatorBase.hpp"

namespace ElectionManipulation::GraphCreator{

  //! Class for generating a R-MAT graph
  /*!
      It exploits the rmat_graph_generator implemented in the Boost Graph Library
      within the common interface of GraphCreatorBase.
      An R-MAT graph has a scale-free distribution w.r.t. vertex degree and is
      implemented using Recursive-MATrix partitioning.
      A R-MAT graph has N vertices and E edges.
      a, b, c, and d represent the probability that a generated edge is placed
      of each of the 4 quadrants of the partitioned adjacency matrix.
      The parameters are from a GetPot file and eventually their consistency is
      checked.
  */
  class GraphCreatorRMAT final: public GraphCreatorBase
  {
  public:
    //! The class is default constructable
    GraphCreatorRMAT()=default;
    //! Another constructor is provided especially for test purposes
    GraphCreatorRMAT(RandomGenerator& gen_, Graph::vertices_size_type N_,
                     Graph::edges_size_type E_,
                     double a_, double b_, double c_, double d_):
                 gen{gen_}, N{N_}, E{E_}, a{a_}, b{b_}, c{c_}, d{d_}
                 {check_params_consistency();}

    std::unique_ptr<GraphCreatorBase> clone() const override;

    std::string name() const override {return "R-MAT";}
    //! Method to set the Random Generator (source of randomness for creating the graph)
    void set_gen(const RandomGenerator& gen_) override {gen=gen_;}

    void read_params(GetPot GPFile) override;
    //! Within this GraphCreator the Graph is not read from a file and so neither each vertex attributes are read
    bool get_read_attributes() const override {return false;}
    //! In a R-MAT graph the parameters a, b, c and d have to satisfy some condition
    void check_params_consistency();

    Graph create() override;

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
