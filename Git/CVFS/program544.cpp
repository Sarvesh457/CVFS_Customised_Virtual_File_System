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
//      Entry Point Function of the Project      
//
///////////////////////////////////////////////////////////////////

int main()
{
    char str[80] = {'\0'};
    char Command[5][20] = {{'\0'}};
    int iCount = 0;

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

        printf("\n Marvellous CVFS : > ");
        fgets(str, sizeof(str), stdin);

        iCount = sscanf(str,"%s %s %s %s %s", Command[0], Command[1], Command[2], Command[3],Command[4]);    // tokenization

        fflush(stdin);

        if(iCount == 1)
        {
            if(strcmp("exit", Command[0]) == 0)         // Imp
            {
                printf("Thank you for using Marvellous CVFS\n");
                printf("Deallocating all the allocated resources\n");

                break;
            }
            else if(strcmp("ls", Command[0]) == 0)
            {
                printf("Inside ls");
            }
            
        }// End of if 1 
        else if(iCount == 2)
        {

        }// End of else if 2
        else if(iCount == 3)
        {

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
}// End of main