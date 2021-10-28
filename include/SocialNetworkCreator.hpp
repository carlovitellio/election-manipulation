#ifndef SOCIALNETWORKCREATOR_HPP
#define SOCIALNETWORKCREATOR_HPP

#include "EMTraits.hpp"
#include "GraphCreators/GCTraits.hpp"
#include "GraphCreators/GraphCreatorBase.hpp"
#include "PersonCreator.hpp"
#include "ExternalUtilities/CloningUtilities.hpp"

namespace ElectionManipulation{

  using Graph = EMTraits::Graph;
  using GraphCreatorBase = GraphCreator::GraphCreatorBase;

  template<class RandGen>
  class SocialNetworkCreator{
  public:
    using GCHandler = GraphCreator::GCHandler;

    SocialNetworkCreator(const GraphCreatorBase & gc, const PersonCreator<RandGen> & pc,
                         bool rename_=false):
        graph_creator{gc.clone()}, person_creator{pc}, rename{rename_} {}
/*
    SocialNetworkCreator(GCHandler && gc, PersonCreator<RandGen> && pc,
                         bool rename_=false):
        graph_creator{std::forward<GraphCreatorBase>(gc)},
        person_creator{std::forward<PersonCreator>(pc)}, rename{rename_} {}
*/
    Graph apply();

  private:
    // make the class copiable/movable with a deep copy
    GCHandler graph_creator;
    PersonCreator<RandGen> person_creator;
    bool rename;
  };

  template<class RandGen>
  Graph SocialNetworkCreator<RandGen>::apply()
  {
    Graph g{graph_creator->create()};

    if(!rename && person_creator.name.empty())  rename = true;
    if(!graph_creator->get_read_attributes())
    {
      for(auto i : boost::make_iterator_range(vertices(g)))
      {
        if(rename==true)
        {
          person_creator.name = "Node" + std::to_string(i);
        }
        person_creator.apply(g[i]);
      }
    }

    return g;
  }

} // end namespace ElectionManipulation

#endif // SOCIALNETWORKCREATOR_HPP
