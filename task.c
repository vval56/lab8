#include "function.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Использование: %s <имя_файла>\n", argv[0]);
        return 1;
    }

    NUMBERS inputNumbers;  // Используем структуру напрямую, а не указатель
    char *filename = argv[1];
    FILE *file = fopen(filename, "wb");
    if (!file) {
        puts("Ошибка открытия файла");
        return 1;
    }

    char *lineNum = NULL;

    puts("Введите числа:");
    inputLine(&lineNum);

    fromCharToInt(lineNum, &inputNumbers);

    int cout = fwrite(inputNumbers.numberInLine, sizeof(int), inputNumbers.coutNum, file);
    printf("Вписал: %d чисел\n", cout);

    fclose(file);

    printf("Искомый элемент в задании 2: %d\n", findTask(filename, &inputNumbers));

    // Освобождение памяти
    free(lineNum);
    free(inputNumbers.numberInLine);

    return 0;
}
