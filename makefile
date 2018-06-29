default_target: sequential

CXX = g++
MPI = mpicxx
CXXFLAGS = --std=c++11 -O3  -Wall -Werror
INC = -Iinc
LDLIBS += -lm

RM = rm -rf

UTIL_DIR=src/utils
UTILS=$(wildcard $(UTIL_DIR)/*.cpp)
SEQ_DIR=src/serial
SEQ=$(wildcard $(SEQ_DIR)/*.cpp)
#PAR_DIR=src/parallel
#PAR=$(wildcard $(PAR_DIR)/*.cpp)


sequential:
		$(CXX) $(CXXFLAGS) $(LDLIBS) $(INC) $(UTILS) \
												$(SEQ) -o optimize.out

parallelSA:
		mkdir -p results
		$(MPI) $(CXXFLAGS) $(LDLIBS) $(INC) src/parallel/test_functions.cpp	 \
											src/parallel/simAnn.cpp -o psa.out

parallelNM:
		mkdir -p results
		$(MPI) $(CXXFLAGS) $(LDLIBS) $(INC) src/parallel/test_functions.cpp \
											src/parallel/nelder_mead.cpp -o pnm.out

clean:
		rm *.out
clean_seq:
		rm optimize.out
clean_psa:
		rm pSA.out
clean_pnm:
		pNM.out
clean_all:
			rm optimize.out.
			rm pnm.out
			rm psa.out
			rm -R results

all: sequential parallelSA parallelNM
seq: sequential
psa: parallelSA
pnm: parallelNM
