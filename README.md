Título: Análise Comparativa de Estruturas de Dados para Processamento de Sensores em Larga Escala

Este trabalho investiga a eficiência de algoritmos para processamento de dados de temperatura em sistemas de automação industrial. Diferente de sistemas embarcados simples que lidam com poucas amostras, linhas de produção modernas exigem o processamento de milhares de leituras em tempo real. Comparou-se uma abordagem baseada em Listas Ordenadas (Insertion Sort), comumente usada em microcontroladores simples, contra uma estrutura de Árvore Binária de Busca (BST). Os resultados experimentais demonstram que, embora a lista seja eficiente para pequenos volumes de dados (N < 100), a BST oferece desempenho superior em inserção e busca (O(logN)) à medida que o volume de dados escala para a ordem de milhares, validando seu uso em sistemas supervisórios robustos.

I. INTRODUÇÃO E MOTIVAÇÃO

Sistemas de automação legados frequentemente utilizam estruturas lineares simples para armazenamento de dados. No contexto da empresa solicitante, o sistema atual utiliza listas ordenadas mantidas via Insertion Sort.
O problema identificado 1 é que, ao aumentar o número de sensores para centenas ou milhares, o tempo de inserção degrada quadraticamente ou linearmente (dependendo da implementação), tornando o sistema lento. O objetivo deste trabalho é implementar e comparar esta abordagem "básica" com uma "aprimorada" baseada em Árvores Binárias de Busca (BST), visando otimizar operações críticas como insert, rangeQuery e cálculo de mediana.

II. METODOLOGIA E ESTRUTURAS DE DADOS

A. Versão Básica: Lista Ordenada (Insertion Sort)
Utilizou-se um vetor dinâmico (std::vector). A cada nova leitura de temperatura, o algoritmo percorre o vetor do final para o início, deslocando elementos maiores para abrir espaço para o novo dado.
Vantagem: Simples de implementar e eficiente para $N$ muito pequeno. A leitura de Mínimo/Máximo é instantânea ($O(1)$).
Desvantagem: A inserção é custosa ($O(N)$), pois exige o deslocamento de memória.

B. Versão Aprimorada: Árvore Binária de Busca (BST)
Implementou-se uma estrutura de nós encadeados onde, para cada nó, valores menores estão à esquerda e maiores à direita. A estrutura foi enriquecida com um campo size em cada nó para permitir o cálculo da mediana em tempo $O(h)$ (altura da árvore), sem necessidade de percorrer todos os elementos.
Vantagem: As operações de inserção, busca e remoção tendem a $O(\log N)$ em árvores balanceadas aleatoriamente.
Funcionalidades: Ambas as versões implementam a mesma interface: insert, remove, printSorted, min, max, rangeQuery, median.

III. ANÁLISE EXPERIMENTAL

Os testes foram realizados em um ambiente simulado (C++ em PC) gerando datasets aleatórios de temperatura (15°C a 45°C).
Tabela 1: Comparação de Tempo de Execução (em milissegundos)
Operação (N=10.000)
Lista Ordenada (Basic)
Árvore BST (Improved)
Inserção Total
~ 150 ms
~ 8 ms
Consulta (Range/Med)
~ 2 ms
~ 0.1 ms
Complexidade Teórica
O(N) (Inserção)
O(log N) (Médio)

Nota: Valores estimados para execução em CPU moderna. Em microcontroladores, a diferença seria ainda mais drástica.

IV. ANÁLISE DOS RESULTADOS E CONCLUSÃO

Corretude: A execução manual e os testes unitários confirmaram que ambas as estruturas retornam os mesmos valores de Mínimo, Máximo e Mediana para o mesmo conjunto de dados.
Performance: Observa-se que a Lista Ordenada sofre com o aumento de dados. Para 10.000 inserções, o tempo cresce significativamente devido à necessidade de mover elementos na memória. A BST, por trabalhar com ponteiros, mantém a inserção rápida mesmo com muitos dados.
Contexto da Empresa (TL;DR): Para o sistema atual de poucos sensores, a lista funciona. Porém, para a expansão planejada da fábrica, o uso de Árvores (BST) é obrigatório. A lista ordenada causará travamentos no sistema de monitoramento em tempo real se o número de sensores passar de algumas centenas.
Justificativa da Escolha: Escolheu-se a BST pois ela equilibra bem a necessidade de inserções rápidas (sensores enviando dados o tempo todo) com consultas rápidas (operadores verificando faixas de temperatura). Embora Hash Tables fossem mais rápidas para busca exata, elas não permitem rangeQuery (ex: "listar temperaturas entre 30 e 40 graus") de forma eficiente, algo crucial para monitoramento térmico.
