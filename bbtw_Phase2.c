#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// BattleShips Beneath The Waves - Phase 2 - correct reading

#define GRID 10

struct Ship
{
    char name[20];
    int size;
    int hits;
    int sank;
};

struct Player
{
    char name[50];
    char grid[GRID][GRID];
    char copiedGird[GRID][GRID];
    char hitMissGrid[GRID][GRID];
    char difficulty[10];
    struct Ship ships[4];
    int Rsweep;
    int smokeScreens;
    int smokeRound;
    int artill;
    bool shot;
    int tor;
    int torCount;
    int botTurn;

    int lastHit;    // 0 for miss, 1 for hit
    int lastHitRow; // Row index of the last hit
    int lastHitCol; // Column index of the last hit
    int botleft;
    int botright;
    int botup;
    int botdown;
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
    player->ships[0] = (struct Ship){"Carrier", 5, 0, 0};
    player->ships[1] = (struct Ship){"Battleship", 4, 0, 0};
    player->ships[2] = (struct Ship){"Destroyer", 3, 0, 0};
    player->ships[3] = (struct Ship){"Submarine", 2, 0, 0};
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
                if (defender->ships[i].hits >= defender->ships[i].size && defender->ships[i].sank == 0)
                {
                    defender->ships[i].sank = 1;
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

    printf("Smoke screen deployed at %c%d.\n", 'A' + col, row + 1);
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

/*
void BOTFireball(struct Player *attacker, struct Player *defender)
{
    int x = -1, y = -1;
    for (int i = 0; i < GRID; i++)
    {
        for (int j = 0; j < GRID; j++)
        {
            if (defender->grid[i][j] == 'o' && defender->grid[i][j] != '~')
            {
                int directions[4] = {0, 1, 2, 3};
                for (int k = 0; k < 4; k++)
                {
                    int r = rand() % 4;
                    int temp = directions[k];
                    directions[k] = directions[r];
                    directions[r] = temp;
                }
                for (int d = 0; d < 4; d++)
                {
                    int xx = i, yy = j;
                    if (directions[d] == 0)
                        xx--; // up
                    else if (directions[d] == 1)
                        xx++; // down
                    else if (directions[d] == 2)
                        yy--; // lfet
                    else if (directions[d] == 3)
                        yy++; // right
                    if (xx >= 0 && xx < GRID && yy >= 0 && yy < GRID &&
                        defender->grid[xx][yy] != '*' && defender->grid[xx][yy] != 'o')
                    {
                        x = xx;
                        y = yy;
                        break;
                    }
                }
                if (x != -1 && y != -1)
                    break;
            }
        }
        if (x != -1 && y != -1)
            break;
    }
    if (x == -1 && y == -1)
    {
        x = rand() % GRID;
        y = rand() % GRID;
    }
    fire_GP(attacker, defender, x, y);
}*/

void BOTFireball(struct Player *attacker, struct Player *defender)
{
    int row, col;

    // Directions array for adjacent cells: Up, Down, Left, Right
    int directions[4][2] = {
        {-1, 0}, // Up
        {1, 0},  // Down
        {0, -1}, // Left
        {0, 1},  // Right
    };

    /*
    if (attacker->lastHit = 1)
    {
        if (true) // left
        {
            row = attacker->lastHitRow;
            col = attacker->lastHitCol - 1; // Left is column - 1

            if (col > 0 && defender->grid[row][col] != '*' && defender->grid[row][col] != 'o')
            {
                fire_GP(attacker, defender, row, col);
                if (defender->grid[row][col] == '*')
                {
                    attacker->botleft = 1;
                }
            }
        }
        return;
    }
    */

    // random
    do
    {
        row = rand() % 10;
        col = rand() % 10;
    } while (defender->grid[row][col] == '*' || defender->grid[row][col] == 'o');

    fire_GP(attacker, defender, row, col);

    if (defender->grid[row][col] == '*')
    {
        attacker->lastHit = 1;
        attacker->lastHitRow = row;
        attacker->lastHitCol = col;
    }
    else
    {
        attacker->lastHit = 0;
    }

    printf("Bot fired at position: (%d, %d). Result: %s\n",
           row, col, (defender->grid[row][col] == '*' ? "Hit" : "Miss"));
}

void BOTGamePlay(struct Player *attacker, struct Player *defender)
{
    char command[15];
    int row;
    char col;

    checkSmokeScreen(attacker);

    while (1)
    {

        int randComm = (rand()) % 3;

        if (attacker->artill == 1 && attacker->shot) // artil
        {
            // chose random nb row or col,  will decide randomly which to choose
            int row;
            char col;

            row = rand() % (GRID - 1);
            printf("\n%d", row);

            col = 'A' + (rand() % (GRID - 1)); // ensures bounds
            printf("\n%c", col);

            int colIndex = col - 'A';

            artillery(attacker, defender, row, colIndex);
            break;
        }
        else if (countSink(attacker, defender) >= 3 && attacker->torCount == 0) // torpedo
        {
            if (attacker->lastHit) // generates depending on the last
            {
                int row_or_col = rand() % 2;
                char type[3];

                if (row_or_col == 0)
                {
                    // use last row to get it
                    sprintf(type, "%d", attacker->lastHitRow);
                }
                else
                {

                    type[0] = 'A' + attacker->lastHitCol; // Convert column index to letter
                    type[1] = '\0';
                }

                torpedo(attacker, defender, type);
                break;
            }
            else
            {
                // random

                int row_or_col = rand() % 2; // chose random nb for row or col, and will decide randomly which to choose late
                char type[3];

                if (row_or_col == 0)
                {
                    int row = rand() % GRID + 1; // got rand nb
                    sprintf(type, "%d", row);    // converts from string to char
                }
                else
                {
                    char col = 'A' + (rand() % GRID); // gets rand letter
                    type[0] = col;
                    type[1] = '\0';
                }

                torpedo(attacker, defender, type);
                break;
            }
            break;
        }
        else
        {
            gamePlay(attacker, defender);
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
    p1.lastHit = 0;
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
        p2.lastHit = 0;
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
        p2.lastHit = 0;
        gridStart(p2.grid);
        createShip(&p2);
        BOTPlaceShips(&p2);
        // wont display
        // printf("Bob's Grid:\n");
        // gridDisplay(p2.grid);
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
