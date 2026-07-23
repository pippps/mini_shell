#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"

static char *read_line(void) {
    char *line = NULL;
    size_t size = 0;

    if (getline(&line, &size, stdin) == -1) {
        free(line);
        if (feof(stdin)) {
            exit(0);
        } else {
            perror("getline");
            exit(EXIT_FAILURE);
        }
    }
    return line;
}

static char **split_line(char *line) {
    int bufsize = TOK_BUFSIZE;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOK_DELIM);
    while (token != NULL) {
        tokens[position++] = token;

        if (position >= bufsize) {
            bufsize += TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOK_DELIM);
    }

    tokens[position] = NULL;
    return tokens;
}

static int run_command(char **args) {
    pid_t pid;
    int status;

    if (args[0] == NULL) {
        return 1;
    }

    if (strcmp(args[0], "exit") == 0) {
        return 0;
    }

    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "shell: expected argument to \"cd\"\n");
        } else if (chdir(args[1]) != 0) {
            perror("cd");
        }
        return 1;
    }

    if (strcmp(args[0], "pwd") == 0) {
        char cwd[4096];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("pwd");
        }
        return 1;
    }

    pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fork");
    } else {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int main(void) {
    char *line;
    char **args;
    int status = 1;

    while (status) {
        printf("myshell> ");
        line = read_line();
        args = split_line(line);
        status = run_command(args);

        free(args);
        free(line);
    }

    return 0;
}