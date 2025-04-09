# Compiler
CXX = g++

# Compiler Flags
CXXFLAGS = -Wall -std=c++17

# SFML Libraries
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Output Executable
TARGET = clock_display

# Include Paths
INCLUDES = -I. -I../receptor

# Source Files
SRC = main.cpp \
      features/Hand.cpp \
      features/Clock.cpp \
      ../receptor/communication.cpp

# Transition Source Files
SRC_TRANSITIONS = $(wildcard transitions/*.cpp)
SRC_TRANSITIONS += type_transitions/type_transitions.cpp

# Combine All
SRC += $(SRC_TRANSITIONS)

# Object Files
OBJ = $(SRC:.cpp=.o)

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJ)
	$(CXX) -o $(TARGET) $(OBJ) $(SFML_LIBS)

# Compiling
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean
clean:
	rm -f $(OBJ) $(TARGET)

# Run
run: $(TARGET)
	./$(TARGET)
