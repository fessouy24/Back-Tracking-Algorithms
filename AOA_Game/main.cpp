#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <SFML/audio.h>
#include <iostream>
#include <string>
#include <vector>
#include "LinkedList.h"
int gameSize = 5;
static int quantom = 0;
using namespace std;
Stack matries;

void printMatrix(int** matrix);
int** createCopy(int** matrix);

vector<int**> generateAllPossibleMoves(int** matrix, int turn);

int checkWinner(int** matrix);

bool checkDraw(int type, int** matrix, int size);
int** createMatrix(int** matrix);

bool playAnyGame(int** matrix, int turn);

void getFirstMove(int** matrix);
void updateMatrix(int** matrix);
void redrawSprites(int** matrix, sfSprite* greenPieceSprite, sfSprite* redPieceSprite, sfRenderWindow* window);

int main()
{

    sfSoundBuffer* moveBuffer = sfSoundBuffer_createFromFile("C:/Users/MoazE/source/repos/AOA_Game/x64/Debug/click-36683.wav");
    if (!moveBuffer) return 1;

    sfSound* moveSound = sfSound_create();
    sfSound_setBuffer(moveSound, moveBuffer);

    sfSoundBuffer* errorBuffer = sfSoundBuffer_createFromFile("C:/Users/MoazE/source/repos/AOA_Game/x64/Debug/asdf.wav");
    if (!errorBuffer) return 1;

    sfSound* errorSound = sfSound_create();
    sfSound_setBuffer(errorSound, errorBuffer);

    sfSoundBuffer* winBuffer = sfSoundBuffer_createFromFile("C:/Users/MoazE/source/repos/AOA_Game/x64/Debug/As.wav");
    if (!winBuffer) return 1;

    sfSound* winSound = sfSound_create();
    sfSound_setBuffer(winSound, winBuffer);

    sfSoundBuffer* loseBuffer = sfSoundBuffer_createFromFile("C:/Users/MoazE/source/repos/AOA_Game/x64/Debug/lose.wav");
    if (!loseBuffer) return 1;

    sfSound* loseSound = sfSound_create();
    sfSound_setBuffer(loseSound, loseBuffer);
    
    sfFont* font = sfFont_createFromFile("C:/Windows/Fonts/arial.ttf"); // Setting the font

    sfText* winText = sfText_create();
    sfText_setFont(winText, font);
    sfText_setCharacterSize(winText, 40);
    sfText_setFillColor(winText, sfColor_fromRGB(255, 255, 0));

    sfRenderWindow* window;
    sfVideoMode mode = { gameSize * 100, gameSize * 100, 32 };
    window = sfRenderWindow_create(mode, "Animus_AOA-Game", sfResize | sfClose, NULL);
    if (!window)
        return 1;

    int inMenu = 1; // check if you in the main menu or not

    // Creating the start button
    sfRectangleShape* startButton = sfRectangleShape_create();
    sfVector2f buttonSize = { 200, 80 };
    sfRectangleShape_setSize(startButton, buttonSize);
    sfVector2f buttonPos = { (gameSize * 100 - 200) / 2, (gameSize * 100 - 50) / 2 };
    sfRectangleShape_setPosition(startButton, buttonPos);
    sfRectangleShape_setFillColor(startButton, sfColor_fromRGB(100, 150, 255));

    //Creating the restart button
    sfRectangleShape* restartButton = sfRectangleShape_create();
    sfVector2f restartButtonSize = { 200, 80 };
    sfRectangleShape_setSize(restartButton, restartButtonSize);
    sfVector2f restartButtonPos = { (gameSize * 100 - 200) / 2, (gameSize * 100) / 2 };
    sfRectangleShape_setPosition(restartButton, restartButtonPos);
    sfRectangleShape_setFillColor(startButton, sfColor_fromRGB(100, 150, 255));

    // Add text to the start button


    if (!font)
        return 1;

    sfText* startText = sfText_create();
    sfText_setString(startText, "Start");
    sfText_setFont(startText, font);
    sfText_setCharacterSize(startText, 36);
    sfText_setFillColor(startText, sfColor_fromRGB(255, 255, 255));
    sfVector2f textPos = { buttonPos.x + 52, buttonPos.y + 20 };
    sfText_setPosition(startText, textPos);

    // Add text for the restart button
    

    sfText* restartText = sfText_create();
    sfText_setString(restartText, "Restart");
    sfText_setFont(restartText, font);
    sfText_setCharacterSize(restartText, 36);
    sfText_setFillColor(restartText, sfColor_fromRGB(0, 0, 255));
    sfVector2f restartTextPos = { restartButtonPos.x + 52, restartButtonPos.y +20  };
    sfText_setPosition(restartText, restartTextPos);

    // Creating cells
    sfRectangleShape* cell = sfRectangleShape_create();
    sfVector2f size = { 98, 98 };
    sfRectangleShape_setSize(cell, size);

    // Creating pieces (green & red)
    sfTexture* greenPieceTexture = sfTexture_createFromFile("C:/Users/MoazE/source/repos/AOA_Game/x64/Debug/GreenRock.png", NULL);
    if (!greenPieceTexture) return 1;
    sfSprite* greenPieceSprite = sfSprite_create();
    sfSprite_setTexture(greenPieceSprite, greenPieceTexture, sfTrue);
    sfVector2f greenPieceSize = { .098, .098 };
    sfSprite_setScale(greenPieceSprite, greenPieceSize);

    sfTexture* redPieceTexture = sfTexture_createFromFile("C:/Users/MoazE/source/repos/AOA_Game/x64/Debug/RedRock.png", NULL);
    if (!redPieceTexture) return 1;
    sfSprite* redPieceSprite = sfSprite_create();
    sfSprite_setTexture(redPieceSprite, redPieceTexture, sfTrue);
    sfVector2f redPieceSize = { .098, .098 };
    sfSprite_setScale(redPieceSprite, redPieceSize);
    sfSprite_setRotation(redPieceSprite, 270);


    //logo of the game

    sfTexture* logoTexture = sfTexture_createFromFile("C:/Users/MoazE/source/repos/AOA_Game/x64/Debug/Icon.png", NULL);
    if (!logoTexture) return 1;
    sfSprite* logoSprite = sfSprite_create();
    sfSprite_setTexture(logoSprite, logoTexture, sfTrue);
    sfVector2f logoSize = { .2, .2 };
    sfSprite_setScale(logoSprite, logoSize);
    

    int** matrix = new int* [gameSize];
    for (int i = 0; i < gameSize; ++i)
        matrix[i] = new int[gameSize];

    matrix = createMatrix(matrix);
    int turn = 1;

    //Run 
    while (sfRenderWindow_isOpen(window))
    {
        sfEvent event;

        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);

            if (inMenu && event.type == sfEvtMouseButtonPressed && event.mouseButton.button == sfMouseLeft)
            {
                float mouseX = event.mouseButton.x;
                float mouseY = event.mouseButton.y;
                if (mouseX >= buttonPos.x && mouseX <= buttonPos.x + buttonSize.x &&
                    mouseY >= buttonPos.y && mouseY <= buttonPos.y + buttonSize.y)
                {
                    inMenu = 0; // Closing menu & opening the game
                }
            }


            sfRenderWindow_clear(window, sfColor_fromRGB(245, 222, 179));

            if (inMenu)
            {
                sfRenderWindow_drawRectangleShape(window, startButton, NULL);
                sfRenderWindow_drawText(window, startText, NULL);

                sfVector2f Pos = {100.0f + 65, 50.0f -6 };
                sfSprite_setPosition(logoSprite, Pos);
                sfSprite_setColor(logoSprite, sfColor_fromRGB(255, 255, 255));
                sfRenderWindow_drawSprite(window, logoSprite, NULL);
            }
            else
            {


                for (int row = 0; row < gameSize; row++)
                {
                    for (int col = 0; col < gameSize; col++)
                    {
                        sfVector2f pos = { col * 100.0f, row * 100.0f };
                        sfRectangleShape_setPosition(cell, pos);

                        if ((row == 0 || row == gameSize - 1) && col != 0 && col < gameSize - 1)
                            sfRectangleShape_setFillColor(cell, sfColor_fromRGB(100, 255, 100));
                        else if ((col == 0 || col == gameSize - 1) && row != 0 && row < gameSize - 1)
                            sfRectangleShape_setFillColor(cell, sfColor_fromRGB(255, 100, 100));
                        else
                            sfRectangleShape_setFillColor(cell, sfColor_fromRGB(200, 200, 200));

                        sfRenderWindow_drawRectangleShape(window, cell, NULL);
                    }
                }
                /// Drawing the sprites everytime 
                for (int row = 0; row < gameSize; row++) {
                    for (int col = 0; col < gameSize; col++) {

                        if (matrix[row][col] == 1) {

                            sfVector2f greenPos = { col * 100.0f + 12.5, row * 100.0f + 12.5 };
                            sfSprite_setPosition(greenPieceSprite, greenPos);
                            sfSprite_setColor(greenPieceSprite, sfColor_fromRGB(0, 255, 0));
                            sfRenderWindow_drawSprite(window, greenPieceSprite, NULL);
                        }
                        if (matrix[row][col] == 2) {
                            sfVector2f redPos = { col * 100.0f + 12.5, row * 100.0f + (100 - 12.5) };
                            sfSprite_setPosition(redPieceSprite, redPos);
                            sfSprite_setColor(redPieceSprite, sfColor_fromRGB(255, 0, 0));
                            sfRenderWindow_drawSprite(window, redPieceSprite, NULL);
                        }
                    }
                }

                /// The player's Moves
                if (event.type == sfEvtMouseButtonPressed && checkWinner(matrix) == 0) {
                    sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
                    int col = mousePos.x / 100;
                    int row = mousePos.y / 100;
                    bool Draw = false;
                    if (matrix[row][col] == 1) {
                        Draw = checkDraw(1, matrix, gameSize);
                        if (!Draw) {
                        if (turn == 1) {
                            if (matrix[row + 1][col] == 0) {
                                matrix[row + 1][col] = 1;
                                matrix[row][col] = 0;
                                turn = 2;
                                sfSound_play(moveSound);
                                redrawSprites(matrix, greenPieceSprite, redPieceSprite, window);
                            }
                            else if (matrix[row + 2][col] == 0) {
                                matrix[row + 2][col] = 1;
                                matrix[row][col] = 0;
                                turn = 2;
                                sfSound_play(moveSound);
                                redrawSprites(matrix, greenPieceSprite, redPieceSprite, window);
                            }
                            else {

                                cout << "This rock cannot move\n";

                            }


                        }
                        else {
                            sfSound_play(errorSound);
                        }
                        
                        }
                        else {
                            turn = 2;
                            cout << "the turn has been changed because of draw" << endl;
                        }

                        
                    }
                    if (turn == 2 && checkWinner(matrix) == 0) {
                        updateMatrix(matrix);
                        redrawSprites(matrix, greenPieceSprite, redPieceSprite, window);
                        sfSound_play(moveSound);
                        turn = 1;
                        cout << " This solution has got " << quantom << " round to get it\n";
                        quantom = 0;
                    }
                    //draw the matrix 
                    printMatrix(matrix);

                }

                int checker = checkWinner(matrix);

                if (checker == 1) {
                    sfText_setString(winText, "Player 1 Wins!");
                    sfVector2f winPos = { 100, (gameSize * 100) / 2.0f + 70 };
                    sfText_setPosition(winText, winPos);
                    sfRenderWindow_drawText(window, winText, NULL);
                    sfSound_play(winSound);
                    sfRenderWindow_drawRectangleShape(window, restartButton, NULL);
                    sfRenderWindow_drawText(window, restartText, NULL);
                    
                }
                else if (checker == 2) {
                    sfText_setString(winText, "You lose ,AI player Wins!");
                    sfVector2f winPos = { 50, (gameSize * 100) / 2.0f + 70};
                    sfText_setPosition(winText, winPos);
                    sfRenderWindow_drawText(window, winText, NULL);
                    sfSound_play(loseSound);
                    sfRenderWindow_drawRectangleShape(window, restartButton, NULL);
                    sfRenderWindow_drawText(window, restartText, NULL);
                    
                }

                if ( checkWinner(matrix)!=0 && event.type == sfEvtMouseButtonPressed && event.mouseButton.button == sfMouseLeft)
                {
                    float mouseX = event.mouseButton.x;
                    float mouseY = event.mouseButton.y;
                    if (mouseX >= restartButtonPos.x && mouseX <= restartButtonPos.x + restartButtonSize.x &&
                        mouseY >= restartButtonPos.y && mouseY <= restartButtonPos.y + restartButtonSize.y)
                    {
                        inMenu = 1; // Opening menu & opening the game
                        (checkWinner(matrix) == 1) ? sfSound_stop(winSound) : sfSound_stop(loseSound);
                        
                        matrix = createMatrix(matrix);
                        turn = 1;

                    }
                }

               
            }



            sfRenderWindow_display(window);
        }
    }
    // ????? ???????
    sfText_destroy(startText);
    sfFont_destroy(font);
    sfRectangleShape_destroy(startButton);
    sfTexture_destroy(greenPieceTexture);
    sfSprite_destroy(greenPieceSprite);
    sfTexture_destroy(redPieceTexture);
    sfSprite_destroy(redPieceSprite);
    sfRectangleShape_destroy(cell);
    sfRenderWindow_destroy(window);
    sfSound_destroy(moveSound);
    sfSoundBuffer_destroy(moveBuffer);
    sfSound_destroy(errorSound);
    sfSoundBuffer_destroy(errorBuffer);
    sfSound_destroy(winSound);
    sfSoundBuffer_destroy(winBuffer);

    return 0;
}
void printMatrix(int** matrix) {
    for (int i = 0; i < gameSize; i++) {
        for (int j = 0; j < gameSize; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
    cout << "-------------------------" << endl;
}

int** createCopy(int** matrix) {
    int** copyMatrix = new int* [gameSize];
    for (int i = 0; i < gameSize; ++i)
        copyMatrix[i] = new int[gameSize];

    for (int row = 0; row < gameSize; row++) {
        for (int col = 0; col < gameSize; col++) {
            copyMatrix[row][col] = matrix[row][col];
        }
    }
    return copyMatrix;
}

vector<int**> generateAllPossibleMoves(int** matrix, int turn) {
    vector<int**> allMoves;

    for (int row = 0; row < gameSize; row++) {
        for (int col = 0; col < gameSize; col++) {
            if (turn == 2 && matrix[row][col] == 2) {
                if ((col + 1) < gameSize && matrix[row][col + 1] == 0) {
                    int** newMatrix = createCopy(matrix);
                    newMatrix[row][col + 1] = 2;
                    newMatrix[row][col] = 0;
                    allMoves.push_back(newMatrix);
                }
                else if ((col + 2) < gameSize && matrix[row][col + 1] > 0 && matrix[row][col + 2] == 0) {
                    int** newMatrix = createCopy(matrix);
                    newMatrix[row][col + 2] = 2;
                    newMatrix[row][col] = 0;
                    allMoves.push_back(newMatrix);
                }
            }

            else if (turn == 1 && matrix[row][col] == 1) {
                if ((row + 1) < gameSize && matrix[row + 1][col] == 0) {
                    int** newMatrix = createCopy(matrix);
                    newMatrix[row + 1][col] = 1;
                    newMatrix[row][col] = 0;
                    allMoves.push_back(newMatrix);
                }
                else if ((row + 2) < gameSize && matrix[row + 1][col] > 0 && matrix[row + 2][col] == 0) {
                    int** newMatrix = createCopy(matrix);
                    newMatrix[row + 2][col] = 1;
                    newMatrix[row][col] = 0;
                    allMoves.push_back(newMatrix);
                }
            }
        }
    }
    for (int i = 0; i < allMoves.size(); i++) {
        printMatrix(allMoves[i]);
        quantom++;
    }

    return allMoves;
}

int checkWinner(int** matrix) {
    int winner = 0;
    bool playerOne = true;
    bool playerTwo = true;
    int row = gameSize - 1;
    for (int col = gameSize - 2; col > 0; col--) {
        if (matrix[row][col] == 1) {
            playerOne *= true;
        }
        else {
            playerOne *= false;
        }
    }

    int col = gameSize - 1;
    for (int row = gameSize - 2; row > 0; row--) {
        if (matrix[row][col] == 2) {
            playerTwo *= true;
        }
        else {
            playerTwo *= false;
        }
    }

    if (playerOne) {
        winner = 1;
    }
    else if (playerTwo) {
        winner = 2;
    }
    else {
        winner = 0;
    }

    return winner;
}

bool checkDraw(int type, int** matrix, int size) {
    bool isDraw = false;
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1; j++) {
            if (matrix[i][j] == type) {
                if (type == 1) {
                    if (matrix[i + 1][j] == 2 && matrix[i + 2][j] == 2) {
                        isDraw = true;
                    }
                    else {
                        isDraw = false;
                        break;
                    }
                }
                else if (type == 2) {
                    if (matrix[i][j + 1] == 1 && matrix[i][j + 2] == 1) {
                        isDraw = true;
                    }
                    else {
                        isDraw = false;
                        break;
                    }
                }
            }
        }
    }
    return isDraw;
}

int** createMatrix(int** matrix) {
    for (int i = 0; i < gameSize; i++) {
        for (int j = 0; j < gameSize; j++) {
            if ((j == gameSize - 1 && i == 0) || (j == 0 && i == 0) || (j == gameSize - 1 && i == gameSize - 1) || (j == 0 && i == gameSize - 1)) {
                matrix[i][j] = -1;
            }
            else if (i == 0) {
                matrix[i][j] = 1;
            }
            else if (j == 0) {
                matrix[i][j] = 2;
            }
            else {
                matrix[i][j] = 0;
            }
        }
    }
    return matrix;
}
bool playAnyGame(int** matrix, int turn) {
    if (checkWinner(matrix) == 2) {
        matries.push(createCopy(matrix)); // ???? ?????? ???? ???? ?????
        return true;
    }
    if (checkWinner(matrix) == 1) {
        return false;
    }

    vector<int**> possibleMoves = generateAllPossibleMoves(matrix, turn);
    int nextTurn = (turn == 1) ? 2 : 1;

    for (auto move : possibleMoves) {
        if (playAnyGame(move, nextTurn)) {
            matries.push(createCopy(move)); // ???? ??? ???? ??
            return true;
        }
    }

    return false;
}
void getFirstMove(int** matrix) {
    bool noMove = true;
    for (int row = 0; row < gameSize && noMove; row++) {
        for (int col = 0; col < gameSize; col++) {
            if (matrix[row][col] == 2) {
                if (matrix[row][col + 1] == 0) {
                    matrix[row][col + 1] = 2;
                    matrix[row][col] = 0;
                    noMove = false;
                    break;

                }
                else if (matrix[row][col + 2] == 0) {
                    matrix[row][col + 2] = 2;
                    matrix[row][col] = 0;
                    noMove = false;
                    break;
                }
            }
        }
    }
}

void updateMatrix(int** matrix) {
    bool Draw = checkDraw(2, matrix, gameSize);

    if (!Draw) {
        if (playAnyGame(matrix, 2)) {
            if (!matries.isEmpty()) {
                int** bestMove = matries.pop(); // ???? ??? ???? ????? ??
                // ?????? ?? ???????? ????????
                for (int i = 0; i < gameSize; i++) {
                    for (int j = 0; j < gameSize; j++) {
                        matrix[i][j] = bestMove[i][j];
                    }
                }
            }
        }
        else {
            getFirstMove(matrix);
        }
    }
    else {

        cout << "No winning move. AI will play any valid move." << endl;
        getFirstMove(matrix);

    }
}

void redrawSprites(int** matrix, sfSprite* greenPieceSprite, sfSprite* redPieceSprite,sfRenderWindow* window) {

    for (int row = 0; row < gameSize; row++) {
        for (int col = 0; col < gameSize; col++) {

            if (matrix[row][col] == 1) {

                sfVector2f greenPos = { col * 100.0f + 12.5, row * 100.0f + 12.5 };
                sfSprite_setPosition(greenPieceSprite, greenPos);
                sfSprite_setColor(greenPieceSprite, sfColor_fromRGB(0, 255, 0));
                sfRenderWindow_drawSprite(window, greenPieceSprite, NULL);
            }
            if (matrix[row][col] == 2) {
                sfVector2f redPos = { col * 100.0f + 12.5, row * 100.0f + (100 - 12.5) };
                sfSprite_setPosition(redPieceSprite, redPos);
                sfSprite_setColor(redPieceSprite, sfColor_fromRGB(255, 0, 0));
                sfRenderWindow_drawSprite(window, redPieceSprite, NULL);
            }
        }
    }
}