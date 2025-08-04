// Client TCP - Gestion de comptes bancaires
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int read_size;

    // Création du socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Serveur local

    // Connexion au serveur
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erreur de connexion");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Connecté au serveur. Entrez vos commandes :\n");
    printf("Exemples :\n");
    printf("  AJOUT <id_client> <id_compte> <password> <montant>\n");
    printf("  RETRAIT <id_client> <id_compte> <password> <montant>\n");
    printf("  SOLDE <id_client> <id_compte> <password>\n");
    printf("  OPERATIONS <id_client> <id_compte> <password>\n");
    printf("  EXIT pour quitter.\n");

    while (1) {
        printf("> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;  // Supprimer le saut de ligne

        // Quitter le programme
        if (strcmp(buffer, "EXIT") == 0) {
            break;
        }

        // Envoyer la commande au serveur
        if (send(sock, buffer, strlen(buffer), 0) < 0) {
            perror("Erreur lors de l'envoi");
            break;
        }

        // Recevoir la réponse du serveur
        read_size = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (read_size > 0) {
            buffer[read_size] = '\0';

            // Vérifier et afficher les résultats
            if (strncmp(buffer, "RES_SOLDE", 9) == 0) {
                printf("➡️  Résultat : %s\n", buffer);
            } else if (strncmp(buffer, "RES_OPERATIONS", 14) == 0) {
                printf("📋 Historique des opérations :\n%s\n", buffer);
            } else if (strncmp(buffer, "OK", 2) == 0) {
                printf("✅ Succès : %s\n", buffer);
            } else if (strncmp(buffer, "KO", 2) == 0) {
                printf("❌ Échec : %s\n", buffer);
            } else if (strncmp(buffer, "Commande invalide", 17) == 0) {
                printf("⚠️ Erreur : %s\n", buffer);
            } else {
                printf("🔎 Réponse : %s\n", buffer);
            }
        } else if (read_size == 0) {
            printf("⚠️  Le serveur a fermé la connexion.\n");
            break;
        } else {
            perror("Erreur de réception");
            break;
        }
    }

    // Fermeture du socket
    close(sock);
    printf("Connexion fermée.\n");
    return 0;
}
