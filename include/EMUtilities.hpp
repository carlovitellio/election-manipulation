#ifndef EMUTILITIES_HPP
#define EMUTILITIES_HPP

#include <iostream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-override"
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#include <boost/graph/graphviz.hpp>
#pragma GCC diagnostic pop

namespace ElectionManipulation{

  //! Helper function to print the instructions for the use
  void printHelp(){

  }

  //! Helper function to print the methods registered in the factory
  template <class Factory>
  void printRegistered(Factory const & MyFactory)
  {
    auto registered_vec = MyFactory.registered();
    if (registered_vec.empty())
    {
      std::cout << "There are none registered methods" << std::endl;
      std::cout << "Try to load a plug-in library pointing it in the input file" << std::endl;
    } else {
      std::cout << "The following graph creator methods are registered" << std::endl;

      for (auto i : registered_vec)
      std::cout << i << std::endl;
    }
  }

  template<class Graph> void output_graph(Graph& my_graph)
  {
    boost::dynamic_properties dp = create_dynamicProperties(my_graph);

    std::ofstream dotfile;
    dotfile.open("../out/graph_w_property.dot");
    write_graphviz_dp(dotfile, my_graph, dp);
    dotfile.close();
  }


} // end namespace ElectionManipulation

#endif // EMUTILITIES_HPP
