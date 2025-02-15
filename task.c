#include <stdio.h>
#include <stdlib.h>

int checkNumValue(char * line){
    int i = 0;
    while(line[i] != '\0' && line[i] != '\n'){
        if (!((line[i] >= '0' && line[i] <= '9') || line[i] == ' ')) {
            puts("Неверный ввод");
            return 0;
        }
        i++;
    }
    return 1;
}

void inputLine(char ** line){
    do{
        int read = 0;
        size_t n = 0;

        read = getline(line, &n, stdin);
        if(read == -1){
            puts("Ошибка выделенной памяти");
            continue;
        }

    }while(checkNumValue(*line) == 0);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Использование: %s <имя_файла>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    FILE *file = fopen(filename, "wb");
    if (!file) {
        puts("Ошибка открытия файла");
        return 1;
    }

    char *lineNum = NULL;

    puts("Введите числа:");
    inputLine(&lineNum);

    free(lineNum);
    fclose(file);
    return 0;
}
