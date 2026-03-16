/////////////////////////////////////////////////////////////////////////
//      
//      Header file Inclusion
//
/////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>

/////////////////////////////////////////////////////////////////////////
//      
//      User Defined MACROS      
//
/////////////////////////////////////////////////////////////////////////

// Maximum file size is allowed in the project
#define MAXFILESIZE 50

// Maximum number of files we can open
#define MAXOPENFILES 20

// Maximum number of Inode are allowed
#define MAXINODE 5

// Permission
#define READ 1
#define WRITE 3
#define EXECUTE 4

// lseek() function self defined
#define START 0
#define CURRENT 1
#define END 2

// To flag success
#define EXECUTE_SUCCESS 0

// File type 
#define REGULAR_FILE 1
#define SPECIAL_FILE 2

/////////////////////////////////////////////////////////////////////////
//      
//      User Defined MACROS for error handling     
//
/////////////////////////////////////////////////////////////////////////

#define ERR_INVALID_PARAMETER -1                // create

#define ERR_NO_INODES -2                        // max number of reached

#define ERR_FILE_ALREADY_EXIST -3
#define ERR_FILE_NOT_EXIST -4

#define ERR_PERMISSION_DENIED -5

#define ERR_INSUFFICENT_SPACE -6
#define ERR_INSUFFICENT_DATA -7
#define ERR_MAX_FILE_OPEN -8

/////////////////////////////////////////////////////////////////////////
//      
//      User Defined Structures      
//
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//      
//      Structures Name :   BootBlock 
//      Description     :   Hold the information to boot the OS
//
/////////////////////////////////////////////////////////////////////////
struct BootBlock
{
    char Information[100];
};

///////////////////////////////////////////////////////////////////
//      
//      Structures Name :   SuperBlock 
//      Description     :   Hold the information about the file Systems
//
///////////////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInode;
    int FreeInode;
};

///////////////////////////////////////////////////////////////////
//      
//      Structures Name :   Inode
//      Description     :   Hold the information about the file
//
///////////////////////////////////////////////////////////////////

#pragma (1)
struct Inode
{
    char FileName[20];              // Data Block
    int InodeNumber;
    int FileSize;
    int ActualFileSize;
    int FileType;
    int ReferenceCount;
    int Permission;
    char *Buffer;                   // points to data block
    struct Inode *next;             // LinkedList DILB
};

typedef struct Inode INODE;
typedef struct Inode* PINODE;
typedef struct Inode** PPINODE;

///////////////////////////////////////////////////////////////////
//      
//      Structures Name :   FileTable
//      Description     :   Hold the information about the opened file
//
///////////////////////////////////////////////////////////////////

struct FileTable
{
    int ReadOffSet;
    int WriteOffSet;
    int Mode;
    PINODE ptrinode;            // points to iNode 
};

typedef FileTable FILETABLE;
typedef FileTable* PFILETABLE;

///////////////////////////////////////////////////////////////////
//      
//      Structures Name :   UAREA
//      Description     :   Hold the information about Process
//
///////////////////////////////////////////////////////////////////

struct UAREA                                    // RAM Data Strucure
{
    char ProcessName[20];
    PFILETABLE UFDT[MAXOPENFILES];              // Arr of Pointer
};

///////////////////////////////////////////////////////////////////
//      
//      Global variable or object used in the project     
//
///////////////////////////////////////////////////////////////////

BootBlock bootobj;
SuperBlock superobj;
UAREA uareaobj;

PINODE head = NULL;             //data section

///////////////////////////////////////////////////////////////////
//      
//      Function Name : InitialiseUAREA
//      Description   : It is used to initialise thr UAREA members  
//      Author        : Sarvesh Atul Mahajan
//      Date          : 13/01/2026
//
///////////////////////////////////////////////////////////////////

void InitialiseUAREA()
{
    strcpy(uareaobj.ProcessName,"Myexe");

    int i = 0;

    for(i = 0;i < MAXOPENFILES;i++)             // Max open files
    {
        uareaobj.UFDT[i] = NULL;                // to avoid the segmentation fault 
    }

    printf("Marvellous CVFS : UAREA gets initialise successfully\n");


}

///////////////////////////////////////////////////////////////////
//      
//      Function Name : InitialiseSuperBlock
//      Description   : It is used to initialise thr SuperBlock members  
//      Author        : Sarvesh Atul Mahajan
//      Date          : 13/01/2026
//
///////////////////////////////////////////////////////////////////

void InitialiseSuperBlock()
{
    superobj.TotalInode = MAXINODE;
    superobj.FreeInode = MAXINODE;

    printf("Marvellous CVFS : SuperBlock gets initialise successfully\n");

}

///////////////////////////////////////////////////////////////////
//      
//      Function Name : CreateDILB
//      Description   : It is used to create the linked list of iNode
//      Author        : Sarvesh Atul Mahajan
//      Date          : 13/01/2026
//
///////////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;
    
    for(i = 1; i <= MAXINODE;i++)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        strcpy(newn->FileName,"\0");
        newn->InodeNumber = i;
        newn->FileSize = 0;
        newn->ActualFileSize = 0;
        newn->FileType = 0;
        newn->Permission = 0;
        newn->Buffer = NULL;
        newn->next = NULL;

        if( temp == NULL)                   // LL is empty
        {
            head = newn;
            temp = head;
        }
        else                                // LL Contains atleast 1 Node
        {
            temp->next = newn;
            temp = temp->next; 
        }
    }
    printf("Marvellous CVFS : DILB created successfully\n");
}

///////////////////////////////////////////////////////////////////
//      
//      Function Name : StartAuxillaryDataInitialisation
//      Description   : It is used to to all such fuctions which are used to initialise auxillary data
//      Author        : Sarvesh Atul Mahajan
//      Date          : 13/01/2026
//
///////////////////////////////////////////////////////////////////

void StartAuxillaryDataInitialisation()
{   
    strcpy(bootobj.Information,"Booting process of Marvellous CVFS is done");
    
    printf("%s\n",bootobj.Information);

    InitialiseSuperBlock();

    CreateDILB();

    InitialiseUAREA();

    printf("Marvellous CVFS : Auxillary Data initialised successfully\n");
}


///////////////////////////////////////////////////////////////////
//      
//      Function Name : DisplayHelp
//      Description   : It is used to display help page
//      Author        : Sarvesh Atul Mahajan
//      Date          : 14/01/2026
//
///////////////////////////////////////////////////////////////////

void DisplayHelp()
{
    printf("--------------------------------------------------------------\n");
    printf("----------- Marvellous CVFS Help page successfully -----------\n");
    printf("--------------------------------------------------------------\n");

    printf("man     :   It is used to display manual page\n");
    printf("clear   :   It is used to clear the screen\n");
    printf("create  :   It is usd to create the file\n");
    printf("write   :   It is used to insert the data in the file\n");
    printf("read    :   It is used to read the data form the file\n");
    printf("state   :   It is used to display statistical information\n");
    printf("unlink  :   It is used to delete the file\n");
    printf("exit    :   It is used to exit the application\n");

    printf("--------------------------------------------------------------\n");

}

///////////////////////////////////////////////////////////////////
//      
//      Function Name : ManPageDisplay
//      Description   : It is used to display man page
//      Author        : Sarvesh Atul Mahajan
//      Date          : 14/01/2026
//
///////////////////////////////////////////////////////////////////

void ManPageDisplay(char Name[])
{
    if(strcmp("ls",Name) == 0)
    {
        printf("About : It is used to list the names of all files\n");
        printf("Usage : ls\n");
    }
    else if(strcmp("man",Name) == 0)
    {
        printf("About : It is used to display manual page\n");
        printf("Usage : man command_name\n");
        printf("Command_name : it is the name of the command\n");
    }
    else if(strcmp("exit",Name) == 0)
    {
        printf("About : It is used to terminate the shell \n");
        printf("Usage : exit\n");
    }
    else if(strcmp("clear",Name) == 0)
    {
        printf("About : It is used to clear the shell \n");
        printf("Usage : clear\n");
    }
    else
    {
        printf("no manual entry for %s\n",Name);
    }
}

///////////////////////////////////////////////////////////////////
//      
//      Function Name : IsFileExist
//      Description   : It is used to check wether the file is already exist or not
//      Input         : It accept file name
//      Output        : It return true or false
//      Author        : Sarvesh Atul Mahajan
//      Date          : 16/01/2026
//
///////////////////////////////////////////////////////////////////

bool IsFileExist(
                    char *name          // file name
                )
{
    PINODE temp = head;
    bool bFlag = false;

    while(temp != NULL)
    {
        if(strcmp(name,temp->FileName) == 0 && (temp->FileType == REGULAR_FILE))
        {
            bFlag = true;
            break;
        }
        temp = temp->next;
    }

    return bFlag;
}

///////////////////////////////////////////////////////////////////
//      
//      Function Name : CrateFile
//      Description   : It is used to create new regular file
//      Input         : It accept file name and permission
//      Output        : It return file descriptor
//      Author        : Sarvesh Atul Mahajan
//      Date          : 16/01/2026
//
///////////////////////////////////////////////////////////////////

int CrateFile(
                char *name,             // name of file
                int permission         // permission for that file
            )
{
    PINODE temp = head;
    int i = 0;
    PINODE fd = NULL;

    printf("Total number of Inode remaining : %d\n",superobj.FreeInode);

    // if name is missing
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    // if the permission value is wrong
    // permission-> 1 -> READ
    // permission-> 2 -> WRITE
    // permission-> 3 -> READ+WRITE **
    if(permission < 1 || permission > 3)
    {
        return ERR_INVALID_PARAMETER;
    }

    // if Inodes are full
    if(superobj.FreeInode == 0)
    {
        return ERR_NO_INODES;
    }

    // if file is already present
    if(IsFileExist(name) == true)
    {
        return ERR_FILE_ALREADY_EXIST;
    }
    
    // search empty inode

    while(temp != NULL)
    {
        if(temp->FileType == 0)
        {
            break;
        }   
        temp = temp->next; 
    }
    
    // unreachable function but for safety
    if(temp == NULL)
    {
        printf("There is no Inode\n");
        return ERR_NO_INODES;
    }
    
    // Search for empty UFDT entry
    // Note :  0,1,2 are reserved
    for(i = 3;i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    // UFDT is full
    if(i == MAXOPENFILES)
    {
        return ERR_MAX_FILE_OPEN;
    }

    // Allocate memory for file table
    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FileTable));

    // Initialize file table
    uareaobj.UFDT[i]->ReadOffSet = 0;
    uareaobj.UFDT[i]->WriteOffSet = 0;
    uareaobj.UFDT[i]->Mode = permission;

    // Connect filetable with INODE
    uareaobj.UFDT[i]->ptrinode = temp;

    // Initialise INODE

    strcpy(uareaobj.UFDT[i]->ptrinode->FileName, name);
    uareaobj.UFDT[i]->ptrinode->FileSize = MAXFILESIZE;
    uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
    uareaobj.UFDT[i]->ptrinode->FileType = REGULAR_FILE;
    uareaobj.UFDT[i]->ptrinode->ReferenceCount = 1;
    uareaobj.UFDT[i]->ptrinode->Permission = permission;

    // Allocate memory for files data
    uareaobj.UFDT[i]->ptrinode->Buffer = (char *)malloc(MAXFILESIZE);

    superobj.FreeInode--;

    return i;       // file descriptor returned
}

///////////////////////////////////////////////////////////////////
//      
//      Function Name : LsFile
//      Description   : It is used to list all files 
//      Input         : Nothing
//      Output        : Nothing
//      Author        : Sarvesh Atul Mahajan
//      Date          : 16/01/2026
//
///////////////////////////////////////////////////////////////////

// ls -l
void LsFile()
{
    PINODE temp = head;

    printf("----------------------------------------------------------------\n");
    printf("--------------- Marvellous CVFS Files Information --------------\n");
    printf("----------------------------------------------------------------\n");
    
    while(temp != NULL)
    {
        if(temp->FileType != 0)
        {
        printf("%d\t%s\t%d\n",temp->InodeNumber,temp->FileName,temp->ActualFileSize);
        }
        temp = temp->next;
    }

    printf("----------------------------------------------------------------\n");

}


///////////////////////////////////////////////////////////////////
//      
//      Entry Point Function of the Project      
//
///////////////////////////////////////////////////////////////////

int main()
{
    char str[80] = {'\0'};
    char Command[5][20] = {{'\0'}};
    int iCount = 0;
    int iRet = 0;

    StartAuxillaryDataInitialisation();

    printf("\n");

    printf("--------------------------------------------------------------\n");
    printf("------------ Marvellous CVFS started successfully ------------\n");
    printf("--------------------------------------------------------------\n");

    // Infinite Listening Shell
    while(1)
    {
        fflush(stdin);

        strcpy(str, "");

        printf("\nMarvellous CVFS : > ");
        fgets(str, sizeof(str), stdin);

        iCount = sscanf(str,"%s %s %s %s %s", Command[0], Command[1], Command[2], Command[3],Command[4]);    // tokenization

        fflush(stdin);

        if(iCount == 1)
        {
            // Marvellous CVFS : > exit
            if(strcmp("exit", Command[0]) == 0)         // Imp
            {
                printf("Thank you for using Marvellous CVFS\n");
                printf("Deallocating all the allocated resources\n");

                break;
            }
            // Marvellous CVFS : > ls
            else if(strcmp("ls", Command[0]) == 0)
            {
                LsFile();
            }
            // Marvellous CVFS : > help
            else if(strcmp("help",Command[0]) == 0)
            {
                DisplayHelp();
            }
            // Marvellous CVFS : > clear
            else if(strcmp("clear",Command[0]) == 0)
            {
                #ifdef _WIN32           
                    system("cls");
                #else 
                    system("clear");
                #endif
            }
            
        }// End of if 1
        // Marvellous CVFS : > man ls
        else if(iCount == 2)
        {
            if(strcmp("man",Command[0]) == 0)
            {
                ManPageDisplay(Command[1]);
            }
        }// End of else if 2
        // Marvellous CVFS : > creat Ganesh.txt 3
        else if(iCount == 3)
        {
            if(strcmp("creat",Command[0]) == 0)
            {
                iRet = CrateFile(Command[1],atoi(Command[2]));              // atoi -> ASCII to integer
            }
            
            if(iRet == ERR_INVALID_PARAMETER)
            {
                printf("Error : Enable to creat the file as parameters are invalid\n");
                printf("Please refer man page\n");
            }

            if(iRet == ERR_NO_INODES)
            {
                printf("Error : Enable to creat the file as there are no Inodes\n");
            } 

            if(iRet == ERR_FILE_ALREADY_EXIST)
            {
                printf("Error : Enable to creat the file because file is already present \n");
            } 

            if(iRet == ERR_MAX_FILE_OPEN)
            {
                printf("Error : Enable to create file\n");
                printf("Max opened files limit reached\n");
            }

            printf("File gets successfully created with fd %d\n", iRet);

        }// End of else if 3 
        else if(iCount == 4)
        {

        }// End of else if 4 
        else
        {
            printf("Command not found\n");
            printf("Please refer help option to get more information\n");
        }// End of else
    }// End of while

    return 0;
}// End of main'