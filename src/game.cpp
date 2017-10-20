#include "../include/game.h"
#include <dirent.h>
#include <iostream>
#include <string>
using namespace std;

void readFile()
{
    DIR *directory;
    directory = opendir ("../map/");
    struct dirent *ent;
    if (directory != NULL)
    {
        ent = readdir(directory);
        while (ent != NULL)
        {
            cout << ent->d_name << endl;
        }
        closedir(directory);
    }
    else
    {
        perror("we could not open directory");
        return;
    }
}

int main()
{
    readFile();
    int nbrPlayers = 6;
    Player** players = new Player*[nbrPlayers];
    for(int i = 0; i < nbrPlayers; i++)
    {
        players[i];
    }
}