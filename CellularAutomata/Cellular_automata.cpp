#include <iostream>
#include <vector>
#include <time.h>
using namespace std;

const int height = 50;
const int width = 30;
const int chance_to_start_alive = 45;
const int birth_limit = 3;
const int death_limit = 4;

void printMap(vector<vector<bool>> cell_map);
void initMap(vector<vector<bool>> &cell_map)
{
    srand(time(0));
    for(int x=0;x<width;x++)
    {
        for(int y=0;y<height;y++)
        {
            if(rand()%100 < chance_to_start_alive)
                cell_map[x][y] = true;
        }
    }
}


//for getting the size of a row of an array: sizeof(yourObj)/sizeOf(yourObj[0])
//for getting the size of the columns of and array:  (sizeof(yourObj[0])/sizeof(int))
//for vectors its similar my_array.size(); /* size of y */
//my_array[0].size(); /* size of x */
int countAliveNeighbors(vector<vector<bool>> cell_map, int x, int y)
{
    int counter = 0;
    for(int i=-1;i<2;i++)
    {
        for(int j=-1;j<2;j++)
        {
            int neighbor_x = x+i;
            int neighbor_y = y+j;
            //if were looking at the middle part
            if(i==0 && j == 0)
            {//do nothingthis is our node
            }
            //count the index off the edge of the map for more walls around the edges
            else if(neighbor_x < 0 || neighbor_y < 0 || neighbor_x >= cell_map.size() || neighbor_y >= cell_map[0].size())
            {counter++;}
            //else a normal check of the neighbor
            else if(cell_map[neighbor_x][neighbor_y])
            counter++;
        }
    }
    return counter;
}


void doSimulationStep(vector<vector<bool>> &old_map)
{
    vector<vector<bool>> new_map(width,vector<bool> (height,0));
    for (int x = 0;x<old_map.size();x++)
    {
        for(int y = 0; y<old_map[0].size();y++)
        {
            int nbs = countAliveNeighbors(old_map,x,y);
            //if cell is alive, has too few neighbors, kill it
            if(old_map[x][y])
            {
                if(nbs>= 4)
                {
                    new_map[x][y] = true;
                }
                else if(nbs < 2){
                    new_map[x][y] = false;
                }
            }
            else 
            {
                if(nbs>=5)
                {
                    new_map[x][y]=true;
                }
            }
        }
    }
    for(int j=0;j<width;j++)
    {
        for(int k=0;k<height;k++)
        {
            old_map[j][k] = new_map[j][k];
        }
    }
    
}

void printMap(vector<vector<bool>> cell_map)
{
    for(int x=0;x<width;x++)
    {
        for(int y=0;y<height;y++)
        {
            if(cell_map[x][y])
                cout << "#";
            else
                cout << ".";
        }
        cout << "\n";
    }
}

int main() 
{   
    int num_steps = 4;
    vector<vector<bool>> cell_map(width,vector<bool> (height,0));
    initMap(cell_map);
    //for loop to run thorugh however many simulation steps, dont forget to set num steps
    //bool new_cell_map[height][width] = {doSimulationStep(cell_map)};
    for(int j = 0; j<num_steps;j++)
    {
        doSimulationStep(cell_map);
    }
    printMap(cell_map);
    system("PAUSE");
    return 0;
}

/*
treasure placement
public void placeTreasure(boolean[][] world){
    //How hidden does a spot need to be for treasure?
    //I find 5 or 6 is good. 6 for very rare treasure.
    int treasureHiddenLimit = 5;
    for (int x=0; x < worldWidth; x++){
        for (int y=0; y < worldHeight; y++){
            if(!world[x][y]){
                int nbs = countAliveNeighbours(world, x, y);
                if(nbs >= treasureHiddenLimit){
                    placeTreasure(x, y);
                }
            }
        }
    }
}

*/