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

  template<class Generator>
  class SocialNetworkCreator{
  public:
    using GCHandler = GraphCreator::GCHandler;
    using PCHandler = apsc::PointerWrapper<PersonCreator<Generator>>;

    SocialNetworkCreator(const GraphCreatorBase & gc, const PersonCreator<Generator>& pc,
                         bool rename_=false):
        graph_creator{gc.clone()}, person_creator{pc.clone()}, rename{rename_} {}

    template<class GCH, class PCreator>
    SocialNetworkCreator(GCH && gc, PCreator&& pc, bool rename_=false):
        graph_creator{std::forward<GCH>(gc)},
        person_creator{std::forward<PCreator>(pc)}, rename{rename_} {}

    Graph apply();

  private:
    GCHandler graph_creator;
    PCHandler person_creator;
    bool rename;
  };

  template<class Generator>
  Graph SocialNetworkCreator<Generator>::apply()
  {
    Graph g{graph_creator->create()};

    if(!rename && person_creator->name.empty())  rename = true;
    if(!graph_creator->get_read_attributes())
    {
      for(auto i : boost::make_iterator_range(vertices(g)))
      {
        if(rename==true)
        {
          person_creator->name = "Node" + std::to_string(i);
        }
        person_creator->apply(g[i]);
      }
    }

    return g;
  }

} // end namespace ElectionManipulation

#endif // SOCIALNETWORKCREATOR_HPP
