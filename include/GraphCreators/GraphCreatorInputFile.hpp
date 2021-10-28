#ifndef GRAPHCREATORINPUTFILE_HPP
#define GRAPHCREATORINPUTFILE_HPP
#include "EMTraits.hpp"
#include "GraphCreators/GraphCreatorBase.hpp"
#include <string>

namespace ElectionManipulation::GraphCreator{

  enum InputFileSwitch {GRAPHVIZ, GRAPHML};

  template<InputFileSwitch>
  struct InputFileType{};
  //!
  /*!

  */

  class GraphCreatorInputFile final: public GraphCreatorBase
  {
  public:
    GraphCreatorInputFile()=default;

    GraphCreatorInputFile(std::string filename_, bool read_attrib):
                  filename{filename_}, read_attributes{read_attrib} {}

    std::unique_ptr<GraphCreatorBase> clone() const override;

    Graph create() override;

    void read_params(GetPot) override;

    std::string name() const override {return "Input-file";}

    bool get_read_attributes() const override {return read_attributes;}

  private:
    std::string filename;
    bool read_attributes;
    Graph create(InputFileType<GRAPHVIZ>);
    Graph create(InputFileType<GRAPHML>);
  };

} // end namespace ElectionManipulation::GraphCreator


#endif // GRAPHCREATORINPUTFILE_HPP
