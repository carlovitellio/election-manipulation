#include "GraphCreatorRMAT.hpp"
#include <boost/graph/rmat_graph_generator.hpp>
#include <iostream>
#include <cmath>

namespace ElectionManipulation::GraphCreator{
  using Graph = EMTraits::Graph;

  GraphCreatorRMAT::GraphCreatorRMAT(
                  RandomGenerator& gen_, Graph::vertices_size_type N_,
                  Graph::edges_size_type E_,
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
  }

  std::unique_ptr<GraphCreatorBase> GraphCreatorRMAT::clone() const
  {
    return std::unique_ptr<GraphCreatorBase>(new GraphCreatorRMAT(*this));
  }

  Graph GraphCreatorRMAT::create()
  {
    using RMATGen = boost::rmat_iterator<RandomGenerator, Graph>;
    std::clog << "Implementig a R-MAT graph with " << N << " vertices and " << E
              << " edges. \nProbability of each edge to be placed in each quadrants are "
              << a << " " << b << " " << c << " " << d << std::endl;
    Graph g(RMATGen(gen, N, E, a, b, c, d), RMATGen(), N);

    return g;
  }

  void GraphCreatorRMAT::read_params(GetPot GPfile)
  {
    N = GPfile("Graph_option/N", 100);
    E = GPfile("Graph_option/E", 200);

    a = GPfile("Graph_option/R-MAT/a", 0.25);
    b = GPfile("Graph_option/R-MAT/b", 0.25);
    c = GPfile("Graph_option/R-MAT/c", 0.25);
    d = GPfile("Graph_option/R-MAT/d", 0.25);
  }

} // end namespace ElectionManipulation::GraphCreator
