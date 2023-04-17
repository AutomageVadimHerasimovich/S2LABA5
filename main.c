#include "f.h"
int main() { int ch;
    do {
        printf("Введите номер задания :\n");
        ch = input(1, 3);
        switch (ch) {
            case 1:
                task1(); break;
            case 2:
                task2(); break;
            case 3:
                task3(); break;
        }
        printf("\nХотите выбрать задание ещё раз?\n1-Да 2-Нет\n");
        ch = input(1, 2);
    } while (ch!=2);
    return 0;
}
