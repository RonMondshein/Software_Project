#include <stdio.h> 
#include <math.h>
#include <stdlib.h>

int i = 0;
int sum = 0;
int read_digit(char c, int base) {
    if (c >= '0' && c <= '9') {
        int digit = c - '0';
        if (digit < base) {
            return digit;
        }
    } else if (c >= 'a' && c <= 'f') {
        int digit = c - 'a' + 10;
        if (digit < base) {
            return digit;
        }
    }
    printf("Invalid input number!\n");
    exit(1);
}

void read_number(int base) {
    char c;
    int digit = 0;
    
    if ((c = getchar()) != '\n') {
        digit = read_digit(c, base);
        read_number(base);
        sum += digit * pow(base, i);
        i++;
        
    }
    return ;
}

void print_digit(int digit) {
    if (digit < 10) {
        printf("%d", digit);
    } else {
        printf("%c", digit - 10 + 'a');
    }
}

void print_number(int number, int base) {
    if (number > 0) {
        print_number(number / base, base);
        print_digit(number % base);
    }
}


int main(void) {
    int source, target, number;
    printf("enter the source base:\n");
    scanf("%d", &source);
    if (source < 2 || source > 16) {
        printf("Invalid source base!\n");
        exit(1);
    }
    printf("enter the target base:\n");
    scanf("%d", &target);
    if (target < 2 || target > 16) {
        printf("Invalid target base!\n");
        exit(1);
    }
    printf("enter a number in base %d:\n", source);
    i = 0;
    getchar();
    read_number(source);
    number = sum;
    printf("The number in base %d is: ", target);
    if (number == 0) {
        printf("0");
    } else {
        print_number(number, target);
    }
    printf("\n");
    return 0;
}
