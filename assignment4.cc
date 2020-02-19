//******************************************************************//
//   PROGRAM:    CSCI 330 Assignment 4                              //
//   PROGRAMMER: Margaret Higginbotham                              //
//   LOGON ID:   z1793581                                           //
//   DUE DATE:   March 9, 2018                                      //
//                                                                  //
//   FUNCTION:   Prompts user for 2 inputs commands using           //
//               pipe, fork, exec, dupe                             //
//******************************************************************//

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char *argv[]){
  int arg_count = 1;		//Number of arguments
  int pipefd[2];		//Pipe file descriptors
  int stdin_copy = dup(0);	//Stores stdin
  int pid;			//Pipe
  int fid;			//Fork
  int wait_status;		//When wait is finished
  int wait_exit;		//When wait is finished

  char input1[127];		//User input1
  char input2[127];		//User input2
  char *cin1_strtok[5];		//User input1 tokenized
  char *cin2_strtok[5];		//User input2 tokenized

//////////////////////////////////////////////////////////////////////

				//Prompts user for first command
  cout << "Enter command one or type \"quit\" to quit: ";

				//Loop if "quit" is not inputed
  while((cin.getline(input1, 127)) && (strcmp(input1, "quit") != 0)){
				//Prompts user for second command
    cout << "Enter command two or type \"quit\" to quit: ";

    cin.getline(input2, 127);

				//Tokenizes first input
    cin1_strtok[0] = strtok(input1, " ");

    while((cin1_strtok[arg_count] = strtok(NULL, " ")) != 0){
      arg_count++;		//Number of arguments
    }

    arg_count = 1;		//Resets count

				//Tokenizes second input
    cin2_strtok[0] = strtok(input2, " ");

    while((cin2_strtok[arg_count] = strtok(NULL, " ")) != 0){
      arg_count++;		//Number of arguments
    }

    arg_count = 1;		//Resets count

//////////////////////////////////////////////////////////////////////

    pid = pipe(pipefd);		//Creates pipe

    if(pid != 0){		//Pipe error check
      perror("Error. Could not create pipe");
      exit(-1);
    }

//////////////////////////////////////////////////////////////////////

    fid = fork();		//Creates fork

    if(fid == -1){		//Fork error check
      perror("Error. Could not create fork");
      exit(-1);
    }

//////////////////////////////////////////////////////////////////////

    if(fid == 0){		//Child process
      close(pipefd[0]);
      dup2(pipefd[1], 1);
      close(pipefd[1]);

				//Calls execvp for user input
      pid = execvp(cin1_strtok[0], cin1_strtok);

      if(pid == -1){		//Exec error check
        perror(cin1_strtok[0]);
        exit(-1);
      }

    }

//////////////////////////////////////////////////////////////////////

    else{
      close(pipefd[1]);		//Closes write at end of pipe
      dup2(pipefd[0], 0);	//Changes input from user to read at end of pipe
      close(pipefd[0]);		//Closes read at end of pipe

      wait(&wait_status);	//Waits for child
      wait_exit = WEXITSTATUS(wait_status);

      if(wait_exit == 0){	//Forks again if exec and child were sucessful
        fid = fork();

        if(fid == -1){		//Fork error check
          perror("Error. Could not create fork");
          exit(-1);
        }

        if(fid == 0){		//Child process
          pid = execvp(cin2_strtok[0], cin2_strtok);

          if(pid == -1){	//Exec error check
            perror(cin2_strtok[0]);
            exit(-1);
          }

        }

        else{			//Parent process
          wait(NULL);		//Waits for child
        }

      }

    }

//////////////////////////////////////////////////////////////////////

    dup2(stdin_copy, 0);	//Changes input from read to user

				//Loops prompt for the first command
    cout << "Enter command one or type \"quit\" to quit: ";
  }

  return 0;
}
