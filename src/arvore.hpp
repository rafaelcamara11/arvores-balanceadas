#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

class ArvoreBase {
public:
    long long contRotacoes = 0;
    long long contRecoloracoes = 0;
    virtual ~ArvoreBase() {}
    virtual bool adicionar(int codigo) = 0;
    virtual bool excluir(int codigo) = 0;
    virtual bool contem(int codigo) = 0;
    virtual void listarEmOrdem(vector<int>& saida) = 0;
    virtual void exibirArvore() = 0;
    virtual int obterAltura() = 0;
    virtual int obterTotal() = 0;
    virtual bool checarBalanceamento() = 0;
    virtual string nomeEstrutura() = 0;
};
