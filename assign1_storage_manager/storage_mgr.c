// 1) Firstly we will import all the necessary libraries and the header files that we need in the storage manager.

#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<math.h>
#include "storage_mgr.h"

// 2) Now we shall create the pointer variable for the storage manager.
FILE *pageBlock;

// 3) here let's initialise the pointer variable or the storage manager.
extern void initStorageManager (void) {
    printf("Storage manager successfully works yayyyy!");
    pageBlock = NULL;
}

extern RC createPageFile (char *fileName) {
    RC rc = -1;
    FILE *filePointer = fopen(fileName, "wb+");
    if (filePointer == NULL) {
        RC_message = "File requested is not found";
        return RC_FILE_NOT_FOUND;
    }
    SM_PageHandle newPage = (SM_PageHandle) calloc(PAGE_SIZE, sizeof(char));
    int write_check = fwrite(newPage, sizeof(char), PAGE_SIZE, filePointer);
    if (write_check == 0) {
        RC_message = "Sorry you cannot write in this file";
        return RC_WRITE_FAILED;
    }
    else {
        return RC_OK;
    }
    free(newPage);
    int closeFile = fclose(filePointer);
    if (closeFile == EOF) {
        rc = RC_CLOSE_FILE_FAILED;
    }
    return rc;
}

extern RC openPageFile (char *fileName, SM_FileHandle *fHandle) {
    FILE *filePointer = fopen(fileName, "r+");
    if (filePointer == NULL) {
        RC_message = "File requested is not found";
        return RC_FILE_NOT_FOUND;
    }
    int file_position_check = fseek(filePointer, 0L, SEEK_END);
    if(file_position_check != 0) {
        RC_message = "File Position error";
        return RC_FILE_POSITION_ERROR;
    }
    long tailPosition = ftell(filePointer);
    if(tailPosition == -1L) {
        RC_message = "File posiiton error";
        return RC_FILE_TAIL_POSIITON_ERROR;
    }
    int fileLength = (int) tailPosition + 1;
    int totalNumber = fileLength / PAGE_SIZE;
    file_position_check = fseek(filePointer, 0L, SEEK_END);
    if(file_position_check) {
        RC_message = "File Position error";
        return RC_FILE_POSITION_ERROR;
    }
    fHandle -> fileName = fileName;
    fHandle -> totalNumPages = totalNumber;
    fHandle -> curPagePos = 0;
    fHandle -> mgmtInfo = filePointer;

    return RC_OK;
}

extern RC closePageFile (SM_FileHandle *fHandle) {
    int close_file_check = fclose(fHandle -> mgmtInfo);
    if(close_file_check == EOF) {
        RC_message = "File close has been failed";
        return RC_CLOSE_FILE_FAILED;
    }
    return RC_OK;
}
// Firstly we will have to check if the file exists or not and then remove and same goes for even accessing the blocks
extern RC destroyPageFile (char *fileName) {
    pageBlock = fopen(fileName, "r");
    if(pageBlock != NULL) {
        int checkFileremove = remove(fileName);
        if(checkFileremove == 0) {
            RC_message = "File has been deleted successfully";
            return RC_OK;
        }
        else {
            RC_message = "File has not been deleted successfully";
            return RC_FILE_NOT_DELETED;
        }
    }
    else {
        RC_message = "File does not exist!";
        return RC_FILE_NOT_FOUND;
    }
}

extern RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {
    if(fHandle -> totalNumPages < pageNum || pageNum < 0) {
        RC_message = "The page does not exist";
        return RC_PAGE_DOES_NOT_EXIST;
    }
    int balance = sizeof(char) * pageNum * PAGE_SIZE;
    int set_pointer_check = fseek(fHandle -> mgmtInfo, balance, SEEK_SET);
    if(set_pointer_check != 0) {
        RC_message = "File page error";
        return RC_FILE_POSITION_ERROR;
    }
    int file_read_check = fread(memPage, sizeof(char), PAGE_SIZE, fHandle -> mgmtInfo);
    if(file_read_check != PAGE_SIZE) {
        RC_message = "File read failed here, try again";
        return RC_FILE_READ_FAILED;
    }
    pageNum = fHandle -> curPagePos;
    return RC_OK;
}

// Check if the file exists or not then also check if correct index is provided for the page or not or even handle is correct or not
extern int getBlockPos (SM_FileHandle *fHandle) {
    if (fHandle != NULL) {
        return fHandle -> curPagePos;
    }
    else {
        RC_message = "File not been read correctly";
        return RC_FILE_READ_FAILED;
    }
}

extern RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
    return readBlock(0, fHandle, memPage);
}

extern RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
    return readBlock(fHandle -> curPagePos - 1,fHandle, memPage);
}

extern RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
    return readBlock(fHandle -> curPagePos, fHandle, memPage);
}

extern RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
    return readBlock(fHandle -> curPagePos + 1, fHandle, memPage);
}

extern RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
    return readBlock(fHandle -> totalNumPages - 1, fHandle, memPage);
}

extern RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {
    if(fHandle -> totalNumPages < pageNum || pageNum < 0) {
        RC_message = "The page does not exist";
        return RC_PAGE_DOES_NOT_EXIST;
    }
    int balance = pageNum * PAGE_SIZE * sizeof(char);
    int set_pointer_check = fseek(fHandle -> mgmtInfo, balance, SEEK_SET);
    if(set_pointer_check != 0) {
        RC_message = "File page error";
        return RC_FILE_POSITION_ERROR;
    }
    int file_write_check = fwrite(memPage, sizeof(char), PAGE_SIZE, fHandle -> mgmtInfo);
    if(file_write_check != PAGE_SIZE) {
        RC_message = "File write failed here, try again";
        return RC_WRITE_FAILED;
    }
    pageNum = fHandle -> curPagePos;
    return RC_OK;
}

extern RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
    return writeBlock(fHandle -> curPagePos, fHandle, memPage);
}

extern RC appendEmptyBlock (SM_FileHandle *fHandle) {
    RC rc = -1;
    FILE *filePointer = fHandle -> mgmtInfo;
    if (filePointer == NULL) {
        RC_message = "File requested is not found";
        return RC_FILE_NOT_FOUND;
    }
    SM_PageHandle emptyPage = (SM_PageHandle) calloc(PAGE_SIZE, sizeof(char));
    int file_position_check = fseek(filePointer, 0L, SEEK_END);
    if(file_position_check != 0) {
        RC_message = "File Position error";
        return RC_FILE_POSITION_ERROR;
    }
    int write_check = fwrite(emptyPage, sizeof(char), PAGE_SIZE, filePointer);
    if (write_check == 0) {
        RC_message = "Sorry you cannot write in this file";
        return RC_WRITE_FAILED;
    }
    else {
        return RC_OK;
    }
    free(emptyPage);
    return rc;
}

extern RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle) {
    if(fHandle -> totalNumPages >= numberOfPages) {
        RC_message = "There are already enough pages";
        return RC_ENOUGH_PAGES;
    }
    int totalNumber = fHandle -> totalNumPages;
    int i;
    for(i = 0; i < (numberOfPages - totalNumber); i++) {
        RC rc = appendEmptyBlock(fHandle);
        if(rc != RC_OK) {
            return rc;
        }
    }
    return RC_OK;
}