#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "accounts.h"
#include "console.h"

#define ACCOUNTS_FILE "accounts.bin"

void userMenu(Account *user, AccountNode *accounts, char **status) {
  int choice;
  bool showUserInfo = false;
  double amount;

  while (true) {
    clearScreen();
    printTitle("USER MENU", 4);

    if (showUserInfo) {
      printf("\nAccount Information\n");
      printBorder('-', 19);
      printf("Account ID: %d\n", user->id);
      printf("Name: %s\n", user->name);
      printf("Balance: %.2lf\n", user->balance);
    }

    if (*status != NULL) {
      printf("\n%s\n", *status);
      *status = NULL;
    }

    printf("\n[1] %s Account Information\n", showUserInfo ? "Hide" : "Show");
    printf("[2] Withdraw\n");
    printf("[3] Deposit\n");
    printf("[4] Logout\n");
    printf("[5] Exit\n\n");
    printf("Enter your choice: (1-5) ");
    scanf("%d", &choice);

    switch (choice) {
      case 1:
        showUserInfo = showUserInfo ? false : true;
        break;

      case 2:
        clearScreen();
        printTitle("WITHDRAW", 4);
        printf("Amount: ");
        scanf("%lf", &amount);

        if (user->balance - amount < 0) {
          *status = "Insufficient balance!";
          break;
        }

        if (user->balance > 0) {
          user->balance -= amount;
          saveAccounts(accounts, ACCOUNTS_FILE);
          *status = "Successfully withdrawn!";
        }
        break;

      case 3:
        clearScreen();
        printTitle("DEPOSIT", 4);
        printf("Amount: ");
        scanf("%lf", &amount);

        user->balance += amount;
        saveAccounts(accounts, ACCOUNTS_FILE);
        *status = "Successfully deposited!";
        break;

      case 4:
        *status = "Successfully logged out.";
        return;

      case 5:
        exit(0);

      default:
        *status = "** Unknown option **";
    }
  }
}

int main() {
  AccountNode *accounts = loadAccounts(ACCOUNTS_FILE);
  char *status = NULL;
  int choice;

  while (true) {
    clearScreen();
    printTitle("BANK MANAGEMENT SYSTEM", 2);

    if (status != NULL) {
      printf("\n%s\n", status);
      status = NULL;
    }

    printf("\n[1] Login\n");
    printf("[2] Create Account\n");
    printf("[3] Delete Account\n");
    printf("[4] Exit\n\n");
    printf("Enter your choice: (1-4) ");
    scanf("%d", &choice);

    // Variables used for case 1 & 3.
    int accID;
    char inputPwd[100];
    Account *acc;

    // Variables used for case 2.
    char name[100];
    char pwd[100];
    char uPwd[100];

    switch (choice) {
      case 1:
        if (accounts == NULL) {
          status = "** There are no accounts yet. **";
          break;
        }

        clearScreen();
        printTitle("LOGIN", 4);

        while (true) {
          printf("Account ID: ");
          scanf("%d", &accID);

          acc = findAccount(accounts, accID);
          if (acc == NULL) {
            printf("** Account not found **\n");
            continue;
          }
          break;
        }

        for (int i = 2; i >= 0; i--) {
          printf("Password: ");
          scanf("%99s", inputPwd);

          if (strcmp(inputPwd, acc->password) != 0) {
            if (i == 0) {
              status = "** Maximum attempts made. Please try again later. **";
              break;
            }

            printf("** Password did not match. %d attempts left. **\n", i);
            continue;
          }

          status = "Successfully logged in!";
          userMenu(acc, accounts, &status);
          break;
        }
        break;

      case 2:
        clearScreen();
        printTitle("CREATE ACCOUNT", 4);
        printf("Name: ");
        scanf("%99s", name);

        while (true) {
          printf("Password: ");
          scanf("%99s", pwd);
          printf("Confirm Password: ");
          scanf("%99s", uPwd);

          if (strcmp(pwd, uPwd) != 0) {
            printf("** Password did not match. Please try again. **\n");
            continue;
          }
          break;
        }

        Account *account = insertAccount(&accounts, name, pwd);
        saveAccounts(accounts, ACCOUNTS_FILE);
        status = "Successfully created!";
        userMenu(account, accounts, &status);
        break;

      case 3:
        if (accounts == NULL) {
          status = "** There are no accounts yet. **";
          break;
        }

        clearScreen();
        printTitle("DELETE ACCOUNT", 4);

        while (true) {
          printf("Account ID: ");
          scanf("%d", &accID);

          acc = findAccount(accounts, accID);
          if (acc == NULL) {
            printf("** Account not found **\n");
            continue;
          }
          break;
        }

        for (int i = 2; i >= 0; i--) {
          printf("Password: ");
          scanf("%99s", inputPwd);

          if (strcmp(inputPwd, acc->password) != 0) {
            if (i == 0) {
              status = "** Maximum attempts made. Please try again later. **";
              break;
            }

            printf("** Password did not match. %d attempts left. **\n", i);
            continue;
          }

          deleteAccount(&accounts, accID);
          saveAccounts(accounts, ACCOUNTS_FILE);
          status = "Account successfully deleted.";
          break;
        }
        break;

      case 4:
        return 0;
      default:
        status = "** Unknown option **";
    }
  }

  return 0;
}
