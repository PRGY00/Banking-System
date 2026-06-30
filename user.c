#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "user.h"

# define BUFF_SIZE 65

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
    printf("Enter Password: ");
    scanf("%s", a.Password);

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
        fprintf(fp_write, "%d;%s;%s;%d;%s;%.2f\n", account_number, a.Name, a.Surname, a.Age, a.Password, 0.00);
        printf("Account added successfully!\n");
        printf("Assigned account number: %d\n", account_number);
        fclose(fp_write);
    }
    else {
        perror("fp_write");
    }
}

void checkBalance(int account_number) {

    FILE *fp = fopen("bank_accounts.txt", "r");

    // setting up a buffer for fgets() and variable for current account number
    char buff[BUFF_SIZE];
    int current_acc_num;
    float balance;

    if (fp != NULL) {
        while (fgets(buff, sizeof(buff), fp) != NULL) {
            // read the whole line, skip middle values (Name, Surname, Age)
            if (sscanf(buff, "%d;%*[^;];%*[^;];%*d;%*[^;];%f", &current_acc_num, &balance) == 2) {
                if (current_acc_num == account_number) {
                    printf("Account number: %d \n", account_number);
                    printf("Account balance: %.2f $\n", balance);
                    fclose(fp);
                    return;
                }
            }
        }
        fclose(fp);
    }
    else {
        perror("fopen");
    }
}

void transferMoney(int account_number, float transfer_amount) {

    FILE *fp_read = fopen("bank_accounts.txt", "r");

    // setting up a buffer for fgets(), transfer_acc_num and transfer_acc_bal
    char buff[BUFF_SIZE];
    int transfer_acc_num;
    float transfer_acc_bal;

    // creating pointer for file copy
    FILE *fp_write = fopen("bank_accounts_copy.txt", "w");

    if (fp_read != NULL) {
        while (fgets(buff, sizeof(buff), fp_read)) {
            // copy buff content before last ";"
            // strcpy(prefix, buff);
            // char *last = strrchr(buff, ';');
            // *last = '\0';
            // get account number and it's balance
            if (sscanf(buff, "%d;%*[^;];%*[^;];%*d;%*[^;];%f", &transfer_acc_num, &transfer_acc_bal) == 2) {
                if (transfer_acc_num == account_number) {
                    // move the pointer before last ";"
                    char *last = strrchr(buff, ';');
                    *last = '\0';
                    // update balance amount
                    transfer_acc_bal += transfer_amount;
                    // write the amount to the file
                    fprintf(fp_write, "%s;%.2f\n", buff, transfer_acc_bal);
                }
                else {
                    // coping original file content
                    fputs(buff, fp_write);
                }
            }
        }
    }

    fclose(fp_read);
    fclose(fp_write);

    char to_be_deleted[] = "bank_accounts.txt";

    // removing the original file
    int remove_flag = remove(to_be_deleted);

    if (remove_flag != 0)
        perror("Deletion Error");

    char old_name[] = "bank_accounts_copy.txt";
    char new_name[] = "bank_accounts.txt";

    // renaming the copy file
    int rename_flag = rename(old_name, new_name);

    if (rename_flag != 0)
         perror("Rename Error");
}

void drawMoney(int account_number) {

    int draw_amount;

    printf("Enter amount to draw (integer 10 - 500$): ");
    scanf("%d", &draw_amount);

    // setting up a buffer for fgets() and acc_balance
    char buff[BUFF_SIZE];
    float acc_balance;

    FILE* fp_read = fopen("bank_accounts.txt", "r");

    if (fp_read != NULL) {
        while (fgets(buff, sizeof(buff), fp_read)) {
            if(sscanf(buff, "%*d;%*[^;];%*[^;];%*d;%*[^;];%f", &acc_balance) == 1) {
                if (draw_amount > acc_balance) {
                    printf("Insufficient funds in the account!\n");
                    return;
                }
                else{
                    printf("Wait for the money...\n");
                    sleep(2);
                    printf("Take money from the tray.\n");

                    // calling transferMoney() to decrease the balance of the account
                    transferMoney(account_number, -1 * draw_amount);

                    return;
                }
            }
        }
    }
    else {
        perror("fopen");
    }

    fclose(fp_read);

}

void login() {

    int account_number;
    char password[10];
    float acc_balance;

    printf("Enter account number: ");
    scanf("%d", &account_number);
    printf("Enter account password: ");
    scanf("%s", password);

    FILE *fp = fopen("bank_accounts.txt", "r");

    // setting up a buffer for fgets(), acc_num and password_check to check with the user input
    char buff[BUFF_SIZE];
    int acc_num;
    char password_check[10];

    if (fp != NULL) {
        while (fgets(buff, sizeof(buff), fp)) {
            // %*[^;] - reads and discards content between ";" | %9[^;] - reads up to 9 characters (plus terminating \0) until the next ;
            if (sscanf(buff, "%d;%*[^;];%*[^;];%*d;%9[^;];%f", &acc_num, password_check, &acc_balance) == 3) {
                if (acc_num == account_number) {
                    // compare password from the file and password provided by the user
                    if (strcmp(password_check, password) == 0) {

                        // clear terminal using ANSI escape sequences
                        system("clear"); // printf("\033[2J\033[H");
                        printf("Login successful!\n");
                        sleep(2);

                        // clear terminal using ANSI escape sequences
                        system("clear"); // printf("\033[2J\033[H");
                        // call user interface after successful login
                        userProfile(account_number, acc_balance);

                        return;
                    }
                }
            }
        }
    }
    else {
        perror("fopen");
    }
}

void userProfile(int account_number, float account_balance) {

    int action;

    do {
        printf("\n");
        printf("1.Check Balance\n");
        printf("2.Transfer money\n");
        printf("3.Draw money\n");
        printf("4.Logout\n");
        printf("Choose action: ");

        scanf("%d", &action);

        switch (action) {
            case 1:
                system("clear");
                checkBalance(account_number);
                break;

            case 2:
                int transfer_account_number;
                float transfer_amount;

                printf("\nProvide the account number for transfer: ");
                scanf("%d", &transfer_account_number);
                printf("Provide the amount to transfer: ");
                scanf("%f", &transfer_amount);

                if (account_balance > transfer_amount) {
                    // transfer money to the target account
                    transferMoney(transfer_account_number, transfer_amount);
                    // decrease users money balance
                    transferMoney(account_number, -1 * transfer_amount);
                    // infor the user about the transfer
                    printf("Transfer to the account %d was successful.\n", transfer_account_number);
                }
                else {
                    printf("Not enough money for transfer!\n");
                }

                break;

            case 3:
                drawMoney(account_number);
                break;

            case 4:
                printf("\n");
                printf("Logging out... \n");
                break;

            default:
                printf("Invalid input!\n");
                break;
        }
    } while (action != 4);

}