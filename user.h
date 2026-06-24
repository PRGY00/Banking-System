#ifndef BANKING_SYSTEM_USER_H
#define BANKING_SYSTEM_USER_H

typedef struct {
    // int AccountNumber;
    char Name[20];
    char Surname[30];
    int Age;
    float AccountBalance;
}Account;

void createAccount();
void checkBalance();
void transferMoney(int account_number, int amount);
void drawMoney(int amount);

#endif //BANKING_SYSTEM_USER_H