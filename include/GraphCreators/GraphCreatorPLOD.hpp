#ifndef GRAPHCREATORPLOD_HPP
#define GRAPHCREATORPLOD_HPP

#include "EMTraits.hpp"
#include "GraphCreators/GraphCreatorBase.hpp"

namespace ElectionManipulation::GraphCreator{

  //! Class for generating a scale-free graph
  /*!
      It exploits the plod_generator implemented in the Boost Graph Library
      within the common interface of GraphCreatorBase.
      The algorithm used to generate the Graph is thee Power Law Out Degree (PLOD)
      algorithm [C. Palmer and J. Steffan, Generating Network Topologies That
      Obey Power Laws, 2000].
      A scale-free graph typically has a very skewed degree distribution, where
      few vertices have a very high degree and a large number of vertices have a
      very small degree.
      The Power Law Out Degree (PLOD) algorithm generates a graph with N vertices
      by allocating a certain number of degree "credits" to each vertex, drawn
      from a power-law distribution.
      It then creates edges between vertices, deducting a credit from each
      involved vertex (in the undirected case) or the source vertex (in the
      directed case). The number of credits assigned to a vertex is
      \f$ \beta*x^{-\alpha} \f$, where x is a random value between 0 and N-1 and
      \f$ \alpha \f$ and \f$ \beta \f$ are given parameters.
      \f$ \beta \f$ controls the y-intercept of log-log plot, so that increasing
      \f$ \beta \f$ increases the average degree of vertices.
      \f$ \alpha \f$ controls how steeply the curve drops off, with larger values
      indicating a steeper curve.
      Self-loops are not allowed within this implementation.
  */
  class GraphCreatorPLOD final: public GraphCreatorBase
  {
  public:
    //! The class is default constructable
    GraphCreatorPLOD()=default;
    //! Another constructor is provided especially for test purposes
    GraphCreatorPLOD(RandomGenerator& gen_, Graph::vertices_size_type N_,
                     double alpha_, double beta_):
                     gen{gen_}, N{N_}, alpha{alpha_}, beta{beta_}
    {};

    std::unique_ptr<GraphCreatorBase> clone() const override;

    std::string name() const override {return "PLOD";}
    //! Method to set the Random Generator (source of randomness for creating the graph)
    void set_gen(const RandomGenerator& gen_) override {gen=gen_;}

    void read_params(GetPot) override;
    //! Within this GraphCreator the Graph is not read from a file and so neither each vertex attributes are read
    bool get_read_attributes() const override {return false;}

    Graph create() override;

  private:
    RandomGenerator gen;          //!< Source of randomness
    Graph::vertices_size_type N;  //!< Number of vertices in the graph
    double alpha;                 //!< Controls how steeply the curve drops off
    double beta;                  //!< Controls the y-intercept of log-log plot

  };

} // end namespace ElectionManipulation::GraphCreator


#endif // GRAPHCREATORPLOD_HPP
