vaccineMonitor: src/main.cpp src/viruses.cpp src/bloom.cpp src/skip_list.cpp src/citizens.cpp src/countries.cpp src/hash.cpp
	g++ -o vaccineMonitor src/main.cpp src/viruses.cpp src/bloom.cpp src/skip_list.cpp src/citizens.cpp src/countries.cpp src/hash.cpp -std=c++11

run:
	./vaccineMonitor -c script/inputFile -b 100000

valgrind: 
	valgrind --leak-check=full --show-leak-kinds=all ./vaccineMonitor -c script/inputFile -b 100000

clean:
	rm -f vaccineMonitor
