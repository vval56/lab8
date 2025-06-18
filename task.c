#include "function.h"

void show_menu(){
    puts("1 - Записать числа в файл.");
    puts("2 - Вывести номер элемента, произведение которого с предыдущим элементом минимально.");
    puts("3 - Попарно поменять элементы местами.");
    puts("4 - Удалить все кратные 5 элементы");
    puts("5 - Вывести данные из файлы");
    puts("0 - Выход");
}

void press_enter_to_continue() {
    printf("\nНажмите Enter для продолжения...");
    getchar(); getchar(); // для считывания \n после scanf
}

int main(int argc, char *argv[]) {
    int * numbers = NULL;
    char *filename = argv[1];
    int choice = 0, size = 0;

    if (argc < 2) {
        printf("Использование: %s <имя_файла>\n", argv[0]);
        return 1;
    }
    
    FILE *file = fopen(filename, "wb");
    if (!file) {
        puts("Ошибка открытия файла");
        return 0;
    }

    while(1){
        system("clear");
        show_menu();
        choice = input();

        switch (choice){
            case 1:
                system("clear");
                puts("Введите числа (или два раза Enter для завершения): ");
                while(input_int_numbers(&numbers, &size) == 0){
                    puts("Введите еще раз");
                }
                fwrite(numbers, sizeof(int), size, file);
                fclose(file);
                break;

            case 2:
                system("clear");
                printf("номер элемента, произведение которого с предыдущим элементом минимально - %d\n", find_task(filename, size));
                press_enter_to_continue();
                break;

            case 3:
                system("clear");
                swapNeighInFile(filename, size);
                press_enter_to_continue();
                break;

            case 4:
                system("clear");
                deleteMultipFive(filename, &size);
                press_enter_to_continue();
                break;

            case 5:
                system("clear");
                print_file(filename);
                press_enter_to_continue();
                break;

            case 0:
                return 1;
            
            default:
                system("clear");
                puts("Введите значения от 0 до 5");
                press_enter_to_continue();
                break;

        }
    }

    free(numbers);
    fclose(file);
}