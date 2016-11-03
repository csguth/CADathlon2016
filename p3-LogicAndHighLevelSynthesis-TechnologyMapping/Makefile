installPath = $(PWD)/graph-template-library/installDir

all: main.cpp
	g++ -std=c++11 -static main.cpp -o tplDetailedPlacement -L $(installPath)/lib -lGTL -I $(installPath)/include

partition_graph_test: partition_graph_test.cpp
	g++ -std=c++11 -static partition_graph_test.cpp -o partition_graph_test -L $(installPath)/lib -lGTL -I $(installPath)/include
