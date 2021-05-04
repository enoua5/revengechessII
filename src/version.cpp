#include "version.h"
#include <iostream>

void Version::print()
{
  std::cout << name << " " << stage << major << "." << minor << "." << patch << std::endl;
}
