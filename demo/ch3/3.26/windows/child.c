/**
 * child.c
 *
 * This program reads a message from the pipe and switches the case of each letter
 * and writes the result back to the pipe.
 * For example, if the message 'Hello' was read, it would change the message to
 * 'hELLO' and write that message to the pipe.
 */
   
#include <stdio.h>
#include <windows.h>
#include <ctype.h>

#define BUFFER_SIZE 25

int main(VOID)
{
    HANDLE ReadHandle, WriteHandle; 
    CHAR buffer[BUFFER_SIZE];
    DWORD read, written, i;
    BOOL success;
    
    ReadHandle = GetStdHandle(STD_INPUT_HANDLE);
    WriteHandle= GetStdHandle(STD_OUTPUT_HANDLE);
    
    // now have the child read from the pipe
    success = ReadFile(ReadHandle, buffer, BUFFER_SIZE, &read, NULL);
    
    // we have to output to stderr as stdout is redirected to the pipe
    if (success)
       fprintf(stderr,"child: message = >%s<\n", buffer);
    else {
        fprintf(stderr, "child: error reading from pipe\n");
    
        return -1;
    }
    
    // now write something to the pipe
    for (i = 0; i < strlen(buffer); i++)
        if (isupper(buffer[i]))
           buffer[i] = tolower(buffer[i]);
        else
            buffer[i] = toupper(buffer[i]);
            
    if (!WriteFile(WriteHandle, buffer, BUFFER_SIZE, &written, NULL))
       fprintf(stderr, "Error writing to pipe\n");
    
    return 0;
}
