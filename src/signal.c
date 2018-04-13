#include "libraries.h"
#include "SearchFunctions.h"
static pid_t pai;
void childsig1(int signo)
{
  writeLogFile(getpid(),"SINAL USR1");
}
void childsig2(int signo)
{
  writeLogFile(getpid(),"SINAL USR2");
  exit(0);
}
void ler(int signo)
{
	char c='X';
	if(getpid()==pai)
	{
    writeLogFile(getpid(),"SINAL INT");
		printf("\n\nAre you sure you want to terminate the program? (Y/N)\n");
		while(c!='Y' && c!='y' && c!='N' && c!='n')
		{
			scanf("%c",&c);
		}
		if(c == 'Y' || c=='y')
		{
      signal(SIGQUIT, SIG_IGN);
      kill(pai, SIGQUIT);
      exit(0);
		}
		else
		{
      kill(0,SIGUSR1);
		}
	}
	else
	{
    pause();
	}
}
void processSignal()
{
  pai=getpid();
  struct sigaction action;
  action.sa_flags = 0;
  action.sa_handler = ler;
  if (sigaction(SIGINT,&action,NULL) < 0)
  {
    fprintf(stderr,"Unable to install SIGINT handler\n");
    exit(1);
  }
  action.sa_handler = childsig1;
  if (sigaction(SIGUSR1,&action,NULL) < 0)
  {
    fprintf(stderr,"Unable to install SIGINT handler\n");
    exit(1);
  }
}
void processSignalChild()
{
  struct sigaction action;
  action.sa_flags = 0;
  action.sa_handler = childsig1;
  if (sigaction(SIGUSR1,&action,NULL) < 0)
  {
    fprintf(stderr,"Unable to install SIGINT handler\n");
    exit(1);
  }
  action.sa_handler = childsig2;
  if (sigaction(SIGUSR2,&action,NULL) < 0)
  {
    fprintf(stderr,"Unable to install SIGINT handler\n");
    exit(1);
  }
}
