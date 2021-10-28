#ifndef GCTRAITS_HPP
#define GCTRAITS_HPP

#include "ExternalUtilities/CloningUtilities.hpp"
#include "GraphCreators/GraphCreatorBase.hpp"

namespace ElectionManipulation::GraphCreator{

  using GCHandler = apsc::PointerWrapper<GraphCreatorBase>;
} // end namespace ElectionManipulation::GraphCreator

#endif // GCTRAITS_HPP
