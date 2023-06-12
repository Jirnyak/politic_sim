#define SDL_MAIN_HANDLED

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <unistd.h>
#include <string>
#include <math.h>   
using namespace std;

#define WORLD_WIDTH 1000

#define ISLANDS 100

#define NATIONS 100

#define RESOURCES 1000

#define PIXEL_KM 100

#define KOEF 0.25

#define MODES 3

enum modes 
{
  POL,
  RES,
  POP
};

/* */

/*

cd /Users/jirnyak/Mirror/terrain

./ter

*/

using rng_t = std::mt19937;

std::random_device dev;

std::mt19937 rng(dev());

uint32_t randomer(rng_t& rng, uint32_t range) 
{
    range += 1;
    uint32_t x = rng();
    uint64_t m = uint64_t(x) * uint64_t(range);
    uint32_t l = uint32_t(m);
    if (l < range) {
        uint32_t t = -range;
        if (t >= range) {
            t -= range;
            if (t >= range) 
                t %= range;
        }
        while (l < t) {
            x = rng();
            m = uint64_t(x) * uint64_t(range);
            l = uint32_t(m);
        }
    }
    return m >> 32;
}

class cell 
{
    public:  
        int x;  
        int y;   
        int number;

        cell *sosed_up;   
        cell *sosed_left; 
        cell *sosed_down; 
        cell *sosed_right; 
        cell *sosed_upleft;
        cell *sosed_upright;
        cell *sosed_downleft;
        cell *sosed_downright; 

        bool island;

        bool water;

        int zapas;

        int buff;

        int nation;
        int nation_buff;

        double metal;
        double food;
        double oil;
        double metal_buff;
        double food_buff;
        double oil_buff;

        double pop;
        double pop_buff;

        bool city;

        bool fleet;
        int fleet_nat;

        cell(int x, int y) 
        { 
            this->x = x;
            this->y = y;

            this->number = x*WORLD_WIDTH + y;

            this->island = 0;

            this->water = 0;

            this->zapas = 0;

            this->buff = 0;

            this->nation = 0;
            this->nation_buff = 0;

            this->metal = 0;
            this->food = 0;
            this->oil = 0;
            this->metal_buff = 0;
            this->food_buff = 0;
            this->oil_buff = 0;

            this->pop = 0;
            this->pop_buff = 0;

            this->city = 0;

            this->fleet = 0;
            this->fleet_nat = 0;

        }

        void up(cell *c)
        {
            sosed_up = c;
        }
        void down(cell *c)
        {
            sosed_down = c;
        }
        void left(cell *c)
        {
            sosed_left = c;
        }
        void right(cell *c)
        {
            sosed_right = c;
        }
        void upright(cell *c)
        {
            sosed_upright = c;
        }
        void upleft(cell *c)
        {
            sosed_upleft = c;
        }
        void downright(cell *c)
        {
            sosed_downright = c;
        }
        void downleft(cell *c)
        {
            sosed_downleft = c;
        }
        cell* side(int d) 
        {
            switch(d)
            {
                case 0:
                    return sosed_up; 
                    break;
                case 1:
                    return sosed_upleft;
                    break;
                case 2:
                    return sosed_left; 
                    break;
                case 3:
                    return sosed_downleft; 
                    break;
                case 4:
                    return sosed_down; 
                    break;
                case 5:
                    return sosed_downright;
                    break;
                case 6:
                    return sosed_right;
                    break;
                case 7:
                    return sosed_upright;
                    break;
                default:
                    exit(1);
            }
        }
        cell* side_spiral(int d) const
        {
            if (d == 0)
                return sosed_up;
            if (d == 1)
                return sosed_left;
            if (d == 2)
                return sosed_down;
            if (d == 3)
                return sosed_right;
            else
                exit(1);
        }
};

class nation 
{
    public:
        int number;

        int R;
        int G;
        int B;

        int area;

        double oil;
        double food;
        double metal;
        double pop;

        double pop_buff;

        double army;

        int fleets;

        string name;

        vector<bool> flag;
        vector<bool>::iterator f;

        nation(int number) 
        { 
            this->number = number;

            this->R = 0;
            this->G = 0;
            this->B = 0;

            this->area = 0;

            this->oil = 0;
            this->food = 0;
            this->metal = 0;
            this->pop = 0;

            this->pop_buff = 0;

            this->army = 0;

            this->fleets = 0;

            for (int i=0; i<10; ++i)
            {
                for (int j=0; j<10; ++j)
                {       
                    flag.push_back(randomer(rng,1));
                }
            }
        }
};

int tor_cord(int x)
{
    if (x < 0)
    {
        //x = WORLD_WIDTH + x%WORLD_WIDTH;
        x=(WORLD_WIDTH+x)%WORLD_WIDTH;
    }
    else if (x >= WORLD_WIDTH)
    {
        x = x%WORLD_WIDTH;
    }
    return x;
}

double rasstoyanie(double x1, double y1, double x2, double y2)
{
    double dx = abs(x1 - x2);
    if (dx > WORLD_WIDTH / 2) 
    {
        dx = WORLD_WIDTH - dx;
    }
    
    double dy = abs(y1 - y2);
    if (dy > WORLD_WIDTH / 2) 
    {
        dy = WORLD_WIDTH - dy;
    }
    return sqrt(dx*dx + dy*dy);
    //return (dx*dx + dy*dy);
}

string text_gen(int length)
{
    int drop;
    string word = "";
    drop = randomer(rng,10);
    switch(drop)
        {
        case 0:
            word += "Republic of ";
            break;
        case 1:
            word += "Kingdom of ";
            break;
        case 2:
            word += "Nation of ";
            break;
        case 3:
            word += "Empire of ";
            break;
        case 4:
            word += "Sultanat of ";
            break;
        case 5:
            word += "Federation of ";
            break;
        case 6:
            word += "Tsardom of ";
            break;
        case 7:
            word += "Union of ";
            break;
        case 8:
            word += "Land of ";
            break;
        case 9:
            word += "Khanate of ";
            break;
        case 10:
            word += "State of ";
            break;
        }
    drop = randomer(rng,26);
        switch(drop)
            {
                case 1:
                    word += "Q";
                    break;
                case 2:
                    word += "W";
                    break;
                case 3:
                    word += "E";
                    break;
                case 4:
                    word += "R";
                    break;
                case 5:
                    word += "T";
                    break;
                case 6:
                    word += "Y";
                    break;
                case 7:
                    word += "U";
                    break;
                case 8:
                    word += "I";
                    break;
                case 9:
                    word += "O";
                    break;
                case 10:
                    word += "P";
                    break;
                case 11:
                    word += "A";
                    break;
                case 12:
                    word += "S";
                    break;
                case 13:
                    word += "D";
                    break;
                case 14:
                    word += "F";
                    break;
                case 15:
                    word += "G";
                    break;
                case 16:
                    word += "H";
                    break;
                case 17:
                    word += "J";
                    break;
                case 18:
                    word += "K";
                    break;
                case 19:
                    word += "L";
                    break;
                case 20:
                    word += "Z";
                    break;
                case 21:
                    word += "X";
                    break;
                case 22:
                    word += "C";
                    break;
                case 23:
                    word += "V";
                    break;
                case 24:
                    word += "B";
                    break;
                case 25:
                    word += "N";
                    break;
                case 26:
                    word += "M";
                    break;
            }
    int i = 0;
    while (i < length)
    {
        drop = randomer(rng,27);
        switch(drop)
            {
                case 1:
                    word += "q";
                    break;
                case 2:
                    word += "w";
                    break;
                case 3:
                    word += "e";
                    break;
                case 4:
                    word += "r";
                    break;
                case 5:
                    word += "t";
                    break;
                case 6:
                    word += "y";
                    break;
                case 7:
                    word += "u";
                    break;
                case 8:
                    word += "i";
                    break;
                case 9:
                    word += "o";
                    break;
                case 10:
                    word += "p";
                    break;
                case 11:
                    word += "a";
                    break;
                case 12:
                    word += "s";
                    break;
                case 13:
                    word += "d";
                    break;
                case 14:
                    word += "f";
                    break;
                case 15:
                    word += "g";
                    break;
                case 16:
                    word += "h";
                    break;
                case 17:
                    word += "j";
                    break;
                case 18:
                    word += "k";
                    break;
                case 19:
                    word += "l";
                    break;
                case 20:
                    word += "z";
                    break;
                case 21:
                    word += "x";
                    break;
                case 22:
                    word += "c";
                    break;
                case 23:
                    word += "v";
                    break;
                case 24:
                    word += "b";
                    break;
                case 25:
                    word += "n";
                    break;
                case 26:
                    word += "m";
                    break;
                case 27:
                    word += "-";
                    break;
            }
            i+=1;
    }
    drop = randomer(rng,26);
        switch(drop)
            {
                case 1:
                    word += "q";
                    break;
                case 2:
                    word += "w";
                    break;
                case 3:
                    word += "e";
                    break;
                case 4:
                    word += "r";
                    break;
                case 5:
                    word += "t";
                    break;
                case 6:
                    word += "y";
                    break;
                case 7:
                    word += "u";
                    break;
                case 8:
                    word += "i";
                    break;
                case 9:
                    word += "o";
                    break;
                case 10:
                    word += "p";
                    break;
                case 11:
                    word += "a";
                    break;
                case 12:
                    word += "s";
                    break;
                case 13:
                    word += "d";
                    break;
                case 14:
                    word += "f";
                    break;
                case 15:
                    word += "g";
                    break;
                case 16:
                    word += "h";
                    break;
                case 17:
                    word += "j";
                    break;
                case 18:
                    word += "k";
                    break;
                case 19:
                    word += "l";
                    break;
                case 20:
                    word += "z";
                    break;
                case 21:
                    word += "x";
                    break;
                case 22:
                    word += "c";
                    break;
                case 23:
                    word += "v";
                    break;
                case 24:
                    word += "b";
                    break;
                case 25:
                    word += "n";
                    break;
                case 26:
                    word += "m";
                    break;
            }
    return word;
}

void render_text(SDL_Renderer* renderer,  TTF_Font* font, const std::string& text, int x, int y, int width, int height, const SDL_Color& color) 
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect rect = { x, y, width, height};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}

void read_im(std::vector<cell>& cells, string frame_name) 
{
    SDL_Surface* surface = IMG_Load(frame_name.c_str());
    if (surface) 
    {
        SDL_LockSurface(surface);
        Uint8* pixels = (Uint8*)surface->pixels;
        //#pragma omp parallel
        //#pragma omp for
        for (auto& cell : cells) 
        {
            int x = cell.x;
            int y = cell.y;
            if (y <= WORLD_WIDTH)
            {
                Uint8 r, g, b;
                SDL_GetRGB(*(Uint32*)(pixels + y * surface->pitch + x * surface->format->BytesPerPixel), surface->format, &r, &g, &b);
                if (r > 200)
                {
                    cell.island = 1;
                }
                else
                {
                    cell.island = 0;
                }
            }
            else
            {
                cell.island = 0;
            }
        }
        SDL_UnlockSurface(surface);
        SDL_FreeSurface(surface);
    }
}

bool exist(const char *fileName)
{
    ifstream infile(fileName);
    return infile.good();
}

int main(int argc, char **argv) 
{
    default_random_engine generator;
    normal_distribution<double> distribution(0, 100);
    normal_distribution<double> distribution_2(-1, 0.1);
    normal_distribution<double> distribution_3(-0.1, 0.1);
    normal_distribution<double> distribution_4(100, 10);
    normal_distribution<double> distribution_5(50, 10);
    normal_distribution<double> distribution_6(10000, 1000);
    normal_distribution<double> distribution_7(1, 0.1);

    vector<cell> world;
    vector<cell>::iterator it;
    vector<cell>::iterator jt;

    int WINDOW_WIDTH = ((WORLD_WIDTH*4/3)+0.5);
    int WINDOW_LENGTH = WORLD_WIDTH;
    int WINDOW_EDGE = ((WINDOW_WIDTH-WINDOW_LENGTH)/2+0.5);

    for (int i=0; i<WORLD_WIDTH; ++i)
    {
        for (int j=0; j<WORLD_WIDTH; ++j)
        {       
            world.push_back(cell(i,j));
        }
    }

    for (it = world.begin(); it != world.end(); ++it)
    {
        vector<cell>::iterator it1;
        it->up(&world[tor_cord(it->x)*WORLD_WIDTH + tor_cord(it->y-1)]);
        it->down(&world[tor_cord(it->x)*WORLD_WIDTH + tor_cord(it->y+1)]);
        it->left(&world[tor_cord(it->x-1)*WORLD_WIDTH + tor_cord(it->y)]);
        it->right(&world[tor_cord(it->x+1)*WORLD_WIDTH + tor_cord(it->y)]);
        it->upleft(&world[tor_cord(it->x-1)*WORLD_WIDTH + tor_cord(it->y-1)]);
        it->upright(&world[tor_cord(it->x+1)*WORLD_WIDTH + tor_cord(it->y-1)]);
        it->downleft(&world[tor_cord(it->x-1)*WORLD_WIDTH + tor_cord(it->y+1)]);
        it->downright(&world[tor_cord(it->x+1)*WORLD_WIDTH + tor_cord(it->y+1)]);
    }

    vector<nation> politik;
    vector<nation>::iterator nat;

    for (int i=0; i<=NATIONS; ++i)
    {
         politik.push_back(nation(i));
         politik[i].R = randomer(rng,255);
         politik[i].G = randomer(rng,255);
         politik[i].B = randomer(rng,255);
         politik[i].name = text_gen(1+randomer(rng,10));
    }

    politik[0].R = 0;
    politik[0].G = 0;
    politik[0].B = 0;

    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_LENGTH, 0, &window, &renderer);

    //SDL_SetWindowFullscreen(window, 0);

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    SDL_Rect minimap_dot;
    minimap_dot.w = 10;
    minimap_dot.h = 10;

    int map_state = 0;

    bool quit = 0;

    bool fullscreen = 0;

    bool paused = 0;

    int i = 0;
    int k;

    string frame_name;

    int drop = 0;
    int drop_1 = 0;
    int drop_2 = 0;

    int max = 0;

    int day = 0;
    int year = 0;

    //GENERATION
    /*
    if(exist("map.png") == 1)
    {
        read_im(world, "map.png");
    }
    */

    if(exist("map9999.png") == 1)
    {
        read_im(world, "map999.png");
        SDL_Surface* surface = SDL_CreateRGBSurface(0, WORLD_WIDTH, WORLD_WIDTH, 32, 0, 0, 0, 0);
        #pragma omp parallel
        #pragma omp for
        for (it = world.begin(); it != world.end(); ++it)
        {
            Uint32 pixel = SDL_MapRGB(surface->format, it->island*255, it->island*255, it->island*255); 
            Uint32* target_pixel = static_cast<Uint32*>(surface->pixels) + it->y * surface->pitch / 4 + it->x;
            *target_pixel = pixel;
        }
        string imya;
        imya = "map_bw.png";
        IMG_SavePNG(surface, imya.c_str());
        SDL_FreeSurface(surface);
    }

    //ISLANDS

    else
    {

    while (i < ISLANDS)
    {
        drop_1  = randomer(rng, WORLD_WIDTH);
        drop_2 = randomer(rng, WORLD_WIDTH);
        world[drop_1*WORLD_WIDTH+drop_2].zapas = 1000;
        k = 0;
        while (k < 50)
        {
            world[tor_cord(drop_1+randomer(rng,10)-randomer(rng,10))*WORLD_WIDTH+tor_cord(drop_2+randomer(rng,10)-randomer(rng,10))].zapas = 1000;
            k+=1;
        }
        i += 1;
    }

    while (max<1000)
                {
                    #pragma omp parallel
                    #pragma omp for
                    for (it = world.begin(); it != world.end(); ++it)
                    {
                        if (it->zapas > 0)
                        {
                            it->island = 1;
                            drop = randomer(rng, 7);
                            it->side(drop)->buff += it->zapas - randomer(rng,1);
                            it->buff -= it->zapas;
                        }
                    
                    }
                    #pragma omp parallel
                    #pragma omp for
                    for (it = world.begin(); it != world.end(); ++it)
                    {
                        if (it->buff != 0)
                        {
                            it->zapas = it->buff;
                            it->buff = 0;
                        }
                    }
                    max+=1;
                }
    max = 0;
    while (max<6)
                {
                    #pragma omp parallel
                    #pragma omp for
                    for (it = world.begin(); it != world.end(); ++it)
                    {
                        if (it->island == 1)
                        {
                            drop = randomer(rng, 7);
                            it->side(drop)->buff = 1;
                        }
                    }
                    #pragma omp parallel
                    #pragma omp for
                    for (it = world.begin(); it != world.end(); ++it)
                    {
                        if (it->buff == 1)
                        {
                            it->island = 1;
                            it->buff = 0;
                        }
                    }
                    max+=1;
                }
    max = 0;
    while (max<6)
                {
                    #pragma omp parallel
                    #pragma omp for
                    for (it = world.begin(); it != world.end(); ++it)
                    {
                        if (it->island == 0)
                        {
                            drop = randomer(rng, 7);
                            it->side(drop)->buff = 1;
                        }
                    }
                    #pragma omp parallel
                    #pragma omp for
                    for (it = world.begin(); it != world.end(); ++it)
                    {
                        if (it->buff == 1)
                        {
                            it->island = 0;
                            it->buff = 0;
                        }
                    }
                    max+=1;
                }
    }



//RESOURCES
i = 0;
while (i < RESOURCES)
{
    drop = randomer(rng, WORLD_WIDTH*WORLD_WIDTH);
    if (world[drop].island == 1)
    {
        world[drop].metal = distribution_6(generator);
    }
    drop = randomer(rng, WORLD_WIDTH*WORLD_WIDTH);
    if (world[drop].island == 1)
    {
        world[drop].oil = distribution_6(generator);
    }
    drop = randomer(rng, WORLD_WIDTH*WORLD_WIDTH);
    if (world[drop].island == 1)
    {
        world[drop].food= distribution_6(generator);
    }
    i+=1;
}

max = 0;

while (max<100)
{
    #pragma omp parallel
    #pragma omp for
    for (it = world.begin(); it != world.end(); ++it)
    {
        /*
        drop = randomer(rng,7);
        if (it->side(drop)->island == 1)
        {
            it->food_buff += (it->side(drop)->food - it->food);
            it->metal_buff += (it->side(drop)->metal - it->metal); 
            it->oil_buff += (it->side(drop)->oil - it->oil);
        }
        */
        if (it->side(0)->island == 1)
        {
            it->food_buff += (it->side(0)->food - it->food);
            it->metal_buff += (it->side(0)->metal - it->metal); 
            it->oil_buff += (it->side(0)->oil - it->oil);
        }
        if (it->side(2)->island == 1)
        {
            it->food_buff += (it->side(2)->food - it->food);
            it->metal_buff += (it->side(2)->metal - it->metal); 
            it->oil_buff += (it->side(2)->oil - it->oil);
        }
        if (it->side(4)->island == 1)
        {
            it->food_buff += (it->side(4)->food - it->food);
            it->metal_buff += (it->side(4)->metal - it->metal); 
            it->oil_buff += (it->side(4)->oil - it->oil);
        }
        if (it->side(6)->island == 1)
        {
            it->food_buff += (it->side(6)->food - it->food);
            it->metal_buff += (it->side(6)->metal - it->metal); 
            it->oil_buff += (it->side(6)->oil - it->oil);
        }

        it->food_buff = KOEF*it->food_buff;
        it->metal_buff = KOEF*it->metal_buff; 
        it->oil_buff  = KOEF*it->oil_buff;

    }
    #pragma omp parallel
    #pragma omp for
    for (it = world.begin(); it != world.end(); ++it)
    {
        
        if (it->food_buff < 1)
        {
            it->food_buff = 0;
        }
        if (it->metal_buff < 1)
        {
            it->metal_buff = 0;
        }
        if (it->oil_buff < 1)
        {
            it->oil_buff = 0;
        }
    
        it->food = it->food_buff;
        it->metal = it->metal_buff;
        it->oil = it->oil_buff;

        it->food_buff = 0;
        it->metal_buff = 0;
        it->oil_buff = 0;
    }
    max+=1;
}
                //NATIONS

                    i = 0;
                    while (i < NATIONS)
                    {
                        drop  = randomer(rng, WORLD_WIDTH*WORLD_WIDTH);
                        if (world[drop].island == 1)
                        {
                            world[drop].nation = i+1;
                            politik[i+1].area+= 1;
                            politik[i+1].oil += world[drop].oil;
                            politik[i+1].metal += world[drop].metal;
                            politik[i+1].food += world[drop].food;
                            i+=1;
                        }
                    }
                    max+=1;
                max = 0;

    while (max < 1000)
                {
                    #pragma omp parallel
                    #pragma omp for
                    for (it = world.begin(); it != world.end(); ++it)
                    {
                        if (it->nation != 0)
                        {
                            drop = randomer(rng, 7);
                            if (it->side(drop)->island == 1 and it->side(drop)->nation == 0)
                            {
                                it->side(drop)->nation_buff = it->nation;
                            }
                        }
                    }
                    #pragma omp parallel
                    #pragma omp for
                    for (it = world.begin(); it != world.end(); ++it)
                    {
                        if (it->nation_buff != 0)
                        {
                            it->nation = it->nation_buff;
                            politik[it->nation].area += 1;
                            it->nation_buff = 0;
                            politik[it->nation].oil += it->oil;
                            politik[it->nation].metal += it->metal;
                            politik[it->nation].food += it->food;
                        }
                    }
                    max +=1;
                }
                max = 0;


                //POPS CITIES

    #pragma omp parallel
    #pragma omp for
    for (it = world.begin(); it != world.end(); ++it)
    {
        if (it->island == 1)
        {
            it->pop = distribution_7(generator);
        }
    }

    while (max < 1000)
    {
        drop = randomer(rng, WORLD_WIDTH*WORLD_WIDTH);
        if (world[drop].island == 1)
        {
            world[drop].pop += 10 * distribution_7(generator);
        }
        max += 1;
    }

    max = 0;

    int score = 0;
    for (int i=0; i<=NATIONS; ++i)
    {
    if (politik[i].area > score)
    {
        score = politik[i].area;
    }
    }
    for (int i=0; i<=NATIONS; ++i)
    {
    if (politik[i].area == score)
    {
        k = i;
        break;
    }
    }
    politik[k].R = 200;
    politik[k].G = 1;
    politik[k].B = 1;
    politik[k].name = "USSR";

    for (int i=0; i<10; ++i)
    {
        for (int j=0; j<10; ++j)
        {      
            if (j<=4)
            {
                politik[k].flag[i*10+j] = 1;
            }
            else
            {
                politik[k].flag[i*10+j] = 1;
            }

        }
    }

    #pragma omp parallel
    #pragma omp for
    for (it = world.begin(); it != world.end(); ++it)
    {
        it->food_buff = it->food;
        it->pop_buff = it->pop;
        politik[it->nation].pop += it->pop;
    }

    #pragma omp parallel
    #pragma omp for
    for (nat = politik.begin(); nat != politik.end(); ++nat)
    {
        nat->pop_buff = nat->pop;
    }
    
    string imya;
    string text;

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("Roboto-Black.ttf", 20);

    int pick_x = 0;
    int pick_y = 0;

    int curs_x;
    int curs_y;

    while (quit == false) 
    {
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                SDL_GetMouseState(&pick_x, &pick_y);
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_0: //SDLK_BACKQUOTE:
                        fullscreen = not fullscreen;
                        if (fullscreen == true)
                            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                        else
                            SDL_SetWindowFullscreen(window, 0);
                        break;
                    case SDLK_SPACE:
                        paused = !paused;
                        break;
                    case SDLK_p:
                        map_state = (map_state+1)%MODES;
                        break;
                    default:
                        break;
                }
            }
        }

            //PHYSICS

            if (paused == false)
            {
                SDL_GetMouseState(&curs_x, &curs_y);

                day += 1;

                if (day>100)
                {
                    year += 1;
                    day = 0;
                }

                #pragma omp parallel
                #pragma omp for
                for (it = world.begin(); it != world.end(); ++it)
                {
                    double tempop = sqrt(it->pop);
                    if (it->pop > 0)
                    {
                        politik[it->nation].food += (tempop*it->food)/1000 + it->food/10000;
                        politik[it->nation].oil += (tempop*it->oil)/1000 + it->oil/10000;
                        politik[it->nation].metal += (tempop*it->metal)/1000 + it->metal/10000;
                    }
                }

                #pragma omp parallel
                #pragma omp for
                for (it = world.begin(); it != world.end(); ++it)
                {
                    if (politik[it->nation].food > 0)
                    {
                        politik[it->nation].food -= it->pop/1000;
                        it->pop_buff += it->pop/10000;
                    }
                    else
                    {
                        it->pop_buff -= it->pop/1000;
                    }
                }

                #pragma omp parallel
                #pragma omp for
                for (it = world.begin(); it != world.end(); ++it)
                {
                    if (it->island == 1)
                    {
                        drop = randomer(rng,7);
                        if (it->side(drop)->island == 1)
                        {
                            it->pop_buff = it->pop_buff + it->side(drop)->pop/10 - it->pop/10;
                            it->side(drop)->pop_buff = it->side(drop)->pop_buff + it->pop/10 - it->side(drop)->pop/10;
                        }

                    }
                }

                max = 0;
                while (max < WORLD_WIDTH*WORLD_WIDTH)
                {
                    max += 1;
                    drop = randomer(rng, WORLD_WIDTH*WORLD_WIDTH);  
                    if (world[drop].nation != 0)
                    {
                        drop_1 = randomer(rng,7);
                        if (world[drop].side(drop_1)->island == 1 and world[drop].side(drop_1)->nation != world[drop].nation)
                        {
                            if (politik[world[drop].nation].army > politik[world[drop].side(drop_1)->nation].army)
                            {
                                world[drop].side(drop_1)->nation = world[drop].nation;
                                politik[world[drop].nation].army -= politik[world[drop].side(drop_1)->nation].army/100;
                                politik[world[drop].side(drop_1)->nation].army -= politik[world[drop].side(drop_1)->nation].army/100;
                                world[drop].side(drop_1)->pop_buff -= world[drop].side(drop_1)->pop_buff/10;
                            }
                            else
                            {
                                world[drop].nation = world[drop].side(drop_1)->nation;
                                politik[world[drop].nation].army -= politik[world[drop].nation].army/100;
                                politik[world[drop].side(drop_1)->nation].army -= politik[world[drop].nation].army/100;
                                world[drop].pop_buff -= world[drop].pop_buff/10;
                            }
                        }
                    }  
                }

                max = 0;
                while (max < WORLD_WIDTH*WORLD_WIDTH)
                {
                    max += 1;
                    drop = randomer(rng, WORLD_WIDTH*WORLD_WIDTH);
                    if (world[drop].nation != 0)
                    {
                        drop_1 = randomer(rng,7);
                        if (world[drop].side(drop_1)->island == 0 and politik[world[drop].nation].metal > 1000 and world[drop].side(drop_1)->fleet == 0 and politik[world[drop].nation].fleets < 10)
                        {
                            world[drop].side(drop_1)->fleet = 1;
                            world[drop].side(drop_1)->fleet_nat = world[drop].nation;
                            politik[world[drop].nation].metal -= 1000;
                            politik[world[drop].nation].fleets += 1;
                        }
                    }  
                }

                #pragma omp parallel
                #pragma omp for
                for (it = world.begin(); it != world.end(); ++it)
                {
                    if (it->fleet == 1)
                    {
                        drop = randomer(rng,7);
                        if (it->side(drop)->island == 0 and it->side(drop)->fleet == 0)
                        {
                            it->side(drop)->fleet = 1;
                            it->side(drop)->fleet_nat = it->fleet_nat;
                            it->fleet = 0;
                            it->fleet_nat = 0;
                        }
                        else if (it->side(drop)->island == 1 and it->side(drop)->nation != it->fleet_nat)
                        {
                            it->side(drop)->nation = it->fleet_nat;
                            politik[it->fleet_nat].fleets -= 1;
                            it->fleet = 0;
                            it->fleet_nat = 0;
                        }
                    }

                }

                #pragma omp parallel
                #pragma omp for
                for (nat = politik.begin(); nat != politik.end(); ++nat)
                {
                    if (nat->metal > 0 and nat->pop > 0 and nat->oil > 0 and nat->army < nat->pop/100)
                    {
                        nat->army += nat->metal/10;
                        nat->metal -= nat->metal/10;
                        nat->oil -= nat->army;
                    }
                }

                #pragma omp parallel
                #pragma omp for
                for (it = world.begin(); it != world.end(); ++it)
                {
                    if (it->pop_buff < 0)
                    {
                        it->pop_buff = 0;
                    }
                    it->pop = it->pop_buff;
                }

                #pragma omp parallel
                #pragma omp for
                for (nat = politik.begin(); nat != politik.end(); ++nat)
                {
                    if (nat->oil < 0)
                    {
                        nat->army += nat->oil;
                        nat->oil = 0;
                    }
                    if (nat->army < 0)
                    {
                        nat->army = 0;
                    }
                    nat->pop = 0;
                    nat->area = 0;
                }

                #pragma omp parallel
                #pragma omp for
                for (it = world.begin(); it != world.end(); ++it)
                {
                    if (it->nation != 0)
                    {
                        politik[it->nation].pop += it->pop;
                        politik[it->nation].area += 1;
                    }
                }
         
        //DRAW
        
                SDL_Surface* surface = SDL_CreateRGBSurface(0, WORLD_WIDTH, WORLD_WIDTH, 32, 0, 0, 0, 0);

                switch(map_state)
                {
                case(0):
                    #pragma omp parallel
                    #pragma omp for
                    for (it = world.begin(); it != world.end(); ++it)
                    {
                        Uint32 pixel = SDL_MapRGB(surface->format, politik[it->nation].R + it->island*255 + it->fleet*255, politik[it->nation].G + it->island*255, 50+politik[it->nation].B+it->island*255);   
                        Uint32* target_pixel = static_cast<Uint32*>(surface->pixels) + it->y * surface->pitch / 4 + it->x;
                        *target_pixel = pixel;
                    }
                    break;
                break;
                case(1):
                    #pragma omp parallel
                    #pragma omp for
                    for (it = world.begin(); it != world.end(); ++it)
                    {
                        Uint32 pixel = SDL_MapRGB(surface->format, it->oil*2, it->food*2, it->metal*2); 
                        Uint32* target_pixel = static_cast<Uint32*>(surface->pixels) + it->y * surface->pitch / 4 + it->x;
                        *target_pixel = pixel;
                    }
                break;
                case(2):
                    #pragma omp parallel
                    #pragma omp for
                    for (it = world.begin(); it != world.end(); ++it)
                    {
                        Uint32 pixel = SDL_MapRGB(surface->format, it->pop, 0, 0); 
                        Uint32* target_pixel = static_cast<Uint32*>(surface->pixels) + it->y * surface->pitch / 4 + it->x;
                        *target_pixel = pixel;
                    }
                break;
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                SDL_RenderClear(renderer);

                text = "x: " + to_string(curs_x) + " y: " + to_string(curs_y);
                render_text(renderer, font, text, WORLD_WIDTH, 0, 400, 100, {255,255,255});
                text = "Year: " + to_string(year) + " Day: " + to_string(day);
                render_text(renderer, font, text, WORLD_WIDTH, 100, 400, 50, {255,255,255});

                if (world[pick_x*WORLD_WIDTH+pick_y].nation != 0)
                {
                    text = politik[world[pick_x*WORLD_WIDTH+pick_y].nation].name;
                    render_text(renderer, font, text, WORLD_WIDTH, 150, 400, 50, {255,255,255});
                    text = "Area: " + to_string(politik[world[pick_x*WORLD_WIDTH+pick_y].nation].area*PIXEL_KM) + " km^2";
                    render_text(renderer, font, text, WORLD_WIDTH, 300, 400, 50, {255,255,255});
                    text = "Oil: " + to_string(politik[world[pick_x*WORLD_WIDTH+pick_y].nation].oil);
                    render_text(renderer, font, text, WORLD_WIDTH, 350, 400, 50, {255,255,255});
                    text = "Food: " + to_string(politik[world[pick_x*WORLD_WIDTH+pick_y].nation].food);
                    render_text(renderer, font, text, WORLD_WIDTH, 400, 400, 50, {255,255,255});
                    text = "Metal: " + to_string(politik[world[pick_x*WORLD_WIDTH+pick_y].nation].metal);
                    render_text(renderer, font, text, WORLD_WIDTH, 450, 400, 50, {255,255,255});
                    text = "Population: " + to_string(politik[world[pick_x*WORLD_WIDTH+pick_y].nation].pop/100) + " mln";
                    render_text(renderer, font, text, WORLD_WIDTH, 500, 400, 50, {255,255,255});
                    for (int i = 0; i < 100; i++)
                    {
                        for (int j = 0; j < 100; j++)
                        {
                            SDL_SetRenderDrawColor(renderer, politik[world[pick_x*WORLD_WIDTH+pick_y].nation].R*politik[world[pick_x*WORLD_WIDTH+pick_y].nation].flag[(i/10+0.5)*10+j/10], politik[world[pick_x*WORLD_WIDTH+pick_y].nation].G*politik[world[pick_x*WORLD_WIDTH+pick_y].nation].flag[(i/10+0.5)*10+j/10+0.5], politik[world[pick_x*WORLD_WIDTH+pick_y].nation].B*politik[world[pick_x*WORLD_WIDTH+pick_y].nation].flag[(i/10+0.5)*10+j/10+0.5]+50, 255); 
                            SDL_RenderDrawPoint(renderer, WORLD_WIDTH+i+150, 200 + j);
                        }
                    }
                }

                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_Rect dest_rect = {0, 0, WORLD_WIDTH, WORLD_WIDTH};
                SDL_RenderCopy(renderer, texture, nullptr, &dest_rect);

                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                minimap_dot.x = curs_x;
                minimap_dot.y = curs_y;
                SDL_RenderFillRect(renderer, &minimap_dot);

                SDL_RenderPresent(renderer);

                SDL_DestroyTexture(texture);

                //imya = to_string(max) + ".png";
                //imya = "gen200.png";
                //IMG_SavePNG(surface, imya.c_str());
        
                SDL_FreeSurface(surface);
            }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}