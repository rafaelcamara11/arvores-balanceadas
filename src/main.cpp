#include "avl.hpp"
#include "rb.hpp"

int main() {
    cout << "=== Prototipo de Logistica Amazon ===\n";
    cout << "Selecione a estrutura:\n1 - Arvore AVL\n2 - Arvore Rubro-Negra\nOpcao: ";
    int esc;
    if (!(cin >> esc)) return 0;

    ArvoreBase* estrutura = nullptr;
    if (esc == 1) estrutura = new ArvoreAVL();
    else if (esc == 2) estrutura = new ArvoreRB();
    else {
        cout << "Estrutura invalida.\n";
        return 1;
    }

    cout << "Modo selecionado: " << estrutura->nomeEstrutura() << "\n";

    int cmd = -1;
    while (cmd != 0) {
        cout << "\n[1] Inserir ID\n[2] Remover ID\n[3] Buscar ID\n[4] Percurso em ordem\n[5] Visualizar arvore\n[6] Ver metricas\n[7] Checar balanceamento\n[0] Encerrar\nEscolha: ";
        if (!(cin >> cmd)) break;
        int idPacote;
        if (cmd == 1) {
            cout << "ID da encomenda: ";
            cin >> idPacote;
            if (estrutura->adicionar(idPacote)) cout << "Pacote " << idPacote << " registrado.\n";
            else cout << "Aviso: ID " << idPacote << " duplicado (recusado).\n";
        } else if (cmd == 2) {
            cout << "ID da encomenda: ";
            cin >> idPacote;
            if (estrutura->excluir(idPacote)) cout << "Pacote " << idPacote << " removido.\n";
            else cout << "Aviso: ID " << idPacote << " inexistente.\n";
        } else if (cmd == 3) {
            cout << "ID da encomenda: ";
            cin >> idPacote;
            if (estrutura->contem(idPacote)) cout << "ID " << idPacote << " localizado no sistema.\n";
            else cout << "ID " << idPacote << " nao encontrado.\n";
        } else if (cmd == 4) {
            vector<int> elementos;
            estrutura->listarEmOrdem(elementos);
            if (elementos.empty()) cout << "(nenhum pacote registrado)";
            for (int v : elementos) cout << v << " ";
            cout << "\n";
        } else if (cmd == 5) {
            estrutura->exibirArvore();
        } else if (cmd == 6) {
            cout << "Total de pacotes: " << estrutura->obterTotal()
                 << "\nAltura da arvore: " << estrutura->obterAltura()
                 << "\nTotal de rotacoes: " << estrutura->contRotacoes
                 << "\nTotal de recoloracoes: " << estrutura->contRecoloracoes << "\n";
        } else if (cmd == 7) {
            cout << (estrutura->checarBalanceamento() ? "Invariantes preservadas (OK)\n" : "Falha nas propriedades!\n");
        }
    }

    delete estrutura;
    return 0;
}
