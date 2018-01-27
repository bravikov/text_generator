all: markov_chain_generator 

markov_chain_generator: markov_chain_generator.cpp
	g++ -std=c++11 -Wall -o markov_chain_generator markov_chain_generator.cpp
