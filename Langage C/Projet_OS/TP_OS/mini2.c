#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char **split(char *raw_cmd, char *limit)
{
	char	*ptr = NULL;
	char	**cmd = NULL;
	size_t	idx = 0;

	// split sur les espaces
	ptr = strtok(raw_cmd, limit);

	while (ptr) {
		cmd = (char **)realloc(cmd, ((idx + 1) * sizeof(char *)));
		cmd[idx] = strdup(ptr);
		ptr = strtok(NULL, limit);
		++idx;
	}
	// On alloue un element qu'on met a NULL a la fin du tableau
	cmd = (char **)realloc(cmd, ((idx + 1) * sizeof(char *)));
	cmd[idx] = NULL;
	return (cmd);
}

void free_array(char **array)
{
	for (int i = 0; array[i]; i++) {
		free(array[i]);
		array[i] = NULL;
	}
	free(array);
	array = NULL;
}

void exec_cmd(char **cmd)
{
    pid_t pid = fork(); // Création d'un processus enfant

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        // Processus enfant : exécute la commande
        if (execvp(cmd[0], cmd) == -1) {
            perror("exec failed");
            exit(EXIT_FAILURE);
        }
    }
    else {
        // Processus parent : attendre la fin du processus enfant
        wait(NULL);
    }
}

int	main()
{
	char	*buffer = NULL;
	size_t	buf_size = 2048;
	char **cmd;

	// alloc buffer qui stockera la commande entree par l'user
	buffer = (char *)calloc(sizeof(char), buf_size);
	if (buffer == NULL) {
		perror("Malloc failure");
		return (EXIT_FAILURE);
	}

	// ecriture d'un prompt
	write(1, "$> ", 3);

	// lecture de STDIN en boucle
	while (getline(&buffer, &buf_size, stdin) > 0) {
		printf("buffer = |%s|\n", buffer);

		cmd = split(buffer, " \n\t");
		int i=0;
		while (cmd[i]!=NULL)
		{
			printf("%d:%s\n",i,cmd[i]);
			i++;
		}

		if (cmd[0]!=NULL)
		{
			exec_cmd(cmd);
			free_array(cmd);
		}
		
		write(1, "$> ", 3);
	}

	printf("Bye \n");
	free(buffer);
}
