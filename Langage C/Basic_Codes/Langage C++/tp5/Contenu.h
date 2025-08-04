#ifndef CONTENU_H
#define CONTENU_H

#include <string>
#include <vector>

class Contenu {
protected:
    std::string id;
    std::string idAuteur;
    std::string texte;
    std::vector<std::string> motsCles;

public:
    // Constructeur
    Contenu(const std::string& id, const std::string& idAuteur, const std::string& texte);

    // Getters
    std::string getId() const;
    std::string getIdAuteur() const;
    std::string getTexte() const;

    // Setter
    void setTexte(const std::string& nouveauTexte);

    // Méthode pour ajouter un mot-clé
    void ajouterMotCle(const std::string& motCle);

    // Méthodes virtuelles pures
    virtual void afficher() const = 0;
    virtual std::string getType() const = 0;

    // Destructeur virtuel
    virtual ~Contenu();
};

#endif // CONTENU_H
