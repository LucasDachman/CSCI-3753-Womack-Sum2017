/*
 * parent.c
 *
 * This program writes a message to a pipe and reads the result back from the pipe.
 */
  
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define BUFFER_SIZE 25

int main(VOID)
{
 HANDLE StdinRead, StdinWrite;      /* pipe for writing parent to child */ 
 HANDLE StdoutRead, StdoutWrite;    /* pipe for writing child to parent */
 STARTUPINFO si;
 PROCESS_INFORMATION pi;
 char message[BUFFER_SIZE] = "Greetings";
 DWORD read, written;
 BOOL success;
 
 /* set up security attributes so that pipe handles are inherited */
 SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL,TRUE};

 /* allocate memory */
 ZeroMemory(&pi, sizeof(pi));
 
  /* create the pipe for writing from parent to child */
 if ( !CreatePipe(&StdinRead, &StdinWrite, &sa, 0)) {
    fprintf(stderr,"Create Pipe Failed\n");
    return 1;
 }
 
 /* create the pipe for writing from child to parent */
 if ( !CreatePipe(&StdoutRead, &StdoutWrite, &sa, 0)) {
    fprintf(stderr,"Create Pipe Failed\n");
    return 1;
 }
  
 /* establish the START_INFO structure for the child process */
 GetStartupInfo(&si);
 si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
 
 /* redirect the standard input to the read end of the pipe */
 si.hStdOutput = StdoutWrite;
 si.hStdInput = StdinRead;
 si.dwFlags = STARTF_USESTDHANDLES;
 
 /* create the child process */
 if (!CreateProcess(NULL, 
                     ".//child.exe",
                     NULL,
                     NULL,
                     TRUE, /* inherit handles */
                     0,
                     NULL,
                     NULL,
                     &si,
                     &pi))
  {
     fprintf(stderr, "Process Creation Failed\n");
     return -1;
  }
               
 /* close the unused ends of the pipe */
 CloseHandle(StdoutWrite);
 CloseHandle(StdinRead);
       
 /* the parent now wants to write to the pipe */
 if (!WriteFile (StdinWrite, message, BUFFER_SIZE, &written, NULL))
    fprintf(stderr, "Error writing to pipe\n");
    
 /* close the write end of the pipe */
 CloseHandle(StdinWrite);
 
 /* now read from the pipe */
 success = ReadFile(StdoutRead, message, BUFFER_SIZE, &read, NULL);
 if (success)
    printf("parent: message = >%s<\n",message);
 else {
      fprintf(stderr, "parent: Error reading from pipe\n");
      
      return -1;
 }
 
 /* close the read end of the pipe */
 CloseHandle(StdoutRead);
 
 /* wait for the child to exit */
 WaitForSingleObject(pi.hProcess, INFINITE);  
 
 /* close all handles */
 CloseHandle(pi.hProcess);
 CloseHandle(pi.hThread);                 
}
