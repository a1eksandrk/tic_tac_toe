#include <iostream>
#include <conio.h>

using namespace std;

const int SIZE = 3;
const int STEP = 11;
const int WALL_SIZE = 1;
const int SPRITE_SIZE = 3;
const int FRAME_SIZE = 5;
const int CELL_SIZE = WALL_SIZE + FRAME_SIZE;
const int BUFFER_SIZE = CELL_SIZE * SIZE - 1;

int** allocateMemoryForMatrix(int size) {
    // Выделяем память на массив указателей
    int** matrix = new int* [size];

    // Последовательно выделяем память для каждого указателя из массива
    for (int i = 0; i < size; i++) {
        matrix[i] = new int[size];

    }
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

void keydownHandling(int** gameDataModel, int button, int& x, int& y) {
    switch (button)
    {
    case 72: //вверх
        if (y - 1 >= 0) {
            gameDataModel[y][x] -= STEP;
            y--;
            gameDataModel[y][x] += STEP;
        }
        break;
    case 80: //вниз
        if (y + 1 < SIZE) {
            gameDataModel[y][x] -= STEP;
            y++;
            gameDataModel[y][x] += STEP;
        }
        break;
    case 75: //влево 
        if (x - 1 >= 0) {
            gameDataModel[y][x] -= STEP;
            x--;
            gameDataModel[y][x] += STEP;
        }
        break;
    case 77: //вправо
        if (x + 1 < SIZE) {
            gameDataModel[y][x] -= STEP;
            x++;
            gameDataModel[y][x] += STEP;
        }
        break;
    case 32:
        gameDataModel[y][x] -= STEP;

        if (gameDataModel[y][x] == 0) gameDataModel[y][x]--;

        gameDataModel[y][x] += STEP;
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

// Иницилизация константрой структуры с спрафтами
struct Templates
{
    char** cross = createCrossSprite();
    char** ring = createRingSprite();
    char** frame = createFrameSprite();
} const templates;

// Выделение памяти на буффер
char** allocateMemoryForBuffer(int size) {
    char** buffer = new char* [size];

    for (int i = 0; i < size; i++) {
        buffer[i] = new char[size];
    }

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
            if ((i + 1) % cellSize == 0 || (j + 1) % cellSize == 0) {
                buffer[i][j] = '#';
            }
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

            switch (symbol)
            {
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

// Рендер игрового поля
void renderGameCanvas(int** matrix, char** buffer, int matrixSize, int bufferSize, int key) {
    clearGameCanvas(buffer, bufferSize);

    createWallsOnCanvas(buffer, bufferSize, CELL_SIZE);

    fillingGameCanvasCells(matrix, buffer, matrixSize, key);

    system("cls");

    printGameCanvas(buffer, bufferSize);
}

bool dataModelProcessing(int** martix) {
    // TODO доделать обработку состояния матрицы для окончания игры
    return true;
}

void clearMatrixFromMemory(int** martix, int size) {
    // Последовательно очищаем каждый указатель в массиве
    for (int i = 0; i < size; i++) {
        delete[] martix[i];
    }

    // Очищаем сам массив указателей
    delete[] martix;
}

// Очистка памяти буфера
void clearBufferFromMemory(char** buffer, int size) {
    for (int i = 0; i < size; i++) {
        delete[] buffer[i];
    }

    delete[] buffer;
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

    // Рендерим начальное состояния модели данных
    renderGameCanvas(gameDataModel, gameCanvas, SIZE, BUFFER_SIZE, STEP);

    // Инициализируем нажатую кнопку и состояние игры
    int button = 0;

    bool isGame = true;

    // Запускаем игровой цикл
    while (isGame) {
        if (_kbhit()) {
            // Получаем нажатие кнопки
            button = _getch();
            
            // Обработка нажатия
            keydownHandling(gameDataModel, button, x, y);

            // Перересовка игрового поля
            renderGameCanvas(gameDataModel, gameCanvas, SIZE, BUFFER_SIZE, STEP);

            // Логика состояния матрицы
            dataModelProcessing(gameDataModel);
        }
    }

    clearMatrixFromMemory(gameDataModel, SIZE);
    clearBufferFromMemory(gameCanvas, BUFFER_SIZE);
}

int main() {
    game();

    return 0;
}