#include "avl.hpp"
#include "rb.hpp"
#include <set>
#include <random>
#include <chrono>
#include <iomanip>

int qtdVerificacoes = 0;
int qtdFalhas = 0;

void validarItem(bool condicao, const string& descricao) {
    qtdVerificacoes++;
    if (condicao) {
        cout << "  [PASSOU] " << descricao << "\n";
    } else {
        qtdFalhas++;
        cout << "  [FALHA]  " << descricao << "\n";
    }
}

bool compararComConjunto(ArvoreBase& arv, const set<int>& espelho) {
    vector<int> lista;
    arv.listarEmOrdem(lista);
    return lista == vector<int>(espelho.begin(), espelho.end());
}

struct NodoComum {
    int ch;
    NodoComum* e;
    NodoComum* d;
};

int calcularAlturaBSTSemBalanceamento(const vector<int>& entrada) {
    NodoComum* r = nullptr;
    int maiorProf = 0;
    vector<NodoComum*> memoria;
    for (int valor : entrada) {
        NodoComum* n = new NodoComum{valor, nullptr, nullptr};
        memoria.push_back(n);
        int p = 1;
        if (r == nullptr) r = n;
        else {
            NodoComum* at = r;
            while (true) {
                p++;
                if (valor < at->ch) {
                    if (!at->e) { at->e = n; break; }
                    at = at->e;
                } else {
                    if (!at->d) { at->d = n; break; }
                    at = at->d;
                }
            }
        }
        if (p > maiorProf) maiorProf = p;
    }
    for (NodoComum* ptr : memoria) delete ptr;
    return maiorProf;
}

vector<int> criarSequenciaCrescente(int tam) {
    vector<int> v(tam);
    for (int i = 0; i < tam; i++) v[i] = i + 1;
    return v;
}

vector<int> criarSequenciaDecrescente(int tam) {
    vector<int> v(tam);
    for (int i = 0; i < tam; i++) v[i] = tam - i;
    return v;
}

vector<int> criarSequenciaAleatoria(int tam, unsigned int semente) {
    vector<int> v = criarSequenciaCrescente(tam);
    mt19937 gerador(semente);
    shuffle(v.begin(), v.end(), gerador);
    return v;
}

void bateriaArvoreVazia(ArvoreBase& arv) {
    cout << "\n>>> Testando Arvore Vazia (" << arv.nomeEstrutura() << ")\n";
    vector<int> saida;
    arv.listarEmOrdem(saida);
    validarItem(!arv.contem(42), "Busca em estrutura vazia devolve falso");
    validarItem(!arv.excluir(42), "Remocao em estrutura vazia devolve falso");
    validarItem(saida.empty(), "Listagem em ordem retorna vetor vazio");
    validarItem(arv.obterAltura() == 0, "Altura de arvore vazia igual a zero");
    validarItem(arv.checarBalanceamento(), "Invariantes validas na arvore vazia");
}

void bateriaUnicoElemento(ArvoreBase& arv) {
    cout << "\n>>> Testando Unico Elemento (" << arv.nomeEstrutura() << ")\n";
    validarItem(arv.adicionar(75), "Insercao do pacote 75");
    validarItem(arv.contem(75), "Busca encontra pacote 75");
    validarItem(arv.obterAltura() == 1, "Altura igual a 1 com unico no");
    validarItem(arv.checarBalanceamento(), "Propriedades intactas apos insercao");
    validarItem(arv.excluir(75), "Remocao do unico no (75)");
    validarItem(arv.obterTotal() == 0 && arv.obterAltura() == 0, "Retorno ao estado vazio");
    validarItem(arv.checarBalanceamento(), "Propriedades intactas apos esvaziar");
}

void bateriaDuplicadosInexistentes(ArvoreBase& arv) {
    cout << "\n>>> Testando Duplicatas e Inexistentes (" << arv.nomeEstrutura() << ")\n";
    arv.adicionar(20);
    arv.adicionar(10);
    arv.adicionar(30);
    validarItem(!arv.adicionar(20), "Rejeicao de chave duplicada (20)");
    validarItem(arv.obterTotal() == 3, "Total de elementos mantido em 3");
    validarItem(!arv.contem(500), "Busca por chave ausente (500) resulta falso");
    validarItem(!arv.excluir(500), "Remocao de chave ausente (500) resulta falso");
    validarItem(arv.checarBalanceamento(), "Propriedades de balanceamento preservadas");
}

void bateriaConfiguracoesRemocao(ArvoreBase& arv) {
    cout << "\n>>> Testando Remocao (0, 1 e 2 filhos) (" << arv.nomeEstrutura() << ")\n";
    set<int> espelho;
    int pacoteIDs[] = {60, 30, 80, 15, 45, 70, 95, 40, 99};
    for (int id : pacoteIDs) {
        arv.adicionar(id);
        espelho.insert(id);
    }
    arv.excluir(15); espelho.erase(15);
    validarItem(compararComConjunto(arv, espelho) && arv.checarBalanceamento(), "Exclusao de no folha (15)");
    arv.excluir(45); espelho.erase(45);
    validarItem(compararComConjunto(arv, espelho) && arv.checarBalanceamento(), "Exclusao de no com 1 filho (45)");
    arv.excluir(30); espelho.erase(30);
    validarItem(compararComConjunto(arv, espelho) && arv.checarBalanceamento(), "Exclusao de no com 2 filhos (30)");
    arv.excluir(60); espelho.erase(60);
    validarItem(compararComConjunto(arv, espelho) && arv.checarBalanceamento(), "Exclusao do no raiz (60)");
}

void bateriaOrdensDeEntrada(ArvoreBase& arv, const string& rotulo, const vector<int>& dados) {
    set<int> espelho;
    for (int v : dados) {
        arv.adicionar(v);
        espelho.insert(v);
    }
    validarItem(compararComConjunto(arv, espelho) && arv.checarBalanceamento(),
                "Carga " + rotulo + " com " + to_string(dados.size()) + " pacotes");
}

void bateriaCicloAleatorio(ArvoreBase& arv) {
    cout << "\n>>> Testando Carga Intensiva (15000 operacoes) (" << arv.nomeEstrutura() << ")\n";
    mt19937 rng(2025);
    set<int> espelho;
    bool consistente = true;
    for (int i = 0; i < 15000; i++) {
        int ch = rng() % 600;
        if (rng() % 2 == 0) {
            if (arv.adicionar(ch) != espelho.insert(ch).second) consistente = false;
        } else {
            if (arv.excluir(ch) != (espelho.erase(ch) > 0)) consistente = false;
        }
        if (!arv.checarBalanceamento()) consistente = false;
    }
    validarItem(consistente && compararComConjunto(arv, espelho), "Integridade total apos 15000 operacoes mistas");
}

void imprimirLinhaComparativa(const string& cenario, const vector<int>& cargaIns, const vector<int>& cargaRem) {
    ArvoreAVL avl;
    ArvoreRB rb;
    for (int v : cargaIns) {
        avl.adicionar(v);
        rb.adicionar(v);
    }
    long long rAvlI = avl.contRotacoes, rRbI = rb.contRotacoes, cRbI = rb.contRecoloracoes;
    int hAvlI = avl.obterAltura(), hRbI = rb.obterAltura();
    for (int v : cargaRem) {
        avl.excluir(v);
        rb.excluir(v);
    }
    cout << left << setw(22) << cenario
         << " | AVL Ins[h=" << setw(2) << hAvlI << " rot=" << setw(6) << rAvlI << "]"
         << " | RB Ins[h=" << setw(2) << hRbI << " rot=" << setw(6) << rRbI << " cor=" << setw(6) << cRbI << "]"
         << " | AVL Rem[h=" << setw(2) << avl.obterAltura() << " rot=" << setw(5) << (avl.contRotacoes - rAvlI) << "]"
         << " | RB Rem[h=" << setw(2) << rb.obterAltura() << " rot=" << setw(5) << (rb.contRotacoes - rRbI) << "]\n";
}

double cronometrarExecucao(ArvoreBase& arv, const vector<int>& ins, const vector<int>& cons, long long& acertos) {
    auto tIni = chrono::steady_clock::now();
    for (int v : ins) arv.adicionar(v);
    acertos = 0;
    for (int v : cons) acertos += arv.contem(v);
    auto tFim = chrono::steady_clock::now();
    return chrono::duration<double, milli>(tFim - tIni).count();
}

int main() {
    cout << "====================================================\n";
    cout << "   SUITE DE TESTES E ANALISE: AVL vs RUBRO-NEGRA    \n";
    cout << "====================================================\n";

    for (int tipo = 0; tipo < 2; tipo++) {
        auto instanciar = [&]() -> ArvoreBase* {
            return (tipo == 0) ? static_cast<ArvoreBase*>(new ArvoreAVL()) : static_cast<ArvoreBase*>(new ArvoreRB());
        };
        ArvoreBase* p = instanciar(); bateriaArvoreVazia(*p); delete p;
        p = instanciar(); bateriaUnicoElemento(*p); delete p;
        p = instanciar(); bateriaDuplicadosInexistentes(*p); delete p;
        p = instanciar(); bateriaConfiguracoesRemocao(*p); delete p;
        p = instanciar();
        cout << "\n>>> Testando Ordens de Insercao (" << p->nomeEstrutura() << ")\n";
        bateriaOrdensDeEntrada(*p, "crescente", criarSequenciaCrescente(1200)); delete p;
        p = instanciar(); bateriaOrdensDeEntrada(*p, "decrescente", criarSequenciaDecrescente(1200)); delete p;
        p = instanciar(); bateriaOrdensDeEntrada(*p, "aleatoria", criarSequenciaAleatoria(1200, 77)); delete p;
        p = instanciar(); bateriaCicloAleatorio(*p); delete p;
    }

    cout << "\n----------------------------------------------------\n";
    cout << "Resultado Final: " << qtdVerificacoes << " testes executados | " << qtdFalhas << " erros.\n";
    cout << "----------------------------------------------------\n";

    cout << "\n[1] Degeneracao da BST Comum sem Balanceamento (N = 1200):\n";
    cout << "  - Entrada Crescente:   altura = " << calcularAlturaBSTSemBalanceamento(criarSequenciaCrescente(1200)) << "\n";
    cout << "  - Entrada Decrescente: altura = " << calcularAlturaBSTSemBalanceamento(criarSequenciaDecrescente(1200)) << "\n";
    cout << "  - Entrada Aleatoria:   altura = " << calcularAlturaBSTSemBalanceamento(criarSequenciaAleatoria(1200, 77)) << "\n";

    cout << "\n[2] Comparativo Direto (Mesmos Conjuntos de Dados):\n";
    for (int n : {1200, 80000}) {
        vector<int> seqC = criarSequenciaCrescente(n);
        vector<int> seqD = criarSequenciaDecrescente(n);
        vector<int> seqA = criarSequenciaAleatoria(n, 77);
        vector<int> loteRemocao;
        for (int i = 0; i < n; i += 2) loteRemocao.push_back(seqA[i]);
        cout << "\n>>> Carga N = " << n << " pacotes (seguida da remocao de " << loteRemocao.size() << " pacotes):\n";
        imprimirLinhaComparativa("Crescente", seqC, loteRemocao);
        imprimirLinhaComparativa("Decrescente", seqD, loteRemocao);
        imprimirLinhaComparativa("Aleatoria", seqA, loteRemocao);
    }

    cout << "\n[3] Benchmark de Tempo (500.000 insercoes + 500.000 buscas):\n";
    {
        int volume = 500000;
        vector<int> cargaI = criarSequenciaAleatoria(volume, 101);
        vector<int> cargaB = criarSequenciaAleatoria(volume, 202);
        ArvoreAVL avl;
        ArvoreRB rb;
        long long ok1 = 0, ok2 = 0;
        double msAvl = cronometrarExecucao(avl, cargaI, cargaB, ok1);
        double msRb = cronometrarExecucao(rb, cargaI, cargaB, ok2);
        cout << "  - AVL:         " << fixed << setprecision(0) << msAvl << " ms | Altura Final: " << avl.obterAltura() << " | Rotacoes: " << avl.contRotacoes << "\n";
        cout << "  - Rubro-Negra: " << msRb << " ms | Altura Final: " << rb.obterAltura() << " | Rotacoes: " << rb.contRotacoes << " | Recoloracoes: " << rb.contRecoloracoes << "\n";
    }

    return qtdFalhas == 0 ? 0 : 1;
}
