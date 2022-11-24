    for (uint8_t a; a < ANIMAL_START/NUMBER_OF_SPECIES; a++)
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