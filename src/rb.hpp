#pragma once
#include "arvore.hpp"

enum CorNodo { COR_VERMELHA, COR_PRETA };

struct NodoRB {
    int codigo;
    CorNodo cor;
    NodoRB* esq;
    NodoRB* dir;
    NodoRB* pai;
};

class ArvoreRB : public ArvoreBase {
private:
    NodoRB* folhaNula;
    NodoRB* raiz;
    int totalNos = 0;

    void mudarCor(NodoRB* p, CorNodo nova) {
        if (p != folhaNula && p->cor != nova) {
            p->cor = nova;
            contRecoloracoes++;
        }
    }

    void giroEsquerda(NodoRB* x) {
        NodoRB* y = x->dir;
        x->dir = y->esq;
        if (y->esq != folhaNula) y->esq->pai = x;
        y->pai = x->pai;
        if (x->pai == folhaNula) raiz = y;
        else if (x == x->pai->esq) x->pai->esq = y;
        else x->pai->dir = y;
        y->esq = x;
        x->pai = y;
        contRotacoes++;
    }

    void giroDireita(NodoRB* x) {
        NodoRB* y = x->esq;
        x->esq = y->dir;
        if (y->dir != folhaNula) y->dir->pai = x;
        y->pai = x->pai;
        if (x->pai == folhaNula) raiz = y;
        else if (x == x->pai->dir) x->pai->dir = y;
        else x->pai->esq = y;
        y->dir = x;
        x->pai = y;
        contRotacoes++;
    }

    void ajustarPosInsercao(NodoRB* novo) {
        while (novo->pai->cor == COR_VERMELHA) {
            if (novo->pai == novo->pai->pai->esq) {
                NodoRB* tio = novo->pai->pai->dir;
                if (tio->cor == COR_VERMELHA) {
                    mudarCor(novo->pai, COR_PRETA);
                    mudarCor(tio, COR_PRETA);
                    mudarCor(novo->pai->pai, COR_VERMELHA);
                    novo = novo->pai->pai;
                } else {
                    if (novo == novo->pai->dir) {
                        novo = novo->pai;
                        giroEsquerda(novo);
                    }
                    mudarCor(novo->pai, COR_PRETA);
                    mudarCor(novo->pai->pai, COR_VERMELHA);
                    giroDireita(novo->pai->pai);
                }
            } else {
                NodoRB* tio = novo->pai->pai->esq;
                if (tio->cor == COR_VERMELHA) {
                    mudarCor(novo->pai, COR_PRETA);
                    mudarCor(tio, COR_PRETA);
                    mudarCor(novo->pai->pai, COR_VERMELHA);
                    novo = novo->pai->pai;
                } else {
                    if (novo == novo->pai->esq) {
                        novo = novo->pai;
                        giroDireita(novo);
                    }
                    mudarCor(novo->pai, COR_PRETA);
                    mudarCor(novo->pai->pai, COR_VERMELHA);
                    giroEsquerda(novo->pai->pai);
                }
            }
        }
        mudarCor(raiz, COR_PRETA);
    }

    void trocarSubarvore(NodoRB* alvo, NodoRB* novoRamo) {
        if (alvo->pai == folhaNula) raiz = novoRamo;
        else if (alvo == alvo->pai->esq) alvo->pai->esq = novoRamo;
        else alvo->pai->dir = novoRamo;
        novoRamo->pai = alvo->pai;
    }

    NodoRB* pegarMinimo(NodoRB* p) {
        while (p->esq != folhaNula) p = p->esq;
        return p;
    }

    NodoRB* acharNodo(int codigo) {
        NodoRB* atual = raiz;
        while (atual != folhaNula && atual->codigo != codigo)
            atual = (codigo < atual->codigo) ? atual->esq : atual->dir;
        return atual;
    }

    void ajustarPosRemocao(NodoRB* x) {
        while (x != raiz && x->cor == COR_PRETA) {
            if (x == x->pai->esq) {
                NodoRB* irmao = x->pai->dir;
                if (irmao->cor == COR_VERMELHA) {
                    mudarCor(irmao, COR_PRETA);
                    mudarCor(x->pai, COR_VERMELHA);
                    giroEsquerda(x->pai);
                    irmao = x->pai->dir;
                }
                if (irmao->esq->cor == COR_PRETA && irmao->dir->cor == COR_PRETA) {
                    mudarCor(irmao, COR_VERMELHA);
                    x = x->pai;
                } else {
                    if (irmao->dir->cor == COR_PRETA) {
                        mudarCor(irmao->esq, COR_PRETA);
                        mudarCor(irmao, COR_VERMELHA);
                        giroDireita(irmao);
                        irmao = x->pai->dir;
                    }
                    mudarCor(irmao, x->pai->cor);
                    mudarCor(x->pai, COR_PRETA);
                    mudarCor(irmao->dir, COR_PRETA);
                    giroEsquerda(x->pai);
                    x = raiz;
                }
            } else {
                NodoRB* irmao = x->pai->esq;
                if (irmao->cor == COR_VERMELHA) {
                    mudarCor(irmao, COR_PRETA);
                    mudarCor(x->pai, COR_VERMELHA);
                    giroDireita(x->pai);
                    irmao = x->pai->esq;
                }
                if (irmao->dir->cor == COR_PRETA && irmao->esq->cor == COR_PRETA) {
                    mudarCor(irmao, COR_VERMELHA);
                    x = x->pai;
                } else {
                    if (irmao->esq->cor == COR_PRETA) {
                        mudarCor(irmao->dir, COR_PRETA);
                        mudarCor(irmao, COR_VERMELHA);
                        giroEsquerda(irmao);
                        irmao = x->pai->esq;
                    }
                    mudarCor(irmao, x->pai->cor);
                    mudarCor(x->pai, COR_PRETA);
                    mudarCor(irmao->esq, COR_PRETA);
                    giroDireita(x->pai);
                    x = raiz;
                }
            }
        }
        mudarCor(x, COR_PRETA);
    }

    void ordemSimetrica(NodoRB* p, vector<int>& saida) {
        if (p == folhaNula) return;
        ordemSimetrica(p->esq, saida);
        saida.push_back(p->codigo);
        ordemSimetrica(p->dir, saida);
    }

    void mostraHorizontal(NodoRB* p, int espacos) {
        if (p == folhaNula) return;
        mostraHorizontal(p->dir, espacos + 1);
        cout << string(espacos * 5, ' ') << p->codigo << (p->cor == COR_VERMELHA ? "(V)" : "(P)") << "\n";
        mostraHorizontal(p->esq, espacos + 1);
    }

    int calcProfundidade(NodoRB* p) {
        if (p == folhaNula) return 0;
        return 1 + max(calcProfundidade(p->esq), calcProfundidade(p->dir));
    }

    int auditarNodo(NodoRB* p, long long limInf, long long limSup, bool& integro) {
        if (p == folhaNula) return 1;
        if (p->codigo <= limInf || p->codigo >= limSup) integro = false;
        if (p->cor == COR_VERMELHA && (p->esq->cor == COR_VERMELHA || p->dir->cor == COR_VERMELHA)) integro = false;
        int pe = auditarNodo(p->esq, limInf, p->codigo, integro);
        int pd = auditarNodo(p->dir, p->codigo, limSup, integro);
        if (pe != pd) integro = false;
        return pe + (p->cor == COR_PRETA ? 1 : 0);
    }

    void apagarTudo(NodoRB* p) {
        if (p == folhaNula) return;
        apagarTudo(p->esq);
        apagarTudo(p->dir);
        delete p;
    }

public:
    ArvoreRB() {
        folhaNula = new NodoRB{0, COR_PRETA, nullptr, nullptr, nullptr};
        folhaNula->esq = folhaNula->dir = folhaNula->pai = folhaNula;
        raiz = folhaNula;
    }

    ~ArvoreRB() {
        apagarTudo(raiz);
        delete folhaNula;
    }

    bool adicionar(int codigo) override {
        NodoRB* ant = folhaNula;
        NodoRB* atual = raiz;
        while (atual != folhaNula) {
            ant = atual;
            if (codigo < atual->codigo) atual = atual->esq;
            else if (codigo > atual->codigo) atual = atual->dir;
            else return false;
        }
        NodoRB* novo = new NodoRB{codigo, COR_VERMELHA, folhaNula, folhaNula, ant};
        if (ant == folhaNula) raiz = novo;
        else if (codigo < ant->codigo) ant->esq = novo;
        else ant->dir = novo;
        totalNos++;
        ajustarPosInsercao(novo);
        return true;
    }

    bool excluir(int codigo) override {
        NodoRB* z = acharNodo(codigo);
        if (z == folhaNula) return false;
        NodoRB* y = z;
        CorNodo corRemovida = y->cor;
        NodoRB* x;
        if (z->esq == folhaNula) {
            x = z->dir;
            trocarSubarvore(z, z->dir);
        } else if (z->dir == folhaNula) {
            x = z->esq;
            trocarSubarvore(z, z->esq);
        } else {
            y = pegarMinimo(z->dir);
            corRemovida = y->cor;
            x = y->dir;
            if (y->pai == z) {
                x->pai = y;
            } else {
                trocarSubarvore(y, y->dir);
                y->dir = z->dir;
                y->dir->pai = y;
            }
            trocarSubarvore(z, y);
            y->esq = z->esq;
            y->esq->pai = y;
            mudarCor(y, z->cor);
        }
        delete z;
        totalNos--;
        if (corRemovida == COR_PRETA) ajustarPosRemocao(x);
        return true;
    }

    bool contem(int codigo) override {
        return acharNodo(codigo) != folhaNula;
    }

    void listarEmOrdem(vector<int>& saida) override {
        ordemSimetrica(raiz, saida);
    }

    void exibirArvore() override {
        if (raiz == folhaNula) {
            cout << "(arvore vazia)\n";
            return;
        }
        mostraHorizontal(raiz, 0);
    }

    int obterAltura() override { return calcProfundidade(raiz); }

    int obterTotal() override { return totalNos; }

    bool checarBalanceamento() override {
        bool ok = true;
        if (raiz->cor != COR_PRETA) ok = false;
        auditarNodo(raiz, -2147483648LL - 1, 2147483647LL + 1, ok);
        return ok;
    }

    string nomeEstrutura() override { return "Rubro-Negra"; }
};
