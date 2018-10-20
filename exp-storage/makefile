all:
	g++ -std=c++11 -c adjList.cpp csr.cpp pma.cpp adjpma.cpp pcsr.cpp pma_for_csr.cpp
	g++ -std=c++11 run.cpp adjList.o csr.o pma.o adjpma.o pcsr.o pma_for_csr.o -o run
	# g++ -std=c++11 hops-experiment.cpp adjList.o csr.o pma.o adjpma.o -o hops

clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf hops
	rm -rf run

# run:
# 	bash script.sh