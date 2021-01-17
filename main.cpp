#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int getRandomNumber(int min, int max) {
    return min + rand() % (max - min + 1);
}

const int SIZE = 3;
const int STEP = 11;
const int WALL_SIZE = 1;
const int SPRITE_SIZE = 3;
const int FRAME_SIZE = 5;
const int CELL_SIZE = WALL_SIZE + FRAME_SIZE;
const int BUFFER_SIZE = CELL_SIZE * SIZE - 1;

const enum Keys {
    UP_ARROW = 72,
    LEFT_ARROW = 75,
    DOWN_ARROW = 80,
    RIGHT_ARROW = 77,
    SPACE = 32
};

int** allocateMemoryForMatrix(int size) {
    // Выделяем память на массив указателей
    int** matrix = new int* [size];

    // Последовательно выделяем память для каждого указателя из массива
    for (int i = 0; i < size; i++) matrix[i] = new int[size];
    // Получаем массив указателей ввиде квадратной матрицы

    return matrix;
}

// Заполняем матрицу
void fillMatrix(int** matrix, int size, int num) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = num;
        }
    }
}

void menuKeydownHandling(int& button, int& indicator) {
    button = _getch();

    if (button == Keys::SPACE) return;

    switch (button)
    {
    case Keys::DOWN_ARROW:
        if (indicator < 2) indicator++;
        break;
    case Keys::UP_ARROW:
        if (indicator > 1) indicator--;
        break;
    }
}

void renderMenu(string title, string menu[], int menuSize, int indicator) {
    for (int i = 0; i < menuSize; i++) menu[i][0] = ' ';

    menu[indicator - 1][0] = '>';

    system("cls");

    cout << title << endl;

    for (int i = 0; i < 2; i++) cout << menu[i] << endl;
}

bool getPlayersCount() {
    int playersCount = 1;

    int button = 0;

    const int menuSize = 2;
    string title = "Select the number of players";
    string menu[menuSize] = {
        " 1 Player",
        " 2 Players"
    };

    renderMenu(title, menu, menuSize, playersCount);

    do {
        if (_kbhit()) {
            menuKeydownHandling(button, playersCount);

            renderMenu(title, menu, menuSize, playersCount);
        }
    } while (button != Keys::SPACE);

    return playersCount == 1;
}

int getPlayerSymbol() {
    int playerSymbol = 1;

    int button = 0;

    const int menuSize = 2;
    string title = "Select the player's symbol";
    string menu[menuSize] = {
        " X symbol",
        " O symbol"
    };
    int indicator = 1;

    renderMenu(title, menu, menuSize, indicator);

    do {
        if (_kbhit()) {
            menuKeydownHandling(button, indicator);

            switch (indicator) {
                case 1:
                    playerSymbol = 1;
                    break;
                case 2:
                    playerSymbol = -1;
                    break;
            }

            renderMenu(title, menu, menuSize, indicator);
        }
    } while (button != Keys::SPACE);

    return playerSymbol;
}

int getRandomPlayer() {
    bool player = static_cast<bool>(getRandomNumber(0, 1));

    if (player) return 1;

    return -1;
}

void keydownHandling(int** gameDataModel, int button, int& x, int& y, int key, int& player) {
    switch (button) {
        case Keys::UP_ARROW: //вверх
            if (y - 1 >= 0) {
                gameDataModel[y][x] -= key;
                y--;
                gameDataModel[y][x] += key;
            }
            break;
        case Keys::DOWN_ARROW: //вниз
            if (y + 1 < SIZE) {
                gameDataModel[y][x] -= key;
                y++;
                gameDataModel[y][x] += key;
            }
            break;
        case Keys::LEFT_ARROW: //влево 
            if (x - 1 >= 0) {
                gameDataModel[y][x] -= key;
                x--;
                gameDataModel[y][x] += key;
            }
            break;
        case Keys::RIGHT_ARROW: //вправо
            if (x + 1 < SIZE) {
                gameDataModel[y][x] -= key;
                x++;
                gameDataModel[y][x] += key;
            }
            break;
        case Keys::SPACE:
            gameDataModel[y][x] -= key;

            if (gameDataModel[y][x] == 0) {
                gameDataModel[y][x] += player;
                player = -(player);
                x = 0;
                y = 0;
            }

            gameDataModel[y][x] += key;
            break;
    }
}

// Функции создания шаблонов спрайтов
char** createCrossSprite() {
    char** crossTemplate = new char* [SPRITE_SIZE];
    crossTemplate[0] = new char[SPRITE_SIZE] { '#', ' ', '#' };
    crossTemplate[1] = new char[SPRITE_SIZE] { ' ', '#', ' ' };
    crossTemplate[2] = new char[SPRITE_SIZE] { '#', ' ', '#' };

    return crossTemplate;
}

char** createRingSprite() {
    char** ringTemplate = new char* [SPRITE_SIZE];
    ringTemplate[0] = new char[SPRITE_SIZE] { '#', '#', '#' };
    ringTemplate[1] = new char[SPRITE_SIZE] { '#', ' ', '#' };
    ringTemplate[2] = new char[SPRITE_SIZE] { '#', '#', '#' };

    return ringTemplate;
}

char** createFrameSprite() {
    char** frameTemplate = new char* [FRAME_SIZE];
    frameTemplate[0] = new char[FRAME_SIZE] { 'O', 'O', 'O', 'O', 'O' };
    frameTemplate[1] = new char[FRAME_SIZE] { 'O', ' ', ' ', ' ', 'O' };
    frameTemplate[2] = new char[FRAME_SIZE] { 'O', ' ', ' ', ' ', 'O' };
    frameTemplate[3] = new char[FRAME_SIZE] { 'O', ' ', ' ', ' ', 'O' };
    frameTemplate[4] = new char[FRAME_SIZE] { 'O', 'O', 'O', 'O', 'O' };

    return frameTemplate;
}

// Иницилизация константрой структуры с спрайтами
struct Templates
{
    char** cross = createCrossSprite();
    char** ring = createRingSprite();
    char** frame = createFrameSprite();
} const templates;

// Выделение памяти на буффер
char** allocateMemoryForBuffer(int size) {
    char** buffer = new char* [size];

    for (int i = 0; i < size; i++) buffer[i] = new char[size];

    return buffer;
}

// Очистка буфера
void clearGameCanvas(char** buffer, int bufferSize) {
    for (int i = 0; i < bufferSize; i++) {
        for (int j = 0; j < bufferSize; j++) {
            buffer[i][j] = ' ';
        }
    }
}

// Создание "стен" на поле
void createWallsOnCanvas(char** buffer, int bufferSize, int cellSize) {
    for (int i = 0; i < bufferSize; i++) {
        for (int j = 0; j < bufferSize; j++) {
            if ((i + 1) % cellSize == 0 || (j + 1) % cellSize == 0) buffer[i][j] = '#';
        }
    }
}

// "Врисовка" спрайта в буфер
void insertingSpriteIntoBuffer(char** buffer, char** sprite, int spriteSize, int x, int y) {
    for (int i = y, k = 0; i < y + spriteSize; i++, k++) {
        for (int j = x, n = 0; j < x + spriteSize; j++, n++) {
            buffer[i][j] = sprite[k][n];
        }
    }
}

// Отрисовка нужных спрайтов в ячейках в буфера
void fillingGameCanvasCells(int** matrix, char** gameCanvas, int size, int key) {
    int symbol = 0;
    bool isCurrent = false;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            symbol = matrix[i][j];
            isCurrent = false;

            if (symbol > 1) {
                isCurrent = true;
                symbol -= key;
            }

            if (isCurrent) { // Отрисовываем рамку в текущей ячейке
                insertingSpriteIntoBuffer(gameCanvas, templates.frame, FRAME_SIZE, j * CELL_SIZE, i * CELL_SIZE);
            }

            switch (symbol) {
                case -1: // Отрисовка нолика
                    insertingSpriteIntoBuffer(gameCanvas, templates.ring, SPRITE_SIZE, j * CELL_SIZE + 1, i * CELL_SIZE + 1);
                    break;
                case 1: // Отрисовка крестика
                    insertingSpriteIntoBuffer(gameCanvas, templates.cross, SPRITE_SIZE, j * CELL_SIZE + 1, i * CELL_SIZE + 1);
                    break;
            }
        }
    }
}

// Вывод буфера на экран
void printGameCanvas(char** bufferCanvas, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << bufferCanvas[i][j];
        }
        cout << endl;
    }
}

void printPlayerInformation(int player) {
    switch (player)
    {
    case 1:
        cout << endl << "Player X turn" << endl;
        break;
    case -1:
        cout << endl << "Player O turn" << endl;
        break;
    }
}

// Рендер игрового поля
void renderGameCanvas(int** matrix, char** buffer, int matrixSize, int bufferSize, int key) {
    clearGameCanvas(buffer, bufferSize);

    createWallsOnCanvas(buffer, bufferSize, CELL_SIZE);

    fillingGameCanvasCells(matrix, buffer, matrixSize, key);

    system("cls");

    printGameCanvas(buffer, bufferSize);
}

int dataModelProcessing(int** matrix, int size, int& x, int& y, int key, int player) {
    int result = 0;

    matrix[y][x] -= key;

    // Проверка строк матрицы
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result += matrix[i][j];
        }

        if (abs(result) == 3) return player;

        result = 0;
    }

    // Проверка столбцов матрицы
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result += matrix[j][i];
        }

        if (abs(result) == 3) return player;

        result = 0;
    }

    // Проверка главной диагонали
    for (int i = 0, j = 0; i < size && j < size; i++, j++) result += matrix[i][j];

    if (abs(result) == 3) return player;

    result = 0;

    // Проверка побочной диагонали
    for (int i = 0, j = size - 1; i < size && j >= 0; i++, j--) result += matrix[i][j];

    if (abs(result) == 3) return player;

    result = 0;

    // Проверка на ничью
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result += abs(matrix[i][j]);
        }
    }

    if (result == pow(size, 2)) return key;

    matrix[y][x] += key;

    return 0;
}

void playerTurnHandling(int button, int** gameDataModel, char** gameCanvas, int& gameState, int& x, int& y, int& currentPlayer) {
    if (_kbhit()) {
        // Получаем нажатие кнопки
        button = _getch();

        // Обработка нажатия
        keydownHandling(gameDataModel, button, x, y, STEP, currentPlayer);

        // Перересовка игрового поля
        renderGameCanvas(gameDataModel, gameCanvas, SIZE, BUFFER_SIZE, STEP);

        // Логика состояния матрицы
        gameState = dataModelProcessing(gameDataModel, SIZE, x, y, STEP, -currentPlayer);

        if (gameState != 0) return;

        // Информация о игроке
        printPlayerInformation(currentPlayer);
    }
}

void printGameResult(int gameState) {
    switch (gameState) {
        case 1:
            cout << endl << "Player X wins" << endl;
            break;
        case -1:
            cout << endl << "Player O wins" << endl;
            break;
        default:
            cout << endl << "Draw!" << endl;
            break;
    }

    system("pause");
}

void clearMatrixFromMemory(int** matrix, int size) {
    // Последовательно очищаем каждый указатель в массиве
    for (int i = 0; i < size; i++) delete[] matrix[i];

    // Очищаем сам массив указателей
    delete[] matrix;
}

// Очистка памяти буфера
void clearBufferFromMemory(char** buffer, int size) {
    for (int i = 0; i < size; i++) delete[] buffer[i];

    delete[] buffer;
}

bool getRepeatGame() {
    bool isRepeat = false;

    int button = 0;

    const int menuSize = 2;
    string title = "Repeat the game?";
    string menu[menuSize] = {
        " Yes",
        " No"
    };
    int indicator = 1;

    renderMenu(title, menu, menuSize, indicator);

    do {
        if (_kbhit()) {
            menuKeydownHandling(button, indicator);

            switch (indicator) {
            case 1:
                isRepeat = true;
                break;
            case 2:
                isRepeat = false;
                break;
            }

            renderMenu(title, menu, menuSize, indicator);
        }
    } while (button != Keys::SPACE);

    return isRepeat;
}

void game() {
    // Инициализация матрицы модели данных и буфера для отрисовки
    int** gameDataModel = allocateMemoryForMatrix(SIZE);
    char** gameCanvas = allocateMemoryForBuffer(BUFFER_SIZE);

    // Заполнение матрицы начальными значениями
    fillMatrix(gameDataModel, SIZE, 0);

    // Инициализация координат для перемещения по матрице
    int x = 0;
    int y = 0;

    // Итерируем матрицу по текущим координатам на произвольное положительное натуральное число
    gameDataModel[y][x] += STEP;

    // Инициализируем нажатую кнопку и состояние игры
    int button = 0;
    int gameState = 0;

    // Выбор кол-ва реальных игроков
    bool isSinglePlayer = getPlayersCount();

    int player = 0;

    // Если одиночная игра, то нужно выбрать себе символ хода
    if (isSinglePlayer) player = getPlayerSymbol();

    // Иницилизация случайного хода игрока
    int currentPlayer = getRandomPlayer();

    // Рендерим начальное состояния модели данных и игрока
    renderGameCanvas(gameDataModel, gameCanvas, SIZE, BUFFER_SIZE, STEP);
    printPlayerInformation(currentPlayer);

    // Запускаем игровой цикл
    while (gameState == 0) {
        if (isSinglePlayer) { // Если одиночная игра, то ход игрока чередуется с компьютером
            if (currentPlayer == player) {
                if (_kbhit()) {
                    playerTurnHandling(button, gameDataModel, gameCanvas, gameState, x, y, currentPlayer);
                }
            } else {
                try {
                    throw exception("Computer turn has not yet been implemented");
                } catch (const exception& e) {
                    system("cls");
                    cerr << e.what() << endl;
                    system("pause");
                    return;
                }
            }
        } else { // Если игра на двоих, то игроки просто сменяют друг друга после каждого хода
            playerTurnHandling(button, gameDataModel, gameCanvas, gameState, x, y, currentPlayer);
        }
    }

    // Вывод результата игры
    printGameResult(gameState);

    // Очищение памяти
    clearMatrixFromMemory(gameDataModel, SIZE);
    clearBufferFromMemory(gameCanvas, BUFFER_SIZE);

    // Возможность начать игру заново
    if (getRepeatGame()) game();
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    game();

    return 0;
}