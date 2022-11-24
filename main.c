#include <gbdk/platform.h>
#include <gb/hardware.h>
#include <stdint.h>
#include <gb/drawing.h>
#include <stdio.h>
#include <rand.h>
#include <gb/cgb.h>
#include "graphics.h"
#include "Species.h"
#include "BackgroundData.h"
#define MAP_WIDTH 32
#define MAP_HEIGHT 32
#define PLANT_MAX 50
#define PLANT_START 50
#define ANIMAL_MAX 100
#define NUMBER_OF_SPECIES 2
#define ANIMAL_START 30
struct animal wolves[ANIMAL_MAX / NUMBER_OF_SPECIES];
struct animal goats[ANIMAL_MAX / NUMBER_OF_SPECIES];
struct plant trees[PLANT_MAX];


const UWORD backgroundpalette[] = 
{
    BackgroundDataCGBPal0c0,BackgroundDataCGBPal0c1,BackgroundDataCGBPal0c2,BackgroundDataCGBPal0c3,
    BackgroundDataCGBPal1c0,BackgroundDataCGBPal1c1,BackgroundDataCGBPal1c2,BackgroundDataCGBPal1c3,
    BackgroundDataCGBPal2c0,BackgroundDataCGBPal2c1,BackgroundDataCGBPal2c2,BackgroundDataCGBPal2c3,
    BackgroundDataCGBPal3c0,BackgroundDataCGBPal3c1,BackgroundDataCGBPal3c2,BackgroundDataCGBPal3c3,
};

typedef struct animal
{
    uint8_t thirst;     // if this hits zero, they ded
    uint8_t hunger;     // if this hits zero, they will start to loose energy.
    uint8_t energy;     // refilled by draining the hunger bar, if the hunger bar is zero, the energy level will drop until zero, and at that point, the animal dies.
    uint8_t age;        // age is in years
    uint8_t target;     // what goal they have. (0 for none, 1 for water, 2 for food, 3 for mating, and 4 for)
    uint8_t species;    // what smelly species ar dey?
    uint8_t standingOn; // just the tile id
    BOOLEAN Dead;       // 1 for dead
    uint8_t gotoID; //the id of the thing ur going two
    uint8_t gotox;      // the cords the creature will try and navigate to.
    uint8_t gotoy;
    uint8_t avoidx; // the cords of the nearest threat
    uint8_t avoidy;
    uint8_t x; // tile cord of thing
    uint8_t y;
} animal;

typedef struct plant
{
    BOOLEAN isNearWater; // if this is zero, they ded
    uint8_t waterX;      // the cords the nearest water block.
    uint8_t waterY;
    BOOLEAN Dead;         // 1 for dead
    uint8_t maxDistWater; // the maximum disance the plant can be from water.
    uint8_t age;          // age is in years
    uint8_t standingOn;   // just the tile id
    uint8_t x;            // tile cord of thing
    uint8_t y;
} plant;

uint8_t randomInRange(uint8_t lower, uint8_t upper)
{
    return arand() % (upper - lower + 1) + lower;
}

void closestPlant(uint8_t ID, uint8_t ENTITY_TYPE, uint8_t Species) // 0 for animal, 1 for plant
{

    if (ENTITY_TYPE == 0)
    {
        for (uint8_t p; p < PLANT_MAX; p++)
        {
            if (trees[p].x - goats[ID].x <= 20)
            {
                if (trees[p].y - goats[ID].y <= 20)
                {
                    goats[ID].gotox = trees[p].x;
                    goats[ID].gotoy = trees[p].y;
                }
            }
        }
    }
    else
    {
        for (uint8_t p; p < PLANT_MAX; p++)
        {
            if (trees[p].x - trees[ID].x <= 30)
            {
                if (trees[p].y - trees[ID].y <= 30)
                {
                    trees[ID].isNearWater == 1;
                }
            }
        }
    }
}

void closestAnimal(uint8_t ID, uint8_t ENTITY_TYPE, uint8_t Species)
{
    if (ENTITY_TYPE == 0)
    {
        switch (Species)
        {
        case GOATS:
            /* code */
            break;

        case WOLVES:
            for (uint8_t p; p < ANIMAL_MAX / NUMBER_OF_SPECIES; p++)
            {
                if (goats[p].x - wolves[ID].x <= 20)
                {
                    if (goats[p].y - wolves[ID].y <= 20)
                    {
                        wolves[ID].gotox = goats[p].x;
                        wolves[ID].gotoy = goats[p].y;
                    }
                }
            }
            break;

        default:
            break;
        }
    }
    else
    {
        for (uint8_t p; p < PLANT_MAX; p++)
        {
            if (trees[p].x - trees[ID].x <= 30)
            {
                if (trees[p].y - trees[ID].y <= 30)
                {
                    trees[ID].isNearWater == 1;
                }
            }
        }
    }
}

void avoidClosestAnimal(uint8_t ID, uint8_t ENTITY_TYPE, uint8_t Species)
{
    if (ENTITY_TYPE == 0)
    {
        switch (Species)
        {
        case GOATS:
            for (uint8_t p; p < ANIMAL_MAX / NUMBER_OF_SPECIES; p++)
            {
                if (wolves[p].x - goats[ID].x <= 20)
                {
                    if (wolves[p].y - goats[ID].y <= 20)
                    {
                        goats[ID].avoidx = wolves[p].x;
                        goats[ID].avoidy = wolves[p].y;
                    }
                }
            }
            break;

        case WOLVES:

            break;

        default:
            break;
        }
    }
    else
    {
        for (uint8_t p; p < PLANT_MAX; p++)
        {
            if (trees[p].x - trees[ID].x <= 30)
            {
                if (trees[p].y - trees[ID].y <= 30)
                {
                    trees[ID].isNearWater == 1;
                }
            }
        }
    }
}

void closestWater(uint8_t ID, uint8_t ENTITY_TYPE, uint8_t Species) // 0 for animal, 1 for plant
{
    for (uint8_t x = 0; x < MAP_WIDTH; x++)
    {
        for (uint8_t y = 0; y < MAP_HEIGHT; y++)
        {
            if (get_bkg_tile_xy(x, y) == 2)
            {
                if (ENTITY_TYPE == 0)
                {
                    switch (Species)
                    {
                    case GOATS:
                        goats[ID].gotox = x;
                        goats[ID].gotoy = y;
                        break;
                    case WOLVES:
                        wolves[ID].gotox = x;
                        wolves[ID].gotoy = y;
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    trees[ID].waterX = x;
                    trees[ID].waterY = y;
                }
            }
        }
    }
}

void animalTick()
{
    for (uint8_t a; a < ANIMAL_MAX / NUMBER_OF_SPECIES; a++) // wolves
    {
        if (wolves[a].Dead == 0)
        {
            if (wolves[a].thirst < 60)
            {
                wolves[a].target = 1; // target is water
                closestWater(a, 0, WOLVES);
            }
            else
            {
                wolves[a].target = 0;
            }
            if (wolves[a].hunger < 40 && wolves[a].thirst > 50)
            {
                wolves[a].target = 2; // target is food
                closestAnimal(a, 0, WOLVES);
            }
            else
            {
                wolves[a].target = 1; // target is water
                closestWater(a, 0, WOLVES);
            }
            /*
                        if (wolves[a].thirst >= 60 && wolves[a].hunger >= 60)
                        {
                            wolves[a].target = 0; // target is to chill
                           // wolves[a].gotox = 20;
                           // wolves[a].gotoy = 18;
                        }*/

            if (wolves[a].hunger <= 0)
            {
                wolves[a].energy--;
            }
            else
            {
                wolves[a].hunger--;
            }

            if (wolves[a].target != 0)
            {
                wolves[a].energy--;
            }

            if (wolves[a].energy == 0)
            {
                wolves[a].Dead = 1;
            }
            if (wolves[a].thirst > 0)
            {
                wolves[a].thirst--;
            }
            else
            {
                wolves[a].Dead = 1;
            }
            if (wolves[a].x >= MAP_WIDTH)
            {
                set_bkg_tile_xy(wolves[a].x, wolves[a].y, wolves[a].standingOn);
                wolves[a].x = 0;
                wolves[a].standingOn = get_bkg_tile_xy(wolves[a].x, wolves[a].y);
            }
            else if (wolves[a].y >= MAP_HEIGHT)
            {
                set_bkg_tile_xy(wolves[a].x, wolves[a].y, wolves[a].standingOn);
                wolves[a].y = 0;
                wolves[a].standingOn = get_bkg_tile_xy(wolves[a].x, wolves[a].y);
            }

            if (wolves[a].x < 0)
            {
                set_bkg_tile_xy(wolves[a].x, wolves[a].y, wolves[a].standingOn);
                wolves[a].x = MAP_WIDTH;
                wolves[a].standingOn = get_bkg_tile_xy(wolves[a].x, wolves[a].y);
            }
            else if (wolves[a].y < 0)
            {
                set_bkg_tile_xy(wolves[a].x, wolves[a].y, wolves[a].standingOn);
                wolves[a].y = MAP_HEIGHT;
                wolves[a].standingOn = get_bkg_tile_xy(wolves[a].x, wolves[a].y);
            }

            if (wolves[a].target == 0)
            {

                set_bkg_tile_xy(wolves[a].x, wolves[a].y, wolves[a].standingOn);
                wolves[a].gotox += randomInRange(0, 1);
                wolves[a].standingOn = get_bkg_tile_xy(wolves[a].x, wolves[a].y);
                set_bkg_tile_xy(wolves[a].x, wolves[a].y, wolves[a].standingOn);
                wolves[a].gotoy += randomInRange(0, 1);
                wolves[a].standingOn = get_bkg_tile_xy(wolves[a].x, wolves[a].y);
                set_bkg_tile_xy(wolves[a].x, wolves[a].y, wolves[a].standingOn);
                wolves[a].gotox -= randomInRange(0, 1);
                wolves[a].standingOn = get_bkg_tile_xy(wolves[a].x, wolves[a].y);
                set_bkg_tile_xy(wolves[a].x, wolves[a].y, wolves[a].standingOn);
                wolves[a].gotoy -= randomInRange(0, 1);
                wolves[a].standingOn = get_bkg_tile_xy(wolves[a].x, wolves[a].y);
            }
            else
            {

                if (wolves[a].x < wolves[a].gotox)
                {
                    set_bkg_tile_xy(wolves[a].x, wolves[a].y, wolves[a].standingOn);
                    wolves[a].x++;
                    wolves[a].standingOn = get_bkg_tile_xy(wolves[a].x, wolves[a].y);
                }
                if (wolves[a].x > wolves[a].gotox)
                {
                    set_bkg_tile_xy(wolves[a].x, wolves[a].y, wolves[a].standingOn);
                    wolves[a].x--;
                    wolves[a].standingOn = get_bkg_tile_xy(wolves[a].x, wolves[a].y);
                }
                if (wolves[a].y > wolves[a].gotoy)
                {
                    set_bkg_tile_xy(wolves[a].x, wolves[a].y, wolves[a].standingOn);
                    wolves[a].y--;
                    wolves[a].standingOn = get_bkg_tile_xy(wolves[a].x, wolves[a].y);
                }
                if (wolves[a].y < wolves[a].gotoy)
                {
                    set_bkg_tile_xy(wolves[a].x, wolves[a].y, wolves[a].standingOn);
                    wolves[a].y++;
                    wolves[a].standingOn = get_bkg_tile_xy(wolves[a].x, wolves[a].y);
                }

                if (wolves[a].x == wolves[a].gotox && wolves[a].y == wolves[a].gotoy)
                {
                    switch (wolves[a].target)
                    {
                    case 1:
                        wolves[a].thirst += 5;
                        wolves[a].energy += 10;
                        break;
                    case 2:
                        wolves[a].hunger += 15;
                        wolves[a].energy += 5;

                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }

    for (uint8_t a; a < ANIMAL_MAX / NUMBER_OF_SPECIES; a++) // goats
    {
        if (goats[a].Dead == 0)
        {
            avoidClosestAnimal(a, 0, GOATS);
            if (goats[a].thirst < 60)
            {
                goats[a].target = 1; // target is water
                closestWater(a, 0, GOATS);
            }
            else
            {
                goats[a].target = 0;
            }
            if (goats[a].hunger < 40 && goats[a].thirst > 50)
            {
                goats[a].target = 2; // target is food
                closestPlant(a, 0, GOATS);
            }
            else
            {
                goats[a].target = 1; // target is water
                closestWater(a, 0, GOATS);
            }
            /*
                        if (goats[a].thirst >= 60 && goats[a].hunger >= 60)
                        {
                            goats[a].target = 0; // target is to chill
                           // goats[a].gotox = 20;
                           // goats[a].gotoy = 18;
                        }*/

            if (goats[a].hunger <= 0)
            {
                goats[a].energy--;
            }
            else
            {
                goats[a].hunger--;
            }

            if (goats[a].target != 0)
            {
                goats[a].energy--;
            }

            if (goats[a].energy == 0)
            {
                goats[a].Dead = 1;
            }
            if (goats[a].thirst > 0)
            {
                goats[a].thirst--;
            }
            else
            {
                goats[a].Dead = 1;
            }
            if (goats[a].x >= MAP_WIDTH)
            {
                set_bkg_tile_xy(goats[a].x, goats[a].y, goats[a].standingOn);
                goats[a].x = 0;
                goats[a].standingOn = get_bkg_tile_xy(goats[a].x, goats[a].y);
            }
            else if (goats[a].y >= MAP_HEIGHT)
            {
                set_bkg_tile_xy(goats[a].x, goats[a].y, goats[a].standingOn);
                goats[a].y = 0;
                goats[a].standingOn = get_bkg_tile_xy(goats[a].x, goats[a].y);
            }

            if (goats[a].x < 0)
            {
                set_bkg_tile_xy(goats[a].x, goats[a].y, goats[a].standingOn);
                goats[a].x = MAP_WIDTH;
                goats[a].standingOn = get_bkg_tile_xy(goats[a].x, goats[a].y);
            }
            else if (goats[a].y < 0)
            {
                set_bkg_tile_xy(goats[a].x, goats[a].y, goats[a].standingOn);
                goats[a].y = MAP_HEIGHT;
                goats[a].standingOn = get_bkg_tile_xy(goats[a].x, goats[a].y);
            }

            if (goats[a].target == 0)
            {

                set_bkg_tile_xy(goats[a].x, goats[a].y, goats[a].standingOn);
                goats[a].gotox += randomInRange(0, 1);
                goats[a].standingOn = get_bkg_tile_xy(goats[a].x, goats[a].y);
                set_bkg_tile_xy(goats[a].x, goats[a].y, goats[a].standingOn);
                goats[a].gotoy += randomInRange(0, 1);
                goats[a].standingOn = get_bkg_tile_xy(goats[a].x, goats[a].y);
                set_bkg_tile_xy(goats[a].x, goats[a].y, goats[a].standingOn);
                goats[a].gotox -= randomInRange(0, 1);
                goats[a].standingOn = get_bkg_tile_xy(goats[a].x, goats[a].y);
                set_bkg_tile_xy(goats[a].x, goats[a].y, goats[a].standingOn);
                goats[a].gotoy -= randomInRange(0, 1);
                goats[a].standingOn = get_bkg_tile_xy(goats[a].x, goats[a].y);
            }
            else
            {

                if (goats[a].x < goats[a].gotox)
                {
                    set_bkg_tile_xy(goats[a].x, goats[a].y, goats[a].standingOn);
                    goats[a].x++;
                    goats[a].standingOn = get_bkg_tile_xy(goats[a].x, goats[a].y);
                }
                if (goats[a].x > goats[a].gotox)
                {
                    set_bkg_tile_xy(goats[a].x, goats[a].y, goats[a].standingOn);
                    goats[a].x--;
                    goats[a].standingOn = get_bkg_tile_xy(goats[a].x, goats[a].y);
                }
                if (goats[a].y > goats[a].gotoy)
                {
                    set_bkg_tile_xy(goats[a].x, goats[a].y, goats[a].standingOn);
                    goats[a].y--;
                    goats[a].standingOn = get_bkg_tile_xy(goats[a].x, goats[a].y);
                }
                if (goats[a].y < goats[a].gotoy)
                {
                    set_bkg_tile_xy(goats[a].x, goats[a].y, goats[a].standingOn);
                    goats[a].y++;
                    goats[a].standingOn = get_bkg_tile_xy(goats[a].x, goats[a].y);
                }

                if (goats[a].x == goats[a].gotox && goats[a].y == goats[a].gotoy)
                {
                    switch (goats[a].target)
                    {
                    case 1:
                        goats[a].thirst += 5;
                        goats[a].energy += 10;
                        break;
                    case 2:
                        goats[a].hunger += 5;
                        goats[a].energy += 10;
                        break;
                    default:
                        break;
                    }
                }
            }

            if (goats[a].x < goats[a].avoidx)
            {
                set_bkg_tile_xy(goats[a].x, goats[a].y, goats[a].standingOn);
                goats[a].x--;
                goats[a].standingOn = get_bkg_tile_xy(goats[a].x, goats[a].y);
            }
            if (goats[a].x > goats[a].avoidx)
            {
                set_bkg_tile_xy(goats[a].x, goats[a].y, goats[a].standingOn);
                goats[a].x++;
                goats[a].standingOn = get_bkg_tile_xy(goats[a].x, goats[a].y);
            }
            if (goats[a].y > goats[a].avoidy)
            {
                set_bkg_tile_xy(goats[a].x, goats[a].y, goats[a].standingOn);
                goats[a].y++;
                goats[a].standingOn = get_bkg_tile_xy(goats[a].x, goats[a].y);
            }
            if (goats[a].y < goats[a].avoidy)
            {
                set_bkg_tile_xy(goats[a].x, goats[a].y, goats[a].standingOn);
                goats[a].y--;
                goats[a].standingOn = get_bkg_tile_xy(goats[a].x, goats[a].y);
            }
        }
    }
    /*
        for(uint8_t p; p < PLANT_MAX; p++)
        {
            if(trees[p].Dead != 1)
            {
           trees[p].age = 1;
           trees[p].maxDistWater = randomInRange(5, 10);
           trees[p].x = randomInRange(0, 19);
           trees[p].y = randomInRange(0, 17);
           }
        }  */
}

void display()
{
    // world view updater
    // printf("Hello World!");
    animalTick();
    for (uint8_t a; a < ANIMAL_MAX / NUMBER_OF_SPECIES; a++) // goats
    {

        if (wolves[a].Dead == 1)
        {
            set_bkg_tile_xy(wolves[a].x, wolves[a].y, 9);
        }
        else
        {
            set_bkg_tile_xy(wolves[a].x, wolves[a].y, 3);
        }
    }

    for (uint8_t a; a < ANIMAL_MAX / NUMBER_OF_SPECIES; a++) // goats
    {

        if (goats[a].Dead == 1)
        {
            set_bkg_tile_xy(goats[a].x, goats[a].y, 8);
        }
        else
        {
            set_bkg_tile_xy(goats[a].x, goats[a].y, 3);
        }
    }

    
}

void genRiverX()
{
    uint8_t noise[MAP_WIDTH];
    uint8_t Y_POS = randomInRange(0, 5);
    noise[0] = Y_POS;
    uint8_t level = 0;
#define heightFactor 2

    for (uint8_t i = 1; i < MAP_WIDTH; i++)
    {
        level = noise[i - 1];

        level += randomInRange(0, heightFactor);
        level -= randomInRange(0, heightFactor);

        noise[i] = level;
        set_bkg_tile_xy(i, Y_POS + noise[i], 2);
        set_bkg_tile_xy(i, Y_POS + noise[i] + 1, 2);
        set_bkg_tile_xy(i, Y_POS + noise[i] + 2, 2);
    }
    set_bkg_tile_xy(0, Y_POS + noise[0], 2);
    set_bkg_tile_xy(0, Y_POS + noise[0] + 1, 2);
    set_bkg_tile_xy(0, Y_POS + noise[0] + 2, 2);
}

void clearScreen()
{
    for (uint8_t x = 0; x < MAP_WIDTH; x++)
    {
        for (uint8_t y = 0; y < 31; y++)
        {
            set_bkg_tile_xy(x, y, 7);
        }
    }
}

void genRiverY()
{
    uint8_t noise[MAP_HEIGHT];
    uint8_t X_POS = randomInRange(0, 5);
    noise[0] = X_POS;
    uint8_t level = 0;
#define heightFactor 2

    for (uint8_t i = 1; i < MAP_HEIGHT; i++)
    {
        level = noise[i - 1];

        level += randomInRange(0, heightFactor);
        level -= randomInRange(0, heightFactor);

        noise[i] = level;
        set_bkg_tile_xy(X_POS + noise[i], i, 2);
        set_bkg_tile_xy(X_POS + noise[i] + 1, i, 2);
        set_bkg_tile_xy(X_POS + noise[i] + 2, i, 2);
    }
    set_bkg_tile_xy(X_POS + noise[0], 0, 2);
    set_bkg_tile_xy(X_POS + noise[0] + 1, 0, 2);
    set_bkg_tile_xy(X_POS + noise[0] + 2, 0, 2);
}

void initSim()
{
    initarand(sys_time);
    clearScreen();
    genRiverX();
    genRiverY();
    for (uint8_t a; a < ANIMAL_START / NUMBER_OF_SPECIES; a++)
    {
        goats[a].age = 1;
        goats[a].thirst = 100;
        goats[a].hunger = 100;
        goats[a].energy = 100;
        goats[a].x = randomInRange(0, MAP_WIDTH);
        goats[a].y = randomInRange(0, MAP_HEIGHT);
        goats[a].standingOn = get_bkg_tile_xy(goats[a].x, goats[a].y);
        goats[a].thirst = 100;
        goats[a].Dead = 0;

        while (get_bkg_tile_xy(goats[a].x, goats[a].y) == 2)
        {
            goats[a].x = randomInRange(0, MAP_WIDTH);
            goats[a].y = randomInRange(0, MAP_HEIGHT);
        }
    }

    for (uint8_t a; a < ANIMAL_START / NUMBER_OF_SPECIES; a++)
    {
        wolves[a].age = 1;
        wolves[a].thirst = 100;
        wolves[a].hunger = 100;
        wolves[a].energy = 100;
        wolves[a].x = randomInRange(0, MAP_WIDTH);
        wolves[a].y = randomInRange(0, MAP_HEIGHT);
        wolves[a].standingOn = get_bkg_tile_xy(wolves[a].x, wolves[a].y);
        wolves[a].thirst = 100;
        wolves[a].Dead = 0;

        while (get_bkg_tile_xy(wolves[a].x, wolves[a].y) == 2)
        {
            wolves[a].x = randomInRange(0, MAP_WIDTH);
            wolves[a].y = randomInRange(0, MAP_HEIGHT);
        }
    }

    for (uint8_t p; p < PLANT_START; p++)
    {
        trees[p].age = 1;
        trees[p].Dead = 0;
        trees[p].x = randomInRange(0, MAP_WIDTH);
        trees[p].y = randomInRange(0, MAP_HEIGHT);
        trees[p].standingOn = get_bkg_tile_xy(trees[p].x, trees[p].y);
        trees[p].maxDistWater = randomInRange(5, 10);

        while (get_bkg_tile_xy(trees[p].x, trees[p].y) == 2)
        {
            trees[p].x = randomInRange(0, MAP_WIDTH);
            trees[p].y = randomInRange(0, MAP_HEIGHT);
        }
    }

    for (uint8_t p; p < PLANT_MAX; p++)
    {
        set_bkg_tile_xy(trees[p].x, trees[p].y, 4);
    }
}

inline void set_bkg_tile_attrib_xy(uint8_t x, uint8_t y, uint8_t attrib_data)
{
    // Select writing to attributes
    //#define VBK_ATTRIBUTES   1

    VBK_REG = VBK_ATTRIBUTES;
    set_bkg_tile_xy(x, y, attrib_data);
    // Return VBK to normal setting
    VBK_REG = VBK_TILES;
}

void cgb_check_and_init(void)
{
    if (_cpu == CGB_TYPE)
    {
        cpu_fast();
        set_bkg_palette(0, 4, &backgroundpalette[0]); 
        // maybe set up your cgb palettes
    }
}

void main(void)
{
    SHOW_BKG;
    SHOW_SPRITES;
    set_bkg_data(0, 10, graphics);
    cgb_check_and_init();
    while (1)
    {
        uint8_t cur = joypad();

        if (cur & J_A)
        {
        }

        if (cur & J_B)
        {
        }

        if (cur & J_SELECT)
        {
        }

        if (cur & J_START)
        {
            initSim();
        }

        if (cur & J_UP)
        {
            scroll_bkg(0, -8);
        }
        else if (cur & J_DOWN)
        {
            scroll_bkg(0, 8);
        }

        if (cur & J_LEFT)
        {
            scroll_bkg(-8, 0);
        }
        else if (cur & J_RIGHT)
        {
            scroll_bkg(8, 0);
        }

        display();
    }
}
