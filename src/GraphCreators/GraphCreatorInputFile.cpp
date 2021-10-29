#include "GraphCreators/GraphCreatorInputFile.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-override"
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#include <libs/graph/src/read_graphviz_new.cpp>
#include <libs/graph/src/graphml.cpp>
#pragma GCC diagnostic pop
#include "Person.hpp"
#include "GraphCreators/GraphCreatorFactory.hpp"
#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include <filesystem>


namespace ElectionManipulation::GraphCreator{
  using Graph = EMTraits::Graph;

  std::unique_ptr<GraphCreatorBase> GraphCreatorInputFile::clone() const
  {
    return std::unique_ptr<GraphCreatorBase>(new GraphCreatorInputFile(*this));
  }

  Graph GraphCreatorInputFile::create()
  {
    std::filesystem::path filepath(filename);
    if (!std::filesystem::exists(filepath))
      {
        std::cerr << "Input file " << filename << " does not exists\n";
        std::exit(1);
      }

    std::string extension = filename.substr(filename.find_last_of(".") + 1);

    std::clog << "Reading the graph stored in " << filename << std::endl;

    if((extension == "dot") || (extension == "gv")) {
      return create(InputFileType<GRAPHVIZ>());
    } else if((extension == "graphml") || (extension == "xml")) {
      return create(InputFileType<GRAPHML>());
    } else {
      std::string excep = "Input file must be stored in file with one of the \
                          following extensions \n\t.dot \n\t.gv \n\t.graphml \
                          \n\t.xml\n";
      throw std::invalid_argument(excep);
    }
  }

  Graph GraphCreatorInputFile::create(InputFileType<GRAPHVIZ>)
  {
    std::ifstream dot(filename);
    // Constructs an empty graph
    Graph g(0);
    boost::dynamic_properties dp{create_dynamicProperties(g, read_attributes)};

    read_graphviz(dot, g, dp);
    return g;
  }

  Graph GraphCreatorInputFile::create(InputFileType<GRAPHML>)
  {
    std::ifstream xml(filename);
    // Construct an empty graph
    Graph g(0);
    // boost::read_graphml is not able to cast properly the attributes in the graph
    read_attributes = false;
    boost::dynamic_properties dp{create_dynamicProperties(g, read_attributes)};

    read_graphml(xml, g, dp);
    return g;
  }

  void GraphCreatorInputFile::read_params(GetPot GPfile)
  {
    filename = GPfile("Graph_option/Input_graph", "");
    if(filename=="")
    {
      std::string excep = "Specify an input graph to be read\n";
      throw std::invalid_argument(excep);
    }
    std::string read_attrib = GPfile("Graph_option/Input_graph/Read_attributes", "1");
    read_attributes = boost::lexical_cast<bool>(read_attrib);
  }

  namespace{
    __attribute__((constructor))
    void registerCreator()
    {
      addRuleToFactory<GraphCreatorInputFile>("Input-file");
    }
  }

} // end namespace ElectionManipulation::GraphCreator
