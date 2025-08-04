#ifndef IAIMABLE_H
#define IAIMABLE_H

#include <string>

class IAimable {
public:
    virtual void aimer(const std::string& idUtilisateur) = 0;
    virtual void retirerAime(const std::string& idUtilisateur) = 0;
    virtual int getNombreAimes() const = 0;
    virtual bool estAimePar(const std::string& idUtilisateur) const = 0;
    
    virtual ~IAimable() = default;
};

#endif // IAIMABLE_H
