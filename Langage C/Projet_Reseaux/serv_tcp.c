// Serveur TCP - Gestion de comptes bancaires avec SOLDE et OPERATIONS (Version Threads)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_OPERATIONS 10

typedef struct {
    char type[10];       // AJOUT ou RETRAIT
    char date[20];       // Date de l'opération
    double montant;      // Montant de l'opération
} Operation;

typedef struct {
    char id_compte[20];
    char password[20];
    double solde;
    Operation operations[MAX_OPERATIONS];
    int nb_operations;
} Compte;

Compte comptes[] = {
    {"001", "pass12345", 0.0, {}, 0},
    {"002", "pass67890", 0.0, {}, 0}
};

// Obtenir la date actuelle
void get_current_date(char* buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", t);
}

// Enregistrer une opération
void enregistrer_operation(Compte* compte, const char* type, double montant) {
    if (compte->nb_operations >= MAX_OPERATIONS) {
        for (int i = 1; i < MAX_OPERATIONS; i++) {
            compte->operations[i - 1] = compte->operations[i];
        }
        compte->nb_operations--;
    }

    Operation* op = &compte->operations[compte->nb_operations];
    strcpy(op->type, type);
    get_current_date(op->date, sizeof(op->date));
    op->montant = montant;
    compte->nb_operations++;
}

// Vérification des identifiants
int verifier_identifiants(const char* id_compte, const char* password) {
    for (int i = 0; i < 2; i++) {
        if (strcmp(comptes[i].id_compte, id_compte) == 0) {
            if (strcmp(comptes[i].password, password) == 0) {
                return i;
            } else {
                return -2;
            }
        }
    }
    return -1;
}

// Gestion des commandes du client
void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    free(arg);
    char buffer[BUFFER_SIZE];
    int read_size;

    while ((read_size = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[read_size] = '\0';
        char command[20], id_client[20], id_compte[20], password[20];
        double amount = 0;

        sscanf(buffer, "%s %s %s %s %lf", command, id_client, id_compte, password, &amount);

        int compte_index = verifier_identifiants(id_compte, password);

        if (strcmp(command, "SOLDE") == 0) {
            if (compte_index >= 0) {
                char date[20];
                get_current_date(date, sizeof(date));
                sprintf(buffer, "RES_SOLDE %.2f %s\n", comptes[compte_index].solde, date);
                send(client_socket, buffer, strlen(buffer), 0);
            } else {
                send(client_socket, "KO\n", 3, 0);
            }

        } else if (strcmp(command, "AJOUT") == 0) {
            if (compte_index >= 0) {
                comptes[compte_index].solde += amount;
                enregistrer_operation(&comptes[compte_index], "AJOUT", amount);
                send(client_socket, "OK\n", 3, 0);
            } else {
                send(client_socket, "KO\n", 3, 0);
            }

        } else if (strcmp(command, "RETRAIT") == 0) {
            if (compte_index >= 0) {
                if (comptes[compte_index].solde >= amount) {
                    comptes[compte_index].solde -= amount;
                    enregistrer_operation(&comptes[compte_index], "RETRAIT", amount);
                    send(client_socket, "OK\n", 3, 0);
                } else {
                    send(client_socket, "KO\n", 3, 0);
                }
            } else {
                send(client_socket, "KO\n", 3, 0);
            }

        } else if (strcmp(command, "OPERATIONS") == 0) {
            if (compte_index >= 0) {
                char operations_buffer[BUFFER_SIZE] = "RES_OPERATIONS\n";
                for (int i = 0; i < comptes[compte_index].nb_operations; i++) {
                    char op_info[100];
                    sprintf(op_info, "%s %s %.2f\n",
                            comptes[compte_index].operations[i].type,
                            comptes[compte_index].operations[i].date,
                            comptes[compte_index].operations[i].montant);
                    strcat(operations_buffer, op_info);
                }
                send(client_socket, operations_buffer, strlen(operations_buffer), 0);
            } else {
                send(client_socket, "KO\n", 3, 0);
            }

        } else {
            send(client_socket, "Commande invalide\n", 18, 0);
        }
    }

    close(client_socket);
    pthread_exit(NULL);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Erreur de création du socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erreur de bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) < 0) {
        perror("Erreur de listen");
        exit(EXIT_FAILURE);
    }

    printf("Serveur en attente de connexions...\n");

    while ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len))) {
        int* new_sock = malloc(sizeof(int));
        *new_sock = client_socket;

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void*)new_sock) != 0) {
            perror("Erreur lors de la création du thread");
            free(new_sock);
        } else {
            pthread_detach(thread_id);
        }
    }

    close(server_socket);
    return 0;
}
