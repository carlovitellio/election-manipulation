#ifndef GRAPHCREATORS_HPP
#define GRAPHCREATORS_HPP

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/small_world_generator.hpp>
#include <boost/graph/rmat_graph_generator.hpp>
#include <memory>
#include <functional>
#include <string>
#include <cmath>


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

    GraphCreatorSmallWorld(RandomGenerator& gen_, unsigned int N_,
                           unsigned int k_, double p_):
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

    //! a string that identify the general type of Graph Creator
    std::string name() const override {return "Small World";}

  private:
    RandomGenerator& gen;
    const unsigned int N; //!< Number of vertices in the graph
    const unsigned int k; //!< Number of neighbours connected
    const double p;       //!< Probability of reconnection

  };




  //! Makes available the RMAT graph generator provided by BGL
  /*!

  */

  template< class RandomGenerator,
            class Graph>
  class GraphCreatorRMAT final: public GraphCreatorBase<RandomGenerator, Graph>
  {
  public:
    using GCBase = GraphCreatorBase<RandomGenerator, Graph>;

    GraphCreatorRMAT(RandomGenerator& gen_, unsigned int N_, unsigned int E_,
                     double a_, double b_, double c_, double d_):
    gen{gen_}, N{N_}, E{E_}, a{a_}, b{b_}, c{c_}, d{d_}
    {
      if ( (a<0) || (a>1) || (b<0) || (b>1) ||
           (c<0) || (c>1) || (d<0) || (d>1) ||
           (fabs(a+b+c+d) - 1. > std::numeric_limits<double>::epsilon()))
      {
        std::cerr << "Wrong input parameters for a G-MAT graph" << std::endl
                  << "a,b,c,d should be in [0,1] and a+b+c+d=1" << std::endl
                  << "a+b+c+d = " << a+b+c+d << std::endl;
        std::exit(1);
      }
      typedef typename Graph::directed_category Cat;
      if(boost::detail::is_directed(Cat()) && (b!=c))
      {
        std::cerr << "Wrong input parameters for a G-MAT graph" << std::endl
                  << "b,c should be equal in undirected graphs" << std::endl;
        std::exit(1);
      }

    };

    std::unique_ptr<GCBase> clone() const override
    {
      return std::unique_ptr<GCBase>(new GraphCreatorRMAT(*this));
    }


    Graph create() override
    {
      using RMATGen = boost::rmat_iterator<RandomGenerator, Graph>;
      Graph g(RMATGen(gen, N, E, a, b, c, d), RMATGen(), N);

      return g;
    }

    //! a string that identify the general type of Graph Creator
    std::string name() const override {return "R-MAT";}

  private:
    RandomGenerator& gen;
    const unsigned int N; //!< Number of vertices in the graph
    const unsigned int E; //!< Number of edges in the graph
    const double a, b, c, d; /*!< a, b, c, and d represent the probability that
                                  a generated edge is placed of each of the 4
                                  quadrants of the partitioned adjacency matrix */

  };


  //! Makes available the Erdos Rényi graph generator using the R-MAT generator
  /*!

  */

  template< class RandomGenerator,
            class Graph>
  class GraphCreatorErdosRenyi final: public GraphCreatorBase<RandomGenerator, Graph>
  {
  public:
    using GCBase = GraphCreatorBase<RandomGenerator, Graph>;

    GraphCreatorErdosRenyi(RandomGenerator& gen_, unsigned int N_, unsigned int E_):
                           gen{gen_}, N{N_}, E{E_}
    {};

    std::unique_ptr<GCBase> clone() const override
    {
      return std::unique_ptr<GCBase>(new GraphCreatorErdosRenyi(*this));
    }


    Graph create() override
    {
      GraphCreatorRMAT<RandomGenerator, Graph> gc(gen, N, E, 0.25, 0.25, 0.25, 0.25);

      return gc.create();
    }

    //! a string that identify the general type of Graph Creator
    std::string name() const override {return "Erdos-Rényi";}

  private:
    RandomGenerator& gen;
    const unsigned int N; //!< Number of vertices in the graph
    const unsigned int E; //!< Number of edges in the graph

  };

} // end namespace ElectionManipulation::GraphCreator

#endif // GRAPHCREATOR_HPP
