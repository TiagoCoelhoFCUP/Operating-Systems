/////////////////////////////////////////////////////////
//                                                     //
//               Trabalho I: Mini-Shell                //
//                                                     //
// Compilação: gcc my_prompt.c -lreadline -o my_prompt //
// Utilização: ./my_prompt                             //
//                                                     //
/////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <errno.h>

#define MAXARGS 100
#define PIPE_READ 0
#define PIPE_WRITE 1

typedef struct command {
    char *cmd;              // string apenas com o comando
    int argc;               // número de argumentos
    char *argv[MAXARGS+1];  // vector de argumentos do comando
    struct command *next;   // apontador para o próximo comando
} COMMAND;

// variáveis globais
char* inputfile = NULL;	    // nome de ficheiro (em caso de redireccionamento da entrada padrão)
char* outputfile = NULL;    // nome de ficheiro (em caso de redireccionamento da saída padrão)
int background_exec = 0;    // indicação de execução concorrente com a mini-shell (0/1)

// declaração de funções
COMMAND* parse(char *);
void print_parse(COMMAND *);
void execute_commands(COMMAND *);
void free_commlist(COMMAND *);

// include do código do parser da linha de comandos
#include "parser.c"

int main(int argc, char* argv[]) {
  char *linha;
  COMMAND *com;

  while (1) {
    if ((linha = readline("my_prompt$ ")) == NULL)
      exit(0);
    if (strlen(linha) != 0) {
      add_history(linha);
      com = parse(linha);
      if (com) {
	print_parse(com);
	execute_commands(com);
	free_commlist(com);
      }
    }
    free(linha);
  }
}


void print_parse(COMMAND* commlist) {
  int n, i;

  printf("---------------------------------------------------------\n");
  printf("BG: %d IN: %s OUT: %s\n", background_exec, inputfile, outputfile);
  n = 1;
  while (commlist != NULL) {
    printf("#%d: cmd '%s' argc '%d' argv[] '", n, commlist->cmd, commlist->argc);
    i = 0;
    while (commlist->argv[i] != NULL) {
      printf("%s,", commlist->argv[i]);
      i++;
    }
    printf("%s'\n", commlist->argv[i]);
    commlist = commlist->next;
    n++;
  }
  printf("---------------------------------------------------------\n");
}


void free_commlist(COMMAND *commlist){
  do{ 
    COMMAND* next_aux = commlist->next;
    free(commlist); 
    commlist = next_aux;
  }while(commlist!=NULL);
}


void execute_commands(COMMAND *commlist) {
  pid_t pid,pid2;
  int fd[2];
  if (strcmp(commlist->cmd, "filtro") == 0){
    if((pid = fork()) < 0){
      printf("Error : %s\n",strerror(errno));
      exit(errno);
    }
    
    if(pid==0){
      if(pipe(fd) < 0){
	printf("Error : %s\n", strerror(errno));
	exit(errno);
      }
      if((pid2 = fork()) < 0){
	printf("Error : %s\n",strerror(errno));
	exit(errno);
      }
      if(pid2>0){
	close(fd[0]);
	int fp = open(commlist->argv[1], O_RDONLY);
	if(fp<0){
	  printf("Erro: %s\n",strerror(errno));
	  exit(errno);
	}
	dup2(fp,STDIN_FILENO);
	close(fp);
	dup2(fd[1],STDOUT_FILENO);
	close(fd[1]);
	if(execlp("cat","cat",NULL) < 0){
	  printf("Erro: %s\n",strerror(errno));
	  exit(errno);
	}
      }
      else{
	close(fd[1]);
	int fp = open(commlist->argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if(fp<0){
	  printf("Erro: %s\n",strerror(errno));
	  exit(errno);
	}
	dup2(fp,STDOUT_FILENO);
	close(fp);
	dup2(fd[0],STDIN_FILENO);
	close(fd[0]);
	if(execlp("grep","grep",commlist->argv[3],NULL) < 0){
	  printf("Erro: %s\n",strerror(errno));
	  exit(errno);
	}
      }
    }
    else
      wait(NULL);
  }


  else{
    if((pid = fork()) < 0){
      printf("Error : %s\n", strerror(errno));
      exit(errno);
    }
    if(pid==0){
      if(inputfile!=NULL){
	int fp = open(inputfile, O_RDONLY);
	if(fp<0){
	  printf("Erro: %s\n",strerror(errno));
	  exit(errno);
	}
	dup2(fp,STDIN_FILENO);
	close(fp);
      }
      if(outputfile!=NULL){
	int fp = open(outputfile, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if(fp<0){
	  printf("Erro: %s\n",strerror(errno));
	  exit(errno);
	}
	dup2(fp,STDOUT_FILENO);
	close(fp);
      }
      if(execvp(commlist->cmd,commlist->argv)<0){
	printf("Error : %s\n", strerror(errno));
	exit(errno);
      }
    }
    else{
      wait(NULL);
    }
  }
}
    
    
    

