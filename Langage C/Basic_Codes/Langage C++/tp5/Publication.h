#ifndef PUBLICATION_H
#define PUBLICATION_H

#include "Contenu.h"
#include "IAimable.h"
#include <vector>
#include <string>

class Publication : public Contenu, public IAimable {
private:
    std::vector<std::string> utilisateursQuiAiment;
    std::vector<std::string> commentaires;

public:
    // Constructeur
    Publication(const std::string& id, const std::string& idAuteur, const std::string& texte);
    
    // Implémentation des méthodes de IAimable
    void aimer(const std::string& idUtilisateur) override;
    void retirerAime(const std::string& idUtilisateur) override;
    int getNombreAimes() const override;
    bool estAimePar(const std::string& idUtilisateur) const override;

    // Implémentation des méthodes abstraites de Contenu
    void afficher() const override;
    std::string getType() const override;

    // Gestion des commentaires
    void ajouterCommentaire(const std::string& commentaire);
    std::vector<std::string> getCommentaires() const;
};

#endif // PUBLICATION_H