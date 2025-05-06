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

### 👑 Padrão de Dominância Observado
Assimetria de Recursos:

-3 produtores: Capacidade coletiva de produção = 3 unidades/tempo

-1 consumidor: Capacidade de consumo = 1 unidade/tempo

-Razão 3:1 cria um gargalo inevitável

-Não há intercalação suave entre operações

###Efeito Pipeline:

O sistema se comporta como um cano que enche rapidamente e esvazia lentamente

Períodos de ociosidade alternam com períodos de saturação

### 🏥 Sintomas de Starvation do Consumidor:

-Longos períodos sem atividade de consumo nos logs

-Buffer constantemente cheio ou quase cheio

-Baixa utilização da thread consumidora (9.1% de threads consumidoras vs 54,6% das threads dos produtores(18,2% cada).


### ⚖️ Fairness

Injustiça no Acordar de Threads:

Quando um espaço no buffer é liberado, o sistema operacional não garante que a thread esperando há mais tempo será acordada

Produtores podem ser privilegiados aleatoriamente, perpetuando o starvation do consumidor

Efeito de Enxurrada (Thundering Herd):

Múltiplos produtores competindo podem causar um padrão onde:

O consumidor é acordado

Consome um item

Antes que possa processar, outros produtores já enchem o buffer novamente

Conclusão
