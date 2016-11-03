installPath = $(PWD)/graph-template-library/installDir

all: match_test.cpp
	g++ -std=c++11 -static match_test.cpp -o match_test -L $(installPath)/lib -lGTL -I $(installPath)/include

match_test: match_test.cpp
	g++ -std=c++11 -static match_test.cpp -o match_test -L $(installPath)/lib -lGTL -I $(installPath)/include

partition_graph_test: partition_graph_test.cpp
	g++ -std=c++11 -static partition_graph_test.cpp -o partition_graph_test -L $(installPath)/lib -lGTL -I $(installPath)/include

decompose_graph_test: decompose_graph_test.cpp
	g++ -std=c++11 -static decompose_graph_test.cpp -o decompose_graph_test -L $(installPath)/lib -lGTL -I $(installPath)/include
