# Árvores Balanceadas: AVL e Rubro-Negra

Trabalho da Unidade II de IMD0039 - Estruturas de Dados Básicas II (UFRN / Instituto Metrópole Digital)

Autor: Rafael Medeiros Câmara da Silva - matrícula 20250067640
Repositório: https://github.com/rafaelcamara11/arvores-balanceadas

## O que foi implementado

- Árvore AVL e Árvore Rubro-Negra implementadas do zero em C++17 (sem bibliotecas prontas de balanceamento)
- Operações: inserir, remover, buscar, percurso em ordem, visualizar estrutura da árvore, consultar altura/métricas e validar propriedades
- Na AVL a impressão exibe o fator de balanceamento `[fb=...]`. Na Rubro-Negra exibe a cor de cada nó: `(V)` vermelho e `(P)` preto
- Chave duplicada: a inserção é recusada com aviso e a árvore permanece inalterada
- Busca ou remoção de chave inexistente: retorna falso sem alterar a árvore
- Rotação dupla é contabilizada como 2 rotações simples

## Estrutura de Pastas e Arquivos

- `src/arvore.hpp`: classe base abstrata (`ArvoreBase`) com interface comum e contadores
- `src/avl.hpp`: implementação da Árvore AVL (`ArvoreAVL`)
- `src/rb.hpp`: implementação da Árvore Rubro-Negra (`ArvoreRB`)
- `src/main.cpp`: programa interativo com menu no terminal (permite escolher AVL ou Rubro-Negra na execução)
- `src/testes.cpp`: suíte de testes funcionais (50 verificações) e benchmark comparativo
- `bin/`: versões compiladas (`arvores.exe` e `testes.exe` para Windows; `arvores` e `testes` para Linux)
- `Makefile`: script de compilação
- `resultado_testes.txt`: saída completa da bateria de testes

## Como compilar e executar

Os binários pré-compilados encontram-se na pasta `bin/`.

Para compilar a partir do código-fonte com `make` e `g++` (suporte a C++17):

```bash
make
./bin/arvores
./bin/testes
```

Ou compilando manualmente via terminal:

```bash
g++ -O2 -std=c++17 -o bin/arvores src/main.cpp
g++ -O2 -std=c++17 -o bin/testes src/testes.cpp
```
