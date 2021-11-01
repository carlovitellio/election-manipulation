#ifndef SOCIALNETWORKCREATOR_HPP
#define SOCIALNETWORKCREATOR_HPP

#include "EMTraits.hpp"
#include "GraphCreators/GCTraits.hpp"
#include "GraphCreators/GraphCreatorBase.hpp"
#include "PersonCreator.hpp"
#include "ExternalUtilities/CloningUtilities.hpp"
#include <utility>

namespace ElectionManipulation{

  using Graph = EMTraits::Graph;
  using GraphCreatorBase = GraphCreator::GraphCreatorBase;

  //! Class for creating a Graph with Persons as vertices
  /*!
      Building the structure of the graph is delegated to the polymorphic object
      GraphCreator (derived from GraphCreatorBase).
      Populating each vertex with a Person is delegated to the PersonCreator member
      that is simply aggregated as an object.
      Move semantic is also supported.
      The boolean parameter passed to the constructors set to true renames every
      Person. If they must be renamed, their index in the graph is used. If no
      name is passed to the PersonCreator object, rename is set to true anyway.
  */
  template<class Generator>
  class SocialNetworkCreator{
  public:
    using GCHandler = GraphCreator::GCHandler;
    //! First constructor taking a GraphCreatorBase object
    template<class PCreator>
    SocialNetworkCreator(const GraphCreatorBase & gc, PCreator&& pc,
                         bool rename_=false):
        graph_creator_ptr{gc.clone()}, person_creator{std::forward<PCreator>(pc)},
        rename{rename_} {}
    //! Second constructor taking a GraphCreatorBase unique pointer
    template<class GCH, class PCreator>
    SocialNetworkCreator(GCH && gc, PCreator&& pc, bool rename_=false):
        graph_creator_ptr{std::forward<GCH>(gc)},
        person_creator{std::forward<PCreator>(pc)}, rename{rename_} {}

    //! The actual method used to create the social network
    Graph apply();

  private:
    GCHandler graph_creator_ptr;              //!< Unique pointer for GraphCreatorBase
    PersonCreator<Generator> person_creator;  //!< PersonCreator object for creating a Person
    bool rename;                              //!< true if every Person is renamed, false otherwise
  };

  template<class Generator>
  Graph SocialNetworkCreator<Generator>::apply()
  {
    //! Build the graph structure
    Graph g{graph_creator_ptr->create()};

    if(!rename && person_creator.name.empty())  rename = true;
    if(!graph_creator_ptr->get_read_attributes())
    {
      //! For each vertex a Person is created
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
