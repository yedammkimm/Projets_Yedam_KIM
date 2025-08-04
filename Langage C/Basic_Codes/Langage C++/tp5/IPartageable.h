#ifndef IPARTAGEABLE_H
#define IPARTAGEABLE_H

#include <string>

class IPartageable {
public:
    virtual void partager(const std::string& idUtilisateur) = 0;
    virtual int getNombrePartages() const = 0;
    
    virtual ~IPartageable() = default;
};

#endif // IPARTAGEABLE_H
