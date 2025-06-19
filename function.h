#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define SIZE_BUFFER 256

int input() {
    char s[20];
    int i = 0, valid = 1;
    
    do {
        i = 0;
        valid = 1;
        
        while (1) {
            s[i] = getchar();
            
            if (s[i] == '\n') {
                if (i == 0) {
                    printf("Ошибка: пустой ввод. Попробуйте снова: ");
                    valid = 0;
                }
                break;
            }

            if (s[i] < '0' || s[i] > '9') {
                valid = 0;
            }
            
            if (++i >= 19) {
                printf("Ошибка: слишком длинный ввод. Попробуйте снова: ");
                while (getchar() != '\n');
                valid = 0;
                break;
            }
        }
        
        s[i] = '\0';
        
        if (!valid) {
            printf("Ошибка: недопустимый ввод. Попробуйте снова: ");
        }
    } while (!valid);
    
    return atoi(s);
}

int my_isdigit(char symbol) {
    return (symbol >= '0' && symbol <= '9') ? 1 : 0;
}

int is_line_empty(const char *line) {
    for (int i = 0; line[i]; i++) {
        if (!isspace(line[i])) return 0; 
    }
    return 1;
}

int input_int_numbers(int **numbers, int * size){
    int empty_lines = 0;
    char input[SIZE_BUFFER];

    *numbers = NULL;

    while (empty_lines < 2) {
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            puts("Ошибка чтения строки");
            return 0;
        }

        if (is_line_empty(input)) {
            empty_lines++;
            continue;
        } else {
            empty_lines = 0;
        }

        int i = 0;
        while (input[i] != '\0') {
            while (input[i] == ' ') i++;

            int sign = 1;
            int value = 0;

            if (input[i] == '-') {
                sign = -1;
                i++;
            }

            if (!my_isdigit(input[i])) {
                if (input[i] != '\0' && input[i] != '\n') {
                    printf("Ошибка: недопустимый символ '%c'\n", input[i]);
                    return 0;
                }
                break;
            }

            while (my_isdigit(input[i])) {
                value = value * 10 + (input[i] - '0');
                i++;
            }

            value *= sign;

            int *temp = realloc(*numbers, sizeof(int) * (*size + 1));
            if (!temp) {
                puts("Ошибка выделения памяти");
                free(*numbers);
                return 0;
            }

            *numbers = temp;
            (*numbers)[(*size)++] = value;
        }
    }   

    return 1;
}

int find_task(const char *filename, int size) {
    if (size < 2) {
        puts("Недостаточно данных для вычисления!");
        return -1;
    }

    FILE *file = fopen(filename, "rb");
    if (!file) {
        puts("Ошибка открытия файла");
        return -1;
    }

    int prev, curr;
    int min_product = __INT_MAX__;
    int position = 0;

    fread(&prev, sizeof(int), 1, file);

    for (int i = 1; i < size; i++) {
        fread(&curr, sizeof(int), 1, file);

        int product = prev * curr;
        if (product < min_product) {
            min_product = product;
            position = i + 1; 
        }

        prev = curr;
    }

    fclose(file);
    return position;
}

void swapNeighInFile(char *filename, int size){
    FILE *file = fopen(filename, "r+b");
    if (file == NULL) {
        puts("Ошибка при открытии файла для чтения/записи!");
        exit(0);
    }

    int num1, num2;

    for (int i = 0; i < size - 1; i += 2) {
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

void print_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        puts("Ошибка при открытии файла.");
        exit(0);
    }

    int value;
    while (fread(&value, sizeof(int), 1, file) == 1) {
        printf("%d ", value);
    }
    puts("");

    fclose(file);
}

void deleteMultipFive(const char *filename, int *size) {
    FILE *file = fopen(filename, "r+b");
    if (!file) {
        puts("Ошибка при открытии файла для чтения!");
        return;
    }

    int value;
    int write_pos = 0; 

    for (int read_pos = 0; read_pos < *size; read_pos++) {
        fseek(file, read_pos * sizeof(int), SEEK_SET);
        fread(&value, sizeof(int), 1, file);

        if (value % 5 != 0) {
            fseek(file, write_pos * sizeof(int), SEEK_SET);
            fwrite(&value, sizeof(int), 1, file);
            write_pos++;
        }
    }

    ftruncate(fileno(file), write_pos * sizeof(int));

    if(write_pos == *size){
        puts("Нету элементов, кратных 5");
    }

    if(write_pos == 0){
        puts("Все числа кратны 5");
    }
    else{
        printf("Удалено %d элементов, кратных 5\n", *size - write_pos);
        *size = write_pos;
    }

    fclose(file);
}