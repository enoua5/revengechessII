#include "game/game.h"
#include "ui/ui.h"

#include "game/clock.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  Game g;
  UI::startUI(g);
  
  return 0;
}
