#include <signal.h>
#include <stdio.h>

void
termination_handler (int signum)
{
  struct temp_file *p;
  FILE* f=fopen("log.txt","a");
  fprintf(f, "SIGINT:%d\t SIGTSTP:%d\t SIGTERM:%d\n", SIGINT, SIGTSTP,SIGTERM);
  fprintf(f,"In termination handler Signal:%d\n", signum);
  fclose(f);
}

int
main (void)
{
  printf("Begin of program\n");
  struct sigaction new_action, old_action;

  /* Set up the structure to specify the new action. */
  new_action.sa_handler = termination_handler;
  sigemptyset (&new_action.sa_mask);
  new_action.sa_flags = 0;

  sigaction (SIGINT, &new_action, &old_action);  // Ctrl+C for generating the signal
  sigaction (SIGTSTP, &new_action, &old_action); // Ctrl+Z for generating the signal
  sigaction (SIGTERM, &new_action, &old_action); // kill -15 pid for generating the signal 
 while (1) {}
}