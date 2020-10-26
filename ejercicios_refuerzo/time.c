#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>
int main(int argc, char *argv[]) {
	  int fd[2];
	  char buffer[4];
	  double tconsumed;
	  struct timeval inicial;
	  struct timeval final;

	  srand(time(NULL));

	  pipe(fd);
	  int rc = fork();

	  if(!argv[1]){
	      fprintf(stderr, "missing command to execute\n");
	  }

	  if (rc < 0) {
	    // fork failed; exit
	    fprintf(stderr, "fork failed\n");
	    exit(1);
	  } else if (rc == 0) {
	    // child (new process)
	    close(fd[0]);
	    gettimeofday(&inicial, NULL); 
	    write(fd[1], &inicial, sizeof(inicial));
	    close(fd[1]);
	    execvp(argv[1], NULL);  
	    printf("this shouldn't print out");
	  } else {
	    // parent goes down this path (original process)
	    close(fd[0]);    
	    int wc = wait(NULL);
	    gettimeofday(&final, NULL);
	    inicial = read(fd[0], buffer, sizeof(buffer));
	    tconsumed = final.tv_usec - inicial.tv_usec;
	    printf("%f\n", tconsumed);
	  }
	  return 0;
	}