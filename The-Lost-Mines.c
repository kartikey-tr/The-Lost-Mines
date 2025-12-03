#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

// --- SETTINGS ---
#define WIDTH 40
#define HEIGHT 20
#define VIEW_DISTANCE 6  
#define MAX_ENEMIES 6
#define MAX_HP 100
#define TARGET_DIAMONDS 3

// --- GLOBAL VARIABLES ---
char map[HEIGHT][WIDTH];       
int  memory[HEIGHT][WIDTH];    

int playerX, playerY;
int playerHP;
int diamondsCollected;
bool gameRunning;
char message[60];

struct Enemy {
    int x, y;
    bool alive;
};
struct Enemy enemies[MAX_ENEMIES];

// --- HELPER FUNCTIONS ---

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void goToXY(int x, int y) {
    COORD c; c.X = x; c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// --- GAME LOGIC ---

void resetGame() {
	system("cls");
	
	
    playerHP = MAX_HP;
    diamondsCollected = 0;
    gameRunning = true;
    sprintf(message, "Collect %d Diamonds to open the Gate.", TARGET_DIAMONDS);

    // Clear Map
    for(int y=0; y<HEIGHT; y++) {
        for(int x=0; x<WIDTH; x++) {
            map[y][x] = '#';
            memory[y][x] = 0; 
        }
    }

    // Dig Cave
    int minerX = WIDTH / 2;
    int minerY = HEIGHT / 2;
    map[minerY][minerX] = ' '; 

    for(int i=0; i<900; i++) { 
        int dir = rand() % 4; 
        if(dir == 0 && minerY > 1) minerY--;
        if(dir == 1 && minerY < HEIGHT-2) minerY++;
        if(dir == 2 && minerX > 1) minerX--;
        if(dir == 3 && minerX < WIDTH-2) minerX++;
        map[minerY][minerX] = ' '; 
    }

    // Place Player
    do { playerX = rand() % WIDTH; playerY = rand() % HEIGHT; } while(map[playerY][playerX] != ' ');

    // Place Diamonds (♦)
    for(int i=0; i<TARGET_DIAMONDS; i++) {
        int rx, ry;
        do { rx = rand() % WIDTH; ry = rand() % HEIGHT; } while(map[ry][rx] != ' ');
        map[ry][rx] = 'D'; 
    }

    // Place Hearts (♥)
    for(int i=0; i<3; i++) {
        int hx, hy;
        do { hx = rand() % WIDTH; hy = rand() % HEIGHT; } while(map[hy][hx] != ' ');
        map[hy][hx] = 'H'; 
    }

    // Place Gate (∩)
    int ex, ey;
    do { ex = rand() % WIDTH; ey = rand() % HEIGHT; } while(map[ey][ex] != ' ');
    map[ey][ex] = 'G'; 

    // Place Enemies (Ψ)
    for(int i=0; i<MAX_ENEMIES; i++) {
        enemies[i].alive = true;
        do {
            enemies[i].x = rand() % WIDTH;
            enemies[i].y = rand() % HEIGHT;
        } while(map[enemies[i].y][enemies[i].x] != ' ' || (abs(enemies[i].x - playerX) < 8));
    }
}

void drawGame() {
    goToXY(0, 0); 

    for(int y=0; y<HEIGHT; y++) {
        for(int x=0; x<WIDTH; x++) {
            
            int distX = abs(x - playerX);
            int distY = abs(y - playerY);
            bool isVisible = (distX < VIEW_DISTANCE && distY < VIEW_DISTANCE/2);

            bool isEnemyHere = false;
            if(isVisible) {
                for(int i=0; i<MAX_ENEMIES; i++) {
                    if(enemies[i].alive && enemies[i].x == x && enemies[i].y == y) isEnemyHere = true;
                }
            }

            if(x == playerX && y == playerY) {
                if(playerHP < 30) setColor(12); // Player turns Red if low HP
                else setColor(11); // Cyan
                printf("Ω"); 
            }
            else if (isEnemyHere) {
                setColor(12); printf("Ψ"); 
            }
            else if (isVisible) {
                memory[y][x] = 1; 
                char tile = map[y][x];
                if(tile == '#')      { setColor(8);  printf("▓"); }
                else if(tile == 'D') { setColor(14); printf("♦"); } 
                else if(tile == 'H') { setColor(13); printf("♥"); } 
                else if(tile == 'G') { setColor(10); printf("∩"); } 
                else                 { setColor(6);  printf("."); } 
            }
            else if (memory[y][x] == 1) {
                if(map[y][x] == '#') { setColor(8); printf("░"); } 
                else                 { setColor(0); printf(" "); }
            }
            else {
                setColor(0); printf(" "); 
            }
        }
        printf("\n");
    }

    // UI
    setColor(7);
    printf("\n MSG: %-40s   \n", message); 
    printf(" HP: ");
    if(playerHP < 30) setColor(4); else setColor(10);
    printf("%d/%d", playerHP, MAX_HP);
    setColor(7);
    printf("  |  Diamonds: ");
    setColor(14);
    printf("%d/%d\n", diamondsCollected, TARGET_DIAMONDS);
}

void updateGame() {
    // --- INPUT ---
    int input = _getch(); 
    if (input == 224 || input == 0) input = _getch(); 

    int nextX = playerX;
    int nextY = playerY;

    if(input == 'w' || input == 'W' || input == 72) nextY--;
    if(input == 's' || input == 'S' || input == 80) nextY++;
    if(input == 'a' || input == 'A' || input == 75) nextX--;
    if(input == 'd' || input == 'D' || input == 77) nextX++;

    // 1. PLAYER LOGIC
    char targetTile = map[nextY][nextX];
    
    // Attack Enemy?
    int hitEnemyIndex = -1;
    for(int i=0; i<MAX_ENEMIES; i++) {
        if(enemies[i].alive && enemies[i].x == nextX && enemies[i].y == nextY) hitEnemyIndex = i;
    }

    if(hitEnemyIndex != -1) {
        enemies[hitEnemyIndex].alive = false;
        sprintf(message, "Enemy Defeated!");
        Beep(400, 50);
    }
    else if(targetTile != '#') {
        playerX = nextX;
        playerY = nextY;

        if(targetTile == 'D') {
            diamondsCollected++;
            map[playerY][playerX] = ' '; 
            sprintf(message, "Diamond Found! (%d/%d)", diamondsCollected, TARGET_DIAMONDS);
            Beep(1000, 50);
        }
        else if(targetTile == 'H') {
            playerHP += 30;
            if(playerHP > MAX_HP) playerHP = MAX_HP;
            map[playerY][playerX] = ' '; 
            sprintf(message, "Health Restored!");
            Beep(600, 50);
        }
        else if(targetTile == 'G') {
            if(diamondsCollected >= TARGET_DIAMONDS) {
                gameRunning = false; 
                return;
            } else {
                sprintf(message, "Locked! Need %d more Diamonds.", TARGET_DIAMONDS - diamondsCollected);
                Beep(200, 100);
            }
        }
    }

    // 2. ENEMY LOGIC (FIXED: No Diagonals)
    for(int i=0; i<MAX_ENEMIES; i++) {
        if(!enemies[i].alive) continue;

        int ex = enemies[i].x;
        int ey = enemies[i].y;
        
        // Calculate Distance
        int dx = playerX - ex;
        int dy = playerY - ey;
        
        int nextEx = ex;
        int nextEy = ey;

        // MANHATTAN LOGIC: Move only 1 direction (X or Y), whichever is further
        // This prevents the "Corner Hit" bug.
        if (abs(dx) > abs(dy)) {
            // Player is further away horizontally, move X
            if (dx > 0) nextEx++; else nextEx--;
        } else {
            // Player is further/equal vertically, move Y
            if (dy > 0) nextEy++; else nextEy--;
        }

        // Collision Check for Enemy
        bool blocked = false;
        if(map[nextEy][nextEx] == '#') blocked = true;
        if(map[nextEy][nextEx] == 'G') blocked = true; 
        
        // Don't overlap other enemies
        for(int j=0; j<MAX_ENEMIES; j++) {
            if(i != j && enemies[j].alive && enemies[j].x == nextEx && enemies[j].y == nextEy) blocked = true;
        }

        if(!blocked) {
            if(nextEx == playerX && nextEy == playerY) {
                // HIT PLAYER
                playerHP -= 10;
                sprintf(message, "OUCH! Enemy hit you (-10 HP)");
                setColor(4); 
                printf("\a"); 
            } else {
                enemies[i].x = nextEx;
                enemies[i].y = nextEy;
            }
        }
    }

    if(playerHP <= 0) gameRunning = false;
}

int main() {
    SetConsoleOutputCP(65001); 
    srand(time(0));
    hideCursor();

    bool playAgain = true;

    while(playAgain) {
        resetGame(); 

        while(gameRunning) {
            drawGame();
            updateGame();
        }

        system("cls");
        if(playerHP > 0) {
            setColor(10);
            printf("\n\n   VICTORY! \n   You collected the diamonds and escaped!\n");
        } else {
            setColor(4);
            printf("\n\n   DEFEAT... \n   The darkness consumed you.\n");
        }

        setColor(7);
        printf("\n   Play Again? (Y/N): ");
        char choice = _getch();
        if(choice == 'n' || choice == 'N') playAgain = false;
    }

    return 0;
}