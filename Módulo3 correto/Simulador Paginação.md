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
GerenciadorMemoria gerenciador(
            4096,     // Tamanho da página (4KB)
            32,       // Bits de endereço (32-bit)
            65536,    // Memória física (64KB)
            1048576,  // Memória secundária (1MB)
            GerenciadorMemoria::AlgoritimoSelecionado  // Algoritmo de substituição
        );
```
| Parâmetro               |         | Valor                              |         | Descrição                                                                                                                                      |
|-------------------------|---------|------------------------------------|---------|------------------------------------------------------------------------------------------------------------------------------------------------|
| `Tamanho da página`              |         | `4096`                             |         | Tamanho de cada página de memória virtual, em bytes. Neste caso, 4 KB por página.                                                              |
| `Bits de endereço`           |         | `32`                               |         | Número de bits utilizados nos endereços virtuais. Um endereço de 32 bits permite endereçar até 4 GB.                                          |
| `Memória física`    |         | `65536`                            |         | Tamanho total da memória física (RAM simulada), em bytes. Neste exemplo, 64 KB.                                                               |
| `Memória secundária`   |         | `1048576`                          |         | Tamanho da memória secundária (por exemplo, swap em disco), em bytes. Aqui, 1 MB.                                                              |
| `Algoritmo de substituição`  |         | `GerenciadorMemoria::AlgoritimoSelecionado` |     | Algoritmo de substituição de páginas utilizado quando a memória física estiver cheia. Pode ser `LRU` (Least Recently Used) ou `CLOCK`. |

##### Obs: O último parâmetro (AlgoritimoSelecionado) é uma enumeração definida na classe MemoryManager que permite selecionar o algoritmo desejado:

```c++
enum Algoritmo {
    LRU,
    CLOCK
};
```
Para utilizar, por exemplo, o algoritmo CLOCK, basta alterar o último argumento da criação do gerenciador:
```c++
 GerenciadorMemoria::CLOCK
```
Ou, para usar LRU:

```c++
 GerenciadorMemoria::LRU
```


---




## 2. Estruturas de Dados Principais
### Estruturas para Gerenciamento de Memória
```c++
struct Pagina {
        int id_pagina;
        int id_processo;
        bool referenciada;
        bool modificada;
        time_t ultimo_acesso;
        bool presente;
        int frame;
    };
```
###### A estrutura Pagina representa uma página da memória virtual de um processo. Ela contém diversos atributos que facilitam o gerenciamento e o rastreamento do uso da página no sistema. O campo page_id identifica unicamente a página dentro do processo, enquanto process_id indica a qual processo essa página pertence. O atributo referenced sinaliza se a página foi acessada recentemente, sendo utilizado por algoritmos como o CLOCK. O campo modified indica se a página foi alterada (escrita), o que é essencial para saber se ela precisa ser salva em disco antes de ser substituída. O campo last_used armazena um timestamp da última utilização da página, útil para o algoritmo LRU. Já o campo present mostra se a página está carregada na memória física ou se está na memória secundária. Por fim, frame indica em qual quadro de memória física (frame) a página está mapeada, caso esteja presente.

```c++
struct Frame {
        int id_frame;
        bool alocado;
        int id_pagina;
        int id_processo;
    };
```

###### A estrutura Frame representa um quadro (ou moldura) de memória física, que pode conter uma única página por vez. Cada frame possui um identificador frame_id, além de um campo booleano allocated que indica se o quadro está atualmente em uso. Também há os campos page_id e process_id, que apontam qual página está atualmente alocada naquele frame e a qual processo ela pertence. Essa estrutura é fundamental para gerenciar o uso da memória física e realizar substituições de páginas de forma eficiente.

```c++
struct Processo {
        int id_processo;
        int tamanho;
        std::string estado;
        std::unordered_map<int, Pagina> tabela_paginas;
        int id_arquivo_swap;
    };
```

###### A estrutura Processo descreve um processo ativo no sistema. O campo process_id identifica o processo, enquanto size representa o tamanho total da memória requisitada por ele. O campo status descreve o estado atual do processo (como "executando", "esperando", etc.). A page_table é uma tabela de páginas representada por um unordered_map, que associa os índices das páginas virtuais às suas respectivas estruturas Page, permitindo acesso rápido às informações de mapeamento. O campo swap_file_id indica o identificador do arquivo de swap associado ao processo, utilizado quando as páginas são movidas para a memória secundária.

```c++
struct OperacaoMemoria {
        int id_processo;
        char tipo_operacao; // 'R', 'W', 'P', 'I', 'C'
        unsigned long endereco;
        int tamanho;
        std::string dispositivo;
    };
```

###### Por fim, a estrutura OperacaoMemoria define uma operação de memória solicitada por um processo. Ela contém o process_id para identificar o processo que fez a solicitação e o operation_type, que pode ser 'R' (read/leitura), 'W' (write/escrita), 'P' (page fault/solicitação explícita de página), 'I' (I/O) ou 'C' (create/criação do processo). O campo address representa o endereço lógico acessado, enquanto size indica o tamanho da operação em bytes. O campo device, quando aplicável, identifica o dispositivo envolvido na operação, como operações de I/O. Essa estrutura é usada para simular o comportamento real de acesso à memória durante a execução de programas.

# Métodos Implementados
## Principais Componentes
### Gerenciador de Memória

-Alocação/desalocação de páginas

-Tratamento de page faults

-Swapping entre memória principal e secundária

---

### Algoritmos de Substituição 🔁
Algoritmos de substituição de páginas são estratégias utilizadas pelo sistema de gerenciamento de memória virtual para decidir qual página deve ser removida da memória física quando é necessário carregar uma nova página, mas não há quadros (frames) livres disponíveis. A escolha de qual página será substituída impacta diretamente o desempenho do sistema, especialmente em cenários com alta taxa de falta de página (page fault). Nosso simulador oferece duas opções de algoritmos de substituição:

- #### LRU (Least Recently Used):
Esse algoritmo remove a página que está na memória mas que não foi usada há mais tempo. A ideia por trás do LRU é que páginas usadas recentemente provavelmente serão usadas novamente em breve, enquanto páginas que não são acessadas há algum tempo podem não ser mais necessárias. No nosso código, esse comportamento é implementado por meio do campo last_used em cada página, que armazena o timestamp do último acesso. Apesar de sua precisão na escolha da página a ser substituída, o LRU possui custo computacional elevado. A manutenção das informações de uso requer atualizações frequentes e ordenações ou estruturas auxiliares

- #### Relógio (implementação bônus):
 Esta é uma implementação otimizada do algoritmo Segunda Chance, que simula um ponteiro girando como em um relógio. Cada página possui um bit de referência (referenced); quando uma substituição é necessária, o algoritmo percorre as páginas em ordem circular. Se encontrar uma página com o bit de referência 0, ela é removida. Se o bit for 1, o algoritmo o zera e passa para a próxima página, dando uma “segunda chance” àquela página. Essa abordagem equilibra desempenho e simplicidade, consumindo menos recursos do que o LRU completo. Embora seja mais eficiente em termos de desempenho e simplicidade em comparação com o LRU, o algoritmo de Relógio pode tomar decisões menos precisas, já que o bit de referência oferece uma noção mais genérica do uso recente da página.

---

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
