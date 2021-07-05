#ifndef CLOCK_H
#define CLOCK_H

#include "enum.h"
#include <chrono>
typedef std::chrono::milliseconds timer_res;
typedef std::chrono::time_point<std::chrono::system_clock> TimerTime;

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
    Timer(unsigned int startingTime = 5*60*1000, unsigned int increment = 15000, IncrementMethod inct = BRONSTEIN);
    unsigned int update(); // returns timeLeft
    unsigned int toggle(); // returns timeLeft
    //void set_running(bool);
    inline bool isRunning();
    unsigned int getDelayLeft();
  protected:
    IncrementMethod incType;
    unsigned int increment;
    
    bool running;
    unsigned int timeLeft;
    TimerTime lastToggle;
    unsigned int timeLeftAtStartOfTurn;
    unsigned int timeSpentThisTurn;
    
};

class Clock
{
  public:
    Clock();
    Clock(unsigned int startingTime, unsigned int increment, IncrementMethod inct);
    Clock(unsigned int startingTimeWhite, unsigned int incrementWhite, IncrementMethod inctWhite, unsigned int startingTimeBlack, unsigned int incrementBlack, IncrementMethod inctBlack);
    unsigned int toggle();
    void stop();
    unsigned int getWhiteTime();
    unsigned int getBlackTime();
    bool isWhiteRunning();
    bool isBlackRunning();
    unsigned int getDelayLeft();
    GameResult getResultFromFlag();
  protected:
    Timer white_timer;
    Timer black_timer;
};
#endif
