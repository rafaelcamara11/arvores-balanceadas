all: bin/arvores bin/testes

bin/arvores: src/main.cpp src/avl.hpp src/rb.hpp src/arvore.hpp
	g++ -O2 -std=c++17 -o bin/arvores src/main.cpp

bin/testes: src/testes.cpp src/avl.hpp src/rb.hpp src/arvore.hpp
	g++ -O2 -std=c++17 -o bin/testes src/testes.cpp
