#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_BUTTONS 49
#define BUTTON_SIZE 50
#define WORDLIST_FILE "wordlist.txt"
#define MAX_WORD_LENGTH 500

char buttons[MAX_BUTTONS];
bool buttonUsed[MAX_BUTTONS] = {0};
char selectedWord[MAX_WORD_LENGTH] = "";
int score = 0;

void generateButtons() {
    for (int i = 0; i < MAX_BUTTONS; i++) {
        buttons[i] = 'A' + GetRandomValue(0, 25); 
    }
}

bool checkWord(const char *word) {
FILE *file = fopen(WORDLIST_FILE, "r");
if (file == NULL) {
printf("Error opening wordlist file\n");
return false;
}

char buffer[MAX_WORD_LENGTH];
while (fgets(buffer, MAX_WORD_LENGTH, file) != NULL) {
// Remove newline character
buffer[strcspn(buffer, "\n")] = '\0';
if (strcmp(buffer, word) == 0) 
{
    fclose(file);
    return true;
}
}

fclose(file);
return false;
}


void removeChar(char arr[], char target) {
    int len = strlen(arr);
    int found = 0;

    for (int i = 0; i < len; ++i) {
        if (arr[i] == target) {
            found = 1;
        }
        if (found) {
            arr[i] = arr[i + 1];
        }
    }
}

int main() {
const int WIDTH = 1280;
const int HEIGHT = 720;
InitWindow(WIDTH, HEIGHT, "Word Game");

Texture2D backgroundImage = LoadTexture("asd.png");
SetTargetFPS(60);
int gameState = 0;

generateButtons();

while (!WindowShouldClose()) {
BeginDrawing();
ClearBackground(RAYWHITE);
DrawTexture(backgroundImage, 0, 0, WHITE);

if (gameState == 0) {
    DrawText("WORD GAME", WIDTH / 2 - 200, HEIGHT / 2 - 100, 50, WHITE);
    DrawText("Press SPACE to start", WIDTH / 2, HEIGHT / 2, 20, WHITE);
    if (IsKeyPressed(KEY_SPACE)) {
        gameState = 1;
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        gameState = 2;
    }
} else if (gameState == 1) {
    DrawText(TextFormat("Score: %d", score), WIDTH / 2 - MeasureText("Score: 00", 30) / 2, 20, 30, WHITE);

 
    int totalButtonsWidth = 7 * (BUTTON_SIZE + 10);
    int rows = (MAX_BUTTONS + 6) / 7; // Calculate number of rows
    int totalButtonsHeight = rows * (BUTTON_SIZE + 10); // Calculate total height
    int buttonsStartX = (WIDTH - totalButtonsWidth) / 2;
    int buttonsStartY = (HEIGHT - totalButtonsHeight) / 2; // Adjust start Y position
    int x, y;
    int wordX, wordY;
   
    for (int i = 0; i < MAX_BUTTONS; i++) {
        if (!buttonUsed[i]) {
            x = buttonsStartX + (i % 7) * (BUTTON_SIZE + 10);
            y = buttonsStartY + (i / 7) * (BUTTON_SIZE + 10);
            DrawRectangle(x, y, BUTTON_SIZE, BUTTON_SIZE, LIGHTGRAY);
            DrawText(TextFormat("%c", buttons[i]), x + 15, y + 15, 20, BLACK);
            
            int totalWordWidth = strlen(selectedWord) * (BUTTON_SIZE + 10);
            int wordStartX = (WIDTH - totalWordWidth) / 2; 
            for (int j = 0; j < strlen(selectedWord); j++) {
                wordX = wordStartX + j * (BUTTON_SIZE + 10);
                wordY = HEIGHT / 1.2;
                DrawRectangle(wordX, wordY, BUTTON_SIZE, BUTTON_SIZE, LIGHTGRAY);
                DrawText(TextFormat("%c", selectedWord[j]), wordX + 15, wordY + 15, 20, BLACK);
            }
            
            // Draw reload button
            DrawRectangle(WIDTH - 100, HEIGHT - 50, 80, 30, LIGHTGRAY);
            DrawText("RELOAD", WIDTH - 100, HEIGHT - 50, 20, BLACK);

            // Check for mouse input on reload button
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){WIDTH - 100, HEIGHT - 50, 80, 30})) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    // Reset button states and selected word
                    memset(buttonUsed, 0, sizeof(buttonUsed));
                    strcpy(selectedWord, "");
                }
            }
                       
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){x, y, BUTTON_SIZE, BUTTON_SIZE})) {
                    buttonUsed[i] = true;
                    strncat(selectedWord, &buttons[i], 1);
                }
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){wordX, wordY, BUTTON_SIZE, BUTTON_SIZE})) {
                    buttonUsed[i] = false;
                    removeChar(selectedWord, buttons[i]);
                }
                if (checkWord(selectedWord)) 
                {
                    score++;
                    printf("Correct word! Score: %d\n", score);
                    strcpy(selectedWord, "");
                }
                else {
                    printf("Incorrect word!\n");
                }
            }
        }
    }

    
} else if (gameState == 2)
    {
    CloseWindow();
}

EndDrawing();
}
CloseWindow();
return 0;
}