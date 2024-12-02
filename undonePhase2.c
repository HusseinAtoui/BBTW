#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// BattleShips Beneath The Waves - Phase 2 - correct fire and smoke screen, and artil 
// adding torpedo LATEST
#define GRID 10

struct Ship
{
    char name[20];
    int size;
    int hits;
    int sank;
    int row;
    int col;
    int direction;
};

struct Player
{
    char name[50];
    char grid[GRID][GRID];
    char copiedGird[GRID][GRID];
    char hitMissGrid[GRID][GRID];
    char difficulty[10];
    struct Ship ships[7];
    int Rsweep;
    int smokeScreens;
    int smokeRound;
    int artill;
    bool shot;
    int tor;
    int torCount;
    int botTurn;
    int numOfHits;
    int lastHitX;
    int lastHitY;
    int detectedRow; 
    int detectedCol;
};

void gridStart(char grid[GRID][GRID])
{
    for (int i = 0; i < GRID; i++)
    {
        for (int j = 0; j < GRID; j++)
        {
            grid[i][j] = '~';
        }
    }
}

void gridDisplay(char grid[GRID][GRID])
{
    printf("   A B C D E F G H I J\n");
    for (int i = 0; i < GRID; i++)
    {
        printf("%2d ", i + 1);
        for (int j = 0; j < GRID; j++)
        {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
}

void gridDisplayOpp(char grid[GRID][GRID])
{
    char NewGird[GRID][GRID];

    for (int i = 0; i < GRID; i++)
    {
        for (int j = 0; j < GRID; j++)
        {
            if (grid[i][j] == '*')
            {
                NewGird[i][j] = '*';
            }
            else if (grid[i][j] == 'o')
            {
                NewGird[i][j] = 'o';
            }
            else
            {
                NewGird[i][j] = '~';
            }
        }
    }

    printf("   A B C D E F G H I J\n");
    for (int i = 0; i < GRID; i++)
    {
        printf("%2d ", i + 1);
        for (int j = 0; j < GRID; j++)
        {
            printf("%c ", NewGird[i][j]);
        }
        printf("\n");
    }
}

void gridDisplayOppHARD(char grid[GRID][GRID])
{
    char NewGird[GRID][GRID];

    for (int i = 0; i < GRID; i++)
    {
        for (int j = 0; j < GRID; j++)
        {
            if (grid[i][j] == '*' || grid[i][j] == 'o')
            {
                NewGird[i][j] = 'o';
            }
            else
            {
                NewGird[i][j] = '~';
            }
        }
    }

    printf("   A B C D E F G H I J\n");
    for (int i = 0; i < GRID; i++)
    {
        printf("%2d ", i + 1);
        for (int j = 0; j < GRID; j++)
        {
            printf("%c ", NewGird[i][j]);
        }
        printf("\n");
    }
}

void gridCopy(char grid[GRID][GRID], char copyGrid[GRID][GRID])
{
    for (int i = 0; i < GRID; i++)
    {
        for (int j = 0; j < GRID; j++)
        {
            copyGrid[i][j] = grid[i][j];
        }
    }
}

void initHitMissGrid(char grid[GRID][GRID])
{
    for (int i = 0; i < GRID; i++)
    {
        for (int j = 0; j < GRID; j++)
        {
            grid[i][j] = '~';
        }
    }
}

int randomFP()
{
    return rand() % 2;
}

void createShip(struct Player *player)
{
    player->ships[0] = (struct Ship){"Carrier", 5, 0, 0, 0, 0, 0};
    player->ships[1] = (struct Ship){"Battleship", 4, 0, 0, 0, 0, 0};
    player->ships[2] = (struct Ship){"Destroyer", 3, 0, 0, 0, 0, 0};
    player->ships[3] = (struct Ship){"Submarine", 2, 0, 0, 0, 0, 0};
}

int canPlaceShip(char grid[GRID][GRID], int x, int y, int size, char dir)
{
    if (dir == 'h')
    {
        if (y + size > GRID)
            return 0;
        for (int j = y; j < y + size; j++)
        {
            if (grid[x][j] != '~')
                return 0;
        }
    }
    else if (dir == 'v')
    {
        if (x + size > GRID)
            return 0;
        for (int i = x; i < x + size; i++)
        {
            if (grid[i][y] != '~')
                return 0;
        }
    }
    return 1;
}

void placeShip(struct Player *player)
{
    for (int i = 0; i < 4; i++)
    {
        struct Ship *ship = &player->ships[i];
        int x, y;

        while (1)
        {
            printf("Enter the coordinates for the %s (%d) (for example, D7 h or v): ", ship->name, ship->size);
            char col, dir;
            int read = scanf(" %c%d %c", &col, &x, &dir);

            col = toupper((unsigned char)col);
            dir = toupper((unsigned char)dir);
            x--;

            if (read != 3 || x < 0 || x >= GRID || col < 'A' || col > 'J')
            {
                printf("Invalid input. Please try again.\n");
                while (getchar() != '\n')
                    ;
                continue;
            }

            y = col - 'A';

            if (dir == 'H' && canPlaceShip(player->grid, x, y, ship->size, 'h'))
            {
                for (int j = y; j < y + ship->size; j++)
                {
                    player->grid[x][j] = ship->name[0];
                }
                break;
            }
            else if (dir == 'V' && canPlaceShip(player->grid, x, y, ship->size, 'v'))
            {
                for (int j = x; j < x + ship->size; j++)
                {
                    player->grid[j][y] = ship->name[0];
                }
                break;
            }
            else
            {
                printf("Invalid placement for %s. Try again.\n", ship->name);
            }
        }
    }
}

void BOTPlaceShips(struct Player *player)
{
    for (int i = 0; i < 4; i++)
    {
        struct Ship *ship = &player->ships[i];
        int row, col;
        char dir;

        while (1)
        {

            dir = (rand() % 2 == 0) ? 'h' : 'v';
            if (dir == 'h')
            {
                row = rand() % GRID;
                col = rand() % (GRID - ship->size + 1); // ensures length of ship fits
            }
            else
            {
                row = rand() % (GRID - ship->size + 1); // ensures length of ship fits
                col = rand() % GRID;
            }

            if (canPlaceShip(player->grid, row, col, ship->size, dir))
            {

                ship->row = row;
                ship->col = col;
                ship->direction = dir;

                if (dir == 'h')
                {
                    for (int j = col; j < col + ship->size; j++) // starting col + size ship
                    {
                        player->grid[row][j] = ship->name[0];
                    }
                }
                else
                {
                    for (int j = row; j < row + ship->size; j++) // starting row + size ship
                    {
                        player->grid[j][col] = ship->name[0];
                    }
                }
                break;
            }
        }
    }
}

int countSink(struct Player *attacker, struct Player *defender)
{
    int counter = 0;
    // loop over array and x++ whenever arr[i] = 1;
    for (int i = 0; i < 4; i++)
    {
        if (defender->ships[i].sank == 1)
        {
            counter++;
        }
    }
    return counter;
}

bool isSunk(struct Player *defender, int row, int col)
{
    char shipLetter = defender->grid[row][col];
    for (int i = 0; i < 4; i++)
    {
        if (defender->ships[i].name[0] == shipLetter && defender->ships[i].sank == 1)
        {
            return true;
        }
    }
    return false; // Ship is still afloat
}

void fire_GP(struct Player *attacker, struct Player *defender, int row, int col)
{
    if (defender->grid[row][col] == 'X')
    {
        printf("Cannot attack, smoke screen is blocking this area at %c%d.\n", 'A' + col, row + 1);
        return;
    }

    if (defender->grid[row][col] == '*' || defender->grid[row][col] == 'o') // already attacked
    {
        printf("Position %c%d has already been attacked. Try a different coordinate.\n", 'A' + col, row + 1);
        return;
    }

    if (defender->grid[row][col] != '~')
    {
        if (strcmp(attacker->difficulty, "easy") == 0)
        {
            printf("Hit at %c%d!\n", 'A' + col, row + 1);
        }
        char shipLetter = defender->grid[row][col];
        defender->grid[row][col] = '*';

        for (int i = 0; i < 4; i++)
        {
            if (defender->ships[i].name[0] == shipLetter)
            {
                defender->ships[i].hits++;
                attacker->numOfHits++;
                if (defender->ships[i].hits >= defender->ships[i].size && defender->ships[i].sank == 0)
                {
                    defender->ships[i].sank = 1;
                    int x = defender->ships[i].size;
                    attacker->numOfHits -= x; // ngl not sure this works i forgot how C works
                    // ill test it out elsewhere later
                    printf("The %s has sunk!\n", defender->ships[i].name);
                    attacker->shot = true;
                }
            }
        }
    }
    else
    {
        if (strcmp(attacker->difficulty, "easy") == 0)
        {
            printf("Miss at %c%d!\n", 'A' + col, row + 1);
        }

        defender->grid[row][col] = 'o';
    }
}

void BOTFireball(struct Player *attacker, struct Player *defender)
{
    static int hitPositions[100][2];
    static int hitCount = 0;
    int directions[4] = {0, 1, 2, 3}; // up, down, left, right
    if (attacker->numOfHits == 0)
    {
        while (1)
        {
            int x = rand() % 10;
            int y = rand() % 10;
            // smokescreeen not hit at all not a hit before not a miss before, either a hit or a miss
            if (defender->grid[x][y] == 'X' || defender->grid[x][y] == '~' || (defender->grid[x][y] != '*' && defender->grid[x][y] != 'o'))
            {
                fire_GP(attacker, defender, x, y); // valid for fireballing
                if (defender->grid[x][y] == '*')
                {
                    attacker->lastHitX = x;
                    attacker->lastHitY = y;
                    hitPositions[hitCount][0] = x;
                    hitPositions[hitCount][1] = y;
                    hitCount++;
                }
                return;
            }
        }
    }
    else
    {
        for (int i = 3; i > 0; i--)
        {
            int j = rand() % (i + 1);
            int temp = directions[i];
            directions[i] = directions[j];
            directions[j] = temp;
        }

        for (int i = 0; i < hitCount; i++)
        {
            int x = hitPositions[i][0];
            int y = hitPositions[i][1];

            // around recent-est hit
            for (int j = 0; j < 4; j++)
            {
                int xx = x, yy = y;
                switch (directions[j])
                {
                case 0:
                    xx--;
                    break; // up
                case 1:
                    xx++;
                    break; // down
                case 2:
                    yy--;
                    break; // lfet
                case 3:
                    yy++;
                    break; // right
                }
                // checking to fire
                if ((xx >= 0 && yy >= 0 && xx < GRID && yy < GRID) && defender->grid[xx][yy] != '*' && defender->grid[xx][yy] != 'o')
                {
                    fire_GP(attacker, defender, xx, yy);
                    if (defender->grid[xx][yy] == '*')
                    {
                        attacker->lastHitX = xx;
                        attacker->lastHitY = yy;
                        hitPositions[hitCount][0] = xx;
                        hitPositions[hitCount][1] = yy;
                        hitCount++;
                    }
                    return;
                }
            }
        }
    }
}

void RadarSweep_GP(struct Player *attacker, struct Player *defender, int row, int col)
{
    if (attacker->Rsweep == 0)
    {
        printf("Player has already used up all of their Radar Sweeps\n");
        return;
    }
    else
    {
        attacker->Rsweep--;

        for (int i = row; i < row + 2 && i < GRID; i++)
        {
            for (int j = col; j < col + 2 && j < GRID; j++)
            {
                if (defender->grid[i][j] == 'X') // Smoke screen area
                {
                    printf("Miss at %c%d!\n", 'A' + j, i + 1);
                    return;
                }
                else if (defender->grid[i][j] != '~' && defender->grid[i][j] != 'o' && defender->grid[i][j] != '*')
                {
                    printf("Enemy ships found!\n");
                    return;
                }
            }
        }
    }
    printf("No enemy ships found\n");
    return;
}

void BotRadarSweep(struct Player *attacker, struct Player *defender) {

    if (attacker->Rsweep <= 0) {
        printf("Bot has no radar sweeps left!\n");
        return;
    }

    int row, col;
    int foundSweepLocation = 0;
    int detectedShip = 0;  // Flag to track if a ship is detected

    if (attacker->lastHitX != -1 && attacker->lastHitY != -1) {
        row = attacker->lastHitX;
        col = attacker->lastHitY;

        // Check a 2x2 
        if (row > 0 && col > 0) {

            if (defender->grid[row - 1][col - 1] != '.' && row - 1 >= 0 && col - 1 >= 0) {
                detectedShip = 1;
                attacker->detectedRow = row - 1; // Save the detected coordinates
                attacker->detectedCol = col - 1;
                printf("Ship detected at (%c%d)!\n", 'A' + col - 1, row);
            }
            RadarSweep_GP(attacker, defender, row - 1, col - 1);
            foundSweepLocation = 1;
        }

        // Check bottom-right diagonal
        if (row < GRID - 1 && col < GRID - 1) {

            if (defender->grid[row + 1][col + 1] != '.' && row + 1 < GRID && col + 1 < GRID) {
                detectedShip = 1;
                attacker->detectedRow = row + 1; // Save the detected coordinates
                attacker->detectedCol = col + 1;
                printf("Ship detected at (%c%d)!\n", 'A' + col + 1, row + 1);
            }
            RadarSweep_GP(attacker, defender, row + 1, col + 1);
            foundSweepLocation = 1;
        }
    }

    // If no previous hit area 
    if (!foundSweepLocation) {
        row = rand() % (GRID - 1);  // Random row
        col = rand() % (GRID - 1);  // Random column
        printf("Bot uses Radar Sweep at (%c%d)\n", 'A' + col, row + 1);

        if (defender->grid[row][col] != '.' && row >= 0 && col >= 0 && row < GRID && col < GRID) {
            detectedShip = 1;
            attacker->detectedRow = row; // Save the detected coordinates
            attacker->detectedCol = col;
            printf("Ship detected at (%c%d)!\n", 'A' + col, row + 1);
        }
        RadarSweep_GP(attacker, defender, row, col);
    }

    // Decrement the radar sweeps after using one
    attacker->Rsweep--;

}


void smokeScreen_GP(struct Player *attacker, struct Player *defender, int row, int col)
{
    int maxSS = countSink(attacker, defender);
    if (attacker->smokeScreens >= maxSS)
    {
        printf("You can not use more smoke screens than ships sunk. You lost your turn.\n");
        return;
    }
    if (row < 0 || row + 1 >= GRID || col < 0 || col + 1 >= GRID)
    {
        printf("Invalid smoke screen placement. Out of bounds.\n");
        return;
    }

    gridCopy(attacker->grid, attacker->copiedGird); // copied the grid to get values back later

    for (int i = row; i < row + 2 && i < GRID; i++)
    {
        for (int j = col; j < col + 2 && j < GRID; j++)
        {
            attacker->grid[i][j] = 'X';
        }
    }

    attacker->smokeScreens++;
    attacker->smokeRound = 2;

    for (int i = 0; i < 100; i++)
    {
        printf("\n");
    }
    printf("\nSmoke screen deployed!");
}

void BotRadarSweep(struct Player *attacker, struct Player *defender) {

    if (attacker->Rsweep <= 0) {
        printf("Bot has no radar sweeps left!\n");
        return;
    }

    int row, col;
    int foundSweepLocation = 0;
    int detectedShip = 0;  // Flag to track if a ship is detected

    if (attacker->lastHitX != -1 && attacker->lastHitY != -1) {
        row = attacker->lastHitX;
        col = attacker->lastHitY;

        // Check a 2x2 
        if (row > 0 && col > 0) {

            if (defender->grid[row - 1][col - 1] != '.' && row - 1 >= 0 && col - 1 >= 0) {
                detectedShip = 1;
                attacker->detectedRow = row - 1; // Save the detected coordinates
                attacker->detectedCol = col - 1;
                printf("Ship detected at (%c%d)!\n", 'A' + col - 1, row);
            }
            RadarSweep_GP(attacker, defender, row - 1, col - 1);
            foundSweepLocation = 1;
        }

        // Check bottom-right diagonal
        if (row < GRID - 1 && col < GRID - 1) {

            if (defender->grid[row + 1][col + 1] != '.' && row + 1 < GRID && col + 1 < GRID) {
                detectedShip = 1;
                attacker->detectedRow = row + 1; // Save the detected coordinates
                attacker->detectedCol = col + 1;
                printf("Ship detected at (%c%d)!\n", 'A' + col + 1, row + 1);
            }
            RadarSweep_GP(attacker, defender, row + 1, col + 1);
            foundSweepLocation = 1;
        }
    }

    // If no previous hit area 
    if (!foundSweepLocation) {
        row = rand() % (GRID - 1);  // Random row
        col = rand() % (GRID - 1);  // Random column
        printf("Bot uses Radar Sweep at (%c%d)\n", 'A' + col, row + 1);

        if (defender->grid[row][col] != '.' && row >= 0 && col >= 0 && row < GRID && col < GRID) {
            detectedShip = 1;
            attacker->detectedRow = row; // Save the detected coordinates
            attacker->detectedCol = col;
            printf("Ship detected at (%c%d)!\n", 'A' + col, row + 1);
        }
        RadarSweep_GP(attacker, defender, row, col);
    }

    // Decrement the radar sweeps after using one
    attacker->Rsweep--;

}


void checkSmokeScreen(struct Player *attacker)
{
    if (attacker->smokeRound > 0)
    {
        attacker->smokeRound--;
        if (attacker->smokeRound == 0)
        {
            for (int i = 0; i < GRID; i++)
            {
                for (int j = 0; j < GRID; j++)
                {
                    if (attacker->grid[i][j] == 'X')
                    {
                        attacker->grid[i][j] = attacker->copiedGird[i][j];
                    }
                }
            }
            printf("Smoke screen effect has expired.\n");
        }
    }
}

void artillery(struct Player *attacker, struct Player *defender, int row, int col)
{
    if (attacker->artill == 0)
    {
        printf("You have already used this function before, can't use it \n");
        return;
    }
    else if (!attacker->shot)
    {
        printf("You cannot use artillery because you did not sink a ship in your last turn.\n");
        return;
    }
    if (row < 0 || row + 1 >= GRID || col < 0 || col + 1 >= GRID)
    {
        printf("Invalid artillery placement. Out of bounds.\n");
        return;
    }

    for (int i = row; i < row + 2 && i < GRID; i++)
    {
        for (int j = col; j < col + 2 && j < GRID; j++)
        {
            if (defender->grid[i][j] != '~' && defender->grid[i][j] != 'o' && defender->grid[i][j] != '*')
            {

                printf("Hit at %c%d!\n", 'A' + j, i + 1);
                char shipLetter = defender->grid[i][j];
                for (int i = 0; i < 4; i++)
                {
                    if (defender->ships[i].name[0] == shipLetter)
                    {
                        defender->ships[i].hits++;
                        attacker->numOfHits++;
                        if (defender->ships[i].hits >= defender->ships[i].size && defender->ships[i].sank == 0)
                        {
                            defender->ships[i].sank = 1;
                            int x = defender->ships[i].size;
                            attacker->numOfHits -= x; // ngl not sure this works i forgot how C works
                            // ill test it out elsewhere later
                            printf("The %s has sunk!\n", defender->ships[i].name);
                            attacker->shot = true;
                        }
                    }
                }
                defender->grid[i][j] = '*';
            }
            else
            {
                printf("Miss at %c%d!\n", 'A' + j, i + 1);
                defender->grid[i][j] = 'o';
            }
        }
    }
    attacker->artill = 0;
    attacker->shot = false;
}

int isValidArtilleryTarget(struct Player *defender, int row, int col)
{ // to check if artillery is valid
    for (int i = row; i < row + 2; i++)
    {
        for (int j = col; j < col + 2; j++)
        {
            if (defender->grid[i][j] == '*' || defender->grid[i][j] == 'o' || defender->grid[i][j] == '~')
            {
                return 0;
            }
        }
    }
    return 1;
}

void BOTartillery(struct Player *attacker, struct Player *defender, int row, int col)
{
    int targetRow = attacker->lastHitX;
    int targetCol = attacker->lastHitY;
    int validTarget = 0;

    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            row = targetRow + dx;
            col = targetCol + dy;

            if (row >= 0 && row + 1 < GRID && col >= 0 && col + 1 < GRID)
            {
                if (isValidArtilleryTarget(defender, row, col))
                {
                    printf("\nBob performs Artillery at (%c%d)\n", 'A' + col, row + 1);
                    artillery(attacker, defender, row, col);
                    attacker->artill = 0;
                    attacker->shot = 0; // Reset shot condition
                    validTarget = 1;
                    break;
                }
            }
        }
        if (validTarget)
            break;
    }

    if (!validTarget)
    {
        // random valid 2x2 area
        while (1)
        {
            row = rand() % (GRID - 1);
            col = rand() % (GRID - 1);

            if (isValidArtilleryTarget(defender, row, col))
            {
                printf("\nBob performs Artillery at (%c%d)\n", 'A' + col, row + 1);
                artillery(attacker, defender, row, col);
                attacker->artill = 0;
                attacker->shot = 0;
                break;
            }
        }
    }
}

void torpedo(struct Player *attacker, struct Player *defender, char *type)
{

    if (attacker->torCount == 0)
    {
        attacker->tor = countSink(attacker, defender);
        attacker->torCount++;
    }

    if (attacker->tor == 0)
    {
        printf("You cannot use Torpedo at this time.\n");
        return;
    }

    int h = (*type >= 'A' && *type <= 'J');
    int v = (*type >= '1' && *type <= '9');

    if (h)
    {
        int y = *type - 'A';
        for (int i = 0; i < GRID; i++)
        {
            if (defender->grid[i][y] != '~' && defender->grid[i][y] != 'o')
            {
                defender->grid[i][y] = '*';
                printf("Hit at %c%d!\n", *type, i + 1);
            }
            else
            {
                defender->grid[i][y] = 'o';
                printf("Miss at %c%d!\n", *type, i + 1);
            }
        }
    }
    else if (v)
    {
        int y = atoi(type) - 1;
        for (int i = 0; i < GRID; i++)
        {
            if (defender->grid[i][y] != '~' && defender->grid[y][i] != 'o')
            {
                defender->grid[y][i] = '*';
                printf("Hit at %c%d!\n", 'A' + i, y + 1);
            }
            else
            {
                defender->grid[y][i] = 'o';
                printf("Miss at %c%d!\n", 'A' + i, y + 1);
            }
        }
    }
    else
    {
        printf("Invalid torpedo input.\n");
    }
    attacker->tor = 0;
}

void BOTtorpedo(struct Player *attacker, struct Player *defender, int row, int col)
{
    int row_or_col = rand() % 2;
    char type[3];

    if (attacker->numOfHits == 0)
    {
        if (row_or_col == 0)
        {
            row = rand() % GRID;
            sprintf(type, "%d", row + 1);
            printf("\nBob performed Torpedo at (%d)\n", row + 1);
        }
        else
        {
            col = 'A' + (rand() % GRID);
            type[0] = col;
            type[1] = '\0';
            printf("\nBob used Torpedo at (%c)\n", col);
        }
        torpedo(attacker, defender, type);
    }
    else
    {
        int row = attacker->lastHitX;
        int col = attacker->lastHitY;
        int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        int found = 0;
        int hitPositions[20][2];
        int hitCount = 0;

        for (int i = -2; i <= 2 && !found; i++)
        {
            for (int j = -2; j <= 2 && !found; j++)
            {
                int newRow = row + i;
                int newCol = col + j;
                if (newRow < 0 || newRow >= GRID || newCol < 0 || newCol >= GRID)
                    continue;
                if (defender->grid[newRow][newCol] == '*' && !isSunk(defender, newRow, newCol))
                {
                    hitPositions[hitCount][0] = newRow;
                    hitPositions[hitCount][1] = newCol;
                    hitCount++;
                    found = 1;
                }
            }
        }
        if (found)
        {
            if (hitCount > 0)
            {
                int attackRow = hitPositions[0][0];
                int attackCol = hitPositions[0][1];
                int row_or_col = rand() % 2;
                if (row_or_col == 0)
                {
                    sprintf(type, "%d", attackRow + 1);
                    printf("\nBob used Torpedo on Row (%d)\n", attackRow + 1);
                    torpedo(attacker, defender, type);
                }
                else
                {
                    sprintf(type, "%c", 'A' + attackCol);
                    printf("\nBob used Torpedo on Column (%c)\n", 'A' + attackCol);
                    torpedo(attacker, defender, type);
                }
            }
        }
        else
        {
            int row_or_col = rand() % 2;

            if (row_or_col == 0)
            {
                int row = rand() % GRID;
                sprintf(type, "%d", row + 1);
                printf("\nBob performed Torpedo at Row (%d)\n", row + 1);
            }
            else
            {
                int col = rand() % GRID;
                sprintf(type, "%c", 'A' + col);
                printf("\nBob used Torpedo at Column (%c)\n", 'A' + col);
            }
            torpedo(attacker, defender, type);
        }
    }
}

void gamePlay(struct Player *attacker, struct Player *defender)
{
    char input[30];
    char command[15];
    int row;  //  -> row (integer part of coordinates)
    char col; // col -> column (character part of coordinates)

    checkSmokeScreen(attacker);

    while (1)
    {
        printf("\nGame Play Commands options:\n1) Fire - Basic Attack, Free Use\n2) RadarSweep - 2x2 Area Ships Reveal, 3 Times Usage only");
        printf("\n3) SmokeScreen - 2x2 Area Ships Hide (2 Rounds Only), One Usage Per Ship Sunk \n4) Artillery - Fire Attack 2x2 Area, Next Turn After Sinking A Ship");
        printf("\n5) Torpedo - Entire Row Or Column Attack, Next Turn After Sinking 3rd Ship\n\n");
        printf("%s, Enter your Command and Coordinates (e.g., Fire B4): ", attacker->name);

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        if (sscanf(input, "%14s %c%d", command, &col, &row) == 3)
        {

            for (int i = 0; command[i]; i++)
            {
                command[i] = tolower((unsigned char)command[i]);
            }

            if (col >= 'a' && col <= 'j')
            {
                col = col - 'a' + 'A';
            }
            if (row < 1 || row > GRID || col < 'A' || col >= 'A' + GRID)
            {
                printf("Invalid coordinates. Row must be A-J and column 1-10.\n");
                continue;
            }

            row--;
            int newCol = col - 'A';

            if (strcmp(command, "fire") == 0)
            {
                fire_GP(attacker, defender, row, newCol);
                break;
            }
            else if (strcmp(command, "radarsweep") == 0)
            {
                RadarSweep_GP(attacker, defender, row, newCol);
                break;
            }
            else if (strcmp(command, "smokescreen") == 0)
            {
                smokeScreen_GP(attacker, defender, row, newCol);
                break;
            }
            else if (strcmp(command, "artillery") == 0)
            {
                artillery(attacker, defender, row, newCol);
                break;
            }
            else
            {
                printf("Unknown command or incorrect format. Please try again.\n");
            }
        }
        else if (sscanf(input, "%14s %2s", command, input) == 2)
        {
            for (int i = 0; command[i]; i++)
            {
                command[i] = tolower((unsigned char)command[i]);
            }

            if (strcmp(command, "torpedo") == 0)
            {
                if ((input[0] >= 'A' && input[0] <= 'J') || (input[0] >= '1' && input[0] <= '9') || strcmp(input, "10") == 0)
                {
                    torpedo(attacker, defender, input);
                    break;
                }
                else
                {
                    printf("Invalid Torpedo coordinate. Please enter a row (A-J) or column (1-10).\n");
                }
            }
            else
            {
                printf("Invalid Torpedo coordinate. Please enter a row (A-J) or column (1-10).\n");
            }
        }
        else
        {
            printf("Invalid input format. Please enter both command and coordinates.\n");
        }
    }
}

void BOTGamePlay(struct Player *attacker, struct Player *defender)
{
    char command[15];
    int row;
    char col;
    int maxSS = countSink(attacker, defender);

    checkSmokeScreen(attacker);

    while (1)
    {

        if (countSink(attacker, defender) >= 1 && attacker->smokeScreens < maxSS) // SMOKESCREEN
        {
            int validSmokeScreen = 0;

            for (int shipIndex = 0; shipIndex < 4; shipIndex++) // Loop through all 4 ships
            {

                int row = attacker->ships[shipIndex].row;
                int col = attacker->ships[shipIndex].col;
                int size = attacker->ships[shipIndex].size;
                char dir = attacker->ships[shipIndex].direction;

                int neededRow = -1;
                int neededCol = -1;

                if (dir == 'v')
                {
                    for (int i = row; i < row + size && i < GRID; i++)
                    {
                        if (i + 1 < GRID && col + 1 < GRID && i >= 0 && col >= 0 &&
                            attacker->grid[i][col] != '*' && attacker->grid[i][col] != 'o')
                        {
                            neededRow = i;
                            neededCol = col;
                            break; // valid
                        }
                    }
                }
                else if (dir == 'h')
                {
                    for (int j = col; j < col + size && j < GRID; j++)
                    {

                        if (row + 1 < GRID && j + 1 < GRID && row >= 0 && j >= 0 &&
                            attacker->grid[row][j] != '*' && attacker->grid[row][j] != 'o')
                        {
                            neededRow = row;
                            neededCol = j;
                            break; // valid
                        }
                    }
                }

                if (neededRow != -1 && neededCol != -1) // checks for valid location
                {
                    if (neededRow + 1 < GRID && neededCol + 1 < GRID)
                    {
                        for (int i = 0; i < 100; i++)
                        {
                            printf("\n");
                        }
                        printf("\nBob deployed smoke screen. He has mastered defending :)");
                        smokeScreen_GP(attacker, defender, neededRow, neededCol);
                        validSmokeScreen = 1;
                        break;
                    }
                }
            }

            if (validSmokeScreen == 0)
            {
                printf("\nNo valid smoke screen position found!\n");
            }
            else
            {
                break;
            }
        }

        else if (countSink(attacker, defender) >= 3 && attacker->torCount == 0) // TORPEDO
        {
            BOTtorperdo(attacker, defender, row, col);
            break;
        }

        else if (attacker->artill == 1 && attacker->shot) // ARTILLERY
        {
            BOTartillery(attacker, defender, row, col);
            break;
        }

        else // FIRE
        {
             BOTFireball(attacker, defender);
            //gamePlay(attacker, defender);
            break;
        }
    }
}

int checkWin(struct Player *player)
{
    for (int i = 0; i < 4; i++)
    {
        if (player->ships[i].sank == 0)
        {
            return 0;
        }
    }
    return 1;
}

void easyMain(struct Player *p1, struct Player *p2)
{
    int firstPlayer = randomFP();
    struct Player *rnPlayer = firstPlayer == 0 ? p1 : p2;
    struct Player *evilPlayer = firstPlayer == 0 ? p2 : p1;

    while (1)
    {
        printf("════");
        for (int i = 0; i < 5; i++)
        {
            printf("════  ⚓ ════");
        }
        printf("════");

        printf("\n");

        printf("\n%s's turn.\n", rnPlayer->name);

        printf("\n");

        printf("Opponent's grid: \n");
        gridDisplayOpp(evilPlayer->grid);

        gamePlay(rnPlayer, evilPlayer);

        if (checkWin(evilPlayer))
        {
            printf("%s wins!\n", rnPlayer->name);
            break;
        }

        struct Player *temp = rnPlayer;
        rnPlayer = evilPlayer;
        evilPlayer = temp;
    }
    return;
}

void hardMain(struct Player *p1, struct Player *p2)
{
    int firstPlayer = randomFP();
    struct Player *rnPlayer = firstPlayer == 0 ? p1 : p2;
    struct Player *evilPlayer = firstPlayer == 0 ? p2 : p1;

    while (1)
    {
        printf("════");
        for (int i = 0; i < 5; i++)
        {
            printf("════  ⚓ ════");
        }
        printf("════");

        printf("\n");

        printf("\n%s's turn.\n", rnPlayer->name);

        printf("\n");

        printf("Opponent's grid: \n");
        gridDisplayOppHARD(evilPlayer->grid);

        gamePlay(rnPlayer, evilPlayer);

        if (checkWin(evilPlayer))
        {
            printf("%s wins!\n", rnPlayer->name);
            break;
        }

        struct Player *temp = rnPlayer;
        rnPlayer = evilPlayer;
        evilPlayer = temp;
    }
    return;
}

void botMain(struct Player *p1, struct Player *p2)
{
    int firstPlayer = randomFP();
    struct Player *rnPlayer = firstPlayer == 0 ? p1 : p2;
    struct Player *evilPlayer = firstPlayer == 0 ? p2 : p1;

    while (1)
    {
        printf("════");
        for (int i = 0; i < 5; i++)
        {
            printf("════  ⚓ ════");
        }
        printf("════");

        printf("\n");

        printf("\n%s's turn.\n", rnPlayer->name);

        printf("\n");

        printf("Opponent's grid: \n");
        gridDisplayOpp(evilPlayer->grid);

        if (strcmp(rnPlayer->name, "Bob") == 0)
        {
            BOTGamePlay(rnPlayer, evilPlayer);
        }
        else
        {
            gamePlay(rnPlayer, evilPlayer);
        }

        if (checkWin(evilPlayer))
        {
            printf("%s wins!\n", rnPlayer->name);
            break;
        }

        struct Player *temp = rnPlayer;
        rnPlayer = evilPlayer;
        evilPlayer = temp;
    }
    return;
}

int main()
{
    struct Player p1, p2;
    srand(0);

    int numPlayers;

    while (1)
    {
        printf("Are there 1 or 2 players? (1 or 2): ");
        scanf("%d", &numPlayers);

        if (numPlayers == 1 || numPlayers == 2)
        {
            break;
        }
        else
        {
            printf("Invalid choice. Please enter '1' or '2'.\n");
        }
    }

    char difficulty[10];

    if (numPlayers == 2)
    {
        while (1)
        {
            printf("Choose difficulty level (easy/hard): ");
            scanf("%s", difficulty);

            for (int i = 0; difficulty[i] != '\0'; i++)
            {
                difficulty[i] = tolower((unsigned char)difficulty[i]);
            }

            if (strcmp(difficulty, "easy") == 0 || strcmp(difficulty, "hard") == 0)
            {
                break;
            }
            else
            {
                printf("Invalid choice. Please enter 'easy' or 'hard'.\n");
            }
        }
    }
    else
    {
        strcpy(difficulty, "easy");
    }

    strcpy(p1.difficulty, difficulty);
    strcpy(p2.difficulty, p1.difficulty);

    printf("Enter the name for Player 1: ");
    scanf("%s", p1.name);

    if (numPlayers == 2)
    {
        printf("Enter the name for Player 2: ");
        scanf("%s", p2.name);
    }
    else
    {
        strcpy(p2.name, "Bob");
        printf("You are playing against our bot, Bob!\n");
    }

    printf("For player 1:\n");
    p1.Rsweep = 3;
    p1.smokeScreens = 0;
    p1.torCount = 0;
    p1.shot = false;
    p1.artill = 1;
    gridStart(p1.grid);
    createShip(&p1);
    placeShip(&p1);
    printf("Player 1's grid:\n");
    gridDisplay(p1.grid);

    while (getchar() != '\n')
        ;
    printf("Press Enter to continue...");
    while (getchar() != '\n')
        ;
    for (int i = 0; i < 100; i++)
    {
        printf("\n");
    }

    if (numPlayers == 2)
    {
        printf("For player 2:\n");
        p2.Rsweep = 3;
        p2.smokeScreens = 0;
        p2.torCount = 0;
        p2.shot = false;
        p2.artill = 1;
        gridStart(p2.grid);
        createShip(&p2);
        placeShip(&p2);
        printf("Player 2's grid:\n");
        gridDisplay(p2.grid);

        while (getchar() != '\n')
            ;
        printf("Press Enter to continue...");
        while (getchar() != '\n')
            ;
        for (int i = 0; i < 100; i++)
        {
            printf("\n");
        }
    }
    else
    {
        printf("For Player 2 (Bob):\n");
        p2.Rsweep = 3;
        p2.smokeScreens = 0;
        p2.torCount = 0;
        p2.shot = false;
        p2.artill = 1;
        p2.botTurn = 0;
        p2.numOfHits = 0;
        p2.lastHitX = -1;
        p2.lastHitY = -1;
        gridStart(p2.grid);
        createShip(&p2);
        BOTPlaceShips(&p2);
        // wont display
        printf("Bob's Grid:\n");
        gridDisplay(p2.grid);
        printf("Bob's Gird has been created successfully!\n");
    }

    if (numPlayers == 1)
    {
        botMain(&p1, &p2);
    }
    else
    {
        if (strcmp(p1.difficulty, "easy") == 0)
        {
            easyMain(&p1, &p2);
        }
        else if (strcmp(p1.difficulty, "hard") == 0)
        {
            hardMain(&p1, &p2);
        }
    }

    return 0;
}
