#include "GraphCreatorFactory.hpp"
#include "GraphCreatorErdosRenyi.hpp"
#include "GraphCreatorPLOD.hpp"
#include "GraphCreatorRMAT.hpp"
#include "GraphCreatorSmallWorld.hpp"

namespace ElectionManipulation::GraphCreator{

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
