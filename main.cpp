#include <iostream>
#include <conio.h>

using namespace std;

int** createDataModelPointer(int size = 3) {
    // Выделяем память на массив указателей
    int** area = new int* [size];

    // Последовательно выделяем память для каждого указателя из массива
    for (int i = 0; i < size; i++) {
        area[i] = new int[size];

        // Заполняем выделенный массив нулями
        for (int j = 0; j < size; j++) {
            area[i][j] = 0;
        }
    }
    // Получаем массив указателей ввиде квадратной матрицы

    return area;
}

void printDataModel(int** pointer, int size) {
    // Выводим матрицу на экран
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << pointer[i][j] << " ";
        }

        cout << endl;
    }
}

void clearDataModelFromMemory(int** pointer, int size) {
    // Последовательно очищаем каждый указатель в массиве
    for (int i = 0; i < size; i++) {
        delete[] pointer[i];
    }

    // Очищаем сам массив указателей
    delete[] pointer;
}

int main() {
    setlocale(LC_ALL, "Russian");

    const int SIZE = 3;

    int** gameAreaPointer = createDataModelPointer(SIZE);

    int x = 0;
    int y = 0;
    int* iterator = &gameAreaPointer[y][x];

    const int STEP = 11;
    *iterator += STEP;

    printDataModel(gameAreaPointer, SIZE);

    int button = 0;
    bool isRightKeydown = false;
    bool isEnterKeydown = false;

    do {
        if (_kbhit()) {
            button = _getch();

            switch (button)
            {
            case 72: //вверх
                if (y - 1 >= 0) {
                    y--;
                    isRightKeydown = true;
                }
                break;
            case 80: //вниз
                if (y + 1 < SIZE) {
                    y++;
                    isRightKeydown = true;
                }
                break;
            case 75: //влево 
                if (x - 1 >= 0) {
                    x--;
                    isRightKeydown = true;
                }
                break;
            case 77: //вправо
                if (x + 1 < SIZE) {
                    x++;
                    isRightKeydown = true;
                }
                break;
            case 32:
                isEnterKeydown = true;
                break;
            }
            
            if (isRightKeydown) {
                system("cls");

                *iterator -= STEP;

                if (isEnterKeydown && (*iterator) == 0) {
                    (*iterator)++; 
                }

                iterator = &gameAreaPointer[y][x];

                *iterator += STEP;

                printDataModel(gameAreaPointer, SIZE);

                isRightKeydown = false;
                isEnterKeydown = false;
            }
        }
    } while (button != 27);

    clearDataModelFromMemory(gameAreaPointer, SIZE);

    return 0;
}