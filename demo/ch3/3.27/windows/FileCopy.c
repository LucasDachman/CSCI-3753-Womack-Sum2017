/*
 * FileCopy.c
 * 
 * This program opens a file and writes its contents to a pipe 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define BUFFER_SIZE 25
#define STRING_SIZE 25

int main(int argc, char *argv[])
{
 HANDLE ReadHandle, WriteHandle, InputFile;
 STARTUPINFO si;
 PROCESS_INFORMATION pi;
 char buffer[BUFFER_SIZE];
 char child_command[STRING_SIZE];
 DWORD read, written;
 
 if (argc != 3) {
    fprintf(stderr, "Usage: FileCopy <source> <copy>");
    
    return -1;
 }
 
 /* now open the file */
  InputFile = CreateFile(argv[1], GENERIC_READ, 0, NULL, 
  OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL); 

  if (InputFile == INVALID_HANDLE_VALUE) {
      fprintf(stderr,"CreateFile failed");
      
      return -1;
  }
  
 /* Build the string containing the child command */
 sprintf(child_command,".//child.exe %s",argv[2]);

 /* set up security attributes so that pipe handles are inherited */
 SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL,TRUE};

 /* allocate memory */
 ZeroMemory(&pi, sizeof(pi));
 
  /* create the pipe */
 if ( !CreatePipe(&ReadHandle, &WriteHandle, &sa, 0)) {
    fprintf(stderr,"Create Pipe Failed\n");
    return 1;
 }
 
 /* we do not want the pipe write handle inherited */
 SetHandleInformation( WriteHandle, HANDLE_FLAG_INHERIT, 0);
  
 /* establish the START_INFO structure for the child process */
 GetStartupInfo(&si);
 si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
 si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
 /* redirect the standard input to the read end of the pipe */
 si.hStdInput = ReadHandle;
 si.dwFlags = STARTF_USESTDHANDLES;
 
 /* create the child process */
 if (!CreateProcess(NULL, 
                     //".\\child.exe",
                     child_command,
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
               
 /* write the file to the pipe */
 for (;;) 
   { 
      if (! ReadFile(InputFile, buffer, BUFFER_SIZE, &read, NULL) || 
         read == 0) break; 
      if (! WriteFile(WriteHandle, buffer, read, &written, NULL)) 
         break; 
   } 
 
 CloseHandle(InputFile);
 /* close the pipe */
 CloseHandle(WriteHandle);

 // wait for the child to exit
 WaitForSingleObject(pi.hProcess, INFINITE);  
 
 // close all handles
 CloseHandle(pi.hProcess);
 CloseHandle(pi.hThread);                 
}
