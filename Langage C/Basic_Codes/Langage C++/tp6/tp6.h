#ifndef TP6_H
#define TP6_H


#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>

//  LOGGER 
class EventLogger {
private:
    std::ofstream logFile;  // fichierLog

public:
    EventLogger(const std::string& fileName = "log.txt");  // Logger
    ~EventLogger();

    void writeLog(const std::string& message);  // log()
    void close();  // fermer()

private:
    static std::string getTimeStamp();  // getTimestamp()
};

// Fichier
class FileObject {
public:
    std::string name, content;  // nom, contenu

    FileObject();
    FileObject(const std::string& n, const std::string& c = "");

    void write(const std::string& text, bool append = false);  // ecrire()
    void display() const;  // afficher()

    friend std::istream& operator>>(std::istream& is, FileObject& f);  // operator>>
};

// Dossier
class Folder {
public:
    std::string name;  // nom
    std::vector<FileObject> files;  // fichiers
    std::vector<Folder*> subFolders;  // sousDossiers

    Folder();
    Folder(const std::string& n);
    ~Folder();

    void addFile(const FileObject& f);  // ajouterFichier()
    void addFolder(Folder* f);  // ajouterDossier()
    void removeFile(const std::string& filename);  // supprimerFichier()
    void removeFolder(const std::string& foldername);  // supprimerDossier()
    void display() const;  // afficher()

    friend std::istream& operator>>(std::istream& is, Folder& d);
};


// DisqueVirtuel
class VirtualDisk {
public:
    static void save(Folder* root);  // sauvegarder()
    static void load(Folder* root);  // charger()

private:
    static void serialize(std::ofstream& file, Folder* folder);  // serialiserDossier()
    static void deserialize(std::ifstream& file, Folder* folder);  // deserialiserDossier()
};


// SystemeFichier
class FileSystem {
public:
    Folder* root;
    Folder* current;

    FileSystem();
    ~FileSystem();

    void changeDirectory(const std::string& folderName);  // changerDossierCourant()
    void createFolder();  // creerDossier()
    void createFile();  // creerFichier()
    void updateFileContent(const std::string& name);  // modifierContenuFichier()
    void deleteFile(const std::string& name);  // supprimerFichier()
    void deleteFolder(const std::string& name);  // supprimerDossier()
    void listDirectory();  // afficherFichiers()
    void readFile(const std::string& name);  // afficherContenuFichier()
    void printCurrentDirectory();  // afficherDossierCourant()
};

#endif //TP6_H
