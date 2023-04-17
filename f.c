//
// Created by Вадим Герасимович on 4/17/23.
//

#include "f.h"
int input(int min, int max) {
    int x;
    while (scanf("%d", &x) != 1 || x < min || x > max || x % 1 != 0 || getchar() != '\n') {
        printf("Error. Try again: ");
        rewind(stdin);
    }
    return x;
}

void push(Stack* stack, int item) {
    if (stack->top == MAX_SIZE) {
        printf("Error: stack overflow\n");
        exit(EXIT_FAILURE);
    }
    stack->data[++stack->top] = item;
}
int pop(Stack* stack) {
    if (stack->top == 0) {
        printf("Error: stack underflow\n");
        exit(EXIT_FAILURE);
    }
    return stack->data[stack->top--];
}
// Стек для хранения операндов
double operand_stack[MAX_STACK_LEN];

int operand_top = -1;
// Стек для хранения операторов
char operator_stack[MAX_STACK_LEN];

int operator_top = -1;

// Функция, которая возвращает true, если символ - оператор
int is_operator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

// Функция, которая возвращает true, если символ - скобка
int is_bracket(char c) {
    return (c == '(' || c == ')');
}

// Функция, которая возвращает true, если приоритет оператора1 >= приоритета оператора2
int has_higher_or_equal_precedence(char operator1, char operator2) {
    int precedence1, precedence2;
    switch (operator1) {
        case '+':
        case '-':
            precedence1 = 1;
            break;
        case '*':
        case '/':
            precedence1 = 2;
            break;
        case '^':
            precedence1 = 3;
            break;
        default:
            precedence1 = 0;
            break;
    }
    switch (operator2) {
        case '+':
        case '-':
            precedence2 = 1;
            break;
        case '*':
        case '/':
            precedence2 = 2;
            break;
        case '^':
            precedence2 = 3;
            break;
        default:
            precedence2 = 0;
            break;
    }
    return (precedence1 >= precedence2);
}

// Функция, которая преобразует арифметическое выражение в постфиксную нотацию
int infix_to_postfix(char* infix_expr, char* postfix_expr) {
    int i, j;
    char c;
    int len = strlen(infix_expr);
    operand_top = -1;
    operator_top = -1;
    for (i = 0, j = 0; i < len; i++) {
        c = infix_expr[i];
        if (isdigit(c) || c == '.') {
            postfix_expr[j++] = c;
            while (isdigit(infix_expr[i+1]) || infix_expr[i+1] == '.') {
                postfix_expr[j++] = infix_expr[++i];
            }
            postfix_expr[j++] = ' ';
        } else if (is_operator(c)) {
            while (operator_top >= 0 && operator_stack[operator_top] != '(' && has_higher_or_equal_precedence(operator_stack[operator_top], c)) {
                postfix_expr[j++] = operator_stack[operator_top--];
                postfix_expr[j++] = ' ';
            }
            operator_stack[++operator_top] = c;
        } else if (c == '(') {
            operator_stack[++operator_top] = c;
        } else if (c == ')') {
            while (operator_top >= 0 && operator_stack[operator_top] != '(') {
                postfix_expr[j++] = operator_stack[operator_top--];
                postfix_expr[j++] = ' ';
            }
            if (operator_top < 0 || operator_stack[operator_top] != '(') {
                return i + 1; // ошибка - несогласованные скобки
            }
            operator_top--;
        } else if (!isspace(c)) {
            return i + 1; // ошибка - недопустимый символ
        }
    }
    while (operator_top >= 0) {
        if (is_bracket(operator_stack[operator_top])) {
            return len + 1; // ошибка - несогласованные скобки
        }
        postfix_expr[j++] = operator_stack[operator_top--];
        postfix_expr[j++] = ' ';
    }
    postfix_expr[j] = '\0';
    return 0; // успешное завершение
}

// Функция, которая вычисляет значение постфиксного выражения
double evaluate_postfix(char* postfix_expr) {
    int i;
    double operand1, operand2;
    char* p;
    operand_top = -1;
    i = 0;
    while (postfix_expr[i] != '\0') {
        if (isdigit(postfix_expr[i]) || postfix_expr[i] == '.') {
            operand_stack[++operand_top] = strtod(&postfix_expr[i], &p);
            i = p - postfix_expr;
        } else if (is_operator(postfix_expr[i])) {
            operand2 = operand_stack[operand_top--];
            operand1 = operand_stack[operand_top--];
            switch (postfix_expr[i]) {
                case '+':
                    operand_stack[++operand_top] = operand1 + operand2;
                    break;
                case '-':
                    operand_stack[++operand_top] = operand1 - operand2;
                    break;
                case '*':
                    operand_stack[++operand_top] = operand1 * operand2;
                    break;
                case '/':
                    if (operand2 == 0.0) {
                        return NAN; // ошибка - деление на ноль
                    }
                    operand_stack[++operand_top] = operand1 / operand2;
                    break;
                case '^':
                    operand_stack[++operand_top] = pow(operand1, operand2);
                    break;
            }
            i++;
        } else {
            i++;
        }
    }
    return operand_stack[0];
}
// Главная функция
int task3() {
    FILE* input_file;
    FILE* output_file;
    char infix_expr[MAX_EXPR_LEN];
    char postfix_expr[MAX_EXPR_LEN];
    double result;
    int error_pos;
    input_file = fopen("/Users/admin/Desktop/OAiP/S2LABA5/input.txt", "rt");
    if (input_file == NULL) {
        printf("Ошибка: не удалось открыть входной файл\n");
        return 1;
    }
    output_file = fopen("/Users/admin/Desktop/OAiP/S2LABA5/output.txt", "wt");
    if (output_file == NULL) {
        printf("Ошибка: не удалось открыть выходной файл\n");
        fclose(input_file);
        return 1;
    }
    while (fgets(infix_expr, MAX_EXPR_LEN, input_file) != NULL) {
        int len = strlen(infix_expr);
        if (infix_expr[len - 1] == '\n') {
            infix_expr[len - 1] = '\0';
        }
        // Преобразуем инфиксное выражение в постфиксное и проверяем на ошибки
        error_pos = infix_to_postfix(infix_expr, postfix_expr);
        if (error_pos > 0) {
            // В выражении есть ошибка - записываем ее в выходной файл
            fprintf(output_file, "Ошибка в позиции %d\n", error_pos);
        } else {
            // Вычисляем значение постфиксного выражения
            result = evaluate_postfix(postfix_expr);

            if (isnan(result)) {
                // В выражении есть ошибка - записываем ее в выходной файл
                fprintf(output_file, "Ошибка: деление на ноль\n");
            } else {
                // Записываем результат в выходной файл
                fprintf(output_file, "%.2f\n", result);
            }
        }
    }
    fclose(input_file);
    fclose(output_file);
    printf("Task completed\n");
}

typedef struct {
    int data[MAX_SIZE];
    int top;
} Stack1;

void init(Stack *s) {
    s->top = -1;
}

int is_empty(Stack *s) {
    return (s->top == -1);
}

int is_full(Stack *s) {
    return (s->top == MAX_SIZE - 1);
}

void push2(Stack *s, int x) {
    if (is_full(s)) {
        printf("Стек переполнен\n");
        exit(1);
    }
    s->top++;
    s->data[s->top] = x;
}

int pop2(Stack *s) {
    if (is_empty(s)) {
        printf("Стек пуст\n");
        exit(1);
    }
    int x = s->data[s->top];
    s->top--;
    return x;
}

int top(Stack *s) {
    if (is_empty(s)) {
        printf("Стек пуст\n");
        exit(1);
    }
    return s->data[s->top];
}

void print(Stack *s) {
    int i;
    printf("[ ");
    for (i = 0; i <= s->top; i++) {
        printf("%d ", s->data[i]);
    }
    printf("]\n");
}

int task2() {
    Stack1 s1, s2, s3;
    int n, i, x, max, min;
    init(&s1);
    init(&s2);
    init(&s3);
    printf("Введите количество элементов в стеках: ");
    n= input(1,99);
    printf("Введите элементы в первый стек по убыванию:\n");
    printf("Enter element 1: ");
    x= input(-9999,9999);
    push2(&s1,x);
    max=x;
    for (i = 0; i < n-1; i++) {
        printf("Enter element %d: ", i+2);
        while (scanf("%d", &x) != 1 || x < -9999 || x > max || x % 1 != 0 || getchar() != '\n') {
            printf("Item > max. Try again: ");
            rewind(stdin);
        }
        push2(&s1, x);
        max=x;
    }
    printf("Введите элементы во второй стек по возрастанию:\n");
    printf("Enter element 1: ");
    x= input(-9999,9999);
    push2(&s2,x);
    min=x;
    for (i = 0; i < n-1; i++) {
        printf("Enter element %d: ", i+2);
        while (scanf("%d", &x) != 1 || x < min || x > 9999 || x % 1 != 0 || getchar() != '\n') {
            printf("Item < max. Try again: ");
            rewind(stdin);
        }
        push2(&s2, x);
        min=x;
    }
    int p=0;
    /*while (!is_empty(&s1) && !is_empty(&s2)) {
        if (top(&s1) <= top(&s2)) {
            push2(&s3, pop2(&s1));
        } else {
            push2(&s3, pop2(&s2));
        }
    }
    while (!is_empty(&s1)) {
        push2(&s3, pop2(&s1));
    }
    while (!is_empty(&s2)) {
        push2(&s3, pop2(&s2));
    }*/
    while (!is_empty(&s1) && p!=n) {
        if (top(&s1) <= s2.data[p]) {
            push2(&s3, pop2(&s1));
        } else {
            push2(&s3, s2.data[p]); p++;
        }
    }
    while (!is_empty(&s1)) {
        push2(&s3, pop2(&s1));
    }
    while (p!=n) {
        push2(&s3, s2.data[p]); p++;
    }
    printf("Упорядоченный по возрастанию третий стек:\n");
    print(&s3);
    return 0;
}
int task1(){
    int size, item, min=9999, sum = 0;
    Stack stack = {.top =0};
    printf("Enter the size of the stack: ");
    size = input(1,99);
    for (int i = 0; i < size; i++) {
        printf("Enter element %d: ", i+1);
        item= input(-9999,9999);
        push(&stack, item);
    }
    while (stack.top != 0) {
        item = pop(&stack);
        if (item<min) {
            min=item;
        }
    }
    stack.top=size;
    while (stack.top != 0 && stack.data[stack.top] > min) {
        item = pop(&stack);
        sum += item;
    }
    printf("Sum of all elements before the minimum element: %d\n", sum);
    return 0;
}