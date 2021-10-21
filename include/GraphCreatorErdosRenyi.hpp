#ifndef GRAPHCREATORERDOSRENYI_HPP
#define GRAPHCREATORERDOSRENYI_HPP

#include "GraphCreatorBase.hpp"
#include "GraphCreatorRMAT.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/rmat_graph_generator.hpp>

namespace ElectionManipulation::GraphCreator{

  //! Makes available the Erdos Rényi graph generator using the R-MAT generator
  /*!

  */

  template< class RandomGenerator,
            class Graph>
  class GraphCreatorErdosRenyi final: public GraphCreatorBase<RandomGenerator, Graph>
  {
  public:
    using GCBase = GraphCreatorBase<RandomGenerator, Graph>;

    GraphCreatorErdosRenyi()=default;

    GraphCreatorErdosRenyi(RandomGenerator& gen_, std::size_t N_, std::size_t E_):
                           gen{gen_}, N{N_}, E{E_}
    {};

    std::unique_ptr<GCBase> clone() const override
    { return std::unique_ptr<GCBase>(new GraphCreatorErdosRenyi(*this));}


    Graph create() override
    { GraphCreatorRMAT<RandomGenerator, Graph> gc(gen, N, E, 0.25, 0.25, 0.25, 0.25);
      return gc.create();
    }

    void set_gen(const RandomGenerator& gen_) override {gen=gen_;}
    void read_params(GetPot GPFile) override;

    //! a string that identify the general type of Graph Creator
    std::string name() const override {return "Erdos-Rényi";}

  private:
    RandomGenerator gen;
    std::size_t N; //!< Number of vertices in the graph
    std::size_t E; //!< Number of edges in the graph

  };

  template<class RG, class G>
  void GraphCreatorErdosRenyi<RG, G>::read_params(GetPot GPfile)
  {
    N = GPfile("Graph_option/N", 100);
    E = GPfile("Graph_option/E", 200);
  }

} // end namespace ElectionManipulation::GraphCreator


#endif // GRAPHCREATORERDOSRENYI_HPP
