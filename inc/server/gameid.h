#ifndef GAMEID_H
#define GAMEID_H

#include <string>
#include <random>

namespace gameIdGenerator
{
  static std::default_random_engine generator;
  static std::uniform_int_distribution<int> roll(0, 31);
  std::string generate_id(uint32_t user_id);
}

#endif
