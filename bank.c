#include <stdio.h>
#include <stdlib.h>

#define MAX_ACC 100

struct clientData {
    unsigned int acctNum;
    char name[30];
    double balance;
};

// Prototypes
int menu();
void createAccount(FILE *fp);
void deposit(FILE *fp);
void withdraw(FILE *fp);
void deleteAccount(FILE *fp);
void viewAccount(FILE *fp);
void viewAll(FILE *fp);

int main() {
    FILE *fp;

    // open or create file
    if ((fp = fopen("bank.dat", "rb+")) == NULL) {
        fp = fopen("bank.dat", "wb+");

        struct clientData blank = {0, "", 0.0};
        for (int i = 0; i < MAX_ACC; i++) {
            fwrite(&blank, sizeof(blank), 1, fp);
        }
    }

    int choice;

    while ((choice = menu()) != 7) {
        switch (choice) {
            case 1: createAccount(fp); break;
            case 2: deposit(fp); break;
            case 3: withdraw(fp); break;
            case 4: deleteAccount(fp); break;
            case 5: viewAccount(fp); break;
            case 6: viewAll(fp); break;
            default: printf("Invalid choice\n");
        }
    }

    fclose(fp);
    return 0;
}

// MENU
int menu() {
    int ch;
    printf("\n===== BANK SYSTEM =====\n");
    printf("1. Create Account\n");
    printf("2. Deposit\n");
    printf("3. Withdraw\n");
    printf("4. Delete Account\n");
    printf("5. View Account\n");
    printf("6. View All\n");
    printf("7. Exit\n");
    printf("Enter choice: ");
    scanf("%d", &ch);
    return ch;
}

// CREATE ACCOUNT
void createAccount(FILE *fp) {
    struct clientData c = {0};
    int acc;

    printf("Enter account number (1-100): ");
    scanf("%d", &acc);

    if (acc < 1 || acc > MAX_ACC) {
        printf("Invalid account number\n");
        return;
    }

    fseek(fp, (acc - 1) * sizeof(c), SEEK_SET);
    fread(&c, sizeof(c), 1, fp);

    if (c.acctNum != 0) {
        printf("Account already exists!\n");
        return;
    }

    printf("Enter name: ");
    scanf("%s", c.name);

    printf("Enter initial balance: ");
    scanf("%lf", &c.balance);

    c.acctNum = acc;

    fseek(fp, (acc - 1) * sizeof(c), SEEK_SET);
    fwrite(&c, sizeof(c), 1, fp);

    printf("Account created successfully\n");
}

// DEPOSIT
void deposit(FILE *fp) {
    struct clientData c;
    int acc;
    double amt;

    printf("Enter account number: ");
    scanf("%d", &acc);

    fseek(fp, (acc - 1) * sizeof(c), SEEK_SET);
    fread(&c, sizeof(c), 1, fp);

    if (c.acctNum == 0) {
        printf("Account not found\n");
        return;
    }

    printf("Enter amount to deposit: ");
    scanf("%lf", &amt);

    if (amt <= 0) {
        printf("Invalid amount\n");
        return;
    }

    c.balance += amt;

    fseek(fp, -sizeof(c), SEEK_CUR);
    fwrite(&c, sizeof(c), 1, fp);

    printf("Deposit successful! Balance: %.2f\n", c.balance);
}

// WITHDRAW
void withdraw(FILE *fp) {
    struct clientData c;
    int acc;
    double amt;

    printf("Enter account number: ");
    scanf("%d", &acc);

    fseek(fp, (acc - 1) * sizeof(c), SEEK_SET);
    fread(&c, sizeof(c), 1, fp);

    if (c.acctNum == 0) {
        printf("Account not found\n");
        return;
    }

    printf("Enter amount to withdraw: ");
    scanf("%lf", &amt);

    if (amt <= 0 || amt > c.balance) {
        printf("Invalid or insufficient balance\n");
        return;
    }

    c.balance -= amt;

    fseek(fp, -sizeof(c), SEEK_CUR);
    fwrite(&c, sizeof(c), 1, fp);

    printf("Withdraw successful! Balance: %.2f\n", c.balance);
}

// DELETE
void deleteAccount(FILE *fp) {
    struct clientData c, blank = {0};
    int acc;

    printf("Enter account number: ");
    scanf("%d", &acc);

    fseek(fp, (acc - 1) * sizeof(c), SEEK_SET);
    fread(&c, sizeof(c), 1, fp);

    if (c.acctNum == 0) {
        printf("Account not found\n");
        return;
    }

    fseek(fp, (acc - 1) * sizeof(c), SEEK_SET);
    fwrite(&blank, sizeof(blank), 1, fp);

    printf("Account deleted\n");
}

// VIEW SINGLE
void viewAccount(FILE *fp) {
    struct clientData c;
    int acc;

    printf("Enter account number: ");
    scanf("%d", &acc);

    fseek(fp, (acc - 1) * sizeof(c), SEEK_SET);
    fread(&c, sizeof(c), 1, fp);

    if (c.acctNum == 0) {
        printf("No record found\n");
    } else {
        printf("\nAccount No: %d\nName: %s\nBalance: %.2f\n",
               c.acctNum, c.name, c.balance);
    }
}

// VIEW ALL
void viewAll(FILE *fp) {
    struct clientData c;

    rewind(fp);

    printf("\n--- ALL ACCOUNTS ---\n");

    while (fread(&c, sizeof(c), 1, fp) == 1) {
        if (c.acctNum != 0) {
            printf("Acc:%d | %s | %.2f\n",
                   c.acctNum, c.name, c.balance);
        }
    }
}