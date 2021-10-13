#include "Person.hpp"
#include <iostream>

namespace ElectionManipulation{

std::ostream & operator << (std::ostream & str, Person const & p)
{
  str << "Name: " << p.name << ", "
      << "resistance: " << p.resistance << ", "
      << "probability of voting c_0: " << p.prob_voting_c0 << ".\n";

  return str;
}

} // end of namespace ElectionManipulation
