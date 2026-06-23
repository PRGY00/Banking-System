#include <stdio.h>

#include "user.h"


void createAccount() {

    Account a;

    printf("Enter Name: ");
    scanf("%s", a.Name);
    printf("Enter Name: ");
    scanf("%s", a.Surname);
    printf("Enter Age: ");
    scanf("%d", &a.Age);

    FILE* fp_read = fopen("bank_accounts.txt", "r");

    // setting up a buffer for fgets()
    char buff[52];

    // setting first user's id
    int account_number;

    // checking latest account number
    if(fp_read!=NULL){ // file was successfully opened
        while (fgets(buff, sizeof(buff), fp_read) != NULL) {
            sscanf(buff, "%d;", &account_number);
        }
        account_number += 1;
        fclose(fp_read);
    }
    else {
        account_number = 100000;
    }

    FILE* fp_write = fopen("bank_accounts.txt", "a");

    if (fp_write != NULL) {   // file was successfully opened
        fprintf(fp_write, "%d;%s;%s;%d;\n", account_number, a.Name, a.Surname, a.Age);
        printf("Account added successfully\n");
        printf("Assigned account number: %d\n", account_number);
        fclose(fp_write);
    }
    else {
        perror("fp_write");
    }

};