#include "./game/game.h"

int main(int argc, char** argv)
{
    if (initializeGame())
    {
        startGame();
        closeGame();

        return 0;
    }

    TraceLog(LOG_ERROR, "\nEchec de lancement du jeux");

    return 1;
}
