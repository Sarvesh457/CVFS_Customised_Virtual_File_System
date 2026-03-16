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

    printf("Marvellous CVFS : UAREA gets initialise successfully");


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

    printf("Marvellous CVFS : SuperBlock gets initialise successfully");

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
    printf("Marvellous CVFS : DILB created successfully");
}

///////////////////////////////////////////////////////////////////
//      
//      Entry Point Function of the Project      
//
///////////////////////////////////////////////////////////////////

int main()
{
    

    return 0;
}