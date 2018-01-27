#!/bin/bash

cat url_list | ./markov_chain_generator 1 | tee markov_chain
