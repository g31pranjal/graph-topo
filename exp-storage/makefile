all:
	g++ -std=c++11 -c adjList.cpp csr.cpp pma.cpp adjpma.cpp 
	g++ -std=c++11 run.cpp adjList.o csr.o pma.o adjpma.o -o run

clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf run

# run:
# 	bash script.sh