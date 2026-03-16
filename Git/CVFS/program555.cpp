///////////////////////////////////////////////////////////////////////////////////////
//
//  CUSTOMIZED VIRTUAL FILE SYSTEM PROJECT
//
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
//
//  Header File Inclusion
//
///////////////////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>

///////////////////////////////////////////////////////////////////////////////////////
//
//  User Defined MACROS
//
///////////////////////////////////////////////////////////////////////////////////////

// Maximum file size that we allow in the project
#define MAXFILESIZE 50

// Maximum No of files that we allow to open in project
#define MAXOPENFILES 20

// Maximum iNodes we allow in project
#define MAXINODE 5

// Permissions
#define READ 1
#define WRITE 2
#define EXECUTE 4

// lseek() functions self defined
#define START 0
#define CURRENT 1
#define END 2

// To flag success
#define EXECUTE_SUCCESS 0

// file_type macors
#define REGULARFILE 1
#define SPECIALFILE 2

///////////////////////////////////////////////////////////////////////////////////////
//
//  User Defined MACROS for error handling
//
///////////////////////////////////////////////////////////////////////////////////////

#define ERR_INVALID_PARAMETER -1

#define ERR_NO_INODES -2

#define ERR_FILE_ALREADY_EXIST -3
#define ERR_FILE_NOT_EXIST -4

#define ERR_PERMISSION_DENIED -5

#define ERR_INSUFFICIENT_SPACE -6
#define ERR_INSUFFICIENT_DATA -7

#define ERR_MAX_FILES_OPEN -8

///////////////////////////////////////////////////////////////////////////////////////
//
//  User Defined Structures
//
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :    BootBlock
//  Description :       Hold the information to boot the OS
//
///////////////////////////////////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];
};

///////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :    SuperBlock
//  Description :       Hold the information about the file system
//
///////////////////////////////////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeInodes;
};

///////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :    Inode
//  Description :       Hold the information about the file
//
///////////////////////////////////////////////////////////////////////////////////////

#pragma pack(1)
struct Inode
{
    char FileName[20];
    int InodeNumber;
    int FileSize;
    int ActualFileSize;
    int FileType;
    int ReferenceCount;
    int Permission;
    char * Buffer;
    struct Inode * next;
};

typedef struct Inode INODE;
typedef struct Inode * PINODE;
typedef struct Inode * * PPINODE;

///////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :    FileTable
//  Description :       Hold the information about the opened file
//
///////////////////////////////////////////////////////////////////////////////////////

struct FileTable
{
    int ReadOffset;
    int WriteOffset;
    int Mode;                               // mode like read, write and both
    PINODE ptrinode;
};

typedef struct FileTable FILETABLE;
typedef struct FileTable * PFILETABLE;

///////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :    UAREA
//  Description :       Hold the information about the process file
//
///////////////////////////////////////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[20];                   // .exe name as it becomes process(dummy)
    PFILETABLE UFDT[MAXOPENFILES];          // user file descriptor table and it is an pointer array
};

///////////////////////////////////////////////////////////////////////////////////////
//
//  Global variables or objects used in project
//
///////////////////////////////////////////////////////////////////////////////////////

BootBlock bootobj;
SuperBlock superobj;
UAREA uareaobj;

PINODE head = NULL;

///////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name : InitialiseUAREA 
//  Description :   It is used to initialise UAREA members
//  Author :        Krishna Govindrav Hitnalikar
//  Date :          13/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////

void InitialiseUAREA()
{
    strcpy(uareaobj.ProcessName, "Myexe");

    int i = 0;

    for(i = 0; i < MAXOPENFILES; i++)
    {
        uareaobj.UFDT[i] = NULL;            // to avoide segmentation fault
    }

    printf("Marvellous CVFS : UAREA gets initialised successfully\n");
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name : InitialiseSuperBlock
//  Description :   It is used to initialise SuperBlock members
//  Author :        Krishna Govindrav Hitnalikar
//  Date :          13/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////

void InitialiseSuperBlock()
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeInodes = MAXINODE;

    printf("Marvellous CVFS : Super block gets initialised successfully\n");
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name : CreateDILB
//  Description :   It is used to create linked-list of Inodes
//  Author :        Krishna Govindrav Hitnalikar
//  Date :          13/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 1;
    PINODE newn =  NULL;
    PINODE temp = head;

    for(i = 1; i <= MAXINODE; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        strcpy(newn->FileName, "\0");
        newn->InodeNumber = i;
        newn->FileSize = 0;
        newn->ActualFileSize = 0;
        newn->FileType = 0;
        newn->ReferenceCount = 0;
        newn->Permission = 0;
        newn->Buffer = NULL;
        newn->next = NULL;

        if(temp == NULL)                    // LL is empty
        {
            head = newn;
            temp = head;
        }
        else                                // LL contains atleast 1 node
        {
            temp->next = newn;
            temp = temp->next;
        }
    }

    printf("Marvellous CVFS : DILB gets created successfully\n");
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name : StartAuxillaryDataInitialisation
//  Description :   It is used to call all such functions which are which 
//                  are used to initialise auxillary data
//  Author :        Krishna Govindrav Hitnalikar
//  Date :          13/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////

void StartAuxillaryDataInitialisation()
{
    strcpy(bootobj.Information, "Booting Process of Marvellous CVFS is done");

    printf("%s\n", bootobj.Information);

    printf("\n");

    InitialiseSuperBlock();

    CreateDILB();

    InitialiseUAREA();

    printf("Marvellous CVFS : Auxillary data initialised successfully\n");
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name : DisplayHelp
//  Description :   It is used to display help page 
//  Author :        Krishna Govindrav Hitnalikar
//  Date :          14/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////

void DisplayHelp()
{
    printf("--------------------------------------------------------------\n");
    printf("----------------- Marvellous CVFS Help Page ------------------\n");
    printf("--------------------------------------------------------------\n");

    printf("man     : It is used to display manual page\n");
    printf("clear   : It is used to clear the terminal\n");
    printf("creat   : It is used to create new file\n");
    printf("write   : It is used to write the data into th file\n");
    printf("read    : It is used to read the data from the file\n");
    printf("stat    : It is used to display statistical information\n");
    printf("unlink  : It is used to delete the file\n");
    printf("exit    : It is used to terminate Marvellous CVFS\n");

    printf("--------------------------------------------------------------\n");
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name : ManPageDisplay
//  Description :   It is used to display man page 
//  Author :        Krishna Govindrav Hitnalikar
//  Date :          14/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////

void ManPageDisplay(char Name[])
{
    if(strcmp("ls", Name) == 0)
    {
        printf("About   : It is used to list the names of all files\n");
        printf("Usage   : ls\n");
    }
    else if(strcmp("man", Name) == 0)
    {
        printf("About   : It is used to display manual page\n");
        printf("Usage   : man command_name\n");
        printf("command_name : It is the name of command\n");
    }
    else if(strcmp("exit", Name) == 0)
    {
        printf("About   : It is used to terminate the shell\n");
        printf("Usage   : exit\n");
    }
    else if(strcmp("clear", Name) == 0)
    {
        printf("About   : It is used to clear the shell\n");
        printf("Usage   : clear\n");
    }
    else
    {
        printf("No manual entry for %s\n", Name);
    }
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name : IsFileExist
//  Description :   It is used to check whether file is already exist or not
//  Input :         It accepts file name
//  Output :        It returns true or false
//  Author :        Krishna Govindrav Hitnalikar
//  Date :          16/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////

bool IsFileExist(
                    char * name            // file name 
)
{
    PINODE temp = head;
    bool bFlag = false;

    while(temp != NULL)
    {
        if(strcmp(name, temp->FileName) == 0 && temp->FileType == REGULARFILE)
        {
            bFlag = true;
            break;
        }

        temp = temp->next;
    }

    return bFlag;
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name : CreateFile
//  Description :   It is used to create new regular file
//  Input :         It accepts file name and permissions
//  Output :        It returns file descriptor 
//  Author :        Krishna Govindrav Hitnalikar
//  Date :          16/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////

int CreateFile(
                    char * name,                // name of new file
                    int permission              // permission for thst file
                )
{
    PINODE temp = NULL;
    temp = head;

    int i = 0;      // counter

    printf("Total number of INODES remaining : %d\n", superobj.FreeInodes);

    // if name is missing
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    // if the permission value is wrong
    // permisssion -> 1 -> READ
    // permisssion -> 2 -> WRITE
    // permisssion -> 3 -> READ and WRITE both
    if(permission < 1 || permission > 3)
    {
        return ERR_INVALID_PARAMETER;
    }

    // if INODES are full
    if(superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES;
    }

    // if file is already present
    if(IsFileExist(name) == true)
    {
        return ERR_FILE_ALREADY_EXIST;
    }

    // Search empty INODE
    while(temp != NULL)
    {
        if(temp->FileType == 0)
        {
            break;
        }
        
        temp = temp->next;
    }

    if(temp == NULL)
    {
        printf("Their is no INODE\n");
        return ERR_NO_INODES;
    }

    // Search for empty UFDT entry
    // NOTE -> 0,1,2 are reserved
    for(i = 3; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    // UFDT array is full
    if(i == MAXOPENFILES)
    {
        return ERR_MAX_FILES_OPEN;
    }

    // Allocate memory for FILE_TABLE
    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    // Initialise filetable
    uareaobj.UFDT[i]->ReadOffset = 0;
    uareaobj.UFDT[i]->WriteOffset = 0;
    uareaobj.UFDT[i]->Mode = permission;

    //connect filetable with iNode
    uareaobj.UFDT[i]->ptrinode = temp;

    // Initialise elements of iNode
    strcpy(uareaobj.UFDT[i]->ptrinode->FileName,name);
    uareaobj.UFDT[i]->ptrinode->FileSize = MAXFILESIZE;
    uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
    uareaobj.UFDT[i]->ptrinode->FileType = REGULARFILE;
    uareaobj.UFDT[i]->ptrinode->ReferenceCount = 1;
    uareaobj.UFDT[i]->ptrinode->Permission = permission;

    // allocate memory for files data
    uareaobj.UFDT[i]->ptrinode->Buffer = (char*)malloc(sizeof(MAXFILESIZE)); 

    superobj.FreeInodes--;

    return i;  // file descriptor
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name : LsFile()
//  Description :   It is used to list all files
//  Input :         Nothing
//  Output :        Nothing
//  Author :        Krishna Govindrav Hitnalikar
//  Date :          16/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////

void LsFile()
{
    PINODE temp = head;

    printf("--------------------------------------------------------------\n");
    printf("-------------- Marvellous CVFS Files Information -------------\n");
    printf("--------------------------------------------------------------\n");

    while(temp != NULL)
    {
        if(temp->FileType != 0)
        {
            printf("%d\t%s\t%d\n", temp->InodeNumber, temp->FileName, temp->ActualFileSize);
        }

        temp = temp->next;
    }

    printf("--------------------------------------------------------------\n");
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name : UnlinkFile()
//  Description :   It is used to delete the file
//  Input :         File name
//  Output :        Nothing
//  Author :        Krishna Govindrav Hitnalikar
//  Date :          22/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////

int UnlinkFile(
                    char * name             // name of file
                )
{
    int i = 0;

    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(IsFileExist(name) == false)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // Travel the UFDT
    for(i = 0; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            if(strcmp(uareaobj.UFDT[i]->ptrinode->FileName, name) == 0)         // == 0 means exact match found
            {
                // Deallocate memory for BUFFER
                free(uareaobj.UFDT[i]->ptrinode->Buffer);
                uareaobj.UFDT[i]->ptrinode->Buffer = NULL;

                // Reset all values of INODE
                // Don't deallocate memory of inode
                uareaobj.UFDT[i]->ptrinode->FileSize = 0;
                uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
                uareaobj.UFDT[i]->ptrinode->FileType = 0;
                uareaobj.UFDT[i]->ptrinode->ReferenceCount = 0;
                uareaobj.UFDT[i]->ptrinode->Permission = 0;

                memset(uareaobj.UFDT[i]->ptrinode->FileName, '\0', sizeof(uareaobj.UFDT[i]->ptrinode->FileName));       // Imp

                // Deallocate memory of file table
                free(uareaobj.UFDT[i]);

                // Set NULL to UFDT
                uareaobj.UFDT[i] = NULL;

                // Increment free inodes count
                superobj.FreeInodes++;

                break;      // IMP
            }
        }
    }

    return EXECUTE_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name : WriteFile()
//  Description :   It is used to write the data into the file
//  Input :         File descriptor
//                  Address of Buffer which contains data
//                  Size of data that we want to write
//  Output :        Number of bytes successfully written
//  Author :        Krishna Govindrav Hitnalikar
//  Date :          22/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////

int WriteFile(
                    int fd,             // file descriptor
                    char * data,        // pointer points to buffer data
                    int size            
                )
{
    printf("File descriptor : %d\n", fd);
    printf("Data that we want to write : %s\n", data);
    printf("Number of bytes that we want to write : %d\n", size);

    // Invalid FD
    if(fd < 0 || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    // FD points to NULL
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // Their is no permission
    if(uareaobj.UFDT[fd]->ptrinode->Permission < WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Insufficient Space
    if((MAXFILESIZE - uareaobj.UFDT[fd]->WriteOffset) < size)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    // Write the data into the file
    strncpy(uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->WriteOffset, data, size);

    // Update the Write Offset
    uareaobj.UFDT[fd]->WriteOffset = uareaobj.UFDT[fd]->WriteOffset + size;

    // Update the Actual file size
    uareaobj.UFDT[fd]->ptrinode->ActualFileSize = uareaobj.UFDT[fd]->ptrinode->ActualFileSize + size;

    return size;
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  Entrypoint Function of the project
//
///////////////////////////////////////////////////////////////////////////////////////

int main()
{
    char str[80] = {'\0'};
    char Command[5][20] = {{'\0'}};
    int iCount = 0;
    int iRet = 0;
    char InputBuffer[MAXFILESIZE] = {'\0'};

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

        iCount = sscanf(str, "%s %s %s %s %s", Command[0], Command[1], Command[2], Command[3], Command[4]);    // tokenization

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
            else if(strcmp("help", Command[0]) == 0)
            {
                DisplayHelp();
            }
            // Marvellous CVFS : > clear
            else if(strcmp("clear", Command[0]) == 0)
            {
                #ifdef _WIN32
                    system("cls");
                #else
                    system("clear");
                #endif
            }

        }// End of if 1 
        else if(iCount == 2)
        {
            // Marvellous CVFS : > man "command_name"
            // Marvellous CVFS : > man ls
            if(strcmp("man", Command[0]) == 0)
            {
                ManPageDisplay(Command[1]);
            }
            // Marvellous CVFS : > unlink "File_Name"
            // Marvellous CVFS : > unlink Demo.txt
            else if(strcmp("unlink", Command[0]) == 0)
            {
                iRet = UnlinkFile(Command[1]);

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid Parameter\n");
                }

                if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : Unable to delete as their is no such file\n");
                }

                if(iRet == EXECUTE_SUCCESS)
                {
                    printf("File gets successfully deleted\n");
                }
            }
            // Marvellous CVFS : > write "File Descriptor"
            // Marvellous CVFS : > write 3
            else if(strcmp("write", Command[0]) == 0)
            {
                printf("Enter the data that you want to write : \n");
                fgets(InputBuffer, MAXFILESIZE, stdin);

                iRet = WriteFile(atoi(Command[1]), InputBuffer, strlen(InputBuffer) - 1);

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid Parameters\n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : Their is no such file\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Unable to write as their is no permission\n");
                }
                else if(iRet == ERR_INSUFFICIENT_SPACE)
                {
                    printf("Error : Unable to write as their is no space\n");
                }
                else 
                {
                    printf("%d bytes gets successfully written\n", iRet);
                }
            }
            else
            {
               printf("Their is no such command\n"); 
            }

        }// End of else if 2
        else if(iCount == 3)
        {
            // Marvellous CVFS : > creat "File_name" "Permission"
            // Marvellous CVFS : > creat "Ganesh.txt" 3
            if(strcmp("creat", Command[0]) == 0)
            {
                iRet = CreateFile(Command[1], atoi(Command[2]));    // atoi -> ascii to integer // inbuilt function
                
                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Unable to create file as parameters are invalid\n");
                    printf("Please refer man page\n");
                }

                if(iRet == ERR_NO_INODES)
                {
                    printf("Error : Unable to create file as their is no INODE\n");
                }

                if(iRet == ERR_FILE_ALREADY_EXIST)
                {
                    printf("Error : Unable to create file because the file is already present\n");
                }

                if(iRet == ERR_MAX_FILES_OPEN)
                {
                    printf("Error : Unable to create file\n");
                    printf("Max opened files limit reached\n");
                }
                printf("File gets created successfully with fd %d\n", iRet);
            }
            else
            {
                printf("Their is no such command\n");
            }

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