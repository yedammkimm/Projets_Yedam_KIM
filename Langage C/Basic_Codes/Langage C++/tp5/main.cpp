#include "Utilisateur.h"
#include <iostream>

int main() {
    // Création d'utilisateurs
    Utilisateur Yedam("utilisateur1", "Yedam");
    Utilisateur Aymane("utilisateur2", "Aymane");
    
    // Test des abonnements
    Yedam.abonner("utilisateur2");
    std::cout << "Yedam est abonnée à Aymane: " << (Yedam.estAbonneA("utilisateur2") ? "Oui" : "Non") << std::endl;
    
    Yedam.desabonner("utilisateur2");
    std::cout << "Yedam est abonnée à Aymane après désabonnement: " << (Yedam.estAbonneA("utilisateur2") ? "Oui" : "Non") << std::endl;
    
    // Test de création de contenu
    Yedam.creerPublication("Bonjour CHILL GUY");
    Yedam.creerMedia("Chill Guy", "https://journalducoin.com/app/uploads/2024/11/chill-guy-V4.jpg", "image");
    
    // Affichage du profil
    std::cout << "\nProfil d'Yedam:" << std::endl;
    Yedam.afficherProfil();
    
    // Interaction avec les contenus créés
    Contenu* publication = Yedam.getContenus()[0];
    Contenu* media = Yedam.getContenus()[1];
    
    // Aymane aime et partage le contenu d'Yedam
    Publication* pub = dynamic_cast<Publication*>(publication);
    if (pub) {
        pub->aimer("utilisateur2");
        pub->ajouterCommentaire("Super post !");
    }
    
    Media* med = dynamic_cast<Media*>(media);
    if (med) {
        med->aimer("utilisateur2");
        med->partager("utilisateur2");
    }
    
    // Affichage des contenus après interaction
    std::cout << "\nContenus d'Yedam après interaction:" << std::endl;
    for (Contenu* contenu : Yedam.getContenus()) {
        contenu->afficher();
    }
    
    return 0;
}