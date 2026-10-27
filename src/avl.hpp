#pragma once
#include "arvore.hpp"

struct NodoAVL {
    int codigo;
    int nivelAlt;
    NodoAVL* esq;
    NodoAVL* dir;
    NodoAVL(int v) : codigo(v), nivelAlt(1), esq(nullptr), dir(nullptr) {}
};

class ArvoreAVL : public ArvoreBase {
private:
    NodoAVL* raiz = nullptr;
    int totalNos = 0;

    int alturaNodo(NodoAVL* p) {
        return p != nullptr ? p->nivelAlt : 0;
    }

    void corrigeAltura(NodoAVL* p) {
        if (p != nullptr) {
            int he = alturaNodo(p->esq);
            int hd = alturaNodo(p->dir);
            p->nivelAlt = 1 + (he > hd ? he : hd);
        }
    }

    int calcFator(NodoAVL* p) {
        if (p == nullptr) return 0;
        return alturaNodo(p->esq) - alturaNodo(p->dir);
    }

    NodoAVL* giroDireita(NodoAVL* desb) {
        NodoAVL* novoTopo = desb->esq;
        NodoAVL* subMeio = novoTopo->dir;
        novoTopo->dir = desb;
        desb->esq = subMeio;
        corrigeAltura(desb);
        corrigeAltura(novoTopo);
        contRotacoes++;
        return novoTopo;
    }

    NodoAVL* giroEsquerda(NodoAVL* desb) {
        NodoAVL* novoTopo = desb->dir;
        NodoAVL* subMeio = novoTopo->esq;
        novoTopo->esq = desb;
        desb->dir = subMeio;
        corrigeAltura(desb);
        corrigeAltura(novoTopo);
        contRotacoes++;
        return novoTopo;
    }

    NodoAVL* reequilibrar(NodoAVL* p) {
        corrigeAltura(p);
        int fb = calcFator(p);
        if (fb > 1) {
            if (calcFator(p->esq) < 0)
                p->esq = giroEsquerda(p->esq);
            return giroDireita(p);
        }
        if (fb < -1) {
            if (calcFator(p->dir) > 0)
                p->dir = giroDireita(p->dir);
            return giroEsquerda(p);
        }
        return p;
    }

    NodoAVL* insereInterno(NodoAVL* p, int codigo, bool& sucesso) {
        if (p == nullptr) {
            sucesso = true;
            return new NodoAVL(codigo);
        }
        if (codigo < p->codigo)
            p->esq = insereInterno(p->esq, codigo, sucesso);
        else if (codigo > p->codigo)
            p->dir = insereInterno(p->dir, codigo, sucesso);
        else {
            sucesso = false;
            return p;
        }
        return reequilibrar(p);
    }

    NodoAVL* pegarMinimo(NodoAVL* p) {
        while (p->esq != nullptr) p = p->esq;
        return p;
    }

    NodoAVL* removeInterno(NodoAVL* p, int codigo, bool& sucesso) {
        if (p == nullptr) {
            sucesso = false;
            return nullptr;
        }
        if (codigo < p->codigo)
            p->esq = removeInterno(p->esq, codigo, sucesso);
        else if (codigo > p->codigo)
            p->dir = removeInterno(p->dir, codigo, sucesso);
        else {
            sucesso = true;
            if (p->esq == nullptr || p->dir == nullptr) {
                NodoAVL* substituto = (p->esq != nullptr) ? p->esq : p->dir;
                delete p;
                return substituto;
            }
            NodoAVL* suc = pegarMinimo(p->dir);
            p->codigo = suc->codigo;
            bool ignorar = false;
            p->dir = removeInterno(p->dir, suc->codigo, ignorar);
        }
        return reequilibrar(p);
    }

    void ordemSimetrica(NodoAVL* p, vector<int>& saida) {
        if (p == nullptr) return;
        ordemSimetrica(p->esq, saida);
        saida.push_back(p->codigo);
        ordemSimetrica(p->dir, saida);
    }

    void mostraHorizontal(NodoAVL* p, int espacos) {
        if (p == nullptr) return;
        mostraHorizontal(p->dir, espacos + 1);
        cout << string(espacos * 5, ' ') << p->codigo << " [fb=" << calcFator(p) << "]\n";
        mostraHorizontal(p->esq, espacos + 1);
    }

    int auditarNodo(NodoAVL* p, long long limInf, long long limSup, bool& integro) {
        if (p == nullptr) return 0;
        if (p->codigo <= limInf || p->codigo >= limSup) integro = false;
        int he = auditarNodo(p->esq, limInf, p->codigo, integro);
        int hd = auditarNodo(p->dir, p->codigo, limSup, integro);
        if (abs(he - hd) > 1) integro = false;
        int altCalc = 1 + max(he, hd);
        if (altCalc != p->nivelAlt) integro = false;
        return altCalc;
    }

    void apagarTudo(NodoAVL* p) {
        if (p == nullptr) return;
        apagarTudo(p->esq);
        apagarTudo(p->dir);
        delete p;
    }

public:
    ~ArvoreAVL() { apagarTudo(raiz); }

    bool adicionar(int codigo) override {
        bool ok = false;
        raiz = insereInterno(raiz, codigo, ok);
        if (ok) totalNos++;
        return ok;
    }

    bool excluir(int codigo) override {
        bool ok = false;
        raiz = removeInterno(raiz, codigo, ok);
        if (ok) totalNos--;
        return ok;
    }

    bool contem(int codigo) override {
        NodoAVL* atual = raiz;
        while (atual != nullptr) {
            if (codigo == atual->codigo) return true;
            atual = (codigo < atual->codigo) ? atual->esq : atual->dir;
        }
        return false;
    }

    void listarEmOrdem(vector<int>& saida) override {
        ordemSimetrica(raiz, saida);
    }

    void exibirArvore() override {
        if (raiz == nullptr) {
            cout << "(arvore vazia)\n";
            return;
        }
        mostraHorizontal(raiz, 0);
    }

    int obterAltura() override { return alturaNodo(raiz); }

    int obterTotal() override { return totalNos; }

    bool checarBalanceamento() override {
        bool ok = true;
        auditarNodo(raiz, -2147483648LL - 1, 2147483647LL + 1, ok);
        return ok;
    }

    string nomeEstrutura() override { return "AVL"; }
};
