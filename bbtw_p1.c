#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Phase one, Draft 1 

#define GRID 10 

struct Ship
{
    char name[20];
    int size;
    int hits;
};

struct Player
{
    char name[50];
    char grid[GRID][GRID];
    char difficulty[10];
    struct Ship ships[4];
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
    printf("  A B C D E F G H I J\n");
    for (int i = 0; i < GRID; i++)
    {
        printf("%d ", i + 1);
        for (int j = 0; j < GRID; j++)
        {
            printf("%c ", grid[i][j]);
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
    player->ships[0] = (struct Ship){"Carrier", 5, 0};
    player->ships[1] = (struct Ship){"Battleship", 4, 0};
    player->ships[2] = (struct Ship){"Destroyer", 3, 0};
    player->ships[3] = (struct Ship){"Submarine", 2, 0};
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

        if (defender->grid[x][y] == 'X' || defender->grid[x][y] == 'M')
        {
            printf("You already attacked this position. yalla again.\n");
            continue;
        }

        if (defender->grid[x][y] != '~')
        {
            printf("Hit at %c%d!\n", 'A' + y, x + 1);
            defender->grid[x][y] = 'X';
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
            defender->grid[x][y] = 'M';
        }
        break;
    }
}

int checkWin(struct Player *player)
{
    for (int i = 0; i < 4; i++)
    {
        if (player->ships[i].hits >= player->ships[i].size)
        {
            printf("%s has sunk the %s!\n", player->name, player->ships[i].name);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (player->ships[i].hits < player->ships[i].size)
        {
            return 0;
        }
    }
    return 1;
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
    gridStart(p1.grid);
    shipsFR(&p1);
    placeShip(&p1);
    printf("Player 1's grid:\n");
    gridDisplay(p1.grid);

    printf("For player 2:\n");
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
        printf("\n%s's turn. Opponent's grid:\n", rnPlayer->name);
        gridDisplay(evilPlayer->grid);

        playerTurn(rnPlayer, evilPlayer);
        printf("\n%s's grid:\n", rnPlayer->name);
        gridDisplay(rnPlayer->grid);

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
