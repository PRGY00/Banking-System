#ifndef BANKING_SYSTEM_USER_H
#define BANKING_SYSTEM_USER_H

typedef struct {
    // int AccountNumber;
    char Name[20];
    char Surname[30];
    int Age;
    char Password[10];
    float AccountBalance;
}Account;

void createAccount();
void login();
void userProfile(int account_number, float account_balance);
void checkBalance(int account_number);
void transferMoney(int account_number, float transfer_amount);
void drawMoney(int amount);

#endif //BANKING_SYSTEM_USER_H