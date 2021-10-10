#include "Person.hpp"
#include <iostream>

namespace ElectionManipulation{

std::ostream & operator << (std::ostream & str, Person const & p)
{
  str << "Name: " << p.name << ", ";
  str << "resistance: " << p.resistance << ", ";
  str << "probability of voting c_0: " << p.prob_voting_c0 << ".";

  return str;
}

} // end of namespace ElectionManipulation
