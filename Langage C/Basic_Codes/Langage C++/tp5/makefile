CXX = g++
CXXFLAGS = -Wall -std=c++11

OBJ = main.o Utilisateur.o Contenu.o Publication.o Media.o

all: main

main: $(OBJ)
	$(CXX) $(CXXFLAGS) -o main $(OBJ)

main.o: main.cpp Utilisateur.h Contenu.h Publication.h Media.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Utilisateur.o: Utilisateur.cpp Utilisateur.h Contenu.h
	$(CXX) $(CXXFLAGS) -c Utilisateur.cpp

Contenu.o: Contenu.cpp Contenu.h
	$(CXX) $(CXXFLAGS) -c Contenu.cpp

Publication.o: Publication.cpp Publication.h Contenu.h IAimable.h
	$(CXX) $(CXXFLAGS) -c Publication.cpp

Media.o: Media.cpp Media.h Contenu.h IAimable.h IPartageable.h
	$(CXX) $(CXXFLAGS) -c Media.cpp

clean:
	rm -f *.o main
