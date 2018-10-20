all:
	g++ -std=c++11 -c adjList.cpp csr.cpp pma.cpp adjpma.cpp 
	g++ -std=c++11 hops-experiment.cpp adjList.o csr.o pma.o adjpma.o -o hops

clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf hops

# run:
# 	bash script.sh