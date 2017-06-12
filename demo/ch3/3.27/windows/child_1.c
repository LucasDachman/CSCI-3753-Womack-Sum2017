/*
 * child.c
 *
 * This program reads the contents of the pipe and writes it to a file.
 */
  
#include <stdio.h>
#include <windows.h>

#define BUFFER_SIZE 10

int main(int argc, char *argv[])
{
    HANDLE ReadHandle, WriteHandle, OutputFileHandle; 
    CHAR buffer[BUFFER_SIZE];
    DWORD read, written;
    BOOL success;
    
    /* this is the read end of the pipe */
    ReadHandle = GetStdHandle(STD_INPUT_HANDLE);
    
    /* this is standard output */
    WriteHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    
    /* handle to the output file */
    OutputFileHandle = CreateFile(/*"copy.txt"*/argv[1], GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL, NULL);
    
    //fprintf(stderr,"child: started reading\n");
    // now have the child read from the pipe and write its contents to copy.txt
    for (;;) {
        //printf("reading");
        if ( !ReadFile(ReadHandle, buffer, BUFFER_SIZE, &read, NULL))
           break;
        if (read == 0)
           break;
         
         //printf("read = %d >%s<\n",read, buffer);
         
         if (! WriteFile(OutputFileHandle, buffer, read, &written, NULL)) 
            break;    
    }
    
    //fprintf(stderr,"child: out of for loop\n");
    
    CloseHandle(ReadHandle);    
    CloseHandle(OutputFileHandle);
    
    return 0;
}
