#include "tp6.h"

using namespace std; 

//  LOGGER IMPLEMENTATION 
EventLogger::EventLogger(const string& fileName) {
    logFile.open(fileName, ios::app);
    if (!logFile) {
        cerr << "Error: Cannot open log file!" << endl;
    }
}

EventLogger::~EventLogger() {
    if (logFile.is_open()) logFile.close();
}

void EventLogger::writeLog(const string& message) {
    if (!logFile.is_open()) {
        logFile.open("log.txt", ios::app);
        if (!logFile) {
            cerr << "Error: Cannot open log file!" << endl;
            return;
        }
    }
    logFile << getTimeStamp() << " - " << message << endl;
}

string EventLogger::getTimeStamp() {
    time_t now = time(nullptr);
    tm* local = localtime(&now);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", local);
    return string(buf);
}

void EventLogger::close() {
    if (logFile.is_open()) logFile.close();
}



// FOLDER IMPLEMENTATION 
FileObject::FileObject() : name(""), content("") {}
FileObject::FileObject(const string& n, const string& c) : name(n), content(c) {}

void FileObject::write(const string& text, bool append) {
    ofstream file(name, append ? ios::app : ios::out);
    if (!file) {
        cerr << "Error: Cannot open file " << name << "!" << endl;
        return;
    }
    file << text;
    file.close();
}

void FileObject::display() const {
    ifstream file(name);
    if (!file) {
        cerr << "Error: Cannot open file " << name << "!" << endl;
        return;
    }
    string line;
    while (getline(file, line)) cout << line << endl;
    file.close();
}

istream& operator>>(istream& is, FileObject& f) {
    cout << "File name: ";
    is >> f.name;
    cout << "File content: ";
    is.ignore();
    getline(is, f.content);
    return is;
}

Folder::Folder() : name("") {}
Folder::Folder(const string& n) : name(n) {}
Folder::~Folder() {
    for (auto d : subFolders) delete d;
    subFolders.clear();
}

void Folder::addFile(const FileObject& f) {
    files.push_back(f);
}

void Folder::addFolder(Folder* f) {
    subFolders.push_back(f);
}

void Folder::removeFile(const string& filename) {
    auto it = remove_if(files.begin(), files.end(), [&](const FileObject& f) { return f.name == filename; });
    if (it != files.end()) {
        files.erase(it, files.end());
    } else {
        cerr << "File not found: " << filename << endl;
    }
}

void Folder::removeFolder(const string& foldername) {
    auto it = find_if(subFolders.begin(), subFolders.end(), [&](Folder* d) { return d->name == foldername; });
    if (it != subFolders.end()) {
        delete *it;
        subFolders.erase(it);
        cout << "Folder '" << foldername << "' deleted." << endl;
    } else {
        cerr << "Folder not found: " << foldername << endl;
    }
}

void Folder::display() const {
    cout << "Directory: " << name << "\nFiles:\n";
    for (const auto& f : files) cout << "  - " << f.name << endl;

    cout << "Subdirectories:\n";
    for (const auto& d : subFolders) cout << "  [DIR] " << d->name << endl;
}

istream& operator>>(istream& is, Folder& d) {
    cout << "Folder name: ";
    is >> d.name;
    return is;
}



//  VIRTUAL DISK IMPLEMENTATION 
void VirtualDisk::save(Folder* root) {
    ofstream file("system_fs.dat", ios::binary);
    if (!file) {
        cerr << "Error: Cannot write system_fs.dat!" << endl;
        return;
    }
    serialize(file, root);
    file.close();
}

void VirtualDisk::load(Folder* root) {
    ifstream file("system_fs.dat", ios::binary);
    if (!file) {
        cerr << "No system_fs.dat found, starting fresh." << endl;
        return;
    }
    deserialize(file, root);
    file.close();
}

void VirtualDisk::serialize(ofstream& file, Folder* folder) {
    size_t nameLen = folder->name.size();
    file.write(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
    file.write(folder->name.c_str(), nameLen);

    size_t fileCount = folder->files.size();
    file.write(reinterpret_cast<char*>(&fileCount), sizeof(fileCount));
    for (const auto& f : folder->files) {
        size_t nameSize = f.name.size(), contentSize = f.content.size();
        file.write(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        file.write(f.name.c_str(), nameSize);
        file.write(reinterpret_cast<char*>(&contentSize), sizeof(contentSize));
        file.write(f.content.c_str(), contentSize);
    }

    size_t subCount = folder->subFolders.size();
    file.write(reinterpret_cast<char*>(&subCount), sizeof(subCount));
    for (auto s : folder->subFolders) {
        serialize(file, s);
    }
}

void VirtualDisk::deserialize(ifstream& file, Folder* folder) {
    size_t nameLen;
    file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
    folder->name.resize(nameLen);
    file.read(&folder->name[0], nameLen);

    size_t fileCount;
    file.read(reinterpret_cast<char*>(&fileCount), sizeof(fileCount));
    for (size_t i = 0; i < fileCount; ++i) {
        size_t nameSize, contentSize;
        file.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        string fileName(nameSize, '\0');
        file.read(&fileName[0], nameSize);
        file.read(reinterpret_cast<char*>(&contentSize), sizeof(contentSize));
        string content(contentSize, '\0');
        file.read(&content[0], contentSize);
        folder->files.emplace_back(fileName, content);
    }

    size_t subCount;
    file.read(reinterpret_cast<char*>(&subCount), sizeof(subCount));
    for (size_t i = 0; i < subCount; ++i) {
        Folder* sub = new Folder();
        deserialize(file, sub);
        folder->addFolder(sub);
    }
}


//  FILE SYSTEM IMPLEMENTATION 
FileSystem::FileSystem() {
    root = new Folder("root");
    current = root;
}

FileSystem::~FileSystem() {
    delete root;
}

void FileSystem::listDirectory() {
    current->display();
}

void FileSystem::changeDirectory(const string& folderName) {
    if (folderName == ".." && current != root) {
        current = root; // no parent tracking implemented
    } else {
        for (auto d : current->subFolders) {
            if (d->name == folderName) {
                current = d;
                return;
            }
        }
        cerr << "Folder not found: " << folderName << endl;
    }
}

void FileSystem::createFolder() {
    string name;
    cout << "Folder name: ";
    cin >> name;
    current->addFolder(new Folder(name));
}

void FileSystem::createFile() {
    string name;
    cout << "File name: ";
    cin >> name;
    current->addFile(FileObject(name));
}

void FileSystem::updateFileContent(const string& name) {
    for (auto& f : current->files) {
        if (f.name == name) {
            string content;
            cout << "New content: ";
            cin.ignore();
            getline(cin, content);
            f.write(content, false);
            return;
        }
    }
    cerr << "File not found: " << name << endl;
}

void FileSystem::deleteFile(const string& name) {
    current->removeFile(name);
}

void FileSystem::deleteFolder(const string& name) {
    current->removeFolder(name);
}

void FileSystem::readFile(const string& name) {
    for (const auto& f : current->files) {
        if (f.name == name) {
            f.display();
            return;
        }
    }
    cerr << "File not found: " << name << endl;
}

void FileSystem::printCurrentDirectory() {
    cout << "Current folder: " << current->name << endl;
}


//  MAIN SHELL 
int main() {
    EventLogger logger("log.txt");
    logger.writeLog("Program started.");

    FileSystem system;
    VirtualDisk::load(system.root);

    string cmd;
    while (true) {
        cout << "> ";
        cin >> cmd;

        if (cmd == "ls") {
            system.listDirectory();
        } else if (cmd == "cd") {
            string name;
            cin >> name;
            system.changeDirectory(name);
        } else if (cmd == "mkdir") {
            system.createFolder();
        } else if (cmd == "touch") {
            system.createFile();
        } else if (cmd == "edit") {
            string name;
            cin >> name;
            system.updateFileContent(name);
        } else if (cmd == "rm") {
            string name;
            cin >> name;
            bool isFile = false;
            for (const auto& f : system.current->files) {
                if (f.name == name) {
                    isFile = true;
                    break;
                }
            }
            if (isFile) system.deleteFile(name);
            else system.deleteFolder(name);
        } else if (cmd == "cat") {
            string name;
            cin >> name;
            system.readFile(name);
        } else if (cmd == "pwd") {
            system.printCurrentDirectory();
        } else if (cmd == "exit") {
            VirtualDisk::save(system.root);
            break;
        } else {
            cout << "Unknown command!" << endl;
        }
    }

    return 0;
}
