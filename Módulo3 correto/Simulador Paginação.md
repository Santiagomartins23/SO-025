# Simulador de Gerenciamento de Memória Virtual com Paginação

Projeto desenvolvido para a disciplina de Sistemas Operacionais (Módulo 3 - ICMC/USP), com o objetivo de simular o funcionamento da memória virtual com paginação, incluindo leitura de arquivos de entrada, alocação de processos, substituição de páginas e gerenciamento de faltas de página.

## 🧠 Funcionalidades

- Criação de processos com imagem em bytes
- Acesso à memória virtual com leitura (R) e escrita (W)
- Execução de instruções de CPU (P) e I/O (I)
- Substituição de páginas usando algoritmos **LRU** ou **Clock**
- Suspensão automática de processos via **swapper** em caso de falta de memória
- Registro de faltas de página, modificações e referências
- Simulação de memória secundária
- Saída detalhada com:
  - Memória principal
  - Tabelas de páginas
  - Estados dos processos
  - Resumo da simulação

## 📂 Estrutura de Entrada

Cada linha do arquivo de entrada representa uma instrução, nos seguintes formatos:


> A notação `(x)2` indica valor binário. O simulador converte para decimal automaticamente.

### 💡 Exemplo:
```txt
P1 C 500
P1 R (0)2
P1 W (1024)2
P2 C 1000
P2 R (4095)2
P2 I (2)2
``` 
⚙️ Configurações Internas
Você pode ajustar os seguintes parâmetros no main.cpp:

Tamanho da página (page_size)

Número de bits do endereço lógico (address_bits)

Tamanho da memória física

Tamanho da memória secundária

Algoritmo de substituição: LRU ou CLOCK

Exemplo:
cpp
Copiar
Editar
MemoryManager manager(
    4096,               // Page size: 4KB
    32,                 // Address bits
    65536,              // Physical memory: 64KB
    1048576,            // Secondary memory: 1MB
    MemoryManager::CLOCK // Replacement algorithm
);

🧪 Execução
#Compilação

`g++ -std=c++17 -o simulador simulador.cpp`

#Execução

`./simulador entrada.txt`

Exemplo com algoritmo via argumento (opcional)

#📊 Saída
Exibição da memória principal (quadros)

Tabela de páginas de cada processo

Estado de cada processo (ready, waiting_io, suspended)

Resumo final com estatísticas

#🏗️ Estruturas de Dados
Process: status, tamanho da imagem e tabela de páginas

Page: bits de referência, modificação e presença

Frame: representa um quadro da memória física

MemoryManager: controla toda a simulação

Algoritmos: LRU com lista de uso recente e Clock com ponteiro circular


