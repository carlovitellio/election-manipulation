#ifndef GRAPHCREATORINPUTFILE_HPP
#define GRAPHCREATORINPUTFILE_HPP
#include "EMTraits.hpp"
#include "GraphCreatorBase.hpp"
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

    GraphCreatorInputFile(std::string filename_): filename{filename_} {}

    std::unique_ptr<GraphCreatorBase> clone() const override;

    Graph create() override;

    void read_params(GetPot) override;

    //! a string that identify the general type of Graph Creator
    std::string name() const override {return "Input-file";}

  private:
    std::string filename;

    Graph create(InputFileType<GRAPHVIZ>);
    Graph create(InputFileType<GRAPHML>);
  };

} // end namespace ElectionManipulation::GraphCreator


#endif // GRAPHCREATORINPUTFILE_HPP
