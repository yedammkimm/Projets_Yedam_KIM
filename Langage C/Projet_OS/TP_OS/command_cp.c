#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void my_cp(const char *src, const char *dest)
{
    int fd_src, fd_dest;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    // Ouvrir le fichier source en lecture seule
    fd_src = open(src, O_RDONLY);
    if (fd_src < 0) {
        perror("Erreur ouverture source");
        exit(EXIT_FAILURE);
    }

    // Ouvrir le fichier destination en écriture (création si inexistant, écrasement sinon)
    fd_dest = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_dest < 0) {
        perror("Erreur ouverture destination");
        close(fd_src);
        exit(EXIT_FAILURE);
    }

    // Lire le fichier source et écrire dans le fichier destination
    while ((bytes_read = read(fd_src, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(fd_dest, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Erreur écriture");
            close(fd_src);
            close(fd_dest);
            exit(EXIT_FAILURE);
        }
    }

    if (bytes_read < 0) {
        perror("Erreur lecture");
    }

    // Fermer les fichiers
    close(fd_src);
    close(fd_dest);
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        return EXIT_FAILURE;
    }

    my_cp(argv[1], argv[2]);
    printf("Copie terminée : %s -> %s\n", argv[1], argv[2]);

    return EXIT_SUCCESS;
}
