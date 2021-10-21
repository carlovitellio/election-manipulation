#ifndef LOADFACTORY_HPP
#define LOADFACTORY_HPP
#include "ElectionManipulationTraits.hpp"

namespace ElectionManipulation{

  template<class Graph>
  void loadFactory()
  {
    using GCSmallWorld = typename EMTraits<Graph>::GCSmallWorld;
    using GCRMAT = typename EMTraits<Graph>::GCRMAT;
    using GCErdosRenyi = typename EMTraits<Graph>::GCErdosRenyi;
    using Builder = typename EMTraits<Graph>::Builder;
    using GraphFactory = typename EMTraits<Graph>::GraphFactory;

    GraphFactory & MyFactory = GraphFactory::Factory::Instance();

    Builder build1=[]{return std::make_unique<GCSmallWorld>();};
    Builder build2=[]{return std::make_unique<GCRMAT>();};
    Builder build3=[]{return std::make_unique<GCErdosRenyi>();};

    MyFactory.add("Small-World", build1);
    MyFactory.add("R-MAT", build2);
    MyFactory.add("Erdos-Redyi", build3);

  }

  //! Helper function to print the registered graph creator methods
  template <class Factory>
  void printRegistered(Factory const & MyFactory)
  {
    auto registered_list = MyFactory.registered();
    std::cout << "The following graph creator methods are registered" << std::endl;

    for (auto i : registered_list)
      std::cout << i << std::endl;
  }

} // end namespace ElectionManipulation


#endif // LOADFACTORY_HPP
