#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define account types for better readability
typedef enum { SAVINGS, CHECKING, BUSINESS } AccountType;

// Structure to store account details
typedef struct {
    int accountNumber;
    char ownerName[50];
    double balance;
    AccountType type;
} Account;

// Function prototypes
void deposit(Account *acc);
void withdraw(Account *acc);
void transfer(Account *source, Account *dest);
void modifyAccountType(Account *acc);
void displayAccount(Account acc);
const char* getTypeName(AccountType type);

int main() {
    // Initializing a dummy account for demonstration
    Account myAcc = {1001, "John Doe", 500.00, CHECKING};
    Account otherAcc = {2002, "Jane Smith", 1000.00, SAVINGS};
    
    char choice;
    int running = 1;

    printf("--- Welcome to the Bank Management System ---\n");

    while (running) {
        printf("\nOperations: (T) Deposit | (V) Transfer | (R) Withdrawal | (M) Modify | (C) Inquiry | (Q) Quit\n");
        printf("Select an operation: ");
        scanf(" %c", &choice);

        switch (choice) {
            case 'T': case 't':
                deposit(&myAcc);
                break;
            case 'V': case 'v':
                transfer(&myAcc, &otherAcc);
                break;
            case 'R': case 'r':
                withdraw(&myAcc);
                break;
            case 'M': case 'm':
                modifyAccountType(&myAcc);
                break;
            case 'C': case 'c':
                displayAccount(myAcc);
                break;
            case 'Q': case 'q':
                running = 0;
                printf("Exiting system. Goodbye!\n");
                break;
            default:
                printf("Invalid option. Please try again.\n");
        }
    }

    return 0;
}

// Logic for Deposit (T)
void deposit(Account *acc) {
    double amount;
    printf("Enter amount to deposit: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0) {
        printf("Error: Invalid deposit amount.\n");
        while(getchar() != '\n'); // Clear buffer
        return;
    }
    acc->balance += amount;
    printf("Successfully deposited $%.2f. New Balance: $%.2f\n", amount, acc->balance);
}

// Logic for Withdrawal (R)
void withdraw(Account *acc) {
    double amount;
    printf("Enter amount to withdraw: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0) {
        printf("Error: Invalid withdrawal amount.\n");
        while(getchar() != '\n');
        return;
    }
    
    // Validation for sufficient funds
    if (amount > acc->balance) {
        printf("Error: Insufficient funds. Current balance: $%.2f\n", acc->balance);
    } else {
        acc->balance -= amount;
        printf("Successfully withdrew $%.2f. New Balance: $%.2f\n", amount, acc->balance);
    }
}

// Logic for Transfer (V)
void transfer(Account *source, Account *dest) {
    double amount;
    printf("Transferring from %s to %s.\nEnter amount: ", source->ownerName, dest->ownerName);
    if (scanf("%lf", &amount) != 1 || amount <= 0) {
        printf("Error: Invalid transfer amount.\n");
        while(getchar() != '\n');
        return;
    }

    if (amount > source->balance) {
        printf("Error: Transfer failed. Insufficient funds in source account.\n");
    } else {
        source->balance -= amount;
        dest->balance += amount;
        printf("Transfer complete! $%.2f moved to %s.\n", amount, dest->ownerName);
    }
}

// Logic for Modification (M)
void modifyAccountType(Account *acc) {
    int choice;
    printf("Current Type: %s\n", getTypeName(acc->type));
    printf("New Type (0: SAVINGS, 1: CHECKING, 2: BUSINESS): ");
    if (scanf("%d", &choice) != 1 || choice < 0 || choice > 2) {
        printf("Error: Invalid account type selected.\n");
        while(getchar() != '\n');
        return;
    }
    acc->type = (AccountType)choice;
    printf("Account type updated successfully to %s.\n", getTypeName(acc->type));
}

// Logic for Inquiry (C)
void displayAccount(Account acc) {
    printf("\n--- Account Inquiry ---\n");
    printf("Account Number: %d\n", acc.accountNumber);
    printf("Owner:          %s\n", acc.ownerName);
    printf("Type:           %s\n", getTypeName(acc.type));
    printf("Current Balance: $%.2f\n", acc.balance);
    printf("-----------------------\n");
}

// Helper to convert Enum to String
const char* getTypeName(AccountType type) {
    switch (type) {
        case SAVINGS: return "Savings";
        case CHECKING: return "Checking";
        case BUSINESS: return "Business";
        default: return "Unknown";
    }
}
