#include "game/clock.h"

Time get_current_time()
{
  return std::chrono::time_point_cast<timer_res>(std::chrono::system_clock::now());
}

unsigned int time_to_mill(Time t)
{
  auto epoch = t.time_since_epoch();
  auto value = std::chrono::duration_cast<timer_res>(epoch);
  return value.count();
}

Timer::Timer(unsigned int startingTime, unsigned int inc, IncrementMethod inct)
{
  lastToggle = get_current_time();
  
  incType = inct;
  increment = inc;
  timeLeft = startingTime;
  running = false;
  
  timeSpentThisTurn = 0;
  timeLeftAtStartOfTurn = timeLeft;
}


unsigned int Timer::update() // returns timeLeft
{
  if(running)
  {
    timeSpentThisTurn = std::chrono::duration_cast<timer_res>(get_current_time() - lastToggle).count();
    
    if(incType == DELAY)
    {
      if(timeSpentThisTurn > increment)
      {
        timeLeft = timeLeftAtStartOfTurn - timeSpentThisTurn + increment;
      }
    }
    else
    {
        timeLeft = timeLeftAtStartOfTurn - timeSpentThisTurn;
    }
  }
  return timeLeft;
}

unsigned int Timer::toggle() // returns timeLeft
{
  if(running)
  {
    update();
    running = false;
    
    if(timeLeft > 0)
    {
      if(incType == INCREMENT)
      {
        timeLeft += increment;
      }
      else if(incType == BRONSTEIN)
      {
        unsigned int inc = increment;
        if(timeSpentThisTurn < inc)
          inc = timeSpentThisTurn;
        timeLeft += inc;
      }
    }
  }
  else
  {
    timeSpentThisTurn = 0;
    timeLeftAtStartOfTurn = timeLeft;
    running = true;
  }
  lastToggle = get_current_time();
  return timeLeft;
}

inline bool Timer::isRunning()
{
  return running;
}


Clock::Clock()
{
  white_timer = Timer();
  black_timer = Timer();
}
Clock::Clock(unsigned int startingTime, unsigned int increment, IncrementMethod inct)
{
  white_timer = Timer(startingTime, increment, inct);
  black_timer = Timer(startingTime, increment, inct);
}
Clock::Clock(unsigned int startingTimeWhite, unsigned int incrementWhite, IncrementMethod inctWhite,
             unsigned int startingTimeBlack, unsigned int incrementBlack, IncrementMethod inctBlack)
{
  white_timer = Timer(startingTimeWhite, incrementWhite, inctWhite);
  black_timer = Timer(startingTimeBlack, incrementBlack, inctBlack);
}
void Clock::toggle()
{
  if(white_timer.isRunning() || black_timer.isRunning())
  {
    // don't toggle the black timer when the clock starts,
    // only after the first move
    black_timer.toggle();
  }
  
  white_timer.toggle();
}
unsigned int Clock::getWhiteTime()
{
  return white_timer.update();
}
unsigned int Clock::getBlackTime()
{
  return black_timer.update();
}

GameResult Clock::getResultFromFlag()
{
  if(white_timer.update() <= 0)
    return BLACK_VICTORY;
  else if(black_timer.update() <= 0)
    return WHITE_VICTORY;
  return ONGOING;
}
