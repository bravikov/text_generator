all: markov_chain_generator text_generator

SOURCES = string_util.cpp markov_chain.cpp
HEADERS =  string_util.h markov_chain.h
BUILD_COMMAND = g++ -std=c++11 -Wall $(SOURCES)

markov_chain_generator: $(SOURCES) $(HEADERS) markov_chain_generator.cpp
	$(BUILD_COMMAND) -o markov_chain_generator markov_chain_generator.cpp

text_generator: $(SOURCES) $(HEADERS) text_generator.cpp
	$(BUILD_COMMAND) -o text_generator text_generator.cpp

clean:
	rm -f markov_chain_generator
	rm -f text_generator
