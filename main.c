#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGS 10
#define TRUE 1

int main() {
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGS];
    char *token;
    char cwd[1024];

    while (TRUE) {
        getcwd(cwd, sizeof(cwd));
        printf("\n%s$ ", cwd);
        fflush(stdout);

        // Ler o comando digitado pelo usuário
        fgets(command, MAX_COMMAND_LENGTH, stdin);

        // Remover a quebra de linha no final
        command[strcspn(command, "\n")] = 0;

        // Tokenizar o comando em argumentos separados
        int arg_count = 0;
        token = strtok(command, " ");
        while (token != NULL && arg_count < MAX_ARGS - 1) {
            args[arg_count] = token;
            arg_count++;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL; // Adicionar NULL no final do array de argumentos

        if (strcmp(args[0], "exit") == 0) {
          break;
        } else {
            if (strcmp(args[0], "cd") == 0) {
                if (arg_count == 1) {
                    fprintf(stderr, "Uso: cd <directory>\n");
                } else if (chdir(args[1]) != 0) {
                    perror("Erro ao alterar diretório");     
                }
            } else {
                pid_t pid = fork();

                if (pid < 0) {
                    perror("Erro ao criar processo filho");
                } else if (pid == 0) { // processo filho
                    if (execvp(args[0], args) == -1) {
                        perror("Erro ao executar comando");
                    }
                    exit(EXIT_FAILURE); // Em caso de falha na execução do comando
                } else { // processo pai
                    int status;
                    waitpid(pid, &status, 0);
                }
            }
        }
    }

    return 0;
}