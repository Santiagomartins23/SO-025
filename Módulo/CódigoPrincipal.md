# Problema do Produtor e Consumidor com Pthreads e Semáforos (POSIX)

### Nesta seção, solucionamos o problema proposto na entrega da atividade, utilizando semáforos POSIX e exclusão mútua com mutexes, controlando o acesso concorrente
ao buffer compartilhado. A implementação garante sincronização correta entre produtor e consumidor, evita espera ociosa, e simula comportamento real de multitarefa.

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

### Nesta seção, destacamos o funcionamento do código e como foram aplicadas as tecnlogias para garantir a exclusão mútua, evitar a ociosidade da CPU, e aproveitar ao máximo sua capacidade, para que, na seção de exemplos práticos fique mais claro a importância destas práticas quando são bem aplicadas

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

## 🔐 Controle de Concorrência

### Semáforos
- `sem_t empty`: Conta quantas posições estão **vazias** no buffer. Inicia com `BUFFER_SIZE`.
- `sem_t full`: Conta quantas posições estão **ocupadas**. Inicia com `0`.

### Exclusão Mútua
- `pthread_mutex_t mutex`: Impede que duas threads modifiquem simultaneamente o buffer.

---


