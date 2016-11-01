installPath = $(PWD)/graph-template-library/installDir

all: expression_evaluation_test.cpp
	g++ -std=c++11 -static expression_evaluation_test.cpp -o expression_evaluation_test -L $(installPath)/lib -lGTL -I $(installPath)/include

expression_evaluation_test: expression_evaluation_test.cpp
	g++ -std=c++11 -static expression_evaluation_test.cpp -o expression_evaluation_test -L $(installPath)/lib -lGTL -I $(installPath)/include

find_possible_values_test: find_possible_values_test.cpp
	g++ -std=c++11 -static find_possible_values_test.cpp -o find_possible_values_test -L $(installPath)/lib -lGTL -I $(installPath)/include

generate_states_test: generate_states_test.cpp
	g++ -std=c++11 -static generate_states_test.cpp -o generate_states_test -L $(installPath)/lib -lGTL -I $(installPath)/include
