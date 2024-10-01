#include <_stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_ssize_t.h>
#include <sys/syslimits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int bonk_cd(char **args);
int bonk();
int bonk_exit(char **args);
char *builtin_str[] = {"cd", "help", "exit"};
char cwd[PATH_MAX];

int (*builtin_func[])(char **) = {&bonk_cd, &bonk, &bonk_exit};

int bonk_num_builtins() { return sizeof(builtin_str) / sizeof(char *); }

int bonk_cd(char **args) {
  if (args[1] == NULL) {
    bonk();
    printf("Error: No directory inputted");
  } else {
    if (chdir(args[1]) != 0)
      perror("bonk");
  }
  return 1;
}

int getThisDir() {
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    return EXIT_SUCCESS;
  }
  return 0;
}

int bonk_exit(char **args) { return 0; }

int bonk() {
  system("~/coding/justcoding/clang/bonk/bonk.sh");
  return 1;
}


#define BONKSH_RL_BUFSIZE 1024
char *bonksh_read_line(void) {
  int bufsize = BONKSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "bonksh: Allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // read character
    c = getchar();

    // when we reach the EOF, replace it with a null character and return
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // this is the part where we have to continue reallocating because we can't
    // tell how many characters a user has inputted
    if (position > bufsize) {
      bufsize += BONKSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "bonksh: allocation error \n");
        exit(EXIT_FAILURE);
      }
    }
  }
}


char *bonksh_read_line(void){ 
  char *line = NULL;
  ssize_t bufsize = 0;

  if(getline(&line, &bufsize, stdin) == -1){ 
    if (feof(stdin)) {
      exit(EXIT_SUCCESS);
    }
  }
}


void bonksh_loop(void) {
  char *line;
  char **args;
  int status;

  do {
    getThisDir();
    printf("Bonk! →{%s}", cwd);
    line = bonksh_read_line();
    args = bonksh_split_line(line);
    status = bonksh_execute(args);
    free(line);
    free(args);
  } while (status);
}

int main(int argc, char *argv[]) {
  // if you want to add any configs load them here

  // run command loop
  bonksh_loop();

  // perform any shutdown cleanups if any
  printf("Bye from BONK!!");
  return EXIT_SUCCESS;
}
