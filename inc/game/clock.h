#ifndef CLOCK_H
#define CLOCK_H

#include "enum.h"
#include <chrono>
typedef std::chrono::milliseconds timer_res;
typedef std::chrono::time_point<std::chrono::system_clock> Time;

enum IncrementMethod
{
  INCREMENT,
  DELAY,
  BRONSTEIN
};

class Timer
{
  public:
    // all times in milliseconds
    Timer(unsigned int startingTime = 5*60*1000, unsigned int increment = 15000, IncrementMethod inct = INCREMENT);
    unsigned int update(); // returns timeLeft
    unsigned int toggle(); // returns timeLeft
    //void set_running(bool);
    inline bool isRunning();
  protected:
    IncrementMethod incType;
    unsigned int increment;
    
    bool running;
    unsigned int timeLeft;
    Time lastToggle;
    unsigned int timeLeftAtStartOfTurn;
    unsigned int timeSpentThisTurn;
    
};

class Clock
{
  public:
    Clock();
    Clock(unsigned int startingTime, unsigned int increment, IncrementMethod inct);
    Clock(unsigned int startingTimeWhite, unsigned int incrementWhite, IncrementMethod inctWhite, unsigned int startingTimeBlack, unsigned int incrementBlack, IncrementMethod inctBlack);
    void toggle();
    unsigned int getWhiteTime();
    unsigned int getBlackTime();
    GameResult getResultFromFlag();
  protected:
    Timer white_timer;
    Timer black_timer;
};
#endif
