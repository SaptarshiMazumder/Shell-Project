#include <sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

char *builtin_str[] = {
  "dc",
  "help",
  "exit",
  "cw",
  "ip",
  "repl",
  "hex"
};




int func_dc(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "func: expected argument to \"dc\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("func");
    }
  }
  return 1;
}

int func_hex(char **args)
{
    unsigned char str[100],strH[200];
    int i,j;
     
    printf("Enter string: ");
    scanf("%[^\n]s",str);
    
    memset(strH,0,sizeof(strH));
     
    
    for(i=0,j=0;i<strlen(str);i++,j+=2)
    { 
        sprintf((char*)strH+j,"%02X",str[i]);
    }
    strH[j]='\0'; 
     
    printf("Hexadecimal converted string is: \n");
    printf("%s\n",strH);
     
     
    return 1;
}

#define MAX 256
int func_repl(char **args)
{
    FILE *fptr1, *fptr2;
        int lno, linectr = 0;
        char str[MAX],fname[MAX];        
        char newln[MAX], temp[] = "temp.txt";

printf("\n\n Replace a specific line in a text file with a new text :\n");
// printf("-------------------------------------------------------------\n");
printf(" Input the file name to be opened : ");
        fgets(fname, MAX, stdin);
        fname[strlen(fname) - 1] = '\0';
        fptr1 = fopen(fname, "r");
        if (!fptr1)
        {
                printf("Unable to open the input file!!\n");
                return 0;
        }
        fptr2 = fopen(temp, "w");
        if (!fptr2)
        {
                printf("Unable to open a temporary file to write!!\n");
                fclose(fptr1);
                return 0;
        }
        /* get the new line from the user */
        printf(" Input the content of the new line : ");
        fgets(newln, MAX, stdin);
        /* get the line number to delete the specific line */
        printf(" Input the line no you want to replace : ");
        scanf("%d", &lno);
        lno++;
         // copy all contents to the temporary file other except specific line
        while (!feof(fptr1))
        {
            strcpy(str, "\0");
            fgets(str, MAX, fptr1);
            if (!feof(fptr1))
            {
                linectr++;
                if (linectr != lno)
                    {
                        fprintf(fptr2, "%s", str);
                    }
                    else
                    {
                        fprintf(fptr2, "%s", newln);
                    }
                }
        }
        fclose(fptr1);
        fclose(fptr2);
        remove(fname);
        rename(temp, fname);
        printf(" Replacement did successfully..!! \n");
        return 1;
  }
    



int func_IP(char **args)
{
  struct addrinfo* res;
  char* hostname;
  char* hostaddr;
  struct sockaddr_in* saddr;
 
  // if (argc != 2) {
  //   perror("Usage: hostnamelookup <hostname>\n");
  //   exit(1);
  // }

  hostname = args[1];
 
  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) {
    fprintf(stderr, "Error in resolving hostname %s\n", hostname);
    exit(1);
  }
 
  saddr = (struct sockaddr_in*)res->ai_addr;
  hostaddr = inet_ntoa(saddr->sin_addr);

  printf("Address for %s is %s\n", hostname, hostaddr);
  exit(0);
}



int func_cw(char **args)
{
           int character_count = 0;
           int space_count = 0;
           int word_count = 0;
           int line_count = 0;
           char ch;

           FILE *fp;

         
           fp = fopen(args[1],"r");

           while((ch=fgetc(fp))!=EOF)
           {
                       character_count++;
                       if(ch == ' ')
                       {
                             space_count++;
                             word_count++;
                       }
                       if(ch == '\n')
                       {
line_count++;
word_count++;
                       }
     
           }
           printf("character_count = %d\n",character_count);
           printf("space_count = %d\n",space_count);
           printf("word_count = %d\n",word_count);
           printf("line_count = %d\n",line_count);
           int file_pointer; 
           char *buff[BUFFER_SIZE];
           int op;

           file_pointer = open(args[1], O_RDONLY);

           while (op=read(file_pointer, buff, BUFFER_SIZE)){
        write(1, buff, op);
      }
   
      close(file_pointer);

           return 1;
}
int func_help(char **args)
{
  int i;
  printf("My Custom Shell\n");
  printf("----------------------------------------------------\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are my custom commands:\n");

  for (i = 0; i < 7*sizeof(char); i++) {
    
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}


int func_exit(char **args)
{
  return 0;
}


int func_launch(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    
    if (execvp(args[0], args) == -1) {
      perror("func");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    
    perror("func");
  } else {
    
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}


int func_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    
    return 1;
  }

  for (i = 0; i < 7*sizeof(char); i++) {
    if (strcmp(args[0], "help") == 0) {
      return (func_help(args));
    }
    else if (strcmp(args[0], "dc") == 0) {
      
      return (func_dc(args));
    }
    else if (strcmp(args[0], "exit") == 0) {
      
      return 0;
    }
    else if (strcmp(args[0], "cw") == 0) {
      
      return (func_cw(args));
    }
    else if (strcmp(args[0], "ip") == 0) {
      
      return (func_IP(args));
    }
    else if (strcmp(args[0], "repl") == 0) {
      
      return (func_repl(args));
    }
    else if (strcmp(args[0], "hex") == 0) {
      
      return (func_hex(args));
    }
  }

  return func_launch(args);
}

#define func_RL_BUFSIZE 1024

char *func_read_line(void)
{
  int bufsize = func_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "func: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    
    c = getchar();

    
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

   
    if (position >= bufsize) {
      bufsize += func_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "func: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

#define func_TOK_BUFSIZE 64
#define func_TOK_DELIM " \t\r\n\a"

char **func_split_line(char *line)
{
  int bufsize = func_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "func: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, func_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += func_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "func: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, func_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}


void func_loop(void)
{
  char *line;
  char **args;
  int status;

  do {
    printf("--> ");
    line = func_read_line();
    args = func_split_line(line);
    status = func_execute(args);

    free(line);
    free(args);
  } while (status);
}


int main(int argc, char **argv)
{
  
  func_loop();

  

  return EXIT_SUCCESS;
} 