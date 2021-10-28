#include "GraphCreators/GraphCreatorFactory.hpp"

namespace ElectionManipulation::GraphCreator{
  //! This is the only instance of the Factory
  GraphCreatorFactory& MyFactory = GraphCreatorFactory::Factory::Instance();

} // end namespace ElectionManipulation::GraphCreator
