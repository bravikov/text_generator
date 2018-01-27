#!/bin/bash -e

make
cd test
cp -R mc_texts /tmp
cat url_list | ../markov_chain_generator 1 | tee markov_chain
echo "some text" | ../text_generator markov_chain 2
