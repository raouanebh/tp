#include <stdio.h>

/**
 * Recursive function to count digits
 * Logic: 
 * - Base Case: If the number is 0, return 0.
 * - Recursive Step: Return 1 + countDigits(n / 10)
 */
int countDigits(int n) {
    // We use absolute value to handle negative numbers
    if (n < 0) n = -n;

    // Base case: when the number is reduced to 0
    if (n == 0) {
        return 0;
    }
    
    // Recursive step: add 1 for the current digit and 
    // call the function again with the number divided by 10
    return 1 + countDigits(n / 10);
}

int main() {
    int number, totalDigits;

    printf("Enter a number: ");
    if (scanf("%d", &number) != 1) {
        printf("Invalid input. Please enter an integer.\n");
        return 1;
    }

    // Special case for 0, as the recursive function would return 0
    if (number == 0) {
        totalDigits = 1;
    } else {
        totalDigits = countDigits(number);
    }

    printf("The number of digits in the number is: %d\n", totalDigits);

    return 0;
}
