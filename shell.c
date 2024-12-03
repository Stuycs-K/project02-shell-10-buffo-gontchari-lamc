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

int main(int argc, char *argv[]){
  char line_buff[256];
  while(fgets(line_buff,255,stdin)){
	char *nl = strchr(line_buff, '\n');
	*nl = '\0';
	char *args[256];
	char a[256];
	strcpy(a, line_buff);
	char *reala = a;
	char *token;
	int count= 0;
	while((token = strsep(&reala," "))!= NULL){
	 args[count] = token;
	 count++;
	}
	args[count] = NULL;
	pid_t p;
	p = fork();
	if(p < 0){
     printf("%s\n",strerror(errno));
    }
    else if(p == 0){
	  execvp(args[0], args);
	  exit(0);
    }
    int status;
    wait(&status);
  }
  exit(0);
 
}
