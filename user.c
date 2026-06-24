#include <stdio.h>
#include "user.h"

# define BUFF_SIZE 52

int findLastAccountNumber(FILE *fp, char *buff) {

    char c;
    int account_number;
    long size;

    fopen("books.txt", "r");

    if (fp != NULL) {
        // set the pointer to the last position in the file
        fseek(fp, 0, SEEK_END);

        // check file size
        size = ftell(fp);

        // check if file is empty
        if (size == 0) {
            // printf("File is empty!");
            return 0;
        }

        // the file position advances by one
        c = fgetc(fp);

        // handles files that end with one or more newline characters (skips trailing newlines)
        while (c == '\n') {
            // one byte back to undo the read, and another byte back to reach the previous character
            fseek(fp, -2, SEEK_CUR);
            c = fgetc(fp);
        }
        // continues moving backward until it finds a newline
        while (c != '\n') {
            fseek(fp, -2, SEEK_CUR);
            c = fgetc(fp);
        }

        // get the account number
        fgets(buff, sizeof(buff), fp);
        sscanf(buff, "%d;", &account_number);

        fclose(fp);

        return account_number;
    }
    else {
        perror("fopen");
        return 0;
    }
}

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
    char buff[BUFF_SIZE];

    // setting first user's id
    int account_number;

    // checking last account number
    if(fp_read!=NULL){ // file was successfully opened
        account_number = findLastAccountNumber(fp_read, buff) + 1;
        if (account_number == 1) {
            account_number = 100000;
        }
    }
    else {
        account_number = 100000;
    }

    FILE* fp_write = fopen("bank_accounts.txt", "a");

    if (fp_write != NULL) {
        fprintf(fp_write, "%d;%s;%s;%d;%2.f\n", account_number, a.Name, a.Surname, a.Age, 0.00);
        printf("Account added successfully!\n");
        printf("Assigned account number: %d\n", account_number);
        fclose(fp_write);
    }
    else {
        perror("fp_write");
    }
};