#include "GraphCreators/GraphCreatorFactory.hpp"
#include "GraphCreators/GraphCreatorErdosRenyi.hpp"
#include "GraphCreators/GraphCreatorPLOD.hpp"
#include "GraphCreators/GraphCreatorRMAT.hpp"
#include "GraphCreators/GraphCreatorSmallWorld.hpp"

namespace ElectionManipulation::GraphCreator{

  //! Automatic registration of the Creators as soon as the dynamic library is loaded
  namespace{
    __attribute__((constructor))
    void registerCreator()
    {
      addRuleToFactory<GraphCreatorErdosRenyi>("Erdos-Renyi");
      addRuleToFactory<GraphCreatorPLOD>("PLOD");
      addRuleToFactory<GraphCreatorRMAT>("R-MAT");
      addRuleToFactory<GraphCreatorSmallWorld>("Small-World");
    }
  }

} // end namespace ElectionManipulation::GraphCreator
