# Problema do Produtor e Consumidor com Pthreads e Semáforos (POSIX)

### Nesta seção, solucionamos o problema proposto na entrega da atividade, utilizando semáforos POSIX e exclusão mútua com mutexes, controlando o acesso concorrente ao buffer compartilhado. A implementação garante sincronização correta entre produtor e consumidor, evita espera ociosa, e simula comportamento real de multitarefa.

---

## ✅ Requisitos Atendidos

| Buffer limitado compartilhado  ✅ |
&nbsp;
| Controle de exclusão mútua  ✅ (`pthread_mutex_t`) |
&nbsp;
| Controle de posições livres/ocupadas ✅ (`sem_t empty/full`) |
&nbsp;
| Capacidade de colocar thread em espera  ✅ (`sem_wait()`) |
&nbsp;
| Capacidade de "acordar" threads  ✅ (`sem_post()`) |
&nbsp;
| Controle de leitura/escrita com ponteiros  ✅ (`in` e `out`) |
&nbsp;

---

## 🧠 Funcionamento

### Nesta seção, destacamos o funcionamento do código e como foram aplicadas as tecnlogias para garantir a exclusão mútua, evitar a ociosidade da CPU, e aproveitar ao máximo sua capacidade, para que, na seção de exemplos práticos fique mais claro a importância destas práticas quando são bem aplicadas.

### Buffer

<pre>
#define BUFFER_SIZE 5 // tamanho do buffer
int buffer[BUFFER_SIZE]; // declara o buffer
  
int in = 0; // posição onde o produtor insere o próximo item.
int out = 0; // posição de onde o consumidor retira o item.

 buffer[in] = item; // para produzir
 int item = buffer[out]; // para consumir  
</pre>

### Controle de exclusão mútua
<pre>
pthread_mutex_t mutex; // impede com que duas threads (produtoras ou consumidoras) modifiquem o buffer ao mesmo tempo, protegendo as variávies.
</pre>  
### Controle de posições
<pre>
sem_t empty; // conta quantas posições vazias há no buffer.
sem_t full; // conta quantas posições cheias há no buffer.
sem_init(&empty, 0, BUFFER_SIZE); 
sem_init(&full, 0, 0); 
</pre>  
### Manuseio das Threads
<pre>
pthread_t produtores[NUM_PRODUTORES]; 
pthread_t consumidores[NUM_CONSUMIDORES];
  
pthread_create(&produtores[i], NULL, produtor, NULL); 
pthread_create(&consumidores[i], NULL, consumidor, NULL); 
  
pthread_join(produtores[i], NULL);
pthread_join(consumidores[i], NULL)
</pre>  

### Produtor
<pre>
void* produtor(void* arg) {

    int a= 0;
    while (a <= 9){
        int item = rand() % 100;

          sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        printf("[Produtor] Produziu %d na posicao %d\n",item, in);
        in = (in + 1) % BUFFER_SIZE;
        a++;

        pthread_mutex_unlock(&mutex);

        sem_post(&full);



    }


    return NULL;
}
</pre>
### Consumidor 
<pre>
void* consumidor(void* arg) {

    int b= 9;
    while (b >= 0) {
         sem_wait(&full);
        pthread_mutex_lock(&mutex);
  
        int item = buffer[out];
        printf("[Consumidor] Consumiu %d da posicao %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;
        b--;

        pthread_mutex_unlock(&mutex);

     sem_post(&empty);

    }
    return NULL;
}
</pre>
---
## Saída
Saída do código acima executado:

![saidacertaa](https://github.com/user-attachments/assets/b9b44b5a-b385-48fb-b55e-f63e0e156e0d)


## 🔐 Controle de Concorrência

### Semáforos

#### Os semáforos controlam a quantidade de acesso permitido (quando produzir ou consumir).

- `sem_t empty`: Esse semáforo representa quantas posições vazias há no buffer. Ele é inicializado com o tamanho total do buffer (por exemplo, BUFFER_SIZE). Cada vez que um produtor insere um item no buffer, ele decrementa empty, indicando que há uma posição a menos disponível. Se empty chegar a zero, o produtor precisa esperar até que o consumidor libere uma posição.

- `sem_t full`: Esse semáforo representa quantas posições ocupadas há no buffer, é iniciado com `0`.. Ele é inicializado com 0, pois o buffer começa vazio. Quando o produtor insere um item, ele incrementa full. O consumidor só pode retirar um item se full for maior que zero — caso contrário, ele espera até que haja algo para consumir.

### Exclusão Mútua

#### O mutex controla o acesso exclusivo à região crítica.

- `pthread_mutex_t mutex`: Esse mutex garante que apenas uma thread por vez possa acessar e modificar a estrutura do buffer. Quando uma thread quer inserir ou remover um item, ela deve trancar (lock) o mutex, fazer sua operação com segurança e então liberar (unlock) o mutex. Isso impede acessos simultâneos, evitando corrupção de dados ou inconsistências.

---

## Seção De Exemplos Práticos

### Nesta seção, explicaremos a importância das funcionalidades que implementamos, e como eles impactam no resultados das operações, alterando nosso código solução.

## Caso Prático de Starvation

### 📌 O que é Starvation?
Starvation (inanição) ocorre quando uma thread não consegue acessar recursos compartilhados por um período prolongado, ficando permanentemente bloqueada enquanto outras threads continuam a ser servidas.

## Configuração Problemática:

<pre>
#define NUM_PRODUTORES 3
#define NUM_CONSUMIDORES 1  // Gargalo intencional
#define BUFFER_SIZE 10
</pre>

### 🐌 Cenário de Starvation
#### Comportamento Observado:

-Os 3 produtores enchem o buffer rapidamente

-O único consumidor não consegue esvaziar o buffer na mesma velocidade

### Quando o buffer está cheio:

-Produtores tentam produzir e ficam bloqueados em `sem_wait(&empty)`

-O consumidor libera espaços, mas sempre acorda outro produtor em vez de dar chance para outros consumidores

### Execução e Análise de resultados 

Para executar utilizamos o comando `./starvation`

Resultado: 

![starvaion](https://github.com/user-attachments/assets/80082b1a-f839-412e-8722-0b20b6fd4993)



Utilizamos o comando `top -H -p $(pgrep starvation) -n 1 -b` para obter estátisticas sobre a CPU e as threads.

<pre>

top - 22:42:44 up 16:42,  5 users,  load average: 3.57, 2.22, 1.27
Threads:   5 total,   3 running,   2 sleeping,   0 stopped,   0 zombie
%Cpu(s): 10.0 us, 70.0 sy,  0.0 ni, 15.0 id,  0.0 wa,  0.0 hi,  5.0 si,  0.0 st
MiB Mem :   3916.0 total,   2742.1 free,    524.6 used,    889.5 buff/cache
MiB Swap:   3923.0 total,   3923.0 free,      0.0 used.   3391.4 avail Mem

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND
   3897 gsograd+  20   0  101004   1408   1408 R  18.2   0.0   0:18.41 starvation
   3898 gsograd+  20   0  101004   1408   1408 R  18.2   0.0   0:18.59 starvation
   3899 gsograd+  20   0  101004   1408   1408 R  18.2   0.0   0:31.58 starvation
   3896 gsograd+  20   0  101004   1408   1408 S   9.1   0.0   0:18.50 starvation
   3895 gsograd+  20   0  101004   1408   1408 S   0.0   0.0   0:00.00 starvation
</pre>

## 🎯 Diagnóstico de Starvation – Pontos Críticos
### 1. Alto Tempo em Syscall (70%)
Tempo no kernel > user-space → Indica que o sistema está mais ocupado gerenciando concorrência (semáforos, mutexes) do que processando dados.

Isso gera overhead de sincronização, reduzindo o tempo útil da CPU.

### 2. Assimetria Produtor–Consumidor (3:1)
Produção = 3x maior que consumo, criando um gargalo.

O buffer enche rápido e força os produtores a competir mais por espaço, agravando o uso do kernel.

### 3. Sintomas Visíveis de Starvation
Thread consumidora com baixa atividade em 9,1% de uso de CPU, enquanto threads produtoras resultam em 54,6%, 18,2% cada.

Buffer sempre cheio, o que bloqueia produtores com sem_wait(&empty), gerando muitas syscalls.

Consumidor acordado com menos frequência, pois sem_post(&empty) é chamado por ele sozinho, enquanto sem_post(&full) acorda múltiplos produtores.

### 4. Falta de Fairness
O escalonador não consegue distribuir bem o tempo de CPU entre as threads.

O mutex e os semáforos com acordes não justos (quem acorda pode não ser quem esperava há mais tempo) contribuem para o starvation da thread consumidora.

## Conclusão

O sistema sofre de um thrashing de concorrência, onde o custo para gerenciar threads (70% em syscall) supera o trabalho útil realizado. A assimetria entre produtores e consumidores (3:1) causa um gargalo crônico, levando à starvation temporária da thread consumidora, que permanece inativa por longos períodos enquanto o buffer fica constantemente cheio.

Além disso, a falta de fairness no acordar de threads agrava o problema — o consumidor não recebe tempo de CPU suficiente, e produtores monopolizam os recursos. Isso demonstra que fairness em sistemas é essencial para evitar inanição de threads ou processos, e garantir uma execução equilibrada. Um controle adequado da proporção entre produtores e consumidores é fundamental para evitar sobrecarga, reduzir overhead e permitir que o sistema processe dados de forma eficiente.


gabriel
gabrielhuemer
Invisível

san — 14/03/2025 16:56
./meu_programa
joa — 14/03/2025 16:58
gcc file.c -o file
gabriel — 17/03/2025 17:32
scp C:\Users\gabriel\Downloads\fork.c gsograd08@10.1.1.78:/home/gsograd08/
ssh: Could not resolve hostname c: Name or service not known
scp: Connection closed
gabriel — 17/03/2025 23:02
sftp -P 2178 gsograd08@andromeda.lasdpc.icmc.usp.br
san — 17/03/2025 23:07
"C:\Users\User\OneDrive\Códigos C\projetosC\CódigoQuest3Prova\main.c"
san — 17/03/2025 23:18
#include <stdio.h>

int main(){
printf("Oi tudo bem\n");
return 0;

}
scp usuario@ip-da-vm:/caminho/do/arquivo/main22.c
san — 17/03/2025 23:25
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

void startVM() {
    printf("Iniciando a VM...\n");
    system("virsh start minhaVM");  // Comando para iniciar a VM no Linux
}

void stopVM() {
    printf("Parando a VM...\n");
    system("virsh shutdown minhaVM");
}

void handleRequest(int new_socket) {
    char buffer[1024] = {0};
    read(new_socket, buffer, 1024);

    printf("Requisição recebida:\n%s\n", buffer);

    if (strstr(buffer, "GET /start")) {
        startVM();
        write(new_socket, "HTTP/1.1 200 OK\n\nVM iniciada!", 29);
    } else if (strstr(buffer, "GET /stop")) {
        stopVM();
        write(new_socket, "HTTP/1.1 200 OK\n\nVM parada!", 28);
    } else {
        write(new_socket, "HTTP/1.1 404 Not Found\n\nComando inválido!", 41);
    }

    close(new_socket);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 5);

    printf("Servidor rodando na porta %d...\n", PORT);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr )&address, (socklen_t)&addrlen);
        handleRequest(new_socket);
    }

    return 0;

}
<!DOCTYPE html>
<html lang="pt">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Painel da VM</title>
</head>
<body>
    <h1>Gerenciamento da VM</h1>
    <button onclick="controlVM('/start')">Iniciar VM</button>
    <button onclick="controlVM('/stop')">Parar VM</button>

    <script>
        function controlVM(action) {
            fetch("http://localhost:8080/" + action)
                .then(response => response.text())
                .then(data => alert(data))
                .catch(error => console.error("Erro:", error));
        }
    </script>
</body>
</html>
gabriel — 17/03/2025 23:47
https://youtu.be/McKNP3g6VBA?si=nYVSr_CpyIxYhPD_
YouTube
Programação Web
Curso Javascript Completo (6 Horas)
Imagem
san — 18/03/2025 23:35
$ strace -c ls > /dev/null
san — 18/03/2025 23:47
https://strace.io/
strace -c meuprograma
gabriel — 18/03/2025 23:51
strace: Cannot find executable 'teste1'
gabriel — 19/03/2025 00:02
Chamadas aos Sistema
ada grupo deverá selecionar pelo menos 3 primitivas de chamada de sistema disponíveis no Linux, para cada uma das categorias (gerenciamento de memória, processos, E/S e arquivos), totalizando
9 primitivas a serem analisadas. Deverão ser construídos programas para exemplificar a utilização de cada uma das primitivas escolhidas.
Pontos Importantes
• A ferramenta strace (http://man7.org/linux/man-pages/man1/strace.1.html) deve ser utilizada para obter as estatísticas relacionadas ao uso de chamadas ao sistema em cada um dos
programas desenvolvidos.
• Deve-se obter, por meio desta ferramenta, a relação de todas as system calls invocadas pela aplicação e tempo gasto por cada uma delas.
• Todas as system calls identificadas devem ser descritas, ainda que resumidamente, no texto da monografia proposta.
.
Não há necessidade de descrever a mesma chamada de sistema múltiplas vezes, considerando que algumas primitivas irão se repetir nas aplicações.
Informações mínimas a serem obtidas devem considerar a aplicação da ferramenta no código binário, tanto na Parte 01 quanto na Parte 02
Processos CPU-Bound e I-O bound
ada grupo deverá desenvolver ao menos 2 programas que exibam o comportamento esperado para processos CPU-bound e I-O bound, tal como foi informado em sala de aula. Pode-se utilizar os programas desenvolvidos na etapa anterior, desde que eles atendam ao requisito especificado.
gabriel — 19/03/2025 00:10
#include <stdio.h>
#include <stdlib.h>

int main() {
    int array;
    int n = 5;

    // Aloca memória
    array = (int)malloc(n * sizeof(int));
    if (array == NULL) {
        perror("Erro ao alocar memória");
        return 1;
    }

    // Preenche o array
    for (int i = 0; i < n; i++) {
        array[i] = i * 2;
    }

    // Libera a memória
    free(array);

    return 0;
}
strace -c ./malloc COMANDO PARA STRACE
joa — 19/03/2025 00:24
Alocação de Memória para Estruturas Internas:

A função malloc pode usar mmap para alocar grandes blocos de memória ou para alocar memória para estruturas internas que gerenciam a alocação de memória.

Alocação de Memória para o Ambiente de Execução:

O ambiente de execução do programa (por exemplo, a biblioteca C) pode alocar memória para inicializar estruturas de dados internas, como a tabela de arquivos abertos, buffers de I/O, etc.

Fragmentação de Memória:

Se o sistema estiver fragmentado, malloc pode precisar fazer várias chamadas a mmap para encontrar blocos de memória contíguos.

Alocação de Memória para Threads:

Se o programa estiver usando threads, o sistema pode alocar memória para as pilhas de cada thread usando mmap.

Alocação de Memória para Bibliotecas Compartilhadas:

Bibliotecas compartilhadas carregadas pelo programa podem alocar memória usando mmap.
gabriel — 19/03/2025 00:27
https://linux.die.net/man/1/time
san — 19/03/2025 00:45
perguntar pro estrela como ele quer que seja feita a documentação do projeto
joa — 21/03/2025 16:50
Quando um processo é I/O bound, seu desempenho é limitado pelas operações de entrada e saída, e não pela capacidade de processamento da CPU. Para otimizar um sistema I/O bound, o foco está em melhorar a velocidade de E/S, como usar armazenamento mais rápido (SSD) ou otimizar o uso da rede.
gabriel — 21/03/2025 16:58
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define FILE_SIZE 1024 * 1024  // 1 MB
#define BUFFER_SIZE 4096       // 4 KB
#define NUM_WRITES 256         //number of writes

int main() {
    const char *filename = "output.txt";
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_written;
    off_t offset;

    //fills the buffer with random data
    memset(buffer, 'A', BUFFER_SIZE);

    //open the file to write(or new)
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    printf("Escrevendo no arquivo...\n");

    //sequential writing to the file
    for (int i = 0; i < NUM_WRITES; i++) {
        bytes_written = write(fd, buffer, BUFFER_SIZE);
        if (bytes_written == -1) {
            perror("Erro ao escrever no arquivo");
            close(fd);
            return 1;
        }
    }

    //random writing
    srand(time(NULL));
    for (int i = 0; i < NUM_WRITES; i++) {
        offset = rand() % FILE_SIZE; //choose a random offset
        lseek(fd, offset, SEEK_SET); //repositions the write pointer
        bytes_written = write(fd, buffer, BUFFER_SIZE);
        if (bytes_written == -1) {
            perror("Erro ao escrever no arquivo");
            close(fd);
            return 1;
        }
    }

    //close file
    close(fd);

    printf("Escrita concluida.\n");
    return 0;
}
san — 21/03/2025 17:00
brk(endereço) define diretamente o novo limite da heap. Você precisa calcular e passar o novo endereço manualmente.
sbrk(tamanho) altera a heap de forma incremental, somando o valor passado ao limite atual.
san — 27/03/2025 17:17
PRIMEIRO COLOCAR O CODIGO NO GITHUB
DEPOIS FAZER UM READ ME COM OS PRINTS PELO STRACE
E DEPOIS NO READ ME COLOCAR SE EH IO BOUND OU CPU BOUND JUNTO COM O PRINT DO TIME
joa — 27/03/2025 17:21
Imagem
san — 27/03/2025 17:26
jpbaratella@usp.br
https://github.com/Santiagomartins23/SO-025
GitHub
GitHub - Santiagomartins23/SO-025: Relatório acadêmico do projeto...
Relatório acadêmico do projeto de Sistemas Operacionais - Santiagomartins23/SO-025
GitHub - Santiagomartins23/SO-025: Relatório acadêmico do projeto...
gabriel — 25/04/2025 13:04
rapaziada, organizei lá na vm em modulo1 e modulo2
san — 25/04/2025 17:27
boa
vi agora
melhor
gabriel — 27/04/2025 19:45
a
. Variação de Velocidade Produtor/Consumidor
Teste: Torne o produtor 10x mais rápido que o consumidor (ou vice-versa).

O que analisar:

Como a espera ociosa do consumidor aumenta quando o buffer fica cheio.
Expandir
message.txt
3 KB
joa — 27/04/2025 19:48
2,5,9
gabriel — 27/04/2025 19:48
https://chat.deepseek.com/a/chat/s/a328e236-2a8d-4d18-a02f-4110afda4e49
void* produtor(void* arg) {
    while (1) {
        sem_wait(&empty);  // Ainda necessário
        // REMOVIDO: pthread_mutex_lock(&mutex);

        buffer[in] = item;  // ⚠️ Condição de corrida aqui!
        in = (in + 1) % BUFFER_SIZE;  // ⚠️ E aqui!

        // REMOVIDO: pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_PRODUTORES 2
#define NUM_CONSUMIDORES 2

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

void* produtor(void* arg) {

    int a= 0;
    while (a <= 50000000){
        int item = rand() % 100;

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        printf("[Produtor] Produziu %d na posicao %d\n",item, in);
        in = (in + 1) % BUFFER_SIZE;
        a++;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);





    }


    return NULL;
}

void* consumidor(void* arg) {

    int b= 50000000;
    while (b >= 0) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        int item = buffer[out];
        printf("[Consumidor] Consumiu %d da posicao %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;
        b--;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);


    }
    return NULL;
}

int main() {
    pthread_t produtores[NUM_PRODUTORES];
    pthread_t consumidores[NUM_CONSUMIDORES];

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    // Cria múltiplos produtores e consumidores
    for (int i = 0; i < NUM_PRODUTORES; i++) {
        pthread_create(&produtores[i], NULL, produtor, NULL);
    }
    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        pthread_create(&consumidores[i], NULL, consumidor, NULL);
    }

    // Espera todas as threads terminarem
    for (int i = 0; i < NUM_PRODUTORES; i++) {
        pthread_join(produtores[i], NULL);
    }
    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        pthread_join(consumidores[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
gabriel — 27/04/2025 19:56
Tipo de arquivo em anexo: document
codigoPrincipal.TXT
1.95 KB
joa — 27/04/2025 19:58
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
Expandir
SemMutex.txt
2 KB
gabriel — 27/04/2025 20:18
código principal
exemplos práticos
conclusão de cada exemplo
1.sem mutex 2.diferenças do tamanho do buffer 3.starvation 5.Número diferente de consumidores e produtos
joa — 27/04/2025 20:26
Imagem
"Bom dia/boa tarde! Nós desenvolvemos uma solução poderosa para manipulação de imagens de rosto, focada em aplicações de identificação e reconhecimento. Trabalhamos com imagens padrão de 200x200 pixels e criamos uma ferramenta que realiza translações, rotações e zoom com alta precisão."


"Em muitos sistemas, ao aplicar transformações como rotação ou zoom, a qualidade da imagem cai drasticamente, prejudicando algoritmos de reconhecimento facial e aumentando a taxa de erro. Além disso, imagens capturadas no mundo real muitas vezes vêm com algum grau de desfoque."


"Nossa ferramenta usa interpolação de segunda ordem — interpolação bilinear — para garantir imagens muito mais suaves e realistas após transformações. Essa técnica é significativamente superior ao vizinho mais próximo, preservando os detalhes importantes da imagem, como contornos faciais."

"Ela suporta deslocamentos de até 100 pixels, rotações em qualquer ângulo, e ampliações/reduções de fator entre 1 e 3 vezes."


"Com nossa solução, seu sistema de identificação facial terá imagens transformadas de forma precisa e sem perda de qualidade, aumentando a confiabilidade dos resultados."

"Você poderá processar imagens em tempo real ou em grandes volumes, com consistência e qualidade profissional."

"Tudo isso sem necessidade de hardware especializado — basta a integração com nosso algoritmo Python otimizado."

"Estamos prontos para integrar essa tecnologia ao seu sistema. Podemos oferecer uma demonstração prática ainda hoje para que vejam os resultados na prática. Nosso compromisso é elevar a qualidade da identificação facial do seu negócio ao próximo nível." 
joa — 27/04/2025 20:34
Numa apresentação de venda (pitch), o foco é mostrar as ideias e os métodos utilizados, e os benefícios que eles trazem, não o funcionamento interno detalhado.

Ou seja:

Você explica quais técnicas usou (ex.: interpolação bilinear, deblurring).

Por que essas técnicas são melhores (ex.: imagens mais suaves, mais fiéis ao original, melhor reconhecimento facial).

Como você testou para provar que elas funcionam (ex.: "fizemos testes de rotação, zoom, deslocamento em 500 imagens e mantivemos a qualidade acima de X%").
san — 02/05/2025 23:40
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_PRODUTORES 2
#define NUM_CONSUMIDORES 2

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

void* produtor(void* arg) {

    int a= 0;
    while (a <= 9){
        int item = rand() % 100;

          sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        printf("[Produtor] Produziu %d na posicao %d\n",item, in);
        in = (in + 1) % BUFFER_SIZE;
        a++;

        pthread_mutex_unlock(&mutex);

        sem_post(&full);



    }


    return NULL;
}

void* consumidor(void* arg) {

    int b= 9;
    while (b >= 0) {
         sem_wait(&full);
        pthread_mutex_lock(&mutex);
int item = buffer[out];
        printf("[Consumidor] Consumiu %d da posicao %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;
        b--;

        pthread_mutex_unlock(&mutex);

     sem_post(&empty);

    }
    return NULL;
}

int main() {
    pthread_t produtores[NUM_PRODUTORES];
    pthread_t consumidores[NUM_CONSUMIDORES];

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    // Cria m�ltiplos produtores e consumidores
    for (int i = 0; i < NUM_PRODUTORES; i++) {
        pthread_create(&produtores[i], NULL, produtor, NULL);
    }
    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        pthread_create(&consumidores[i], NULL, consumidor, NULL);
    }

    // Espera todas as threads terminarem
    for (int i = 0; i < NUM_PRODUTORES; i++) {
        pthread_join(produtores[i], NULL);
    }
    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        pthread_join(consumidores[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
gabriel — 23:27
Utilizamos o comando `top -H -p $(pgrep starvation) -n 1 -b` para obter estátisticas sobre a CPU e as threads.

<pre>

top - 22:42:44 up 16:42,  5 users,  load average: 3.57, 2.22, 1.27
Threads:   5 total,   3 running,   2 sleeping,   0 stopped,   0 zombie
Expandir
message.txt
4 KB
﻿
Utilizamos o comando `top -H -p $(pgrep starvation) -n 1 -b` para obter estátisticas sobre a CPU e as threads.

<pre>

top - 22:42:44 up 16:42,  5 users,  load average: 3.57, 2.22, 1.27
Threads:   5 total,   3 running,   2 sleeping,   0 stopped,   0 zombie
%Cpu(s): 10.0 us, 70.0 sy,  0.0 ni, 15.0 id,  0.0 wa,  0.0 hi,  5.0 si,  0.0 st
MiB Mem :   3916.0 total,   2742.1 free,    524.6 used,    889.5 buff/cache
MiB Swap:   3923.0 total,   3923.0 free,      0.0 used.   3391.4 avail Mem

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND
   3897 gsograd+  20   0  101004   1408   1408 R  18.2   0.0   0:18.41 starvation
   3898 gsograd+  20   0  101004   1408   1408 R  18.2   0.0   0:18.59 starvation
   3899 gsograd+  20   0  101004   1408   1408 R  18.2   0.0   0:31.58 starvation
   3896 gsograd+  20   0  101004   1408   1408 S   9.1   0.0   0:18.50 starvation
   3895 gsograd+  20   0  101004   1408   1408 S   0.0   0.0   0:00.00 starvation
</pre>

## 🎯 Diagnóstico de Starvation – Pontos Críticos
### 1. Alto Tempo em Syscall (70%)
Tempo no kernel > user-space → Indica que o sistema está mais ocupado gerenciando concorrência (semáforos, mutexes) do que processando dados.

Isso gera overhead de sincronização, reduzindo o tempo útil da CPU.

### 2. Assimetria Produtor–Consumidor (3:1)
Produção = 3x maior que consumo, criando um gargalo.

O buffer enche rápido e força os produtores a competir mais por espaço, agravando o uso do kernel.

### 3. Sintomas Visíveis de Starvation
Thread consumidora com baixa atividade em 9,1% de uso de CPU, enquanto threads produtoras resultam em 54,6%, 18,2% cada.

Buffer sempre cheio, o que bloqueia produtores com sem_wait(&empty), gerando muitas syscalls.

Consumidor acordado com menos frequência, pois sem_post(&empty) é chamado por ele sozinho, enquanto sem_post(&full) acorda múltiplos produtores.

### 4. Falta de Fairness
O escalonador não consegue distribuir bem o tempo de CPU entre as threads.

O mutex e os semáforos com acordes não justos (quem acorda pode não ser quem esperava há mais tempo) contribuem para o starvation da thread consumidora.

## Conclusão

O sistema sofre de um thrashing de concorrência, onde o custo para gerenciar threads (70% em syscall) supera o trabalho útil realizado. A assimetria entre produtores e consumidores (3:1) causa um gargalo crônico, levando à starvation temporária da thread consumidora, que permanece inativa por longos períodos enquanto o buffer fica constantemente cheio.

Além disso, a falta de fairness no acordar de threads agrava o problema — o consumidor não recebe tempo de CPU suficiente, e produtores monopolizam os recursos. Isso demonstra que fairness em sistemas é essencial para evitar inanição de threads ou processos, e garantir uma execução equilibrada. Um controle adequado da proporção entre produtores e consumidores é fundamental para evitar sobrecarga, reduzir overhead e permitir que o sistema processe dados de forma eficiente.
message.txt
4 KB

Antes que possa processar, outros produtores já enchem o buffer novamente

Conclusão
