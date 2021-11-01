#ifndef GRAPHCREATORINPUTFILE_HPP
#define GRAPHCREATORINPUTFILE_HPP
#include "EMTraits.hpp"
#include "GraphCreators/GraphCreatorBase.hpp"
#include <string>

namespace ElectionManipulation::GraphCreator{
  //! Utility for exploiting Tag Dispatching
  enum InputFileSwitch {GRAPHVIZ, GRAPHML};
  //! An empty struct that transform an enum into a type
  template<InputFileSwitch>
  struct InputFileType{};

  //! Class for reading a graph from an input file
  /*!
      The class enables to read a graph from a file within the interface of
      GraphCreatorBase.

      It is possible to read both graphviz and graphml formats, thanks to the
      methods available in BGL.

      In contrast with other GraphCreators, this class doesn't need a generator
      since nothing is randomly generated.

      @note To use this class, you will need to build and link against the
            "boost_graph" and "boost_regex" libraries. For more information,
            please refer to https://www.boost.org/doc/libs/1_77_0/more/getting_started/index.html#Build_Install
  */
  class GraphCreatorInputFile final: public GraphCreatorBase
  {
  public:
    //! The class is default constructable
    GraphCreatorInputFile()=default;
    //! Another constructor is provided especially for test purposes
    GraphCreatorInputFile(std::string filename_, bool read_attrib):
                  filename{filename_}, read_attributes{read_attrib} {}

    std::unique_ptr<GraphCreatorBase> clone() const override;

    std::string name() const override {return "Input-file";}

    void read_params(GetPot) override;
    //! The user decides whether read vertices' information from a file or not
    bool get_read_attributes() const override {return read_attributes;}

    Graph create() override;

  private:
    std::string filename;
    bool read_attributes;
    //! Method to read a Graph from a file with graphviz format
    Graph create(InputFileType<GRAPHVIZ>);
    //! Method to read a Graph from a file with graphvml format
    Graph create(InputFileType<GRAPHML>);
  };

} // end namespace ElectionManipulation::GraphCreator


#endif // GRAPHCREATORINPUTFILE_HPP
