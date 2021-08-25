#ifndef GAMEID_H
#define GAMEID_H

#include <string>
#include <random>

namespace gameIdGenerator
{
  extern std::default_random_engine generator;
  extern std::uniform_int_distribution<int> roll;
  extern bool seeded;
  std::string generate_id(uint32_t user_id);
}

#endif
