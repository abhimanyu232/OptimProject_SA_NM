CXX = g++
CXXFLAGS = -O3 --std=C++11 -Iinclude
CFLAGS += -Wall -Wall
LDLIBS += -lm

INC =
EXE=optimize
SRC_DIR=src
SRC=$(wildcard $(SRC_DIR)/*.cpp)
OBJ_DIR=intermediate
OBJ=$(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
		$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
		$(CXX) $(CXXFLAGS) $(CFLAGS) -c $< -o $@

clean:
		$(RM) $(OBJ)
		rm -f $(EXE)

clean_all:
		$(RM) $(OBJ)
		rm -f $(EXE)
		rm -R results
