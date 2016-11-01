CXX = g++
STD = --std=c++11

message: main.o sha256.o utils.o network.o
	$(CXX) -o message main.o sha256.o utils.o network.o -lboost_system

main.o: main.cpp utils.h sha256.h network.h
	$(CXX) -c main.cpp $(STD)

sha256.o: sha256.cpp sha256.h
	$(CXX) -c sha256.cpp $(STD)

utils.o: utils.cpp utils.h sha256.h
	$(CXX) -c utils.cpp $(STD)

network.o: network.cpp network.h
	$(CXX) -c network.cpp  


