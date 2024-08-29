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

}

extern RC openPageFile (char *fileName, SM_FileHandle *fHandle) {
    
}

extern RC closePageFile (SM_FileHandle *fHandle) {
    
}
// Firstly we will have to check if the file exists or not and then remove and same goes for even accessing the blocks
extern RC destroyPageFile (char *fileName) {
    pageBlock = fopen(fileName, "r");
    if(pageBlock != NULL) {
        remove(fileName);
        RC_message = "File has been removed successfully";
        return RC_OK;
    }
    else {
        RC_message = "File does not exist!";
        return RC_FILE_NOT_FOUND;
    }
}

extern RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {

}

// Check if the file exists or not then also check if correct index is provided for the page or not or even handle is correct or not
extern int getBlockPos (SM_FileHandle *fHandle) {
    int pageNumber = fHandle -> curPagePos;
    return pageNumber;
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

}

extern RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {

}

extern RC appendEmptyBlock (SM_FileHandle *fHandle) {

}

extern RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle) {
    
}