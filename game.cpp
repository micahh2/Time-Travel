#include <SDL/SDL.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "data.h"
#include "obj.h"
#include <vector>

#define debug false

#define X_SIZE 3
#define Y_SIZE 3

using namespace std;

class screen
{
    private:
        bool running;
        SDL_Surface* Surf_Display;
    public:
        screen();
        int OnExecute();

        bool OnInit();
        void OnEvent(SDL_Event* Event);
        void OnLoop();
        void OnRender();
        void OnCleanup();
};

class map
{
    public:
        obj *subMap;	

        map(map *old)
        {
            subMap = old->subMap;
        }
        map() { }

        obj* mapGen(obj* board);
};

int main()
{
    srand(time(NULL));
    //obj::counter = 0; //Setting up the Id's
    cout << "Hello world!!" << endl;
    bool game = true;
    int cycle = 0;
    vector<map> updateable;
    while(game)
    {

        for (int i = 0; i < updateable.size(); i++)
        {
            //updateable[i].go();
            cout << "OH!!" << endl;
        }
        cycle++;
        if(cycle>200)
            game=false;
    }

}


obj* map::mapGen(obj* board)
{
    //for(int i = 30; i >= 0; i--)
    //{
    //    int k = 0;
    //    int x = 0;
    //    int y = 0;
    //    do
    //    {
    //        k++;
    //        x = rand() % X_SIZE;	
    //        y = rand() % Y_SIZE;	
    //        test = board[x][y];
    //    };while (test != blank && k < (X_SIZE * Y_SIZE * 100))
    //    
    //    if (test == blank)
    //    {
    //        board[x][y] = new player();
    //    }
    //    else if (debug)
    //    {
    //        cout << "Error in random character gen." << endl;
    //        cout << "No place to put char" << endl;
    //    }

    //}
    return board;
}

bool screen::OnInit()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return false;
    }
    if ((Surf_Display = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
    {
        return false;
    }
    return true;
}
