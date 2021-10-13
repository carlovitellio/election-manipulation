#ifndef SOCIALNETWORKCREATOR_HPP
#define SOCIALNETWORKCREATOR_HPP

#include "PersonCreator.hpp"
#include "GraphCreatorBase.hpp"
#include "CloningUtilities.hpp"

namespace ElectionManipulation{

  template<class Graph, class GraphCreatorBase, class PersonCreator>
  class SocialNetworkCreator{
  public:

    SocialNetworkCreator(const GraphCreatorBase & gc, const PersonCreator & pc, bool rename_=false):
        graph_creator{gc}, person_creator{pc}, rename{rename_} {}

    SocialNetworkCreator(GraphCreatorBase && gc, PersonCreator && pc):
        graph_creator{std::forward<GraphCreatorBase>(gc)},
        person_creator{std::forward<PersonCreator>(pc)} {}

    Graph apply()
    {
      Graph g{graph_creator->create()};

      if(!rename && person_creator.name.empty())  rename = true;
      for(auto i : boost::make_iterator_range(vertices(g)))
      {
        if(rename==true)
        {
          person_creator.name = "Node" + std::to_string(i);
        }
        person_creator.apply(g[i]);
      }

      return g;
    }

  private:
    // make the class copiable/movable with a deep copy
    apsc::PointerWrapper<GraphCreatorBase> graph_creator;
    PersonCreator person_creator;
    bool rename;
  };

} // end namespace ElectionManipulation

#endif // SOCIALNETWORKCREATOR_HPP
