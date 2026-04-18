#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { SAVINGS, CHECKING, BUSINESS } AccountType;

typedef struct {
    int accountNumber;
    char ownerName[50];
    double balance;
    AccountType type;
} Account;

const char* FILENAME = "accounts.dat";

// Function Prototypes
void addAccount();
void deposit();
void withdraw();
void transfer();
void modifyType();
void accountInquiry();
Account* findAccount(int accNum, long *pos); // Helper to locate records
const char* getTypeName(AccountType type);

int main() {
    char choice;
    printf("--- Bank Management System (File Persistent) ---\n");

    while (1) {
        printf("\n(A) Add Acc | (T) Deposit | (V) Transfer | (R) Withdraw | (M) Modify | (C) Inquiry | (Q) Quit\n");
        printf("Selection: ");
        scanf(" %c", &choice);

        if (choice == 'Q' || choice == 'q') break;

        switch (choice) {
            case 'A': case 'a': addAccount(); break;
            case 'T': case 't': deposit(); break;
            case 'V': case 'v': transfer(); break;
            case 'R': case 'r': withdraw(); break;
            case 'M': case 'm': modifyType(); break;
            case 'C': case 'c': accountInquiry(); break;
            default: printf("Invalid choice.\n");
        }
    }
    return 0;
}

// Logic for adding a new account to the file
void addAccount() {
    FILE *file = fopen(FILENAME, "ab"); // Append binary
    if (!file) {
        perror("File Error");
        return;
    }
    Account acc;
    printf("Enter Account Number: "); scanf("%d", &acc.accountNumber);
    printf("Enter Owner Name: "); scanf(" %[^\n]", acc.ownerName);
    printf("Enter Initial Balance: "); scanf("%lf", &acc.balance);
    printf("Account Type (0:Savings, 1:Checking, 2:Business): "); 
    int t; scanf("%d", &t); acc.type = (AccountType)t;

    fwrite(&acc, sizeof(Account), 1, file);
    fclose(file);
    printf("Account stored successfully.\n");
}

// Logic for finding an account in the file
// Returns an allocated Account pointer and stores its byte position in 'pos'
Account* findAccount(int accNum, long *pos) {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) return NULL;

    Account *acc = malloc(sizeof(Account));
    while (fread(acc, sizeof(Account), 1, file)) {
        if (acc->accountNumber == accNum) {
            if (pos) *pos = ftell(file) - sizeof(Account);
            fclose(file);
            return acc;
        }
    }
    fclose(file);
    free(acc);
    return NULL;
}

// Logic for Deposit (T) - Updates existing record
void deposit() {
    int accNum;
    long pos;
    printf("Enter Account Number: "); scanf("%d", &accNum);

    Account *acc = findAccount(accNum, &pos);
    if (!acc) {
        printf("Error: Account not found.\n");
        return;
    }

    double amount;
    printf("Enter amount: "); scanf("%lf", &amount);
    if (amount <= 0) { printf("Invalid amount.\n"); free(acc); return; }

    acc->balance += amount;

    // Open in r+b to allow overwriting specific parts of the file
    FILE *file = fopen(FILENAME, "r+b");
    fseek(file, pos, SEEK_SET);
    fwrite(acc, sizeof(Account), 1, file);
    fclose(file);

    printf("Deposit successful. New Balance: $%.2f\n", acc->balance);
    free(acc);
}

// Logic for Inquiry (C)
void accountInquiry() {
    int accNum;
    printf("Enter Account Number: "); scanf("%d", &accNum);
    Account *acc = findAccount(accNum, NULL);
    if (acc) {
        printf("\n--- Details ---\nID: %d\nOwner: %s\nBalance: $%.2f\nType: %s\n", 
               acc->accountNumber, acc->ownerName, acc->balance, getTypeName(acc->type));
        free(acc);
    } else {
        printf("Account not found.\n");
    }
}

// Logic for Withdrawal (R)
void withdraw() {
    int accNum;
    long pos;
    printf("Enter Account Number: "); scanf("%d", &accNum);
    Account *acc = findAccount(accNum, &pos);
    
    if (acc) {
        double amount;
        printf("Enter amount: "); scanf("%lf", &amount);
        if (amount > 0 && amount <= acc->balance) {
            acc->balance -= amount;
            FILE *file = fopen(FILENAME, "r+b");
            fseek(file, pos, SEEK_SET);
            fwrite(acc, sizeof(Account), 1, file);
            fclose(file);
            printf("Withdrawal successful.\n");
        } else {
            printf("Invalid amount or insufficient funds.\n");
        }
        free(acc);
    }
}

// Logic for Transfer (V)
void transfer() {
    int fromNum, toNum;
    long posFrom, posTo;
    printf("Source Account: "); scanf("%d", &fromNum);
    printf("Destination Account: "); scanf("%d", &toNum);

    Account *fromAcc = findAccount(fromNum, &posFrom);
    Account *toAcc = findAccount(toNum, &posTo);

    if (fromAcc && toAcc) {
        double amount;
        printf("Amount to transfer: "); scanf("%lf", &amount);
        if (amount > 0 && amount <= fromAcc->balance) {
            fromAcc->balance -= amount;
            toAcc->balance += amount;

            FILE *file = fopen(FILENAME, "r+b");
            fseek(file, posFrom, SEEK_SET);
            fwrite(fromAcc, sizeof(Account), 1, file);
            fseek(file, posTo, SEEK_SET);
            fwrite(toAcc, sizeof(Account), 1, file);
            fclose(file);
            printf("Transfer successful.\n");
        } else {
            printf("Transfer failed.\n");
        }
    } else {
        printf("One or both accounts not found.\n");
    }
    if (fromAcc) free(fromAcc);
    if (toAcc) free(toAcc);
}

// Logic for Modification (M)
void modifyType() {
    int accNum;
    long pos;
    printf("Enter Account Number: "); scanf("%d", &accNum);
    Account *acc = findAccount(accNum, &pos);

    if (acc) {
        int newType;
        printf("Current Type: %s. New Type (0:Savings, 1:Checking, 2:Business): ", getTypeName(acc->type));
        scanf("%d", &newType);
        acc->type = (AccountType)newType;

        FILE *file = fopen(FILENAME, "r+b");
        fseek(file, pos, SEEK_SET);
        fwrite(acc, sizeof(Account), 1, file);
        fclose(file);
        printf("Account modified.\n");
        free(acc);
    }
}

const char* getTypeName(AccountType type) {
    switch (type) {
        case SAVINGS: return "Savings";
        case CHECKING: return "Checking";
        case BUSINESS: return "Business";
        default: return "Unknown";
    }
}
