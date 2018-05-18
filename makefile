EXE=optimize.out
CXX = g++
CXXFLAGS = --std=c++11 -O3 -Iinc -Wall -Werror
LDLIBS += -lm

SRC_DIR=src
SRC=$(wildcard $(SRC_DIR)/*.cpp)
OBJ_DIR=intermediate
OBJ=$(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
		$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
		$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(CFLAGS) -c $< -o $@

clean:
		$(RM) $(OBJ)
		rm -f $(EXE)
		rm -f $(dat)

clean_all:
	$(RM) $(OBJ)
	rm -f $(EXE)
	rm -R results
