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

#### ✔ Alocação/desalocação de páginas
Objetivo: Mapear páginas virtuais (de processos) em frames da memória física.

Métodos envolvidos:
`alocar_frame()`: Encontra um frame livre ou seleciona uma vítima para substituição.
`alocar_frame_para_pagina()`: Associa uma página a um frame físico.
`swap_out_processo()`: Remove um processo inteiro da memória para liberar espaço (swapping).

#### ✔ Tratamento de page faults
Ocorre quando: Um processo tenta acessar uma página que não está na memória física.

Ações tomadas:
Identifica a página faltante.
Se houver frames livres, carrega a página neles.
Caso contrário, escolhe uma vítima (usando LRU ou Relógio) e a substitui.

#### ✔ Swapping entre memória principal e secundária
Objetivo: Movimentar páginas entre a RAM e o disco (simulado) quando a memória está cheia.

Dois tipos de operações:
Swap-out (para disco): Salva páginas modificadas (modificada = true) no "arquivo de swap" e
Swap-in (para RAM): Carrega páginas do disco para a memória física.

---

### Algoritmos de Substituição 🔁
Algoritmos de substituição de páginas são estratégias utilizadas pelo sistema de gerenciamento de memória virtual para decidir qual página deve ser removida da memória física quando é necessário carregar uma nova página, mas não há quadros (frames) livres disponíveis. A escolha de qual página será substituída impacta diretamente o desempenho do sistema, especialmente em cenários com alta taxa de falta de página (page fault). Nosso simulador oferece duas opções de algoritmos de substituição:

- #### LRU (Least Recently Used):
Esse algoritmo remove a página que está na memória mas que não foi usada há mais tempo. A ideia por trás do LRU é que páginas usadas recentemente provavelmente serão usadas novamente em breve, enquanto páginas que não são acessadas há algum tempo podem não ser mais necessárias. No nosso código, esse comportamento é implementado por meio do campo last_used em cada página, que armazena o timestamp do último acesso. Apesar de sua precisão na escolha da página a ser substituída, o LRU possui custo computacional elevado. A manutenção das informações de uso requer atualizações frequentes e ordenações ou estruturas auxiliares

- #### Relógio (implementação bônus):
 Esta é uma implementação otimizada do algoritmo Segunda Chance, que simula um ponteiro girando como em um relógio. Cada página possui um bit de referência (referenced); quando uma substituição é necessária, o algoritmo percorre as páginas em ordem circular. Se encontrar uma página com o bit de referência 0, ela é removida. Se o bit for 1, o algoritmo o zera e passa para a próxima página, dando uma “segunda chance” àquela página. Essa abordagem equilibra desempenho e simplicidade, consumindo menos recursos do que o LRU completo. Embora seja mais eficiente em termos de desempenho e simplicidade em comparação com o LRU, o algoritmo de Relógio pode tomar decisões menos precisas, já que o bit de referência oferece uma noção mais genérica do uso recente da página.

---
# Instrução de Execução 

## Estrutura de Arquivos

- main.cpp (ou nome do arquivo principal do simulador)
- Outros arquivos de código-fonte e cabeçalho (.cpp, .h)
- Arquivo de entrada de operações: entrada.txt

### Execução

- `./main entrada.txt 0` - Algoritimo Relógio
- `./main entrada.txt 1` - LRU

# Resultado e Análise da entrada formato TXT

## A saída do código está formatada da seguinte maneira :

```
Processo x criado com  n páginas 

Estado da Memória Física:
Frame | Alloc | PID  | Page | Ref | Mod 
------|-------|------|------|-----|----
    0 |    N  |      |      |     |
    1 |    N  |      |      |     |
    2 |    N  |      |      |     |
    3 |    N  |      |      |     |

//OBS* alloc = alocado | PID = ID do processo | page = página | ref = referenciado | mod = modificado |

Estado do Processo x:
Tamanho: ... bytes | Estado: ...

Tabela de Páginas:
Page | Present | Frame | Ref | Mod | Last Used
-----|---------|-------|-----|-----|----------
   0 |    N    |    -1 |  N |  N | 23:27:42
----------------------------------------
//Obs* present = presente na memória | last used = última vez usado |

Carregando página y do processo x dentro do frame z

```
P1 C 500
```
Processo 1 criado com 1 páginas

Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   N  |      |      |     |
    1 |   N  |      |      |     |
    2 |   N  |      |      |     |
    3 |   N  |      |      |     |

Estado do Processo 1:
Tamanho: 500 bytes | Estado: pronto

Tabela de Paginas do Processo 1:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   0 |     N    |     -1 |  N |  N | 00:09:12
----------------------------------------
Carregando página 0 do processo 1 no Frame 0

```
P1 R (0)2
```
Leitura de memória no endereço 0 (página 0, frame 0)

Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    1 |    0 |  S |  N
    1 |   N  |      |      |     |
    2 |   N  |      |      |     |
    3 |   N  |      |      |     |

Estado do Processo 1:
Tamanho: 500 bytes | Estado: pronto

Tabela de Paginas do Processo 1:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   0 |     S    |      0 |  S |  N | 00:09:12
----------------------------------------

```
P1 R (1024)2
```
Leitura de memória no endereço 1024 (página 0, frame 0)

Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    1 |    0 |  S |  N
    1 |   N  |      |      |     |
    2 |   N  |      |      |     |
    3 |   N  |      |      |     |

Estado do Processo 1:
Tamanho: 500 bytes | Estado: pronto

Tabela de Paginas do Processo 1:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   0 |     S    |      0 |  S |  N | 00:09:12
----------------------------------------

```
P1 P  (1)2
```
Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    1 |    0 |  S |  N
    1 |   N  |      |      |     |
    2 |   N  |      |      |     |
    3 |   N  |      |      |     |

Estado do Processo 1:
Tamanho: 500 bytes | Estado: pronto

Tabela de Paginas do Processo 1:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   0 |     S    |      0 |  S |  N | 00:09:12
----------------------------------------
```
P1 R (2)2
```
Processo 1 executando instrução CPU:

Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    1 |    0 |  S |  N
    1 |   N  |      |      |     |
    2 |   N  |      |      |     |
    3 |   N  |      |      |     |

Estado do Processo 1:
Tamanho: 500 bytes | Estado: pronto

Tabela de Paginas do Processo 1:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   0 |     S    |      0 |  S |  N | 00:09:12
----------------------------------------
```
P1 P (2)2
```
Processo 1 executando instrução CPU:

Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    1 |    0 |  S |  N
    1 |   N  |      |      |     |
    2 |   N  |      |      |     |
    3 |   N  |      |      |     |

Estado do Processo 1:
Tamanho: 500 bytes | Estado: pronto

Tabela de Paginas do Processo 1:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   0 |     S    |      0 |  S |  N | 00:17:41
----------------------------------------
```
P1 W  (1024)2
```
Escrita de memória no endereço 1024 (página 0, frame 0)

Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    1 |    0 |  S |  S
    1 |   N  |      |      |     |
    2 |   N  |      |      |     |
    3 |   N  |      |      |     |

Estado do Processo 1:
Tamanho: 500 bytes | Estado: pronto

Tabela de Paginas do Processo 1:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   0 |     S    |      0 |  S |  S | 00:17:41
----------------------------------------
```
P7 C 1000
```
Processo 7 criado com 1 páginas

Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    1 |    0 |  S |  S
    1 |   N  |      |      |     |
    2 |   N  |      |      |     |
    3 |   N  |      |      |     |

Estado do Processo 7:
Tamanho: 1000 bytes | Estado: pronto

Tabela de Paginas do Processo 7:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   0 |     N    |     -1 |  N |  N | 00:26:07
----------------------------------------
Carregando página 0 do processo 7 no Frame 1
```
P7 R (4095)2
```
Leitura de memória no endereço 4095 (página 0, frame 1)

Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    1 |    0 |  S |  S
    1 |   S  |    7 |    0 |  S |  N
    2 |   N  |      |      |     |
    3 |   N  |      |      |     |

Estado do Processo 7:
Tamanho: 1000 bytes | Estado: pronto

Tabela de Paginas do Processo 7:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   0 |     S    |      1 |  S |  N | 00:26:07
----------------------------------------
```
P7 R  (800)2
```
Leitura de memória no endereço 800 (página 0, frame 1)

Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    1 |    0 |  S |  S
    1 |   S  |    7 |    0 |  S |  N
    2 |   N  |      |      |     |
    3 |   N  |      |      |     |

Estado do Processo 7:
Tamanho: 1000 bytes | Estado: pronto

Tabela de Paginas do Processo 7:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   0 |     S    |      1 |  S |  N | 00:26:07
----------------------------------------
```
P7 I  (2)2
```
Processo 7 realizando I/O no dispositivo: 2

Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    1 |    0 |  S |  S
    1 |   S  |    7 |    0 |  S |  N
    2 |   N  |      |      |     |
    3 |   N  |      |      |     |

Estado do Processo 7:
Tamanho: 1000 bytes | Estado: esperando_io

Tabela de Paginas do Processo 7:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   0 |     S    |      1 |  S |  N | 00:26:07
----------------------------------------
```
P7 R (801)2
```
Leitura de memória no endereço 801 (página 0, frame 1)

Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    1 |    0 |  S |  S
    1 |   S  |    7 |    0 |  S |  N
    2 |   N  |      |      |     |
    3 |   N  |      |      |     |

Estado do Processo 7:
Tamanho: 1000 bytes | Estado: esperando_io

Tabela de Paginas do Processo 7:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   0 |     S    |      1 |  S |  N | 00:26:07
----------------------------------------
```
P7 W  (4096)2
```
Erro ao processar linha: P7 W (4096)2
  Motivo: Número de página inválido 1 para processo 7
```
P1 R (3)2
```
Leitura de memória no endereço 3 (página 0, frame 0)

Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    1 |    0 |  S |  S
    1 |   S  |    7 |    0 |  S |  N
    2 |   N  |      |      |     |
    3 |   N  |      |      |     |

Estado do Processo 1:
Tamanho: 500 bytes | Estado: pronto

Tabela de Paginas do Processo 1:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   0 |     S    |      0 |  S |  S | 00:26:07
----------------------------------------
```
P1 R  (4)2
```
Leitura de memória no endereço 4 (página 0, frame 0)

Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    1 |    0 |  S |  S
    1 |   S  |    7 |    0 |  S |  N
    2 |   N  |      |      |     |
    3 |   N  |      |      |     |

Estado do Processo 1:
Tamanho: 500 bytes | Estado: pronto

Tabela de Paginas do Processo 1:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   0 |     S    |      0 |  S |  S | 00:26:07
----------------------------------------
```
P1 W (1025)2
```
Escrita de memória no endereço 1025 (página 0, frame 0)

Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    1 |    0 |  S |  S
    1 |   S  |    7 |    0 |  S |  N
    2 |   N  |      |      |     |
    3 |   N  |      |      |     |

Estado do Processo 1:
Tamanho: 500 bytes | Estado: pronto

Tabela de Paginas do Processo 1:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   0 |     S    |      0 |  S |  S | 00:26:07
----------------------------------------
```
P1 W  (1026)2
```
Escrita de memória no endereço 1026 (página 0, frame 0)

Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    1 |    0 |  S |  S
    1 |   S  |    7 |    0 |  S |  N
    2 |   N  |      |      |     |
    3 |   N  |      |      |     |

Estado do Processo 1:
Tamanho: 500 bytes | Estado: pronto

Tabela de Paginas do Processo 1:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   0 |     S    |      0 |  S |  S | 00:26:07
----------------------------------------

```

Resumo da Simulacao:
- Total de faltas de página: 0
- Total de operações de swap: 2
- Processos ativos: 2

Memoria Secundaria - Simulada
- Operações de swap ate agora: 2

# Testes e Validação

## Objetivo dos Testes
Esta seção tem como objetivo validar o funcionamento correto do simulador de gerenciamento de memória virtual, garantindo que os algoritmos de paginação, alocação, substituição e swap estejam operando conforme esperado.

## 3.2. Estratégia de Testes
Foram definidos diferentes cenários de teste para cobrir os principais casos de uso do simulador:

Criação de processos com diferentes tamanhos de memória
Execução de operações de leitura e escrita em endereços distintos
Ocorrência e tratamento de page faults
Ação dos algoritmos de substituição (LRU e CLOCK)
Operações de swap-in e swap-out
Situações de limite, como falta de frames livres

## 3.3. Casos de Teste

### Caso 1: Page Fault

Arquivo de entrada:

P1 C 5000  (Criação do processo 1 com 5000 bytes de memória)

P1 R (0)1  (Leitura no endereço virtual 0, no processo 1)

P1 R (4096)1  (Leitura no endereço virtual 4096, ainda do processo 1)

#### Situação real: 
```
Processo 1 criado com 2 páginas

Estado da Memória Física:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   N  |
    1 |   N  |
    2 |   N  |
    3 |   N  |

Estado do Processo 1:
Tamanho: 5000 bytes | Estado: pronto

Tabela de Paginas do Processo 1:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   1 |     N    |     -1 |  N |  N | 18:32:07
   0 |     N    |     -1 |  N |  N | 18:32:07
----------------------------------------

=== PAGE FAULT DETECTADO ===
Processo: 1
Página: 0
Alocando no Frame: -1

Carregando página 0 do processo 1 no Frame 0
Leitura de memória no endereço 0 (página 0, frame 0)

Estado da Memória Física:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    1 |    0 |  S |  N
    1 |   N  |
    2 |   N  |
    3 |   N  |

Estado do Processo 1:
Tamanho: 5000 bytes | Estado: pronto

Tabela de Paginas do Processo 1:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   1 |     N    |     -1 |  N |  N | 18:32:07
   0 |     S    |      0 |  S |  N | 18:32:07
----------------------------------------

=== PAGE FAULT DETECTADO ===
Processo: 1
Página: 1
Alocando no Frame: -1

Carregando página 1 do processo 1 no Frame 1
Leitura de memória no endereço 4096 (página 1, frame 1)

Estado da Memória Física:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    1 |    0 |  S |  N
    1 |   S  |    1 |    1 |  S |  N
    2 |   N  |
    3 |   N  |

Estado do Processo 1:
Tamanho: 5000 bytes | Estado: pronto

Tabela de Paginas do Processo 1:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   1 |     S    |      1 |  S |  N | 18:32:07
   0 |     S    |      0 |  S |  N | 18:32:07
----------------------------------------

Resumo da Simulacao:
Total de faltas de página: 2
Total de operações de swap: 2
Processos ativos: 1

[Memoria Secundaria - Simulada]
Operações de swap ate agora: 2
```

#### Situação real explicada:
##### 🗂️ Alocação Inicial:
 - Como cada página tem 4 KB (4096 bytes), o processo de 5000 bytes ocupa duas páginas:

            Página 0: endereços de 0 até 4095.

            Página 1: endereços de 4096 até 4999.

Nenhuma página está inicialmente carregada na memória física. O sistema começa com todos os frames livres.

##### 📌 Primeira Acesso: P1 R (0)1
O endereço virtual 0 pertence à página 0.

Como a página 0 ainda não está na memória, ocorre um Page Fault.

#### 🔁 Page Fault:
- O sistema detecta a ausência da página 0 na memória.
- Aloca a página 0 no frame 0.
- Marca essa página como referenciada (Ref = S) e não modificada (Mod = N), pois é uma leitura.
- Atualiza a tabela de páginas do processo e o estado da memória física.

##### 📌 Segundo Acesso: P1 R (4096)1
- O endereço 4096 está na página 1.
Página 1 ainda não está carregada → novo Page Fault

#### 🔁 Page Fault novamente:
Alocação da página 1 no frame 1.
Marca como referenciada (Ref = S) e não modificada (Mod = N).
Agora ambas as páginas do processo estão na memória.


###### 🎓 Conclusão:

Este exemplo simula **dois page faults** sequenciais gerados por um processo que acessa duas páginas distintas **não carregadas na memória**. Ele ilustra perfeitamente o funcionamento do mecanismo de paginação por demanda em sistemas operacionais modernos:

- O sistema só carrega uma página quando ela é acessada.

- Cada acesso a uma página não presente causa uma interrupção (page fault), que exige a alocação de um frame e, se necessário, o uso da memória secundária (swap).

- A política de alocação inicial é simples (os primeiros frames livres são usados), e os estados das páginas (presença, referência, modificação) são atualizados com precisão.


### Caso 2: Swap-out de processo

 Entrada:
A entrada será o arquivo.txt, porém com algumas modificações:

P1 C 2048

P7 C 8192

P7 W (4099)2 (Nesse passo a tabela de frames do espaço físico estará cheia, assim necessitando um swap-out para que essa nova página criada seja alocada por um frame)

 Esperado:
Quando não houver mais frames, um processo pode ser removido (swap-out) para liberar espaço.


### Situação real usando algoritmo LRU:

Estado da Memória Fisica antes do passo P7 W (4099)2 (Primeira substituição), a Memória Física está cheia e não tem página do endereço 4099 carregada na Memória Física.

```
Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    1 |    0 |  S |  N
    1 |   S  |    1 |    1 |  S |  S
    2 |   S  |    7 |    3 |  S |  N
    3 |   S  |    7 |    0 |  S |  N

```

P7 W (4099)2:
```
Escrita de memória no endereço 4099 (página 4, frame 0)

Carregando página 4 do processo 7 no Frame 0
Falta de página - Frame 0 substituído

Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    7 |    4 |  S |  S
    1 |   S  |    1 |    1 |  S |  S
    2 |   S  |    7 |    3 |  S |  N
    3 |   S  |    7 |    0 |  S |  N

Estado do Processo 7:
Tamanho: 8192 bytes | Estado: esperando_io

Tabela de Paginas do Processo 7:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   7 |     N    |     -1 |  N |  N | 15:23:19
   6 |     N    |     -1 |  N |  N | 15:23:19
   5 |     N    |     -1 |  N |  N | 15:23:19
   4 |     S    |      0 |  S |  S | 15:23:19
   3 |     S    |      2 |  S |  N | 15:23:19
   2 |     N    |     -1 |  N |  N | 15:23:19
   1 |     N    |     -1 |  N |  N | 15:23:19
   0 |     S    |      3 |  S |  N | 15:23:19
----------------------------------------
```
Com a Memória Física cheia, o algoritmo LRU procura e escolhe o frame que foi menos recentemente usado, ou seja, o que está mais tempo sem ser usado. Nesse caso, o frame 0 que carregava a página 0 do P1
foi usado pela últimam vez e (P1 W (2)), sendo assim, ela é transferida para a memória secundária e substituída pela página 4 do processo P7.


P1 W (3)2:
```
Leitura de memória no endereço 3 (página 0, frame 1)

Salvando página 1 do processo 1 do Frame 1 para armazenamento secundário
Carregando página 0 do processo 1 no Frame 1
Falta de página - Frame 1 substituído

Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    7 |    4 |  S |  S
    1 |   S  |    1 |    0 |  S |  N
    2 |   S  |    7 |    3 |  S |  N
    3 |   S  |    7 |    0 |  S |  N

Estado do Processo 1:
Tamanho: 2048 bytes | Estado: pronto

Tabela de Paginas do Processo 1:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   1 |     N    |     -1 |  N |  S | 15:23:19
   0 |     S    |      1 |  S |  N | 15:23:19
----------------------------------------
```
O acesso ao endereço 3 do processo P1 corresponde à página 0. No momento do acesso, a página 0 não estava presente na memória, causando um page fault. O LRU seleciona a página que ficou mais tempo sem ser usada.
Nesse caso, a página 1 do processo 1, localizada no frame 1, foi a menos recentemente usada e, portanto, foi substituída.

P1 W (1025)2:
```
Escrita de memória no endereço 1025 (página 1, frame 2)

Carregando página 1 do processo 1 no Frame 2
Falta de página - Frame 2 substituído

Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    7 |    4 |  S |  S
    1 |   S  |    1 |    0 |  S |  N
    2 |   S  |    1 |    1 |  S |  S
    3 |   S  |    7 |    0 |  S |  N

Estado do Processo 1:
Tamanho: 2048 bytes | Estado: pronto

Tabela de Paginas do Processo 1:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   1 |     S    |      2 |  S |  S | 15:23:19
   0 |     S    |      1 |  S |  N | 15:23:19
----------------------------------------
```
O acesso ao endereço 1025 do processo P1 corresponde à página 1. No momento do acesso, a página 1 não estava presente na memória, causando um page fault. O LRU seleciona a página que ficou mais tempo sem ser usada.
Nesse caso, a página 3 do processo 7, localizada no frame 2, foi a menos recentemente usada e, portanto, foi substituída.

### 🎓Resumo Final:

```
Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    7 |    4 |  S |  S
    1 |   S  |    1 |    0 |  S |  N
    2 |   S  |    1 |    1 |  S |  S
    3 |   S  |    7 |    0 |  S |  N


Tabela de Paginas do Processo 1:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   1 |     S    |      2 |  S |  S | 15:23:19
   0 |     S    |      1 |  S |  N | 15:23:19
----------------------------------------

Tabela de Paginas do Processo 7:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   7 |     N    |     -1 |  N |  N | 15:23:19
   6 |     N    |     -1 |  N |  N | 15:23:19
   5 |     N    |     -1 |  N |  N | 15:23:19
   4 |     S    |      0 |  S |  S | 15:23:19
   3 |     S    |     -1 |  S |  N | 15:23:19
   2 |     N    |     -1 |  N |  N | 15:23:19
   1 |     N    |     -1 |  N |  N | 15:23:19
   0 |     S    |      3 |  S |  N | 15:23:19
----------------------------------------

Resumo da Simulacao:
Total de faltas de página: 7
Total de operações de swap: 8
Processos ativos: 2

[Memoria Secundaria - Simulada]
Operações de swap ate agora: 8
```


### Situação real usando algoritmo RELOGIO:
Antes de P7 W 4099 (Primeira substituição) as tabelas de páginas e da Memória Física são iguais nos dois algoritmos.

P7 W (4099)2:
```
Escrita de memória no endereço 4099 (página 4, frame 0)

Carregando página 4 do processo 7 no Frame 0
Falta de página - Frame 0 substituído

Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    7 |    4 |  S |  S
    1 |   S  |    1 |    1 |  N |  S
    2 |   S  |    7 |    3 |  N |  N
    3 |   S  |    7 |    0 |  N |  N

Estado do Processo 7:
Tamanho: 8192 bytes | Estado: esperando_io

Tabela de Paginas do Processo 7:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   7 |     N    |     -1 |  N |  N | 15:55:28
   6 |     N    |     -1 |  N |  N | 15:55:28
   5 |     N    |     -1 |  N |  N | 15:55:28
   4 |     S    |      0 |  S |  S | 15:55:28
   3 |     S    |      2 |  N |  N | 15:55:28
   2 |     N    |     -1 |  N |  N | 15:55:28
   1 |     N    |     -1 |  N |  N | 15:55:28
   0 |     S    |      3 |  N |  N | 15:55:28
----------------------------------------
```
O ponteiro de substituição do algoritmo Relógio passa por todos os frames, já que o bit de Ref é 1 para todos, mudando esses bits para 0 até que ele completa a volta e volta para o frame 0,
que agora esta com Ref 0 ou N, sendo assim, ele é substituído.

P1 W (3)2:
```
Leitura de memória no endereço 3 (página 0, frame 1)

Salvando página 1 do processo 1 do Frame 1 para armazenamento secundário
Carregando página 0 do processo 1 no Frame 1
Falta de página - Frame 1 substituído

Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    7 |    4 |  S |  S
    1 |   S  |    1 |    0 |  S |  N
    2 |   S  |    7 |    3 |  N |  N
    3 |   S  |    7 |    0 |  N |  N

Estado do Processo 1:
Tamanho: 2048 bytes | Estado: pronto

Tabela de Paginas do Processo 1:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   1 |     N    |     -1 |  N |  S | 15:55:28
   0 |     S    |      1 |  S |  N | 15:55:28
----------------------------------------
```
Agora, como todos os bits de Ref estavam 0 (tirando o do frame 0, que foi recentemente substituído) o próximo frame que o ponteiro apontasse iria ser substituído, logo, o próximo frame (frame 1)
que carregava página 1 do processo 1 foi substituído e agora carrega página 0 do processo 1 e seu bit de Ref é 1. O ponteiro avança para o próximo frame.

P1 W (1025)2:
```
Escrita de memória no endereço 1025 (página 1, frame 2)

Carregando página 1 do processo 1 no Frame 2
Falta de página - Frame 2 substituído

Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    7 |    4 |  S |  S
    1 |   S  |    1 |    0 |  S |  N
    2 |   S  |    1 |    1 |  S |  S
    3 |   S  |    7 |    0 |  N |  N

Estado do Processo 1:
Tamanho: 2048 bytes | Estado: pronto

Tabela de Paginas do Processo 1:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   1 |     S    |      2 |  S |  S | 15:55:28
   0 |     S    |      1 |  S |  N | 15:55:28
----------------------------------------
```
De novo, há falta de página e a página do processo 1 responsável pelo endereço 1025 não está sendo carregada por nenhum frame da Memória Física, sendo assim, é necessário substituir um frame. O ponteiro está 
apontando para o frame 2 que tem bit de Ref 0 ou N, logo, é esse frame que será substituído e a página 1 do processo 1 será adicionada. Seu bit de Ref se torna 1 ou S.

### 🎓Resumo Final:

```
Estado da Memória Fisica:
Frame | Aloc | PID  | Pag  | Ref | Mod
------|------|------|------|-----|----
    0 |   S  |    7 |    4 |  S |  S
    1 |   S  |    1 |    0 |  S |  N
    2 |   S  |    1 |    1 |  S |  S
    3 |   S  |    7 |    0 |  N |  N

Estado do Processo 1:
Tamanho: 2048 bytes | Estado: pronto

Tabela de Paginas do Processo 1:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   1 |     S    |      2 |  S |  S | 15:55:28
   0 |     S    |      1 |  S |  N | 15:55:28
----------------------------------------

Tabela de Paginas do Processo 7:
Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso
-----|----------|--------|-----|-----|--------------
   7 |     N    |     -1 |  N |  N | 15:55:28
   6 |     N    |     -1 |  N |  N | 15:55:28
   5 |     N    |     -1 |  N |  N | 15:55:28
   4 |     S    |      0 |  S |  S | 15:55:28
   3 |     S    |     -1 |  N |  N | 15:55:28
   2 |     N    |     -1 |  N |  N | 15:55:28
   1 |     N    |     -1 |  N |  N | 15:55:28
   0 |     S    |      3 |  N |  N | 15:55:28
----------------------------------------

Resumo da Simulacao:
Total de faltas de página: 7
Total de operações de swap: 8
Processos ativos: 2

[Memoria Secundaria - Simulada]
Operações de swap ate agora: 8
```

# Alternância entre algoritmos de substituição
## Objetivo : 
### Esta documentação demonstra as diferenças fundamentais entre os algoritmos LRU (Least Recently Used) e Clock através de um caso prático de simulação. O foco é evidenciar como o tratamento dos bits de referência afeta as decisões de substituição de páginas.

## 1. Cenário de Teste
### Configuração do Sistema
- Memória física: 4 frames (16 KB total)

- Tamanho de página: 4 KB

### Processos:

- P1: 1 página (4096 bytes)

- P2: 1 página (4096 bytes)

- P3: 3 páginas (12288 bytes)

### Sequência de Operações (arquivo alternancia.txt)
```
P1 C 4096 → Cria P1

  P1 R (0)2 → Acesso P1-p0 (carrega Frame 0)

P2 C 4096 → Cria P2

P2 R (0)2 → Acesso P2-p0 (carrega Frame 1)

P3 C 12288 → Cria P3

P3 R (0)2 → Acesso P3-p0 (carrega Frame 2)

P3 R (4096)2 → Acesso P3-p1 (carrega Frame 3)

P3 R (8192)2 → Acesso P3-p2 (substituição)

P3 R (12288)2 → Acesso P3-p3 (substituição)

P1 R (0)2 → Reacesso P1-p0 (teste decisivo)
```
## 2. Comportamento dos Algoritmos
### 2.1 Mecanismo LRU
Lógica: Mantém histórico exato de acessos. A página não usada há mais tempo é substituída.

- Substituições:

Para P3-p2: Substitui P1-p0 (Frame 0) - página mais antiga

Para P3-p3: Substitui P2-p0 (Frame 1)

Reacesso P1-p0:

Causa falta de página (P1-p0 foi removida)

Substitui P3-p0 (Frame 2) para recarregar P1-p0 no Frame 1

### 2.2 Mecanismo Clock
Lógica: Ponteiro circular que zera bits de referência durante a busca.

- Substituições:

Para P3-p2: Percorre frames: Frame 0 (P1-p0, R=1 → zera bit)

Frame 1 (P2-p0, R=1 → zera bit)

Frame 2 (P3-p0, R=1 → zera bit)

Frame 3 (P3-p1, R=1 → zera bit)

Segunda volta: Substitui Frame 0 (P1-p0, R=0)

Para P3-p3: Substitui Frame 1 (P2-p0, R=0)

Reacesso P1-p0:

Causa falta de página (P1-p0 foi removida)

Substitui Frame 2 (P3-p0, R=0) para recarregar P1-p0

## 3. Estados Finais Comparados
### 3.1 Saída LRU
```
Frame | Aloc | PID | Pag | Ref | Mod
------|-----|-----|-----|-----|----
0     | S   | 3   | 8   | S   | N   # P3-p2 (substituiu P1-p0)
1     | S   | 1   | 0   | S   | N   # P1-p0 (recarregado)
2     | S   | 3   | 0   | S   | N   # P3-p0 
3     | S   | 3   | 4   | S   | N   # P3-p1
```
### 3.2 Saída Clock
```
Frame | Aloc | PID | Pag | Ref | Mod
------|-----|-----|-----|-----|----
0     | S   | 3   | 8   | S   | N   # P3-p2 (substituiu P1-p0)
1     | S   | 1   | 0   | S   | N   # P1-p0 (recarregado)
2     | S   | 3   | 0   | N   | N   # P3-p0 (bit zerado)
3     | S   | 3   | 4   | N   | N   # P3-p1 (bit zerado)
```
## 4. Análise Técnica das Diferenças
### 4.1 Tratamento de Bits de Referência

Para o tratamento de bits de referência o LRU mantém bit sempre ativo para páginas presentes,e há impacto no histórico de uso.
Já o Clock, zera bits durante busca por vítimas e "esquece"  acessos antigos após ciclo completo.

### 4.2 Escolha de Vítimas
- LRU:

Prioriza idade exata da página

No exemplo: Substituiu P1-p0 primeiro (mais antiga)

- Clock:

Prioriza posição do ponteiro + bit zerado

No exemplo: Zerou todos os bits antes de substituir P1-p0

### 4.3 Efeito no Reacesso
Ambos recarregaram P1-p0 no Frame 1

- Diferença crítica:

Clock zerou bits de P3-p0 e P3-p1 durante a busca

LRU manteve todos bits ativos

## 5. Conclusões
### 5.1 Precisão vs. Eficiência

No trade‐off entre precisão e eficiência, o LRU destaca‐se por sua precisão histórica, mantendo um registro exato da ordem de acesso às páginas e garantindo que sempre será substituída a página menos recentemente usada. Essa precisão, porém, tem um alto custo computacional, pois exige atualizações constantes da estrutura de dados que rastreia os acessos. Em contraste, o Clock oferece um custo O(1) muito baixo, usando apenas um bit de referência e um ponteiro circular para dar “segunda chance” às páginas. A desvantagem é que, ao zerar bits de referência durante a rotação, o Clock pode acidentalmente substituir páginas que ainda estão ativas.

# Conclusão
