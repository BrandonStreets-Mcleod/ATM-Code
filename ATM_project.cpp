#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <C:\Users\brand\OneDrive\Documents\Random_stuff\Programs\ATMFile.cpp>
void deposit(int account);
void withdraw(int account);
void transfer(int account);
void statement(int account);
int main()
{
CreateAccount(2,0);
CreateAccount(1,0);
char choice[10], withdraw1[]="withdraw";
int check2, account;
float ret, money_out,transfer2;
printf("what Account:");
scanf("%d", &account);
transfer(account);
printf("\nWould you like to withdraw or deposit: ");
fflush(stdin);
scanf("%s",choice);
check2 = strcmp(choice,withdraw1);
if (check2 == 0)
{
           withdraw(account);
           }
else if (check2 != 0)
{
   deposit(account);
} 
statement(account);
getch();
return 0;
}
// deposit starts
void deposit(int account)
{
	int check;
	char option[10], cash[5]="cash";
	float money_in, total;
	printf("\nWould you like to deposit cash or cheque: ");
	scanf("%s",&option);
	check = strcmp(option,cash);
	if (check == 0)
	{
		printf("\nHow much would you like to deposit: ");
		fflush(stdin);
        scanf("%f",&money_in);
		total = money_in + GetBalance(account,0);
		if (SetBalance(account,total) == true)
 {
                         printf("\nNew balance is %.2f\n",total);
                         AppendTransferLog(account,1,total,0);
                         }
 else 
 {
      printf("\nMoney couldnt transfer please try again");
      }
	}
	else if (check != 0)
	{
		printf("\nHow much would you like to deposit: ");
		fflush(stdin);
		scanf("%f",&money_in);
		total = money_in + GetBalance(account,1);
		if (SetBalance(account,total,1) == true)
 {
                         printf("\nNew balance is %.2f",total);
                         AppendTransferLog(account,1,total,1);
                         }
 else 
 {
      printf("\nMoney couldnt transfer please try again");
      }
	}
}
// withdraw starts
void withdraw(int account)
{
      float money_out,withdraw_out;
      printf("\nHow much would you like to withdraw: ");
      fflush(stdin);
      scanf("%f",&withdraw_out);
      money_out = GetBalance(account,0) - withdraw_out;
      SetBalance(123,money_out);
      printf("\nNew balance is %.2f, you have withdrawn %.2f",money_out,withdraw_out);
      AppendTransferLog(account,-2,money_out,0);
}
// transfer starts
void transfer(int account)
{
      int account_to;
      float money_transfer,money_transfer2,money_to;
      printf("\nHow much would you like to transfer: ");
      fflush(stdin);
      scanf("%f",&money_to);
      printf("\nwhich account would you like to transfer to: ");
      fflush(stdin);
      scanf("%d",&account_to);
      money_transfer = GetBalance(account,0) - money_to;
      SetBalance(account,money_transfer);
      money_transfer2 = GetBalance(account_to,0) + money_to;
      SetBalance(account_to,money_transfer2);
      printf("\nNew balance in account %d is %.2f",account,money_transfer);
      printf("\nNew balance in 2nd account is %.2f",money_transfer2);
      AppendTransferLog(account,-3,money_transfer,0);
}
      
// Statement starts
void statement(int account)
{
     bool log;
     char choice, name[255];
     int no_logs,Array[200];
     float log_money;
     printf("\nWould you like a statement Y for Yes and N for No: ");
     scanf("&c",choice);
     if (choice == 'Y' or choice == 'y')
     {
                GetName(account,name);
                no_logs = GetNumberOfLogs(account);
                for(int x=0;x<no_logs;x++)
                {
                        GetTransferLogAccountData(account,x,Array);
                        log_money = GetTransferLogMoneyData(account,x);
                        printf("\n%d",Array[0]);
                        printf("\n%d",Array[1]);
                        printf("\n%d",Array[2]);
                        printf("%f",log_money);
                        }
     }
     else if (choice == 'N' or choice == 'n')
     {
          printf("Thank you for using the bank");
          }
}
           
      
      
      
      
