#include <iostream>

//! Helper function to print the methods registered in the factory
template <class Factory>
void printRegistered(Factory const & MyFactory)
{
  auto registered_list = MyFactory.registered();
  std::cout << "The following graph creator methods are registered" << std::endl;

  for (auto i : registered_list)
    std::cout << i << std::endl;
}
