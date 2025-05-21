# Simulador de Gerenciamento de Memória Virtual com Paginação

## 1. Introdução
Este projeto implementa um simulador de gerenciamento de memória virtual usando paginação, o código deste simulador foi denseolvido com a linguagem C++. O simulador permite:
- Submissão e execução simulada de processos
- Gerenciamento de memória virtual com paginação
- Implementação de algoritmos de substituição de páginas (LRU ou CLOCK)
- Visualização do estado da memória e processos

O sistema simula referências à memória (leituras e gravações) conforme especificado em arquivo de entrada, aplicando os mecanismos de memória virtual configuráveis.

###### Arquivo de entrada em txt :
O arquivo entrada.txt contém uma sequência de comandos que definem as operações a serem simuladas pelo sistema de gerenciamento de memória virtual. Cada linha representa uma ação específica de um processo, como criação, leitura, escrita ou uso de dispositivo de I/O.

Sintaxe Geral - PID AÇÃO (ENDEREÇO) DISPOSITIVO
---

PID: identificador do processo (ex: P1, P7)



AÇÃO:

C: criação do processo

R: leitura de memória (Read)

W: escrita na memória (Write)

P: instrução de CPU (Processamento)

I: operação de I/O (Input/Output)



(ENDEREÇO): endereço virtual (em bytes) acessado na operação de leitura ou escrita



DISPOSITIVO: número do dispositivo de I/O envolvido, quando aplicável
---

```
# entrada.txt
P1 C 500
P1 R (0)2
P1 R (1024)2
P1 P  (1)2
P1 R (2)2
P1 P (2)2
P1 W  (1024)2
P7 C 1000
P7 R (4095)2
P7 R  (800)2
P7 I  (2)2
P7 R (801)2
P7 W  (4096)2
P1 R (3)2
P1 R  (4)2
P1 W (1025)2
P1 W  (1026)2
```
# EXPLICAR OS PARAMETROS DE ENTRADA TXT

Mecanismos configuráveis na main :
```c++
MemoryManager manager(
            4096,     // Page size (4KB)
            32,       // Address bits (32-bit)
            65536,    // Physical memory (64KB)
            1048576,  // Secondary memory (1MB)
            MemoryManager::AlgoritimoSelecionado  // Replacement algorithm
        );
```
# EXPLICAR A ESTRUTURA DA MAIN 

## 2. Estruturas de Dados Principais
### Estruturas para Gerenciamento de Memória
```c++
struct Page {
        int page_id;
        int process_id;
        bool referenced;
        bool modified;
        time_t last_used;
        bool present;
        int frame;
    };
```
# EXPLICAR

```c++
struct Frame {
        int frame_id;
        bool allocated;
        int page_id;
        int process_id;
    };
```

# EXPLICAR

```c++
struct Process {
        int process_id;
        int size;
        std::string status;
        std::unordered_map<int, Page> page_table;
        int swap_file_id;
    };
```

# EXPLICAR

```c++
struct MemoryOperation {
        int process_id;
        char operation_type; // 'R', 'W', 'P', 'I', 'C'
        unsigned long address;
        int size;
        std::string device;
    };
```

# EXPLICAR

# Métodos Implementados
## Principais Componentes
### Gerenciador de Memória

-Alocação/desalocação de páginas

-Tratamento de page faults

-Swapping entre memória principal e secundária

### Algoritmos de Substituição

-LRU (Least Recently Used)

-Relógio (implementação bônus)

### Simulador

-Leitura e interpretação do arquivo de entrada

-Simulação da execução dos processos

-Geração de saídas e estatísticas

## Fluxo Principal
-Configuração inicial dos parâmetros de memória

-Leitura do arquivo de entrada com sequência de operações

## Para cada operação:

-Processa criação de processos (tag 'C')

-Executa referências à memória (tags 'R'/'W')

-Processa instruções de CPU/I/O (tags 'P'/'I')

-Atualiza estruturas de dados e estatísticas

-Exibe resultados finais
