#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_BUTTONS 49
#define BUTTON_SIZE 50
#define WORDLIST_FILE "wordlist.txt"
#define MAX_WORD_LENGTH 500
#define TIMER 60

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

int main() {
    const int WIDTH = 1280;
    const int HEIGHT = 720;
    InitWindow(WIDTH, HEIGHT, "Word Game");

    Texture2D backgroundImage = LoadTexture("asd.png");
    SetTargetFPS(60);
    int gameState = 0;
    float timer = TIMER; // Timer limit (60 seconds)

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
            
            // Draw the timer
            int minutes = (int)timer / 60;
            int seconds = (int)timer % 60;
            DrawText(TextFormat("%02d:%02d", minutes, seconds), WIDTH - 150, 20, 30, WHITE);
            // Update the timer
            timer -= GetFrameTime();
          

     
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
                    
                    // Draw clear button
                    DrawRectangle(WIDTH - 100, HEIGHT - 80, 90, 30 , LIGHTGRAY);
                    DrawText("CLEAR", WIDTH - 90, HEIGHT - 75 , 20, BLACK);
                    
                    // Draw reload board button
                    DrawRectangle(WIDTH - 100, HEIGHT - 40, 90, 30 , LIGHTGRAY);
                    DrawText("RELOAD", WIDTH - 95, HEIGHT - 35 , 20, BLACK);
                    
                    
                    
                    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){WIDTH - 90, HEIGHT - 40, 90, 30})) {
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                            memset(buttonUsed, 0, sizeof(buttonUsed));
                            strcpy(selectedWord, "");
                            generateButtons();
                        }
                    }

                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){x, y, BUTTON_SIZE, BUTTON_SIZE})) {
                            buttonUsed[i] = true;
                            strncat(selectedWord, &buttons[i], 1);
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
                    
                    
              if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){WIDTH - 100, HEIGHT - 80, 90, 30})) {
              if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                   for (int i = 0; i < strlen(selectedWord); i++) {
                   for (int j = 0; j < MAX_BUTTONS; j++) {
                   if (selectedWord[i] == buttons[j] && buttonUsed[j]) {
                    buttonUsed[j] = false;
                    
                }
            }
        }
        strcpy(selectedWord, "");
    }
}

            // Check if the timer has reached 0
            if (timer <= 0) {
                gameState = 3; // Change the game state to GAME_OVER
            }
                  
                }
            }   
        } else if (gameState == 2) {
            CloseWindow();
        }
   
        else if (gameState == 3) {
            // Display the score screen
            DrawText("GAME OVER", WIDTH / 2 - MeasureText("GAME OVER", 50) / 2, HEIGHT / 2 - 100, 50, WHITE);
            DrawText(TextFormat("Score: %d", score), WIDTH / 2 - MeasureText("Score: 00", 30) / 2, HEIGHT / 2, 30, WHITE);
            DrawText("Press SPACE to continue", WIDTH / 2, HEIGHT / 2 + 50, 20, WHITE);

            // Check if the user has pressed SPACE to continue
            if (IsKeyPressed(KEY_SPACE)) {
                gameState = 0; // Reset the game state
                score = 0; // Reset the score
                memset(buttonUsed, 0, sizeof(buttonUsed));
                strcpy(selectedWord, "");
                generateButtons();
                timer = TIMER; // Reset the timer
            }
        }
    EndDrawing();
    }
    CloseWindow();
    return 0;
}