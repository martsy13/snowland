TARGET = snowland

CXX = clang++
CXXFLAGS = -Wall -Wextra -O2
LIBS = -lraylib -lm -lpthread -ldl -lrt

SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
