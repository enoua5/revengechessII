#include "server/gameid.h"
#include <chrono>

namespace gameIdGenerator
{
  std::default_random_engine generator;
  std::uniform_int_distribution<int> roll(0, 31);
  bool seeded = false;
}

std::string gameIdGenerator::generate_id(uint32_t user_id)
{
  if(!gameIdGenerator::seeded)
  {
    gameIdGenerator::seeded = true;
    gameIdGenerator::generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
  }
  // these are the characters that can make up an id
  char codes[32+1] = "MGSJT91Z5RH2LPC30KF6VQ8W4BNYXD7-";
  
  std::string id = "";
  
  // convert the 32 bit user id into 5-bit characters
  // we ignore the first two bits here
  uint32_t mask = 0b00111110000000000000000000000000;
  uint8_t shift_amount = 25;
  bool has_had_nonzero = false;
  for(int i = 0; i < 6; i++)
  {
    uint8_t c = (user_id & mask) >> shift_amount;

    // this if *shouldn't* run, but I don't trust like that.
    if(c > 32)
      c = 32;
    mask >>= 5;
    shift_amount -= 5;
        
    if(c == 0 && !has_had_nonzero)
      continue;
    has_had_nonzero = true;
    
    id += codes[c];
  }
  
  // add a random part
  
  for(int i = 0; i < 4; i++)
    id += codes[gameIdGenerator::roll(gameIdGenerator::generator)];
  
  return id;
}
