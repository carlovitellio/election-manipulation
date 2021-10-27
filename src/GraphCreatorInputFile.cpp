#include "GraphCreatorInputFile.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-override"
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#include <libs/graph/src/read_graphviz_new.cpp>
#include <libs/graph/src/graphml.cpp>
#pragma GCC diagnostic pop
#include "Person.hpp"
#include "GraphCreatorFactory.hpp"

namespace ElectionManipulation::GraphCreator{
  using Graph = EMTraits::Graph;

  std::unique_ptr<GraphCreatorBase> GraphCreatorInputFile::clone() const
  {
    return std::unique_ptr<GraphCreatorBase>(new GraphCreatorInputFile(*this));
  }

  Graph GraphCreatorInputFile::create()
  {
    std::string extension = filename.substr(filename.find_last_of(".") + 1);

    std::clog << "Reading the graph stored in " << filename << std::endl;

    if((extension == "dot") || (extension == "gv")) {
      return create(InputFileType<GRAPHVIZ>());
    } else if((extension == "graphml") || (extension == "xml")) {
      return create(InputFileType<GRAPHML>());
    } else {
      std::cerr << "Input file must be stored in file with one of the following extensions"
                << "\n\t.dot"
                << "\n\t.gv"
                << "\n\t.graphml"
                << "\n\t.xml" << std::endl;
      std::exit(1);
    }
  }

  Graph GraphCreatorInputFile::create(InputFileType<GRAPHVIZ>)
  {
    std::ifstream dot(filename);
    //! Constructs an empty graph
    Graph g(0);

    boost::dynamic_properties dp = create_dynamicProperties_reading(g);

    read_graphviz(dot, g, dp);
    return g;
  }

  Graph GraphCreatorInputFile::create(InputFileType<GRAPHML>)
  {
    std::ifstream xml(filename);
    //! Constructs an empty graph
    Graph g(0);
    boost::dynamic_properties dp = create_dynamicProperties_reading(g);

    read_graphml(xml, g, dp);
    return g;
  }

  void GraphCreatorInputFile::read_params(GetPot GPfile)
  {
    filename = GPfile("Graph_option/Input_graph", "");
    if(filename="")
    {
      std::cerr << "Specify an input graph to be read" << std::endl;
      std::exit(1);
    }
  }

  namespace{
    __attribute__((constructor))
    void registerCreator()
    {
      addRuleToFactory<GraphCreatorInputFile>("Input-file");
    }
  }

} // end namespace ElectionManipulation::GraphCreator
