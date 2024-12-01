#ifndef ACCOUNTS_H
#define ACCOUNTS_H

typedef struct {
  int id;
  char *name;
  char *password;  // TODO: hash the password
  double balance;
} Account;

typedef struct AccountNode {
  struct AccountNode *prev;
  struct AccountNode *next;
  Account *data;
} AccountNode;

Account *insertAccount(AccountNode **head, const char *name,
                       const char *password);

Account *findAccount(const AccountNode *head, int id);

void deleteAccount(AccountNode **head, int id);

void saveAccounts(const AccountNode *head, const char *fileName);

AccountNode *loadAccounts(const char *fileName);

#endif  // ACCOUNTS_H
