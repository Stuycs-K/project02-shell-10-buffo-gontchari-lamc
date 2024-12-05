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

int main(int argc, char *argv[]){
  int dir_tier, curr_tier;
  homedir = getpwuid(getuid())->pw_dir;
  //Get level of directory
  for (dir_tier=0; homedir[dir_tier]; homedir[dir_tier]=='/' ? dir_tier++ : *homedir++);
  //printf("%d\n", dir_tier);
  //printf("%s\n", homedir);
  char line_buff[256];
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
 if(curr_tier >  dir_tier){
		printf("~");
 }
  for(int i = 0; i < dir_tier + 1; i++){
	 token = strsep(&curr, "/");
 }
  getcwd(tracker, 1024);

  printf("/%s/ $ ", curr);
  fflush(stdout);

  while(fgets(line_buff,255,stdin) != NULL){
  	char *nl = strchr(line_buff, '\n');
  	*nl = '\0';
  	char *args[256];
  	char a[256];
  	strcpy(a, line_buff);
  	char *reala = a;
  	char *token;

    while((token = strsep(&reala,";")) != NULL){
      int count= 0;
      char *holder;
    	while((holder = strsep(&token," "))!= NULL){
    	 args[count] = holder;
    	 count++;
    	}
    	args[count] = NULL;
    	pid_t p;
    	p = fork();
    	if(p < 0){
         printf("%s\n",strerror(errno));
      }
      else if(p == 0){
        if(strcmp(args[0], "cd") == 0){
          // printf("%s\n", args[1]);
          char temp[1024];
          strcpy(temp, tracker);
          strcat(temp, "/");
          strcat(temp, args[1]);
          // printf("%s\n", tracker);
          if(chdir(temp) == 0){
            getcwd(tracker, 1024);
            // printf("CHANGE SUCCESS: %s\n", tracker);
            char prompter[1024];
            strcpy(prompter, tracker);
            curr = prompter;
            for(int i = 0; i < 6; i++){
              strsep(&curr, "/");
              // printf("TOKEN HERE: %s\n", token);
              // printf("CURR HERE: %s\n", curr);
            }
          }
          else{
            // printf("%s\n", tracker);
            printf("Invalid path.\n");
          }
          // chdir()
        }
        else{
          execvp(args[0], args);
      	  exit(0);
        }

      }
      int status;
      wait(&status);
    }

    // char loc[1024];
    // getcwd(loc, 1024);
    printf("~/%s/ $ ", curr);
    fflush(stdout);
  }
  exit(0);

}
