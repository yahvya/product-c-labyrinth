#include <stdio.h>
#include "./game/game.h"

int main(int argc, char** argv)
{
    if (initializeGame())
    {
        startGame();
        closeGame();

        return 0;
    }

    fputs("\nEchec de lancement du jeux", stderr);

    return 1;
}
