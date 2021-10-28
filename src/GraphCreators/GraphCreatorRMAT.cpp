#include "GraphCreators/GraphCreatorRMAT.hpp"
#include <boost/graph/rmat_graph_generator.hpp>
#include <iostream>
#include <cmath>
#include <string>
#include <stdexcept>

namespace ElectionManipulation::GraphCreator{
  using Graph = EMTraits::Graph;

  void GraphCreatorRMAT::check_params_consistency()
  {
    if ( (a<0) || (a>1) || (b<0) || (b>1) ||
         (c<0) || (c>1) || (d<0) || (d>1) ||
         (fabs(a+b+c+d) - 1. > std::numeric_limits<double>::epsilon()))
    {
      std::string excep = "Wrong input parameters for a G-MAT graph\n \
                          a,b,c,d should be in [0,1] and a+b+c+d=1\n \
                          a+b+c+d = " + std::to_string(a+b+c+d) + "\n";
      throw std::invalid_argument(excep);
    }
    typedef typename Graph::directed_category Cat;
    if(boost::detail::is_directed(Cat()) && (b!=c))
    {
      std::string excep = "Wrong input parameters for a G-MAT graph\n \
                          b,c should be equal in undirected graphs\n";
      throw std::invalid_argument(excep);
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

    check_params_consistency();
  }

} // end namespace ElectionManipulation::GraphCreator
