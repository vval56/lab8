#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *numberInLine;
    int coutNum;
} NUMBERS;

int checkNumValue(char *line) {
    int i = 0;
    while (line[i] != '\0' && line[i] != '\n') {
        if (!((line[i] >= '0' && line[i] <= '9') || line[i] == ' ' || line[i] == '-')) {
            puts("Неверный ввод");
            return 0;
        }

        if (line[i] == '-' && !(line[i+1] >= '0' && line[i+1] <= '9')) {
            puts("Неверный ввод: минус должен стоять перед числом");
            return 0;
        }
        i++;
    }
    return 1;
}

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

void fromCharToInt(char *line, NUMBERS *num) {
    int i = 0, value = 0;
    num->coutNum = 0;
    num->numberInLine = NULL;

    while (line[i] != '\0' && line[i] != '\n') {
        while (line[i] == ' ') {
            i++;
        }

        if (line[i] == '-' || (line[i] >= '0' && line[i] <= '9')) {
            int sign = 1;
            if (line[i] == '-') {
                sign = -1;
                i++;
            }

            value = 0;
            while (line[i] >= '0' && line[i] <= '9') {
                value = value * 10 + (line[i] - '0');
                i++;
            }

            value *= sign;

            num->numberInLine = (int *)realloc(num->numberInLine, sizeof(int) * (num->coutNum + 1));
            if (!num->numberInLine) {
                puts("Ошибка выделения памяти!\n");
                exit(0);
            }

            num->numberInLine[num->coutNum] = value;
            num->coutNum++;
        }
    }
}

int findTask(char *filename, NUMBERS *num) {
    if (num->coutNum < 2) {
        puts("Недостаточно данных для вычисления!");
        exit(0);
    }

    int minMultip = 0;
    int *buff = (int *)malloc(sizeof(int) * num->coutNum);
    if (!buff) {
        puts("Ошибка выделения памяти!");
        exit(0);
    }

    FILE *file = fopen(filename, "rb");
    if (!file) {
        puts("Ошибка при открытии файла для чтения!");
        free(buff);
        exit(0);
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
        puts("Ошибка при открытии файла для чтения/записи!");
        exit(0);
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
        puts("Ошибка при открытии файла.");
        exit(0);
    }

    int value;
    while (fread(&value, sizeof(int), 1, file) == 1) {
        printf("%d ", value);
    }
    printf("\n");

    fclose(file);
}

void deleteMultipFive(const char *filename, NUMBERS *num){
    FILE *file = fopen(filename, "rb");
    if (!file) {
        puts("Ошибка при открытии файла для чтения!");
        return;
    }

    int *data = (int *)malloc(sizeof(int) * num->coutNum);
    if (!data) {
        puts("Ошибка выделения памяти!");
        fclose(file);
        return;
    }

    fread(data, sizeof(int), num->coutNum, file);
    fclose(file);

    int newCount = 0;
    for (int i = 0; i < num->coutNum; i++) {
        if (data[i] % 5 != 0) {
            data[newCount] = data[i];
            newCount++;
        }
    }

    file = fopen(filename, "wb");
    if (!file) {
        puts("Ошибка при открытии файла для записи!");
        free(data);
        return;
    }

    fwrite(data, sizeof(int), newCount, file);
    fclose(file);

    num->coutNum = newCount + 1;

    free(data);
}

