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
PAR_DIR=src/parallel
PAR=$(wildcard $(PAR_DIR)/*.cpp)


sequential:
		$(CXX) $(CXXFLAGS) $(LDLIBS) $(INC) $(UTILS) \
												$(SEQ) -o optimize.out

parallelSA:
		$(MPI) $(CXXFLAGS) $(LDLIBS) $(INC) $(UTILS) \
												parallel/simAnn.cpp -o pSA.out

parallelNM:
		$(MPI) $(CXXFLAGS) $(LDLIBS) $(INC) $(UTILS) \
		 										parallel/nelder_mead.cpp -o pNM.out
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
			rm pSA.out
			rm pSA.out
			rm -R results

all: sequential parallelSA parallelNM
seq: sequential
psa: parallelSA
pnm: parallelNM
