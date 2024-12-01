#include "accounts.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Account *insertAccount(AccountNode **head, const char *name,
                       const char *password) {
  AccountNode *newNode = (AccountNode *)malloc(sizeof(AccountNode));
  Account *newAccount = (Account *)malloc(sizeof(Account));
  if (newNode == NULL || newAccount == NULL) exit(EXIT_FAILURE);

  newAccount->name = (char *)malloc(strlen(name) * sizeof(char) + 1);
  newAccount->password = (char *)malloc(strlen(password) * sizeof(char) + 1);
  if (newAccount->name == NULL || newAccount->password == NULL)
    exit(EXIT_FAILURE);

  strcpy(newAccount->name, name);
  strcpy(newAccount->password, password);
  newAccount->id = (*head != NULL) ? (*head)->data->id + 1 : 1;
  newAccount->balance = 0.0;
  newNode->data = newAccount;
  newNode->prev = NULL;
  newNode->next = *head;

  if (*head != NULL) {
    (*head)->prev = newNode;
  }

  *head = newNode;
  return newAccount;
}

Account *findAccount(const AccountNode *head, int id) {
  while (head != NULL) {
    if (head->data->id == id) {
      return head->data;
    }

    head = head->next;
  }

  return NULL;
}

void deleteAccount(AccountNode **head, int id) {
  if (*head == NULL) return;

  AccountNode *target = *head;
  if ((*head)->data->id == id) {
    AccountNode *newHead = (*head)->next;
    free(target->data->name);
    free(target->data->password);
    free(target->data);
    free(target);
    if (newHead != NULL) newHead->prev = NULL;

    *head = newHead;
    return;
  }

  while (target != NULL) {
    if (target->data->id == id) {
      target->prev->next = target->next;
      if (target->next) target->next->prev = target->prev;

      free(target->data->name);
      free(target->data->password);
      free(target->data);
      free(target);
      return;
    }

    target = target->next;
  }
}

void saveAccounts(const AccountNode *head, const char *fileName) {
  FILE *fp = fopen(fileName, "wb");
  if (fp == NULL) exit(EXIT_FAILURE);

  while (head != NULL) {
    Account *data = head->data;
    size_t nameLength = strlen(data->name) + 1;
    size_t pwdLength = strlen(data->password) + 1;

    fwrite(&data->id, sizeof(int), 1, fp);
    fwrite(&nameLength, sizeof(size_t), 1, fp);
    fwrite(data->name, sizeof(char), nameLength, fp);
    fwrite(&pwdLength, sizeof(size_t), 1, fp);
    fwrite(data->password, sizeof(char), pwdLength, fp);
    fwrite(&data->balance, sizeof(double), 1, fp);
    head = head->next;
  }

  fclose(fp);
}

AccountNode *loadAccounts(const char *fileName) {
  FILE *fp = fopen(fileName, "rb");
  if (fp == NULL) return NULL;

  AccountNode *head = NULL;
  int id;
  size_t nameLength, pwdLength;
  char *name;
  char *password;
  double balance;

  while (1) {
    if (fread(&id, sizeof(int), 1, fp) <= 0) break;

    if (fread(&nameLength, sizeof(size_t), 1, fp) <= 0) break;
    name = (char *)malloc(sizeof(char) * nameLength);
    if (name == NULL) exit(EXIT_FAILURE);
    if (fread(name, sizeof(char), nameLength, fp) <= 0) break;

    if (fread(&pwdLength, sizeof(size_t), 1, fp) <= 0) break;
    password = (char *)malloc(sizeof(char) * pwdLength);
    if (password == NULL) exit(EXIT_FAILURE);
    if (fread(password, sizeof(char), pwdLength, fp) <= 0) break;

    if (fread(&balance, sizeof(double), 1, fp) <= 0) break;

    AccountNode *newNode = (AccountNode *)malloc(sizeof(AccountNode));
    Account *newAccount = (Account *)malloc(sizeof(Account));
    if (newNode == NULL || newAccount == NULL) exit(EXIT_FAILURE);

    newAccount->id = id;
    newAccount->name = name;
    newAccount->password = password;
    newAccount->balance = balance;
    newNode->data = newAccount;
    newNode->prev = NULL;
    newNode->next = head;

    if (head != NULL) {
      (head)->prev = newNode;
    }

    head = newNode;
  }

  fclose(fp);
  return head;
}
