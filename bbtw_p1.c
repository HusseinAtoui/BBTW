#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Phase one, Draft 5

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
    char difficulty[10];
    struct Ship ships[4];
    int Rsweep;
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

void playerTurn(struct Player *attacker, struct Player *defender)
{
    int x, y;
    char col;

    while (1)
    {
        printf("%s, enter your attack coordinates (for example, A4): ", attacker->name);
        int read = scanf(" %c%d", &col, &x);
        x--;

        if (read != 2 || x < 0 || x >= GRID || col < 'A' || col > 'J')
        {
            printf("Invalid coordinates. come on try again.\n");
            continue;
        }

        y = col - 'A';

        if (defender->grid[x][y] == '*' || defender->grid[x][y] == 'o')
        {
            printf("You already attacked this position. yalla again.\n");
            continue;
        }

        if (defender->grid[x][y] != '~')
        {
            printf("Hit at %c%d!\n", 'A' + y, x + 1);
            defender->grid[x][y] = '*';
            for (int i = 0; i < 4; i++)
            {
                if (defender->ships[i].name[0] == defender->grid[x][y])
                {
                    defender->ships[i].hits++;
                }
            }
        }
        else
        {
            printf("Miss at %c%d!\n", 'A' + y, x + 1);
            defender->grid[x][y] = 'o';
        }
        break;
    }
}

void fire_GP(struct Player *attacker, struct Player *defender, int x, int y)
{

    if (defender->grid[x][y] == '*' || defender->grid[x][y] == 'o') // already attacked
    {
        printf("Position %c%d has already been attacked. Try a different coordinate.\n", 'A' + y, x + 1);
        return;
    }

    if (defender->grid[x][y] != '~')
    {
        printf("Hit at %c%d!\n", 'A' + y, x + 1);
        defender->grid[x][y] = '*';
        for (int i = 0; i < 4; i++)
        {
            if (defender->ships[i].name[0] == defender->grid[x][y])
            {
                defender->ships[i].hits++;
            }
        }
    }
    else
    {
        printf("Miss at %c%d!\n", 'A' + y, x + 1);
        defender->grid[x][y] = 'o';
    }
}

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
                if (defender->grid[i][j] != '~' && defender->grid[i][j] != 'o' && defender->grid[i][j] != '*')
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

void gamePlay(struct Player *attacker, struct Player *defender)
{
    char command[15];
    int x;    // x -> row
    char col; // col for column character

    while (1)
    {
        printf("Game Play Commands: Fire / RadarSweep / SmokeScreen / Artillery / Torpedo\n");
        printf("%s, Enter your Command (For example, Fire): ", attacker->name);

        if (scanf("%14s", command) != 1)
        {
            printf("Invalid command. Try again.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        printf("%s, enter your attack coordinates (for example, A4): ", attacker->name);

        if (scanf(" %c%d", &col, &x) != 2 || x < 1 || x > GRID || col < 'A' || col >= 'A' + GRID)
        {
            printf("Invalid coordinates. Please try again.\n");
            while (getchar() != '\n')
                ;
            continue;
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
        else
        {
            printf("Unknown command or incorrect format. Try again.\n");
            while (getchar() != '\n')
                ;
        }
    }
}

int checkWin(struct Player *player)
{
    int shipDone = 1;
    for (int i = 0; i < 4; i++)
    {
        if (player->ships[i].hits >= player->ships[i].size && player->ships[i].sank == 0)
        {
            player->ships[i].sank++;
            printf("The %s has sunk!\n", player->ships[i].name);
        }
        else
        {
            shipDone = 0;
        }
    }
    return shipDone;
}

int main()
{
    struct Player p1, p2;
    srand(0);

    printf("Choose difficulty level(easy/hard): ");
    scanf("%s", p1.difficulty);
    strcpy(p2.difficulty, p1.difficulty);

    printf("Enter the name for Player 1: ");
    scanf("%s", p1.name);
    printf("Enter the name for Player 2: ");
    scanf("%s", p2.name);

    printf("For player 1:\n");
    p1.Rsweep = 3;
    gridStart(p1.grid);
    shipsFR(&p1);
    placeShip(&p1);
    printf("Player 1's grid:\n");
    gridDisplay(p1.grid);

    printf("For player 2:\n");
    p2.Rsweep = 3;
    gridStart(p2.grid);
    shipsFR(&p2);
    placeShip(&p2);
    printf("Player 2's grid:\n");
    gridDisplay(p2.grid);

    int firstPlayer = randomFP();
    struct Player *rnPlayer = firstPlayer == 0 ? &p1 : &p2;
    struct Player *evilPlayer = firstPlayer == 0 ? &p2 : &p1;

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
        gridDisplayOpp(evilPlayer->grid);

        //  playerTurn(rnPlayer, evilPlayer); // gameplay instead
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

    return 0;
}
