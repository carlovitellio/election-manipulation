#ifndef GRAPHCREATORFACTORY_HPP
#define GRAPHCREATORFACTORY_HPP

#include <string>
#include <iostream>
#include "ExternalUtilities/Factory.hpp"
#include "GraphCreators/GCTraits.hpp"

namespace ElectionManipulation::GraphCreator{

  using Builder = std::function<std::unique_ptr<GraphCreatorBase>()>;
  using GraphCreatorFactory = GenericFactory::Factory<GraphCreatorBase, std::string, Builder>;

  //! The Factory is implemented as a Singleton
  extern GraphCreatorFactory& MyFactory;


  template <typename ConcreteCreator>
  void addRuleToFactory(std::string const & name)
  {
    // get the factory
    GraphCreatorFactory & factory=MyFactory;
    factory.add(
        name,
        []()
        {return std::unique_ptr<GraphCreatorBase>{std::make_unique<ConcreteCreator>()};
      }
    );
    std::clog << "Added " << name << " graph creator to factory" << std::endl;
  }
} // end namespace ElectionManipulation::GraphCreator

#endif // GRAPHCREATORFACTORY_HPP
