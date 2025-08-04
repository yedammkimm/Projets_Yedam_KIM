#ifndef MEDIA_H
#define MEDIA_H

#include "Contenu.h"
#include "IAimable.h"
#include "IPartageable.h"
#include <vector>
#include <string>

class Media : public Contenu, public IAimable, public IPartageable {
private:
    std::string url;
    std::string typeMedia;
    std::vector<std::string> utilisateursQuiAiment;
    std::vector<std::string> utilisateursQuiPartagent;

public:
    // Constructeur
    Media(const std::string& id, const std::string& idAuteur, const std::string& texte, 
          const std::string& url, const std::string& typeMedia);
    
    // Implémentation des méthodes de IAimable
    void aimer(const std::string& idUtilisateur) override;
    void retirerAime(const std::string& idUtilisateur) override;
    int getNombreAimes() const override;
    bool estAimePar(const std::string& idUtilisateur) const override;
    
    // Implémentation des méthodes de IPartageable
    void partager(const std::string& idUtilisateur) override;
    int getNombrePartages() const override;

    // Implémentation des méthodes abstraites de Contenu
    void afficher() const override;
    std::string getType() const override;

    // Getters
    std::string getUrl() const;
    std::string getTypeMedia() const;
};

#endif // MEDIA_H