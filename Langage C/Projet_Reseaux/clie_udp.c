// Client UDP - Gestion de comptes bancaires
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    socklen_t addr_len = sizeof(server_addr);
    int read_size;

    // Création du socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Serveur local

    printf("Connecté au serveur UDP. Entrez vos commandes :\n");
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
        if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, addr_len) < 0) {
            perror("Erreur lors de l'envoi");
            break;
        }

        // Recevoir la réponse du serveur
        read_size = recvfrom(sock, response, BUFFER_SIZE - 1, 0, NULL, NULL);
        if (read_size > 0) {
            response[read_size] = '\0';

            // Vérifier et afficher les résultats
            if (strncmp(response, "RES_SOLDE", 9) == 0) {
                printf("➡️  Résultat : %s\n", response);
            } else if (strncmp(response, "RES_OPERATIONS", 14) == 0) {
                printf("📋 Historique des opérations :\n%s\n", response);
            } else if (strncmp(response, "OK", 2) == 0) {
                printf("✅ Succès : %s\n", response);
            } else if (strncmp(response, "KO", 2) == 0) {
                printf("❌ Échec : %s\n", response);
            } else if (strncmp(response, "Commande invalide", 17) == 0) {
                printf("⚠️ Erreur : %s\n", response);
            } else {
                printf("🔎 Réponse : %s\n", response);
            }
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
