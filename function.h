#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *numberInLine;
    int coutNum;
} NUMBERS;

// Проверка корректности ввода (только цифры и пробелы)
int checkNumValue(char *line) {
    int i = 0;
    while (line[i] != '\0' && line[i] != '\n') {
        if (!((line[i] >= '0' && line[i] <= '9') || line[i] == ' ')) {
            puts("Неверный ввод");
            return 0;
        }
        i++;
    }
    return 1;
}

// Ввод строки с числами
void inputLine(char **line) {
    do {
        int read = 0;
        size_t n = 0;

        read = getline(line, &n, stdin);
        if (read == -1) {
            puts("Ошибка выделения памяти");
            continue;
        }

    } while (checkNumValue(*line) == 0);
}

// Преобразование строки в массив чисел
void fromCharToInt(char *line, NUMBERS *num) {
    int i = 0, value = 0;
    num->coutNum = 0;
    num->numberInLine = NULL;

    while (line[i] != '\0' && line[i] != '\n') {
        while (line[i] == ' ') {
            i++;
        }

        if (line[i] >= '0' && line[i] <= '9') {
            value = 0;
            while (line[i] >= '0' && line[i] <= '9') {
                value = value * 10 + (line[i] - '0');
                i++;
            }

            num->numberInLine = (int *)realloc(num->numberInLine, sizeof(int) * (num->coutNum + 1));
            if (!num->numberInLine) {
                puts("Ошибка выделения памяти!\n");
                exit(1);
            }

            num->numberInLine[num->coutNum] = value;
            num->coutNum++;
        }
    }
}

// Поиск минимального произведения соседних чисел
int findTask(char *filename, NUMBERS *num) {
    if (num->coutNum < 2) {
        puts("Недостаточно данных для вычисления!");
        return -1;
    }

    int minMultip = 0;
    int *buff = (int *)malloc(sizeof(int) * num->coutNum);
    if (!buff) {
        puts("Ошибка выделения памяти!");
        return -1;
    }

    FILE *file = fopen(filename, "rb");
    if (!file) {
        puts("Ошибка при открытии файла для чтения!");
        free(buff);
        return -1;
    }

    fread(buff, sizeof(int), num->coutNum, file);
    fclose(file);

    minMultip = buff[0] * buff[1];
    int coutMin = 1;

    for (int i = 2; i < num->coutNum; i++) {
        int multipNeigh = buff[i] * buff[i - 1];
        if (multipNeigh < minMultip) {
            minMultip = multipNeigh;
            coutMin = i + 1;
        }
    }

    free(buff);
    return coutMin;
}

void swapNeighInFile(char *filename, NUMBERS *num){
    FILE *file = fopen(filename, "r+b");
    if (file == NULL) {
        printf("Ошибка при открытии файла для чтения/записи!\n");
        return;
    }

    int num1, num2;

    for (int i = 0; i < num->coutNum - 1; i += 2) {
        fseek(file, i * sizeof(int), SEEK_SET);
        fread(&num1, sizeof(int), 1, file);

        fseek(file, (i + 1) * sizeof(int), SEEK_SET);
        fread(&num2, sizeof(int), 1, file);

        fseek(file, i * sizeof(int), SEEK_SET);
        fwrite(&num2, sizeof(int), 1, file);

        fseek(file, (i + 1) * sizeof(int), SEEK_SET);
        fwrite(&num1, sizeof(int), 1, file);
    }

    fclose(file);
}

void printFile(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Ошибка при открытии файла.\n");
        return;
    }

    int value;
    while (fread(&value, sizeof(int), 1, file) == 1) {
        printf("%d ", value);
    }
    printf("\n");

    fclose(file);
}
