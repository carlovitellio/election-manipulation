#ifndef GRAPHCREATORSMALLWORLD_HPP
#define GRAPHCREATORSMALLWORLD_HPP

#include "GraphCreatorBase.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/small_world_generator.hpp>

namespace ElectionManipulation::GraphCreator{

  //! Makes available the Small World graph generator provided by BGL
  /*!

  */

  template< class RandomGenerator,
            class Graph>
  class GraphCreatorSmallWorld final: public GraphCreatorBase<RandomGenerator, Graph>
  {
  public:
    using GCBase = GraphCreatorBase<RandomGenerator, Graph>;

    GraphCreatorSmallWorld()=default;

    GraphCreatorSmallWorld(RandomGenerator& gen_, std::size_t N_,
                           std::size_t k_, double p_):
    gen{gen_}, N{N_}, k{k_}, p{p_}
    {};


    std::unique_ptr<GCBase> clone() const override
    {
      // return std::unique_ptr<GCBase>(*this);
      return std::unique_ptr<GCBase>(new GraphCreatorSmallWorld(*this));
    }


    Graph create() override
    {
      using SWGen = boost::small_world_iterator<RandomGenerator, Graph>;
      Graph g(SWGen(gen, N, k, p), SWGen(), N);

      return g;
    }

    void set_gen(const RandomGenerator& gen_) override {gen=gen_;}
    void read_params(GetPot) override;

    //! a string that identify the general type of Graph Creator
    std::string name() const override {return "Small World";}

  private:
    RandomGenerator gen;
    std::size_t N;  //!< Number of vertices in the graph
    std::size_t k;  //!< Number of neighbours connected
    double p;       //!< Probability of reconnection

  };

  template< class RG, class G>
  void GraphCreatorSmallWorld<RG, G>::read_params(GetPot GPfile)
  {
    N = GPfile("Graph_option/N", 100);
    k = GPfile("Graph_option/small_world_generator/k", 4);
    p = GPfile("Graph_option/small_world_generator/p", 0.05);
  }

} // end namespace ElectionManipulation::GraphCreator


#endif // GRAPHCREATORSMALLWORLD_HPP
