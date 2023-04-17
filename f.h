//
// Created by Вадим Герасимович on 4/17/23.
//

#ifndef S2LABA5_F_H
#define S2LABA5_F_H
#define MAX_SIZE 100

#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_EXPR_LEN 1000
#define MAX_STACK_LEN 1000

#endif //S2LABA5_F_H
int input(int min, int max);
typedef struct {
    int data[MAX_SIZE];
    int top;
} Stack;
void push(Stack* stack, int item);
int pop(Stack* stack);
int task1();
int task2();
int task3();
