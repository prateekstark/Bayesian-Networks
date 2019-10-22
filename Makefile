FLAG = -std=c++11 -O2

all: bayesian_network_learning

bayesian_network_learning: Graph_Node network utils bayesian_learning
		g++ -O2 outputs/Graph_Node outputs/network outputs/utils outputs/bayesian_learning -o bayesian_network_learning

Graph_Node: src/Graph_Node.cpp include/Graph_Node.h
		g++ -c $(FLAG) src/Graph_Node.cpp -o outputs/Graph_Node

network: src/network.cpp include/network.h
		g++ -c $(FLAG) src/network.cpp -o outputs/network

utils: src/utils.cpp include/utils.h
		g++ -c $(FLAG) src/utils.cpp -o outputs/utils

bayesian_learning: src/bayesian_learning.cpp include/utils.h
		g++ -c $(FLAG) src/bayesian_learning.cpp -o outputs/bayesian_learning

clean:
		rm -rf outputs/* bayesian_network_learning
