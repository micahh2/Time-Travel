#ifndef DATA
#define DATA 
struct dim
{
    int x;
    int y;
    int z;
};
class map
{
    public:
        obj *subMap;	

        map(map *old)
        {
            subMap = old->subMap;
        }
        map();
        obj* mapGen(obj* board);
};

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
#endif
