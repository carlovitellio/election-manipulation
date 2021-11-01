#ifndef GCTRAITS_HPP
#define GCTRAITS_HPP

#include "ExternalUtilities/CloningUtilities.hpp"
#include "GraphCreators/GraphCreatorBase.hpp"

namespace ElectionManipulation::GraphCreator{
  //! Alias for a generic wrapper around a unique pointee useful to support
  //! the bridge pattern implemented by Prof. Luca Formaggia
  using GCHandler = apsc::PointerWrapper<GraphCreatorBase>;
} // end namespace ElectionManipulation::GraphCreator

#endif // GCTRAITS_HPP
