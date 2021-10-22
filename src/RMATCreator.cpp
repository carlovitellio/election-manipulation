#include "GraphCreatorFactory.hpp"
#include "GraphCreatorRMAT.hpp"
#include "GraphCreatorErdosRenyi.hpp"

namespace ElectionManipulation::GraphCreator{

  namespace{
    __attribute__((constructor))
    void registerCreator()
    {
      addRuleToFactory<GraphCreatorRMAT>("R-MAT");
      addRuleToFactory<GraphCreatorErdosRenyi>("Erdos-Renyi");
    }
  }

} // end namespace ElectionManipulation::GraphCreator
