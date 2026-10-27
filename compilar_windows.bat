@echo off
g++ -O2 -std=c++17 -static -o bin\arvores.exe src\main.cpp
g++ -O2 -std=c++17 -static -o bin\testes.exe src\testes.cpp
echo Compilado com sucesso em bin\arvores.exe e bin\testes.exe!
pause
