#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// BattleShips Beneath The Waves - Phase 1

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

void shipsFR(struct Player *player)
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
            printf("Enter the coordinates for the %s (%d) (foe example, D7 h or v): ", ship->name, ship->size);
            char col, dir;
            int read = scanf(" %c%d %c", &col, &x, &dir);
            x--;

            if (read != 3 || x < 0 || x >= GRID || col < 'A' || col > 'J')
            {
                printf("Invalid input. Please try again.\n");
                while (getchar() != '\n')
                    ;
                continue;
            }

            y = col - 'A';

            if (dir == 'h' && canPlaceShip(player->grid, x, y, ship->size, 'h'))
            {
                for (int j = y; j < y + ship->size; j++)
                {
                    player->grid[x][j] = ship->name[0];
                }
                break;
            }
            else if (dir == 'v' && canPlaceShip(player->grid, x, y, ship->size, 'v'))
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
        int x, y;
        char dir;

        while (1)
        {
            dir = (rand() % 2 == 0) ? 'h' : 'v';
            if (dir == 'h')
            {
                x = rand() % GRID;
                y = rand() % (GRID - ship->size + 1);
            }
            else
            {
                x = rand() % (GRID - ship->size + 1);
                y = rand() % GRID;
            }
            if (canPlaceShip(player->grid, x, y, ship->size, dir))
            {
                if (dir == 'h')
                {
                    for (int j = y; j < y + ship->size; j++)
                    {
                        player->grid[x][j] = ship->name[0];
                    }
                }
                else
                {
                    for (int j = x; j < x + ship->size; j++)
                    {
                        player->grid[j][y] = ship->name[0];
                    }
                }
                break;
            }
        }
    }
}

int countSink(struct Player *attacker, struct Player *defender)
{
    int x = 0;
    // loop over array and x++ whenever arr[i] = 1;
    for (int i = 0; i < 4; i++)
    {
        if (defender->ships[i].sank == 1)
        {
            x++;
        }
    }
    return x;
}

void fire_GP(struct Player *attacker, struct Player *defender, int x, int y)
{
    if (defender->grid[x][y] == 'X')
    {
        printf("Cannot attack, smoke screen is blocking this area at %c%d.\n", 'A' + y, x + 1);
        return;
    }

    if (defender->grid[x][y] == '*' || defender->grid[x][y] == 'o') // already attacked
    {
        printf("Position %c%d has already been attacked. Try a different coordinate.\n", 'A' + y, x + 1);
        return;
    }

    if (defender->grid[x][y] != '~')
    {
        if (strcmp(attacker->difficulty, "easy") == 0)
        {
            printf("Hit at %c%d!\n", 'A' + y, x + 1);
        }
        char shipLetter = defender->grid[x][y];
        defender->grid[x][y] = '*';

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
            printf("Miss at %c%d!\n", 'A' + y, x + 1);
        }

        defender->grid[x][y] = 'o';
    }
}

void BOTFireball(struct Player *attacker, struct Player *defender) {
    int x = -1, y = -1;
    for (int i = 0; i < GRID; i++) {
        for (int j = 0; j < GRID; j++) {
            if (defender->grid[i][j] == 'o' && defender->grid[i][j] != '~') {
                int directions[4] = {0, 1, 2, 3};
                for (int k = 0; k < 4; k++) {
                    int r = rand() % 4;
                    int temp = directions[k];
                    directions[k] = directions[r];
                    directions[r] = temp;
                }
                for (int d = 0; d < 4; d++) {
                    int xx = i, yy = j;
                    if (directions[d] == 0) xx--; //up
                    else if (directions[d] == 1) xx++; //down
                    else if (directions[d] == 2) yy--; //lfet
                    else if (directions[d] == 3) yy++; //right
                    if (xx>=0 && xx<GRID && yy >= 0 && yy < GRID &&
                        defender->grid[xx][yy] != '*' && defender->grid[xx][yy] != 'o') {
                        x = xx;
                        y = yy;
                        break;
                    }
                }
                if (x!=-1 && y!=-1) break;
            }
        }
        if (x!=-1 && y!=-1) break;
    }
    if (x == -1 && y == -1) {
        x = rand() % GRID;
        y = rand() % GRID;
    }
    fire_GP(attacker, defender, x, y);
}
//I forgot to do this bas eza there is a smokescreened area it doesnt attack it again
void RadarSweep_GP(struct Player *attacker, struct Player *defender, int x, int y)
{
    if (attacker->Rsweep == 0)
    {
        printf("Player has already used up all of their Radar Sweeps\n");
        return;
    }
    else
    {
        attacker->Rsweep--;

        for (int i = x; i < x + 2 && i < GRID; i++)
        {
            for (int j = y; j < y + 2 && j < GRID; j++)
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

/*ok so for BOTRadarSweep, first it makes sure it can do radarsweep,
then it randomizes a place on the grid to radar sweep.
eza ma la2a ships, byetkhatta
w eza tole3 aa smokescreen area kamen byetkhatta
eza it found a ship, it saves the place so that it can attack it next time
either with fireball or artillery
*/

void smokeScreen_GP(struct Player *attacker, struct Player *defender, int x, int y)
{
    int maxSS = countSink(attacker, defender);
    if (attacker->smokeScreens >= maxSS)
    {
        printf("You can not use more smoke screens than ships sunk. You lost your turn.\n");
        return;
    }
    if (x < 0 || x + 1 >= GRID || y < 0 || y + 1 >= GRID)
    {
        printf("Invalid smoke screen placement. Out of bounds.\n");
        return;
    }

    gridCopy(attacker->grid, attacker->copiedGird); // copied the grid to get values back later

    for (int i = x; i < x + 2 && i < GRID; i++)
    {
        for (int j = y; j < y + 2 && j < GRID; j++)
        {
            attacker->grid[i][j] = 'X';
        }
    }

    attacker->smokeScreens++;
    attacker->smokeRound = 2;

    printf("Smoke screen deployed at %c%d.\n", 'A' + y, x + 1);
}

/*
whenever it is available, the bot makes a wager. eza byaamel smokescreen,
the bot randomizes a location that 
a. is within bounds
b. isnt hiding any alr attacked ships, or areas with 3+ attacks
*/

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

void artillery(struct Player *attacker, struct Player *defender, int x, int y)
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
    if (x < 0 || x + 1 >= GRID || y < 0 || y + 1 >= GRID)
    {
        printf("Invalid artillery placement. Out of bounds.\n");
        return;
    }
    for (int i = x; i < x + 2 && i < GRID; i++)
    {
        for (int j = y; j < y + 2 && j < GRID; j++)
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
} /*
again, does a wager when available. Hayde logic tab3a lowkey metel fireball
we have a hit on a ship -> attack hawaleya
we dont -> attack randomly
*/

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

/*
kamen same logic metel fireball lowkey
except now we also check eza we have a hit men abel, which row/column has least
bot hits so that it can attack the one where we have most to gain basically.*/

void gamePlay(struct Player *attacker, struct Player *defender)
{
    char command[15];
    int x;    // x -> row
    char col; // col for column character

    checkSmokeScreen(attacker);

    while (1)
    {
        printf("Game Play Commands: Fire / RadarSweep / SmokeScreen / Artillery / Torpedo\n");
        printf("%s, Enter your Command (For example, Fire): ", attacker->name);

        if (scanf("%14s", command) != 1)
        {
            printf("Invalid command. Try again.\n");
            while (getchar() != '\n')
                continue;
        }

        // coordinates
        if (strcmp(command, "Fire") == 0 || strcmp(command, "RadarSweep") == 0 || strcmp(command, "SmokeScreen") == 0 || strcmp(command, "Artillery") == 0)
        {
            printf("%s, enter your attack coordinates (for example, A4): ", attacker->name);

            if (scanf(" %c%d", &col, &x) != 2 || x < 1 || x > GRID || col < 'A' || col >= 'A' + GRID)
            {
                printf("Invalid coordinates. Please try again.\n");
                while (getchar() != '\n')
                    ;
                continue;
            }
        }

        x--;
        int y = col - 'A';

        if (strcmp(command, "Fire") == 0)
        {
            fire_GP(attacker, defender, x, y);
            break;
        }
        else if (strcmp(command, "RadarSweep") == 0)
        {
            RadarSweep_GP(attacker, defender, x, y);
            break;
        }
        else if (strcmp(command, "SmokeScreen") == 0)
        {
            smokeScreen_GP(attacker, defender, x, y);
            break;
        }
        else if (strcmp(command, "Artillery") == 0)
        {
            artillery(attacker, defender, x, y);
            break;
        }
        else if (strcmp(command, "Torpedo") == 0)
        {
            char ty;
            printf("Enter your attack coordinates (for example, A or 3): ");
            char tortype[3];
            scanf("%s", tortype);

            if ((tortype[0] >= 'A' && tortype[0] <= 'J') || (tortype[0] >= '1' && tortype[0] <= '9') || (tortype[0] == '1' && tortype[0] == '0'))
            {
                torpedo(attacker, defender, tortype);
                break;
            }
            else
            {
                printf("Invalid coordinate. ");
                continue;
            }
        }

        else
        {
            printf("Unknown command or incorrect format. Try again.\n");
            while (getchar() != '\n');
        }
    }
}

void BOTGamePlay(struct Player *attacker, struct Player *defender)
{
    char command[15];
    int x;    // x -> row
    char col; // col for column character

    checkSmokeScreen(attacker);

    while (1)
    {
        int randComm = (rand())%3;
        //check if artillery is available
        //if yes, yes/no for BOTArtillery, 1 FOR YES, 0 OF NO
        //IF it were 0 for no, continue to the rest

        //do the same for torpedo

        //if randcomm == 3, check if they can do smokescreen frs
        //if yes, kamen random between 0 and 1
        //same thing bas this time
        //randComm = (rand())%2;
        if (attacker->botTurn == 0 || attacker->botTurn == 1 || randComm == 0){
            BOTFireball(attacker, defender);
        }
        else if (randComm == 1){
            BOTRadarSweep(attacker, defender); //I have not implemented this.
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
    //checks if we're playing against person or Bob
    int firstPlayer = randomFP();
    struct Player *rnPlayer = firstPlayer == 0 ? p1 : p2;
    struct Player *evilPlayer = firstPlayer == 0 ? p2 : p1;

    while (1)
    {
        for (int i = 0; i < 55; i++)
        {
            printf("-");
        }

        printf("\n");

        printf("\n%s's turn.\n", rnPlayer->name);

        printf("\n");

        printf("Opponent's grid: \n");
        gridDisplayOpp(evilPlayer->grid);

        //if rnPlayer != BOT
        gamePlay(rnPlayer, evilPlayer);

        //if rnPlayer == BOT 
        //BOTGamePlay(rnPlayer, evilPlayer); 

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
        for (int i = 0; i < 55; i++)
        {
            printf("-");
        }

        printf("\n");

        printf("\n%s's turn.\n", rnPlayer->name);
        printf("\n%s's grid:\n", rnPlayer->name);
        gridDisplay(rnPlayer->grid);

        printf("\n");

        printf("Opponent's grid: \n");
        gridDisplayOppHARD(evilPlayer->grid);

        //if rnPlayer != BOT
        gamePlay(rnPlayer, evilPlayer);

        //if rnPlayer == BOT 
        //BOTGamePlay(rnPlayer, evilPlayer); 

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

    int numPlayers; //honestly might make this a global variable or smth so that I can keep track
    printf("Are there 1 or 2 players? (1 or 2): ");
    scanf("%d", &numPlayers);

    printf("Choose difficulty level(easy/hard): ");
    scanf("%s", p1.difficulty);
    strcpy(p2.difficulty, p1.difficulty);

    printf("Enter the name for Player 1: ");
    scanf("%s", p1.name);
    if (numPlayers == 2) {
        printf("Enter the name for Player 2: ");
        scanf("%s", p2.name);
    } else {
        strcpy(p2.name, "Bob");
        printf("You are playing against our bot, Bob!\n");
    }

    printf("For player 1:\n");
    p1.Rsweep = 3;
    p1.smokeScreens = 0;
    p1.torCount = 0;
    p1.shot = false;
    p1.artill = 1;
    p1.botTurn = 0;
    gridStart(p1.grid);
    shipsFR(&p1);
    placeShip(&p1);
    printf("Player 1's grid:\n");
    gridDisplay(p1.grid);

    while (getchar() != '\n');
    printf("Press Enter to continue...");
    while (getchar() != '\n');
    for (int i = 0; i < 100; i++)
    {
        printf("\n");
    }

    if (numPlayers == 2){
        printf("For player 2:\n");
        p2.Rsweep = 3;
        p2.smokeScreens = 0;
        p2.torCount = 0;
        p2.shot = false;
        p2.artill = 1;
        p2.botTurn = 0;
        gridStart(p2.grid);
        shipsFR(&p2);
        placeShip(&p2);
        printf("Player 2's grid:\n");
        gridDisplay(p2.grid);

        while (getchar() != '\n');
        printf("Press Enter to continue...");
        while (getchar() != '\n');
        for (int i = 0; i < 100; i++)
        {
            printf("\n");
        }
    } else {
        printf("For Player 2 (Bob):\n");
        p2.Rsweep = 3;
        p2.smokeScreens = 0;
        p2.torCount = 0;
        p2.shot = false;
        p2.artill = 1;
        p2.botTurn = 0;
        gridStart(p2.grid);
        shipsFR(&p2);
        BOTPlaceShips(&p2);
        printf("Bob's Grid:\n");
        gridDisplay(p2.grid);
    }

    if (strcmp(p1.difficulty, "easy") == 0)
    {
        easyMain(&p1, &p2);
    }
    else if (strcmp(p1.difficulty, "hard") == 0)
    {
        hardMain(&p1, &p2);
    }

    return 0;
}