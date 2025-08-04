# Makefile

# Compiler
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Target
TARGET = tp6

# Source and header files
SRCS = tp6.cpp
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $<

# Clean up build files
clean:
	rm -f *.o $(TARGET)
