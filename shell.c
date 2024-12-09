#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <signal.h>
#include <pwd.h>

const char *homedir;

// ALL OUR FUNCTIONALITY IS IN THIS FUNCTION - EVERYTHING
int main(int argc, char *argv[]){
  int homedir_tier, curr_tier, printfordashes = 1, executedcommand = 1;
  homedir = getpwuid(getuid())->pw_dir;
  //Get level of directory
  for (homedir_tier=0; homedir[homedir_tier]; homedir[homedir_tier]=='/' ? homedir_tier++ : *homedir++);
  //printf("%d\n", dir_tier);
  //printf("%s\n", homedir);
  char tracker[1024];
  char loc[1024];
  char *curr = loc;
  char *token;
  getcwd(loc, 1024);
  char tempstr[1024];
  strcpy(tempstr, loc);
  char *tempcurr = tempstr;
  for (curr_tier=0; tempcurr[curr_tier]; tempcurr[curr_tier]=='/' ? curr_tier++ : *tempcurr++);
  // printf("%d\n",curr_tier);
if(curr_tier >  homedir_tier){
	printf("~");
    for(int i = 0; i < homedir_tier + 1; i++){
  	 token = strsep(&curr, "/");
   }
 }
   else if (curr_tier ==  homedir_tier){
	printf("~");
	printfordashes = 0;
   }

  getcwd(tracker, 1024);
  if(printfordashes)
	printf("/%s $ ", curr);
  printfordashes = 1;
  fflush(stdout);
  char line_buff[256];
  while(fgets(line_buff,255,stdin) != NULL){
    char *nl = strchr(line_buff, '\n');
	if(nl){
		*nl = '\0';
	}
    char *cr = strchr(line_buff, '\r');
    if (cr) {
        *cr = '\0';
    }
    char a[256];
    strcpy(a, line_buff);
    char *reala = a;
    char *token;

    while((token = strsep(&reala,";")) != NULL){
      if (strcmp(token, "exit") == 0){
        exit(0);
      }
	  char *args[256];
      char *args1[256];
      char *args2[256];
	  int count = 0;
      int count1=0;
      int count2=0;
      int which=1;
      char *holder;
      char *redir="";
      while((holder = strsep(&token," "))!= NULL){
        if(strcmp(holder, ">") == 0 || strcmp(holder, ">>") == 0 || strcmp(holder, "<") == 0 || strcmp(holder, "|") == 0){
          which = 2;
          redir = holder;
        }
        else{
          if(which == 1)
            args1[count1++] = holder;
          else
            args2[count2++] = holder;
        }
    	 args[count] = holder;
    	 count++;
      }
	  args[count] = NULL;
      args1[count1] = NULL;
      args2[count2] = NULL;
      pid_t p;
      p = fork();
      if(p < 0){
        printf("%s\n",strerror(errno));
      }
      else if(p == 0){
	if(strcmp(args[0], "cd") == 0){
        executedcommand = 0;
		if(strstr(args[1], "~") != NULL){
			char *home_dir = getenv("HOME");
			chdir(home_dir);
			getcwd(tracker, 1024);
			//printf("tracker: %s\n", tracker);
			curr_tier = homedir_tier;
			if(strcmp(args[1], "~") != 0){
				char evenmoreargs[1024];
				strcpy(evenmoreargs, args[1]);
				char realargs[1024];
				for(int i = 1; i < strlen(evenmoreargs); i++){
					realargs[i - 1] = evenmoreargs[i];
				}
				realargs[strlen(evenmoreargs ) - 1] = '\0';
				strcpy(args[1], realargs);
				//printf("args1: %s\n", args[1]);
				//chdir(args[1]);
				//curr_tier
			}
		}
        // printf("%s\n", args[1]);
        char temp[1024];
        strcpy(temp, tracker);
        strcat(temp, "/");
        strcat(temp, args[1]);
        if(strcmp(args[1], "~") == 0 || chdir(temp) == 0){
          getcwd(tracker, 1024);
          // printf("CHANGE SUCCESS: %s\n", tracker);
          char prompter[1024];
          strcpy(prompter, tracker);
          curr = prompter;
		char stringtemp[1024];
		strcpy(stringtemp, tracker);
		tempcurr = stringtemp;
          // curr contains new pwd printf("%s\n", curr);
		  if(strcmp(args[1], "~") != 0){
			for (curr_tier=0; tempcurr[curr_tier]; tempcurr[curr_tier]=='/' ? curr_tier++ : *tempcurr++); //change tempcurr xd
		  }
          if(curr_tier > homedir_tier){
         		printf("~");
			for(int i = 0; i < homedir_tier + 1; i++){
				token = strsep(&curr, "/");
			}
		  }
	   else if (curr_tier ==  homedir_tier){
		printfordashes = 0;
        }else{
			printfordashes = 0;
		}
        }
        else{
          // printf("%s\n", tracker);
          printf("Invalid path.\n");
		  executedcommand = 1;
		}
        // chdir()
		}
        else{
          if(strcmp(redir, ">") == 0){
            int fd1 = open(args2[0], O_WRONLY | O_TRUNC | O_CREAT, 0777);
            int backup_stdout = dup( STDOUT_FILENO );
            dup2(fd1, STDOUT_FILENO);
            execvp(args1[0], args1);
            fflush(stdout);
            close(fd1);
            exit(0);
          }
          if(strcmp(redir, ">>") == 0){
            int fd1 = open(args2[0], O_WRONLY | O_APPEND);
            int backup_stdout = dup( STDOUT_FILENO );
            dup2(fd1, STDOUT_FILENO);
            execvp(args1[0], args1);
            fflush(stdout);
            close(fd1);
            exit(0);
          }
          if(strcmp(redir, "<") == 0){
            int fd1 = open(args2[0], O_RDONLY);
            int backup_stdin = dup( STDIN_FILENO );
            dup2(fd1, STDIN_FILENO);
            execvp(args1[0], args1);
            fflush(stdout);
            exit(0);
          }
          if(strcmp(redir, "|") == 0){
            pid_t p2;
            p2 = fork();
            if(p2 == 0){
              int fd1 = open("temp", O_WRONLY | O_TRUNC | O_CREAT, 0777);;
              int backup_stdout = dup( STDOUT_FILENO );
              dup2(fd1, STDOUT_FILENO);
              execvp(args1[0], args1);
              fflush(stdout);
              close(fd1);
              exit(0);
            }
            int status;
            wait(&status);
            int fd1 = open("temp", O_RDONLY);
            int backup_stdin = dup( STDIN_FILENO );
            dup2(fd1, STDIN_FILENO);
            execvp(args2[0], args2);
            fflush(stdout);
            close(fd1);
            unlink("temp");
            exit(0);
          }
          execvp(args1[0], args1);
          exit(0);
        }
      }
      int status;
      wait(&status);
    }
  if(executedcommand){
   if(curr_tier == homedir_tier){
		printf("~$ ");
	  }
	else if(homedir_tier > curr_tier){
		printf("%s $ ", curr);
	}
	  else{
		printf("~/%s $ ", curr);
	  }
  }
  else if(printfordashes){
	if(curr_tier == homedir_tier){
		printf("~$ ");
	}
	else if(homedir_tier > curr_tier){
		printf("%s $ ", curr);
	}
	else{
		printf("/%s $ ", curr);
	}
	}

  else if(curr_tier == homedir_tier){
		printf("~$ ");
	}
	else{
		printf("%s $ ",  curr);
	}
	printfordashes = 1;
  if(homedir_tier < curr_tier){
    executedcommand = 1;
  }
    fflush(stdout);
  }
  exit(0);
  /*
 if(curr_tier >  dir_tier){
		printf("~");
 }
  for(int i = 0; i < dir_tier + 1; i++){
	 token = strsep(&curr, "/");
 }
  getcwd(tracker, 1024);

  printf("/%s/ $ ", curr);
  fflush(stdout);*/

}
