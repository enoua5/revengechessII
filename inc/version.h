#pragma once

#include <string>
//#include <iostream>

struct Version
{
  Version() {}
  Version(std::string n, char s, unsigned int ma, unsigned int mi, unsigned int p) : name(n), stage(s), major(ma), minor(mi), patch(p){}
  
  void print();
  
  std::string name;
  char stage;
  unsigned int major;
  unsigned int minor;
  unsigned int patch;
};
