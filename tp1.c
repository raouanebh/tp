#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* ─── Constants ─── */
#define MAX_CLIENTS  100

/* ─── Date structure ─── */
typedef struct {
    int day;
    int month;
    int year;
} Date;

/* ─── Client record (as specified) ─── */
typedef struct {
    int  id;
    char name[30];
    char firstName[30];
    Date dateOfBirth;
    char address[100];
    char tel[20];
} Client;

/* ─── Account record (as specified) ─── */
typedef struct {
    int  clientId;
    char accountType;   /* 'P' = Individual, 'M' = Minor, 'C' = Commercial */
    int  balance;
    bool blocked;
} Account;

/* ─── Global storage ─── */
Client  clients[MAX_CLIENTS];
Account accounts[MAX_CLIENTS];
int     clientCount = 0;

/* ═══════════════════════════════════════
   UTILITY FUNCTIONS
   ═══════════════════════════════════════ */

/* Calculate age from date of birth to today (approximate) */
int calculateAge(Date dob) {
    /* Using a fixed "today" for portability; replace with time() if needed */
    Date today = {17, 4, 2026};
    int age = today.year - dob.year;
    if (today.month < dob.month ||
       (today.month == dob.month && today.day < dob.day)) {
        age--; /* birthday not yet passed this year */
    }
    return age;
}

/* Find a client's index by ID; returns -1 if not found */
int findClientById(int id) {
    for (int i = 0; i < clientCount; i++) {
        if (clients[i].id == id) return i;
    }
    return -1;
}

/* Check if a client ID already has an account */
bool clientHasAccount(int id) {
    for (int i = 0; i < clientCount; i++) {
        if (accounts[i].clientId == id) return true;
    }
    return false;
}

/* Read a Date from the user */
Date inputDate(void) {
    Date d;
    printf("  Day (dd): ");   scanf("%d", &d.day);
    printf("  Month (mm): "); scanf("%d", &d.month);
    printf("  Year (yyyy): "); scanf("%d", &d.year);
    return d;
}

/* ═══════════════════════════════════════
   CORE PROCEDURES
   ═══════════════════════════════════════ */

/* ── 1. ADD CLIENT + ACCOUNT ── */
void addClient(void) {
    if (clientCount >= MAX_CLIENTS) {
        printf("\n[!] Storage full. Cannot add more clients.\n");
        return;
    }

    Client  c;
    Account a;

    printf("\n--- New Client ---\n");
    printf("ID: ");
    scanf("%d", &c.id);

    /* Duplicate ID check */
    if (findClientById(c.id) != -1) {
        printf("[!] A client with ID %d already exists.\n", c.id);
        return;
    }

    printf("Last name: ");  scanf("%29s", c.name);
    printf("First name: "); scanf("%29s", c.firstName);

    printf("Date of birth:\n");
    c.dateOfBirth = inputDate();

    printf("Address: "); scanf(" %[^\n]", c.address);
    printf("Phone:   "); scanf("%19s", c.tel);

    /* ── Account type selection ── */
    printf("\nAccount type  P = Individual  M = Minor  C = Commercial\n");
    printf("Choice: ");
    scanf(" %c", &a.accountType);

    /* ── Enforce conditions per type ── */
    if (a.accountType == 'P' || a.accountType == 'p') {
        a.accountType = 'P';
        int age = calculateAge(c.dateOfBirth);
        if (age < 18) {
            printf("[!] Client is %d years old. Must be at least 18 for an Individual account.\n", age);
            return;
        }

    } else if (a.accountType == 'M' || a.accountType == 'm') {
        a.accountType = 'M';
        int guardianId;
        printf("Enter the guardian's client ID: ");
        scanf("%d", &guardianId);

        /* Guardian must already have an account in this bank */
        if (!clientHasAccount(guardianId)) {
            printf("[!] Guardian (ID %d) does not have an account here. Cannot open minor account.\n", guardianId);
            return;
        }

    } else if (a.accountType == 'C' || a.accountType == 'c') {
        a.accountType = 'C';
        /* No extra conditions for commercial accounts */

    } else {
        printf("[!] Invalid account type.\n");
        return;
    }

    /* ── Set initial balance & status ── */
    printf("Initial balance: ");
    scanf("%d", &a.balance);
    a.clientId = c.id;
    a.blocked  = false;

    /* ── Save client and account ── */
    clients[clientCount]  = c;
    accounts[clientCount] = a;
    clientCount++;

    printf("[+] Client and account created successfully.\n");
}

/* ── 2. SEARCH CLIENT ── */
void searchClient(void) {
    int id;
    printf("\nEnter client ID to search: ");
    scanf("%d", &id);

    int idx = findClientById(id);
    if (idx == -1) {
        printf("[!] Client with ID %d not found.\n", id);
        return;
    }

    Client  *c = &clients[idx];
    Account *a = &accounts[idx];

    printf("\n--- Client Record ---\n");
    printf("ID        : %d\n",   c->id);
    printf("Name      : %s %s\n", c->firstName, c->name);
    printf("Birth     : %02d/%02d/%04d\n", c->dateOfBirth.day, c->dateOfBirth.month, c->dateOfBirth.year);
    printf("Address   : %s\n",  c->address);
    printf("Phone     : %s\n",  c->tel);
    printf("Acc. Type : %c\n",  a->accountType);
    printf("Balance   : %d\n",  a->balance);
    printf("Blocked   : %s\n",  a->blocked ? "Yes" : "No");
}

/* ── 3. MODIFY CLIENT ── */
void modifyClient(void) {
    int id;
    printf("\nEnter client ID to modify: ");
    scanf("%d", &id);

    int idx = findClientById(id);
    if (idx == -1) {
        printf("[!] Client with ID %d not found.\n", id);
        return;
    }

    Client *c = &clients[idx];

    printf("New last name  [current: %s]: ", c->name);
    scanf("%29s", c->name);

    printf("New first name [current: %s]: ", c->firstName);
    scanf("%29s", c->firstName);

    printf("New address    [current: %s]: ", c->address);
    scanf(" %[^\n]", c->address);

    printf("New phone      [current: %s]: ", c->tel);
    scanf("%19s", c->tel);

    printf("[+] Client updated.\n");
}

/* ── 4. DELETE CLIENT ── */
void deleteClient(void) {
    int id;
    printf("\nEnter client ID to delete: ");
    scanf("%d", &id);

    int idx = findClientById(id);
    if (idx == -1) {
        printf("[!] Client with ID %d not found.\n", id);
        return;
    }

    /* Shift remaining entries left to fill the gap */
    for (int i = idx; i < clientCount - 1; i++) {
        clients[i]  = clients[i + 1];
        accounts[i] = accounts[i + 1];
    }
    clientCount--;

    printf("[+] Client (ID %d) deleted.\n", id);
}

/* ── 5. DISPLAY ALL CLIENTS ── */
void displayAllClients(void) {
    if (clientCount == 0) {
        printf("\nNo clients registered yet.\n");
        return;
    }
    printf("\n%-6s %-15s %-15s %-5s %-10s %-8s\n",
           "ID", "Last Name", "First Name", "Type", "Balance", "Blocked");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < clientCount; i++) {
        printf("%-6d %-15s %-15s %-5c %-10d %-8s\n",
               clients[i].id,
               clients[i].name,
               clients[i].firstName,
               accounts[i].accountType,
               accounts[i].balance,
               accounts[i].blocked ? "Yes" : "No");
    }
}

/* ═══════════════════════════════════════
   MAIN MENU
   ═══════════════════════════════════════ */
void displayMenu(void) {
    printf("\n=============================\n");
    printf("   BANK CLIENT MANAGEMENT    \n");
    printf("=============================\n");
    printf(" 1. Add client\n");
    printf(" 2. Search client\n");
    printf(" 3. Modify client\n");
    printf(" 4. Delete client\n");
    printf(" 5. Display all clients\n");
    printf(" 0. Exit\n");
    printf("-----------------------------\n");
    printf("Choice: ");
}

int main(void) {
    int choice;

    do {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1: addClient();        break;
            case 2: searchClient();     break;
            case 3: modifyClient();     break;
            case 4: deleteClient();     break;
            case 5: displayAllClients(); break;
            case 0: printf("Goodbye.\n"); break;
            default: printf("[!] Invalid choice. Try again.\n");
        }

    } while (choice != 0);

    return 0;
}
