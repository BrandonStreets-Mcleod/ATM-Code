/*

This is An ATM File read/write libray!

Author: Lee Armstrong

*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dirent.h>
#include <windows.h>
#include <unistd.h>

//public File functions
int GetPassword(int Account);
bool SetPassword(int Account, int NewPassword);
float GetBalance(int Account, int SubAccount = 0);
bool SetBalance(int Account, float NewBalance, int SubAccount = 0);
bool CreateAccount(int Account, bool Admin = false);
bool DeleteAccount(int Account); 
bool DoesAccountExist(int Account); 
bool IsAccountAdmin(int Account);
bool SetName(int Account, char Name[255]);
char * GetName(int Account, char Name[255]);
int GetNumberOfLogs(int Account);
int * GetAccounts(int * Accounts);
int GetNextAvaliableAccount();
bool AppendTransferLog(int Account,int FromAccount,float Amount,int SubAccount=0);
int * GetTransferLogAccountData(int Account,int LogID,int * ToArray);
float GetTransferLogMoneyData(int Account,int LogID);
bool DoesTransferLogIDExist(int Account,int LogID);
bool DoesNoteExist(int Account,char NoteID[255]);
char * GetNote(int Account,char NoteID[255],char String[255]);
bool DeleteNote(int Account,char NoteID[255]);
bool CreateNote(int Account,char NoteID[255],char Note[255]);
bool UpdateNote(int Account,char NoteID[255],char Note[255]);

//private functions
bool ReadWholeFile(char Dir[]);
bool ReadAccountFile(int Account);
bool WriteWholeAccountFileBack(int Account);
bool CreateLogFile(int Account);
bool ReadLogFile(int Account);
int ConvertStringToInt(char Text[255]);
bool WriteWholeAccountLogFileBack(int Account);
char * ConvertIntToString(int number, char string[255]);
char * ConvertFloatToString(float number, char string[255]);

bool WriteWholeNotesFileBack(int Account);
bool ReadNoteFile(int Account);
char * SplitBetweenFirst(char Text[255],char character,bool Returnbefore);


//global variables
char lines[255][255];

/*
int main() {


	//printf("%d",GetPassword(3));
	
	//char a[255][255];
	
	//char location[] = "./test.txt";
	//ReadWholeFile(location);
	
	//printf("file read: %s",lines[1]);
	
	//printf("\nDoesAccountExist: %d",DoesAccountExist(1));
	
	printf("\nCreateAccount: %d",CreateAccount(1,false));
	
	//printf("\nDeleteAccount: %d",DeleteAccount(1));
	
	//printf("\nIsAccountAdmin: %d",IsAccountAdmin(1));
	
	//printf("\nSetPassword: %d",SetPassword(1,7832));
	
	printf("\nGetPassword: %d",GetPassword(1));
	
	//printf("\nSetBalance: %d",SetBalance(1,765.27));
	//printf("\nSetBalance: %d",SetBalance(1,54.38,1));
	
	//printf("\nGetBalance: %.2f",GetBalance(1));
	//printf("\nGetBalance: %.2f",GetBalance(1,1));
	
	//printf("\nSetName: %d",SetName(1,"FISH PIE"));
	
	//char Name[255];
	//printf("\nGetName: %s",GetName(1,Name));
	//printf("\nWriteWholeAccountFileBack: %d",WriteWholeAccountFileBack(1));
	
	//printf("\nGetNumberOfLogs: %d",GetNumberOfLogs(1));
	
	//printf("\nGetNextAvaliableAccount: %d",GetNextAvaliableAccount());
	
//	int Accounts[255];
//	printf("\nGetAccounts: %d",GetAccounts(Accounts)[0]);
	//printf("\n\n");
	
	printf("\nAppendTransferLog: %d",AppendTransferLog(1,0,473.34));
	
	printf("\nGetTransferLogMoneyData: %.2f",GetTransferLogMoneyData(1,2));
	
	int Logdata[4];
	GetTransferLogAccountData(1,2,Logdata);
	printf("\nGetTransferLogAccountData: %d,%d,%d",Logdata[0],Logdata[1],Logdata[2]);
	
	printf("\nCreateNote: %d",CreateNote(1,"Something","else"));
	
	//char text[255]="Fish=Pie";
	//printf("\nSplitBetweenFirst: %s",SplitBetweenFirst(text,'=',true));
	
	printf("\nDoesNoteExist: %d",DoesNoteExist(1,"wow"));
	
	//printf("\nDeleteNote: %d",DeleteNote(1,"Something"));
	
	char text[255];
	printf("\nGetNote: %s",GetNote(1,"wow",text));
	
	getch();
	

	return 0;
}*/

char * GetNote(int Account,char NoteID[255],char String[255]){
     
     if(DoesNoteExist(Account,NoteID)){
                                       
        ReadNoteFile(Account);
                                       
        int ArraySize = sizeof(lines)/sizeof(lines[0]);
		int numberOfItems = ArraySize;
		char LookingNoteID[255];
		
		for(int i=0;i<=numberOfItems;i++){
            
            strcpy(LookingNoteID,lines[i]);
			SplitBetweenFirst(LookingNoteID,'=',true);
		    
			if(strcmp(LookingNoteID,NoteID)==0){
                                                
                  strcpy(LookingNoteID,lines[i]);
                  strcpy(String,SplitBetweenFirst(LookingNoteID,'=',false));
                                      
                  return String;
                                            
            }
                
        }
                                         
     }else{

           return NULL;      
           
     }
     
}

bool UpdateNote(int Account,char NoteID[255],char Note[255]){
     
     DeleteNote(Account,NoteID);
     return CreateNote(Account,NoteID,Note);     
     
}

bool DeleteNote(int Account,char NoteID[255]){
     
     if(DoesNoteExist(Account,NoteID)){
                       
        ReadNoteFile(Account);
		
		bool found=false;
		
		int ArraySize = sizeof(lines)/sizeof(lines[0]);
		int numberOfItems = ArraySize;
		char LookingNoteID[255];
		
		for(int i=0;i<=numberOfItems;i++){
			
			strcpy(LookingNoteID,lines[i]);
			
			SplitBetweenFirst(LookingNoteID,'=',true);
			
			//printf("\n%s",lines[i]);
			
			if(strcmp(LookingNoteID,NoteID)==0){
                                      
                  found=true;
                                            
            }
            if((found)and(i < numberOfItems)){
            
                  strcpy(lines[i],lines[i+1]);          
                      
            }
			
		}
           
           return WriteWholeNotesFileBack(Account);         
                       
     }else{
     
           return false;      
           
     }
     
}

char * SplitBetweenFirst(char Text[255],char character,bool Returnbefore){
	
	char ministring[]="a";
	char before[255];
	char after[255];
	
	strcpy(before,"");
	strcpy(after,"");
	
	bool lookingBefore=true;
	
	for(int i=0;i<strlen(Text);i++){
		
		//printf("\nText: %s, Before: %s, after: %s",Text,before,after);
		
		if(Text[i]!=character and not lookingBefore){
		
			ministring[0]=Text[i];
			if(lookingBefore){
				strcat(before,ministring);
			}else{
				strcat(after,ministring);
			}
			
		}else{
			
			lookingBefore=false;	
			
		}
		
	}
	
	if(Returnbefore){
		
		strcpy(Text,before);
		
	}else{
		
		strcpy(Text,after);
		
	}
	
	return Text;
	
}

bool DoesNoteExist(int Account,char NoteID[255]){
	
	if(DoesAccountExist(Account)){
	
		ReadNoteFile(Account);
		
		int ArraySize = sizeof(lines)/sizeof(lines[0]);
		int numberOfItems = ArraySize;
		char LookingNoteID[255];
		
		for(int i=0;i<=numberOfItems;i++){
			
			strcpy(LookingNoteID,lines[i]);
			
			SplitBetweenFirst(LookingNoteID,'=',true);
			
			//printf("\n%s",LookingNoteID);
			
			if(strcmp(LookingNoteID,NoteID)==0){
                                      
                  return true;
                                            
            }
			
		}
	
	}
	
	return false;
	
}

bool CreateNote(int Account,char NoteID[255],char Note[255]){
	
	if(DoesAccountExist(Account) and not DoesNoteExist(Account,NoteID)){
		
		ReadNoteFile(Account);
		
		int ArraySize = sizeof(lines)/sizeof(lines[0]);
		int numberOfItems = ArraySize+1;
			
		for(int i=ArraySize;i>=0;i--){
				
			//printf("\nTEST: %d : %s",i,lines[i]);
				
			if(strcmp(lines[i],"")==0){
					
					
				numberOfItems--;
					
			}else{
					
				break;
					
			}
				
		}
		
		char toWrite[255];
		
		strcpy(toWrite,NoteID);
		strcat(toWrite,"=");
		strcat(toWrite,Note);
		
		strcpy(lines[numberOfItems],toWrite);
		
		return WriteWholeNotesFileBack(Account);
		
	}else{
		
		return false;
		
	}
	
}

bool ReadNoteFile(int Account){
	
	memset(lines, 0, sizeof(lines));
	
	char location[255];
	strcpy (location,"./Accounts/");
	char Saccount[255];
	ConvertIntToString(Account,Saccount);
	strcat (location,Saccount);
	strcat (location,"_Notes.txt");
	
	return ReadWholeFile(location);
	
}

int * GetTransferLogAccountData(int Account,int LogID,int * ToArray){
	
	if(DoesTransferLogIDExist(Account,LogID)){
		
		ReadLogFile(Account);		
		
		int count=0;
		
		char fromAccountS[255];
		char ToAccountS[255];
		char SubAccountS[255];
		
		char ministring[]="a";
		
		strcpy(fromAccountS,"");
		strcpy(ToAccountS,"");
		strcpy(SubAccountS,"");
		
		for(int i =0;i<strlen(lines[(LogID+1)]);i++){
			
			if(lines[(LogID+1)][i]=='|'){
				
				count++;
				
			}
			
			if(count==0){
				
				if(lines[(LogID+1)][i]!='|'){
				ministring[0]=lines[(LogID+1)][i];
				strcat(fromAccountS,ministring);
				}
				
			}else if(count==1){
				
				if(lines[(LogID+1)][i]!='|'){
				ministring[0]=lines[(LogID+1)][i];
				strcat(ToAccountS,ministring);
				}
				
			}else if(count==3){
				
				if(lines[(LogID+1)][i]!='|'){
				ministring[0]=lines[(LogID+1)][i];
				strcat(SubAccountS,ministring);
				}
				
			}
			
		}
		
		ToArray[0]=ConvertStringToInt(fromAccountS);
		ToArray[1]=ConvertStringToInt(ToAccountS);
		ToArray[2]=ConvertStringToInt(SubAccountS);
		
		return ToArray;
	
	}else{
		
		return NULL;
		
	}
}

float GetTransferLogMoneyData(int Account,int LogID){
	
	if(DoesTransferLogIDExist(Account,LogID)){
		
		ReadLogFile(Account);
		
		int count=0;
		
		char output[255];
		char ministring[]="a";
		strcpy(output,"");
		
		for(int i =0;i<strlen(lines[(LogID+1)]);i++){
			
			if(lines[(LogID+1)][i]=='|'){
				
				count++;
				
			}
			
			if(count==2){
				
				if(lines[(LogID+1)][i]!='|'){
				ministring[0]=lines[(LogID+1)][i];
				strcat(output,ministring);
				}
				
			}
			
		}
		
		//printf("\nfish: %s",output);
		
		return atof(output);	
		
	}else{
		
		return -1;
		
	}
	
}

bool DoesTransferLogIDExist(int Account,int LogID){

	if(DoesAccountExist(Account)){
	
			if((LogID>=0) and (LogID <= (GetNumberOfLogs(Account)-1))){
				
				return true;
				
			}else{
				
				return false;
				
			}
		
	}else{
		
		return false;
		
	}
	
}

bool AppendTransferLog(int Account,int FromAccount,float Amount,int SubAccount){
     
    if(DoesAccountExist(Account)){
	    ReadLogFile(Account); 
	    
	    int ArraySize = sizeof(lines)/sizeof(lines[0]);
		int numberOfItems = ArraySize+1;
			
		for(int i=ArraySize;i>=0;i--){
				
			//printf("\nTEST: %d : %s",i,lines[i]);
				
			if(strcmp(lines[i],"")==0){
					
					
				numberOfItems--;
					
			}else{
					
				break;
					
			}
				
		}
		
		char SfromAccount[255];
		ConvertIntToString(FromAccount,SfromAccount);
		char SAccount[255];
		ConvertIntToString(Account,SAccount);
		char SAmount[255];
		ConvertFloatToString(Amount,SAmount);
		//itoa(Amount,SAmount,255);
		char SsubAccount[255];
		ConvertIntToString(SubAccount,SsubAccount);
		char toWrite[255];
		
		strcpy(toWrite,SfromAccount);
		strcat(toWrite,"|");
		strcat(toWrite,SAccount);
		strcat(toWrite,"|");
		strcat(toWrite,SAmount);
		strcat(toWrite,"|");
		strcat(toWrite,SsubAccount);
		
		strcpy(lines[numberOfItems],toWrite);
		
		//printf("\nNumberOfLines: %d",numberOfItems);
	    
	    return WriteWholeAccountLogFileBack(Account);
	}else{
		
		return false;
		
	}
}

char IntToChar(int number){
    
    switch(number){
                             
             case 0:
                  return '0';
             case 1:  
                  return '1';
             case 2:  
                  return '2';    
             case 3:  
                  return '3';    
             case 4:  
                  return '4';     
             case 5:  
                  return '5';   
             case 6:  
                  return '6';      
             case 7:  
                  return '7';   
             case 8:  
                  return '8';  
             case 9:  
                  return '9';              
                         
         }
    
         return 'N';
    
    }

char * ConvertFloatToString(float number, char string[255]){
    
    //printf("\n%f",number);
    
    strcpy(string,"");
    
    
    
    int test=number;
    int test2=number/10;
    
    char miniString[]="a";
	    
    do{
        //printf("\nTest 2: %f, Test 1: %f",test2,test);
		//printf("\nTest: %d",test-(test2*10));
		
		int number2 = test-(test2*10);
		
		miniString[0]=IntToChar(number2);
		    
        strcat(string,miniString);
            
        test=test/10;
        test2=test/10; 
		                          
    }while(test>0);
	
	
    char backupstring[255];
    strcpy(backupstring,string);
    
    for(int i=0;i<strlen(string);i++){
    	
    	//printf("\n%c",backupstring[i]);
    	string[i]=backupstring[strlen(string)-1-i];
    	
	}
	
	strcat(string,".");
	
	number=(number-int(number))*1000;
	//printf("\n%d",number);
	char string2[255];
	
	strcpy(string2,"");
	
	test=number;
    test2=number/10;
	    
    do{
        //printf("\nTest 2: %d, Test 1: %d",test2,test);
		//printf("\nTest: %d",test-(test2*10));
		
		int number2 = test-(test2*10);
		
		miniString[0]=IntToChar(number2);
		    
        strcat(string2,miniString);
            
        test=test/10;
        test2=test/10; 
		                          
    }while(test>0);    
    //printf("\n%.2f",number);
    
    
    
    strcpy(backupstring,string2);
    
    for(int i=0;i<strlen(string2);i++){
    	
    	//printf("\n%c",backupstring[i]);
    	string2[i]=backupstring[strlen(string2)-1-i];
    	
	}
    
    strcat(string,string2);
    
    //printf("\n%s",string);
    
    return string;
}

char * ConvertIntToString(int number, char string[255]){
    
    //printf("\n%d",number);
    
    strcpy(string,"");
    
    int test=number;
    int test2=number/10;
    
    char miniString[]="a";
	    
    do{
        //printf("\nTest 2: %d, Test 1: %d",test2,test);
		//printf("\nTest: %d",test-(test2*10));
		
		int number = test-(test2*10);
		
		miniString[0]=IntToChar(number);
		    
        strcat(string,miniString);
            
        test=test/10;
        test2=test/10; 
		                          
    }while(test>0);
    
    //printf("\n%d",number);
    
    char backupstring[255];
    strcpy(backupstring,string);
    
    for(int i=0;i<strlen(string);i++){
    	
    	//printf("\n%c",backupstring[i]);
    	string[i]=backupstring[strlen(string)-1-i];
    	
	}
    
    //printf("\n%s",string);
    
    return string;
}

bool WriteWholeAccountLogFileBack(int Account) {
	
	if(DoesAccountExist(Account)){
		
		//ReadAccountFile(Account);	
		int ArraySize = sizeof(lines)/sizeof(lines[0]);
		int numberOfItems = ArraySize+1;
		
		for(int i=ArraySize;i>=0;i--){
			
			//printf("\nTEST: %d : %s",i,lines[i]);
			
			if(strcmp(lines[i],"")==0){
				
				
				numberOfItems--;
				
			}else{
				
				break;
				
			}
			
		}
		
		//printf("\nTEST: %d",numberOfItems);
		char location[255];
		strcpy (location,"./Accounts/");
		char Saccount[255];
		ConvertIntToString(Account,Saccount);
		strcat (location,Saccount);
		strcat (location,"_Log.txt");
		
		FILE *filePointer;
		filePointer = fopen(location ,"w");
		
		char toWrite[65025];
		
		strcpy(toWrite,"");
		
		for(int i=0;i<=numberOfItems;i++){
			
		strcat (toWrite,lines[i]);
		strcat (toWrite,"\n");
		
		}


		fprintf(filePointer, toWrite);
		
		fclose(filePointer);
		
		return true;
		
	}else{
		
		return false;
		
	}
	
}

bool WriteWholeNotesFileBack(int Account) {
	
	if(DoesAccountExist(Account)){
		
		//ReadAccountFile(Account);	
		int ArraySize = sizeof(lines)/sizeof(lines[0]);
		int numberOfItems = ArraySize+1;
		
		for(int i=ArraySize;i>=0;i--){
			
			//printf("\nTEST: %d : %s",i,lines[i]);
			
			if(strcmp(lines[i],"")==0){
				
				
				numberOfItems--;
				
			}else{
				
				break;
				
			}
			
		}
		
		//printf("\nTEST: %d",numberOfItems);
		char location[255];
		strcpy (location,"./Accounts/");
		char Saccount[255];
		ConvertIntToString(Account,Saccount);
		strcat (location,Saccount);
		strcat (location,"_Notes.txt");
		
		FILE *filePointer;
		filePointer = fopen(location ,"w");
		
		char toWrite[65025];
		
		strcpy(toWrite,"");
		
		for(int i=0;i<=numberOfItems;i++){
			
		strcat (toWrite,lines[i]);
		strcat (toWrite,"\n");
		
		}


		fprintf(filePointer, toWrite);
		
		fclose(filePointer);
		
		return true;
		
	}else{
		
		return false;
		
	}
	
}

int GetNextAvaliableAccount(){
    
    int i=0;
    while(true){
    if(!DoesAccountExist(i)){
           return i;                        
    }
    i++; 
    }
    
}

/*
int * GetAccounts(int * Accounts){
	
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir ("./Accounts")) != NULL) {
		int i=-2;
	  	while ((ent = readdir (dir)) != NULL) {
	    	//printf ("%s|%d\n", ent->d_name,i);
	    	if(i>=0){
	    		
	    		Accounts[i]=atof(ent->d_name);
			}
	    	i++;
	  	}
	  	closedir (dir);
	} else {

	}
	
	return Accounts;
	
}*/

int GetNumberOfLogs(int Account){
	
	if(DoesAccountExist(Account)){
		
		ReadLogFile(Account);
		
		int ArraySize = sizeof(lines)/sizeof(lines[0]);
		int numberOfItems = 0;
		
		for(int i=0;i<=ArraySize;i++){
			
			if(strcmp(lines[i],"")!=0){
				
				//printf("\nTEST: %s",lines[i]);
				numberOfItems++;
				
			}
			
		}
		
		//int numberOfAccounts=numberOfItems-1;
		//printf("\n%d",numberOfAccounts);
		
		return numberOfItems-1;
		
	}
	
	return -1;
	
}

bool ReadLogFile(int Account){ //TODO: EDIT SOMEHOW
	
	memset(lines, 0, sizeof(lines));
	
	char location[255];
	strcpy (location,"./Accounts/");
	char Saccount[255];
	ConvertIntToString(Account,Saccount);
	strcat (location,Saccount);
	strcat (location,"_Log.txt");
	
	return ReadWholeFile(location);
	
}

bool CreateLogFile(int Account){//TODO EDIT
	
	if(DoesAccountExist(Account)){
		
		char location[255];
		strcpy (location,"./Accounts/");
		char Saccount[255];
		ConvertIntToString(Account,Saccount);
		strcat (location,Saccount);
		strcat (location,"_Log.txt");
		
		FILE *filePointer;
		filePointer = fopen(location ,"w");
		
		char toWrite[255];
		strcpy (toWrite, "Log For Account Number: ");
		strcat (toWrite, Saccount);
		strcat (toWrite,"\n");
		fprintf(filePointer, toWrite);
		
		fclose(filePointer);
		
		return true;
	}else{
		return false;
	}

}

char * GetName(int Account, char Name[255]){
	
	if(DoesAccountExist(Account)){
		
		ReadAccountFile(Account);
		
		strcpy(Name,lines[0]);
		
		return Name;
		
	}else{
		return NULL;
	}
	
}

bool SetName(int Account, char Name[255]){
	
	if(DoesAccountExist(Account)){
	
		ReadAccountFile(Account);
		
		strcpy(lines[0],Name);
		
		return WriteWholeAccountFileBack(Account);
		
	}else{
		return false;
	}
	
}

bool SetPassword(int Account, int NewPassword){
	
	if(DoesAccountExist(Account)){
	
		ReadAccountFile(Account);
		char SPassword[255]="";
		snprintf(SPassword, 255, "%d", NewPassword);
		
		strcpy(lines[1],SPassword);
		
		return WriteWholeAccountFileBack(Account);
		
	}else{
		return false;
	}
	
}

bool SetBalance(int Account, float NewBalance, int SubAccount){
	
	if(DoesAccountExist(Account)){
		
		ReadAccountFile(Account);	
		int ArraySize = sizeof(lines)/sizeof(lines[0]);
		int numberOfItems = 0;
		
		for(int i=0;i<=ArraySize;i++){
			
			if(strcmp(lines[i],"")!=0){
				
				//printf("\nTEST: %s",lines[i]);
				numberOfItems++;
				
			}
			
		}
		
		int numberOfAccounts=numberOfItems-3;
		//printf("\n%d",numberOfAccounts);
		
		//if(SubAccount+1<=numberOfAccounts){
			int accountIndex = SubAccount+3;
			
			char SNewBalance[255]="";
			
			//itoa(NewBalance,SNewBalance,255);
			snprintf(SNewBalance, 255, "%.2f", NewBalance);
			
			strcpy(lines[accountIndex],SNewBalance);
			
			//printf("\n%s",lines[accountIndex]);
			
			return WriteWholeAccountFileBack(Account);
		
		//}else{
		//	return false;
		//}		
			
	}else{
		return false;
	}
	
}

bool WriteWholeAccountFileBack(int Account) {
	
	if(DoesAccountExist(Account)){
		
		//ReadAccountFile(Account);	
		int ArraySize = sizeof(lines)/sizeof(lines[0]);
		int numberOfItems = ArraySize+1;
		
		for(int i=ArraySize;i>=0;i--){
			
			//printf("\nTEST: %d : %s",i,lines[i]);
			
			if(strcmp(lines[i],"")==0){
				
				
				numberOfItems--;
				
			}else{
				
				break;
				
			}
			
		}
		
		//printf("\nTEST: %d",numberOfItems);
		char location[255];
		strcpy (location,"./Accounts/");
		char Saccount[255];
		ConvertIntToString(Account,Saccount);
		strcat (location,Saccount);
		strcat (location,"_Data.txt");
		
		FILE *filePointer;
		filePointer = fopen(location ,"w");
		
		char toWrite[65025];
		
		strcpy(toWrite,"");
		
		for(int i=0;i<=numberOfItems;i++){
			
		strcat (toWrite,lines[i]);
		strcat (toWrite,"\n");
		
		}


		fprintf(filePointer, toWrite);
		
		fclose(filePointer);
		
		return true;
		
	}else{
		
		return false;
		
	}
	
}

int GetPassword(int Account){
	
	if(DoesAccountExist(Account)){
		
		ReadAccountFile(Account);
		
 		return ConvertStringToInt(lines[1]);
		
	}else{
		return -1;
	}
	
}

int CharToInt(char character){
    
    switch(character){
                             
             case '0':
                  return 0;
             case '1':  
                  return 1;
             case '2':  
                  return 2;    
             case '3':  
                  return 3;    
             case '4':  
                  return 4;     
             case '5':  
                  return 5;   
             case '6':  
                  return 6;      
             case '7':  
                  return 7;   
             case '8':  
                  return 8;  
             case '9':  
                  return 9;              
                         
         }
    
         return -1;
    
    }

int ConvertStringToInt(char Text[255]){
    
    int number=0;
    
    for(int i=0;i<strlen(Text);i++){
    
         //printf("\n%d",((strlen(Text)-i)-1));
         int multiplyer = 1;
         for(int x=0;x<((strlen(Text)-i)-1);x++){
         
               multiplyer=multiplyer*10;    
                 
         }
         //printf("\n%d",CharToInt(Text[i])*multiplyer);                               
         number+=CharToInt(Text[i])*multiplyer;
         
         
                                   
    }
    
    //printf("\n%d",number);
    return number;
}

float GetBalance(int Account, int SubAccount){
	
	if(DoesAccountExist(Account)){
		
		ReadAccountFile(Account);	
		int ArraySize = sizeof(lines)/sizeof(lines[0]);
		int numberOfItems = 0;
		
		for(int i=0;i<=ArraySize;i++){
			
			if(strcmp(lines[i],"")!=0){
				
				//printf("\nTEST: %s",lines[i]);
				numberOfItems++;
				
			}
			
		}
		
		int numberOfAccounts=numberOfItems-3;
		//printf("\n%d",numberOfAccounts);
		
		if(SubAccount+1<=numberOfAccounts){
		int accountIndex = SubAccount+3;
		
	
		return atof(lines[accountIndex]);
		
		}
		
	}else{
		return -9999999;
	}
	
}

bool IsAccountAdmin(int Account){
	
	if(DoesAccountExist(Account)){
		
		ReadAccountFile(Account);
		
		//printf("\n%d",strcmp(lines[2],"true"));
		
		if(strcmp(lines[2],"true")==0){
			
			return true;
			
		}else{
			
			return false;
			
		}
		
	}else{
		
		return false;
		
	}
	
}

bool DeleteAccount(int Account){
	
	if(DoesAccountExist(Account)){
		char location[255];
		strcpy (location,"rm -r ./Accounts/");
		char Saccount[255];
		ConvertIntToString(Account,Saccount);
		strcat (location,Saccount);		
		strcat (location,"_Data.txt");
		
		system(location);
		
		strcpy (location,"rm -r ./Accounts/");
		strcpy(Saccount,"");
		ConvertIntToString(Account,Saccount);
		strcat (location,Saccount);
		strcat (location,"_Log.txt");
		system(location);
		
		return true;//rmdir(location);//RemoveDirectory(location);
	}else{
		return false;
	}
}

bool CreateAccount(int Account, bool Admin){
	
	if(!DoesAccountExist(Account)){
		
		//create Directory
		char location[255];
		strcpy (location,"./Accounts/");
		char Saccount[255];
		ConvertIntToString(Account,Saccount);
		strcat (location,Saccount);
	
		//CreateDirectory(location,NULL);
		
		//Generate Data.txt
		strcat (location,"_Data.txt");
		
		FILE *filePointer;
		filePointer = fopen(location ,"w");
		
		char toWrite[255];
		strcpy (toWrite,"Name\n");
		strcat (toWrite,"1234\n");
		if(Admin){
			strcat (toWrite,"true\n");
		}else{
			strcat (toWrite,"false\n");	
		}
		strcat (toWrite,"0.00\n");

		fprintf(filePointer, toWrite);
		
		fclose(filePointer);
		
		return CreateLogFile(Account);
	}else{
		return false;
	}

}

bool DoesAccountExist(int Account){
	
	char location[255];
	strcpy (location,"./Accounts/");
	char Saccount[255];
	ConvertIntToString(Account,Saccount);
	strcat (location,Saccount);
	strcat (location,"_Data.txt");
	
	//printf("%s",location);
	
    bool exists = GetFileAttributes(location) != INVALID_FILE_ATTRIBUTES;
	//DIR* dir = opendir(location);
	if (exists)//dir)
	{
	    return true;
	    //closedir(dir);
	}
	else
	{
	    return false;
	}
	
}

bool ReadWholeFile(char Dir[])
{
	FILE *filePointer;

	filePointer = fopen(Dir, "r");

	if (filePointer == NULL) {
		fclose(filePointer);
		return false;
	}else{
		//char lines[255][255];
		int line = 0;
		
		while (fgets(lines[line], 255, (FILE*)filePointer)!=NULL){
			//printf("%s",lines[line]);
     		lines[line][strlen(lines[line])-1] = '\0';
     		//printf("\n%s",lines[line]);
			line++;
		}
			
		fclose(filePointer);
	}
  	
	return true;
}

bool ReadAccountFile(int Account){
	
	memset(lines, 0, sizeof(lines));
	
	char location[255];
	strcpy (location,"./Accounts/");
	char Saccount[255];
	ConvertIntToString(Account,Saccount);
	strcat (location,Saccount);
	strcat (location,"_Data.txt");
	
	return ReadWholeFile(location);
	
}
