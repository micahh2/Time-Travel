#include <string>
#include <vector>
#ifndef DATA
#define DATA 

using namespace std;

struct dim
{
    int x;
    int y;
};

class character
{
    private:
        static int nextId;
    public:
        int colId;
        int id;
        dim loc;
        dim dest;
        int size;
        int speed;
        bool pause;
        character(int locx, int locy);
        character(int locx, int locy, int insize);
        character(int locx, int locy, int insize, int inspeed);
};

class map
{
    public:
        string *characters;
        dim mapDim;
        int numChars;
        fstream levelFile;

        map(string fileName);
        vector<character>* update(vector<character> *objects);
        ~map();

};

#endif
