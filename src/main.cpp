#include <iostream>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <SDL2/SDL.h>
#include "Game.h"
#include "BeizerCurve.h"
#include "ImGuiPlots.h"

using namespace std;

int main(int argc, char* args[]) {
    srand(time(NULL));
    Game lesson;
    lesson.run();
    return 0;
}