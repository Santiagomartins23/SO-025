# Simulador de Gerenciamento de Memória Virtual com Paginação

## 1. Introdução
Este projeto implementa um simulador de gerenciamento de memória virtual usando paginação, o código deste simulador foi denseolvido com a linguagem C++. O simulador permite:
- Submissão e execução simulada de processos
- Gerenciamento de memória virtual com paginação
- Implementação de algoritmos de substituição de páginas (LRU ou CLOCK)
- Visualização do estado da memória e processos

O sistema simula referências à memória (leituras e gravações) conforme especificado em arquivo de entrada, aplicando os mecanismos de memória virtual configuráveis.

---

### Arquivo de entrada em txt :

O arquivo entrada.txt contém uma sequência de comandos que definem as operações a serem simuladas pelo sistema de gerenciamento de memória virtual. Cada linha representa uma ação específica de um processo, como criação, leitura, escrita ou uso de dispositivo de I/O.

##### Sintaxe Geral - PID AÇÃO (ENDEREÇO) DISPOSITIVO



PID: identificador do processo (ex: P1, P7)



AÇÃO:

C: criação do processo

R: leitura de memória (Read)

W: escrita na memória (Write)

P: instrução de CPU (Processamento)

I: operação de I/O (Input/Output)



(ENDEREÇO): endereço virtual (em bytes) acessado na operação de leitura ou escrita



DISPOSITIVO: número do dispositivo de I/O envolvido, quando 




```
# entrada.txt
P1 C 500
// Cria o processo 1 com 500 bytes de memória virtual (aproximadamente 1 página, se o tamanho da página for 1 KB).

P1 R (0)2
// O processo 1 realiza uma leitura de memória no endereço 0 (início da sua memória virtual). O número 2 indica um ID fictício de dispositivo usado para organização (não afeta leitura).

P1 R (1024)2
P1 P  (1)2
// Instrução de processamento da CPU pelo processo 1 (simula uso de CPU por um tempo).

P1 R (2)2
P1 P (2)2
P1 W  (1024)2
// Escrita no endereço 1024. Isso ativa o bit de modificação da página.

P7 C 1000
P7 R (4095)2
P7 R  (800)2
P7 I  (2)2
// Simulação de operação de I/O. O processo 7 entra em estado de espera (waiting_io), como se estivesse interagindo com um dispositivo (ID 2).

P7 R (801)2
P7 W  (4096)2
P1 R (3)2
P1 R  (4)2
P1 W (1025)2
P1 W  (1026)2
```
---

A configuração do simulador é realizada na função main por meio da criação de um objeto da classe MemoryManager. Esse objeto define os principais parâmetros do sistema de memória virtual a ser simulado. A seguir, mostramos o trecho de código responsável por essa configuração e explicamos cada um dos seus argumentos:



```c++
MemoryManager manager(
            4096,     // Page size (4KB)
            32,       // Address bits (32-bit)
            65536,    // Physical memory (64KB)
            1048576,  // Secondary memory (1MB)
            MemoryManager::AlgoritimoSelecionado  // Replacement algorithm
        );
```
| Parâmetro               |         | Valor                              |         | Descrição                                                                                                                                      |
|-------------------------|---------|------------------------------------|---------|------------------------------------------------------------------------------------------------------------------------------------------------|
| `pageSize`              |         | `4096`                             |         | Tamanho de cada página de memória virtual, em bytes. Neste caso, 4 KB por página.                                                              |
| `addressBits`           |         | `32`                               |         | Número de bits utilizados nos endereços virtuais. Um endereço de 32 bits permite endereçar até 4 GB.                                          |
| `physicalMemorySize`    |         | `65536`                            |         | Tamanho total da memória física (RAM simulada), em bytes. Neste exemplo, 64 KB.                                                               |
| `secondaryMemorySize`   |         | `1048576`                          |         | Tamanho da memória secundária (por exemplo, swap em disco), em bytes. Aqui, 1 MB.                                                              |
| `replacementAlgorithm`  |         | `MemoryManager::AlgoritimoSelecionado` |     | Algoritmo de substituição de páginas utilizado quando a memória física estiver cheia. Pode ser `LRU` (Least Recently Used) ou `CLOCK`. |

###### Obs: O último parâmetro (AlgoritimoSelecionado) é uma enumeração definida na classe MemoryManager que permite selecionar o algoritmo desejado:

```c++
enum Algoritmo {
    LRU,
    CLOCK
};
```
Para utilizar, por exemplo, o algoritmo CLOCK, basta alterar o último argumento da criação do gerenciador:
```c++
MemoryManager::CLOCK
```
Ou, para usar LRU:

```c++
MemoryManager::LRU
```


---




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
