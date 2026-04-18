#include <stdio.h>
#include <ctype.h> // Required for isupper()

/**
 * Recursive function to find the first uppercase letter
 * @param str: Pointer to the current character in the string
 * @return: The uppercase character if found, or '\0' if not found
 */
char findFirstUppercase(char *str) {
    // Base Case 1: End of string reached (null terminator)
    if (*str == '\0') {
        return '\0';
    }

    // Base Case 2: Current character is uppercase
    if (isupper(*str)) {
        return *str;
    }

    // Recursive Step: Move to the next character in the string
    return findFirstUppercase(str + 1);
}

int main() {
    char input[100];
    char result;

    printf("Enter a string: ");
    // Using scanf with %[^\n] to allow spaces in the input string
    scanf("%[^\n]", input);

    result = findFirstUppercase(input);

    if (result != '\0') {
        printf("Output : %c\n", result);
    } else {
        printf("No uppercase letter found in the string.\n");
    }

    return 0;
}
