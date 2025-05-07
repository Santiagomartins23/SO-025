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

## 🔐 Controle de Concorrência

### Importância dos Semáforos

#### Os semáforos controlam a quantidade de acesso permitido (quando produzir ou consumir).

- `sem_t empty`: Esse semáforo representa quantas posições vazias há no buffer. Ele é inicializado com o tamanho total do buffer (por exemplo, BUFFER_SIZE). Cada vez que um produtor insere um item no buffer, ele decrementa empty (`sem_wait(&empty)`), indicando que há uma posição a menos disponível. Se empty chegar a zero, o produtor precisa esperar até que o consumidor libere uma posição. Cada vez que o consumidor consome
um item, ele incrementa empty (`sem_post(&empty)`).

- `sem_t full`: Esse semáforo representa quantas posições ocupadas há no buffer, é iniciado com `0`.. Ele é inicializado com 0, pois o buffer começa vazio. Quando o produtor insere um item, ele incrementa full (`sem_post(&full)`). O consumidor só pode retirar um item se full for maior que zero — caso contrário, ele espera até que haja algo para consumir. Quando o consumidor consome um item, ele decrementa full (`sem_wait(&full)`).

#### ❌ O que acontece se removermos os semáforos?

Sem os semáforos, o produtor tenta escrever em posições inválidas quando o buffer estiver cheio, e o consumidor tenta consumir do buffer mesmo estando vazio. Isso pode gerar comportamento incorreto ou falhas.


### Importância da Exclusão Mútua

#### O mutex controla o acesso exclusivo à região crítica.

- `pthread_mutex_t mutex`: Esse mutex garante que apenas uma thread por vez possa acessar e modificar a estrutura do buffer. Quando uma thread quer inserir ou remover um item, ela deve trancar (lock) o mutex, fazer sua operação com segurança e então liberar (unlock) o mutex. Isso impede acessos simultâneos, evitando corrupção de dados ou inconsistências.

---
## Resultado
Usando o comando `./pc`, a saída do código acima:

![saidacertaa](https://github.com/user-attachments/assets/b9b44b5a-b385-48fb-b55e-f63e0e156e0d)

Agora, usando o comando para encontrar o id do processo `pid pc` e em seguida `top -H -p <pid>` :

![Captura de tela 2025-05-07 175815](https://github.com/user-attachments/assets/b919d02f-07d9-445b-ada3-a03855bc6c8b)

## Conclusão 
A análise dos resultados apresentam claramente o funcionamento do programa produtor-consumidor em execução, demonstrando vários aspectos importantes da solução implementada:

**Concorrência e Paralelismo:** O programa está executando com 5 threads no total (2 running e 3 sleeping, e alterna conforme a dominância do buffer, isto é em um instante produtores dominam, em outro consumidores dominam), o que indica que a solução está utilizando efetivamente a programação concorrente através da biblioteca pthreads. Também, é notável o isolamento do kernel conforme o saída do código, pois não existem duas threads ocupando a mesma posição no buffer simultaneamente (race condition). 

**Controle de Processamento:** A distribuição de threads em estados running e sleeping mostra que o programa está gerenciando adequadamente a espera ociosa, colocando threads para dormir quando necessário (3 sleeping) e acordando-as quando há trabalho a ser feito (2 running).

**Uso de CPU:** O alto percentual de CPU no modo sistema (82.4 sy) sugere que há muita coordenação entre threads (operações de sincronização), enquanto o uso em modo usuário (11.0 us) indica o tempo gasto no processamento real das tarefas.

**Balanceamento:** As threads do programa (pc) estão consumindo porcentagens similares de CPU (entre 12.3% e 10.7%), indicando um bom balanceamento entre produtores e consumidores.

**Eficiência:** A ausência de threads em estado zombie ou stopped mostra que o programa está gerenciando corretamente o ciclo de vida das threads.

**Controle de Recursos:** O baixo uso de memória (0% MEM) sugere que o buffer compartilhado está sendo dimensionado adequadamente e que não há vazamentos de memória.

---

## Seção De Exemplos Práticos

### Nesta seção, explicaremos a importância das funcionalidades que implementamos, e como eles impactam no resultados das operações, alterando nosso código solução.


## 1. Caso Prático de Semáforos

### Configuração Problemática:
Vamos retirar os semáforos a fim de demonstrar a importância dos semáforos na execução e resultado do programa.

#### Exemplo de saída sem semáforos:

![semwait9](https://github.com/user-attachments/assets/5415dd3c-11aa-4037-b65d-4a0e5dc8d7c0)

O produtor acaba produzindo em posições que já estão ocupadas e não foram consumidas ainda e o consumidor acaba consumindo de posições que ja foram consumidas e que estão vazias. isso gera dados inválidos, 
condições de corrida, e impressões aleatórias/lixo.


Utilizamos o comando top -H -p para para obter estátisticas sobre a CPU e as threads.

![topsemwait](https://github.com/user-attachments/assets/02291c06-61b8-40dc-a161-2a7fe7b32000)

### 🔴Alta ociosidade da CPU (75% id)
Esse comportamento ocorre porque, sem os semáforos controlando corretamente o acesso ao buffer, as threads produtoras e consumidoras:

Não esperam de forma eficiente por recursos (como espaço no buffer ou itens para consumir);

Executam operações inválidas ou rápidas demais, como tentativas de acesso simultâneo ao buffer;

Muitas vezes, apenas imprimem mensagens ou giram em loops rápidos, sem fazer computação real.

### Threads em estado "sleeping" 
A ausência dos semáforos de controle (full e empty) no programa resulta em falhas na sincronização entre produtores e consumidores. Isso ocorre porque:

Consumidores podem tentar acessar o buffer mesmo quando ele está vazio.

Ao não encontrarem itens para consumir, essas threads entram em espera indefinida, ficando no estado "sleeping", aguardando por uma condição que nunca será satisfeita.

Como os semáforos que controlam a disponibilidade de itens (full) e de espaços livres (empty) não estão presentes, nenhuma outra thread é responsável por acordar os consumidores.

O mutex, por si só, apenas garante exclusão mútua, mas não impede condições inválidas, como tentar consumir de um buffer vazio ou produzir em um cheio.

Consequentemente, todas as threads acabam bloqueadas, causando deadlock ou paralisação da ação real do sistema.

### 🎯Conclusão
Código sem semáforo:

A CPU pode parecer ociosa porque as threads ficam imprimindo mensagens rapidamente ou acessando o buffer de forma inválida, sem realizar trabalho útil real.

O uso de CPU por thread é mais baixo, mas isso não significa eficiência. Pelo contrário: pode significar que há condições de corrida, sobrescrita de dados, perda de itens produzidos, ou leitura de posições vazias, forçando a CPU a realizar muitas operações I/O bound desnecessáriamente.

A ausência de sincronização provavelmente gera condições de corrida, perda de dados, ou comportamentos erráticos.


## 2. Caso Prático de Starvation

### 📌 O que é Starvation?
Starvation (inanição) ocorre quando uma thread não consegue acessar recursos compartilhados por um período prolongado, ficando permanentemente ou temporariamente bloqueada enquanto outras threads continuam a ser servidas.

### Configuração Problemática:

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

![Captura de tela 2025-05-07 181617](https://github.com/user-attachments/assets/f2d4ff32-1104-464b-ad89-f74adafc9262)

## 🎯 Diagnóstico de Starvation – Pontos Críticos
### 1. Alto Tempo em Syscall (70%)
Tempo no kernel > user-space → Indica que o sistema está mais ocupado gerenciando concorrência (semáforos, mutexes) do que processando dados.

Isso gera overhead de sincronização, reduzindo o tempo útil da CPU.

### 2. Assimetria Produtor–Consumidor (3:1)
Produção = 3x maior que consumo, criando um gargalo.

O buffer enche rápido e força os produtores a competir mais por espaço, agravando o uso do kernel.

### 3. Sintomas Visíveis de Starvation
Thread consumidora com baixa atividade em 9,1% de uso de CPU, enquanto threads produtoras resultam em 54,6%, 18,2% cada.

Buffer quase sempre cheio, o que bloqueia produtores com sem_wait(&empty), gerando muitas syscalls.

Consumidor acordado com menos frequência, pois sem_post(&empty) é chamado por ele sozinho, enquanto sem_post(&full) acorda múltiplos produtores.

A thread consumidora não sofre starvation absoluto, mas sim starvation relativo — recebe menos tempo de CPU do que o necessário para equilibrar o sistema, o que é suficiente para criar congestionamento e ineficiência.

### 4. Falta de Fairness
O escalonador não consegue distribuir bem o tempo de CPU entre as threads.

O mutex e os semáforos com acordes não justos (quem acorda pode não ser quem esperava há mais tempo) contribuem para o starvation da thread consumidora.

## Conclusão

O sistema sofre de um thrashing de concorrência, onde o custo para gerenciar threads (70% em syscall) supera o trabalho útil realizado. A assimetria entre produtores e consumidores (3:1) causa um gargalo crônico, levando à starvation temporária da thread consumidora, que permanece inativa por longos períodos enquanto o buffer fica constantemente cheio.

Além disso, a falta de fairness no acordar de threads agrava o problema — o consumidor não recebe tempo de CPU suficiente, e produtores monopolizam os recursos. Isso demonstra que fairness em sistemas é essencial para evitar inanição de threads ou processos, e garantir uma execução equilibrada. Um controle adequado da proporção entre produtores e consumidores é fundamental para evitar sobrecarga, reduzir overhead e permitir que o sistema processe dados de forma eficiente.

## 3. Sem Mutex

#### O que é um mutex?

Um mutex é uma ferramenta de sincronização usada para garantir que somente um thread tenha acesso a uma parte crítica do código ou recurso compartilhado por vez. Sem ele, múltiplos threads poderiam acessar e modificar o mesmo recurso simultaneamente, o que causaria condições de corrida, resultando em dados corrompidos ou comportamento inesperado do programa.

### 🧱 Onde o Mutex é Utilizado no Código
O mutex é usado para proteger a região crítica — o trecho de código onde buffer, in e out são acessados e modificados.



##### Na função do produtor:

 ```
sem_wait(&empty);
pthread_mutex_lock(&mutex); // INÍCIO DA REGIÃO CRÍTICA

buffer[in] = item;
printf("[Produtor] Produziu %d na posicao %d\n",item, in);
in = (in + 1) % BUFFER_SIZE;

pthread_mutex_unlock(&mutex); // FIM DA REGIÃO CRÍTICA
sem_post(&full); 
```


##### Na função do consumidor:

```
 sem_wait(&full);
pthread_mutex_lock(&mutex); // INÍCIO DA REGIÃO CRÍTICA

int item = buffer[out];
printf("[Consumidor] Consumiu %d da posicao %d\n", item, out);
out = (out + 1) % BUFFER_SIZE;

pthread_mutex_unlock(&mutex); // FIM DA REGIÃO CRÍTICA
sem_post(&empty);
```

Sem esse controle, é possível que duas threads acessem  `buffer[in]` ou `buffer[out]` ao mesmo tempo, causando condições de corrida.

### 🚫 O Que Acontece se o Mutex for Removido?
Se você remover as linhas:

`pthread_mutex_lock(&mutex);`
e
`pthread_mutex_unlock(&mutex);`

das `funções produtor()` e `consumidor()`, o programa ainda irá compilar e rodar, mas o comportamento se torna imprevisível.

### 💥 Problemas Observados ao Remover o Mutex

- ### 1. Condições de Corrida (Race Conditions):

Como várias threads acessam e modificam in, out e o buffer ao mesmo tempo, ocorrem situações como:
- Dois produtores escrevendo na mesma posição
- Um consumidor lendo uma posição ainda não preenchida

-  ##### Essas situações geram saídas incoerentes no terminal:

Exemplo:

![imagem56](https://github.com/user-attachments/assets/15e47ad0-aee3-430e-a735-f26b222b12e7)

### 📌 Evidências de Condição de Corrida no Resultado

#### 🔁 Valores duplicados consumidos:

 [Produtor] Produziu 71 na posicao 2 

[Consumidor] Consumiu 36 da posicao 2

[Consumidor] Consumiu 36 da posicao 2

→ O item 36 foi consumido duas vezes, mesmo que o produtor já tivesse colocado 71 na posição 2.

#### ❌ Item consumido antes de ser produzido:

[Consumidor] Consumiu 24 da posicao 0

[Produtor] Produziu 50 na posicao 0

→ O consumidor pegou algo da posição 0 antes de o produtor produzir qualquer coisa ali, violando a lógica correta.


- ### 2.  Perda de dados:

Quando não utilizamos mutex, as operações de escrita no buffer por diferentes threads produtoras não são atômicas, ou seja, podem ser interrompidas durante a produção. Isso permite que duas threads acessem a mesma posição do buffer ao mesmo tempo, utilizando o mesmo valor da variável in. Como resultado, uma thread pode sobrescrever o valor produzido pela outra antes que ele seja consumido. Assim, o item original é perdido sem nunca ter sido lido.


### 📊 Por Que os Semáforos Sozinhos Não São Suficientes?

Mesmo com o uso correto de:

`sem_wait(&empty);`
`sem_post(&full);`

e vice-versa, o problema persiste, pois os semáforos apenas controlam a quantidade de elementos disponíveis para produzir ou consumir — eles não protegem o acesso simultâneo à memória compartilhada.

### 🧵  Análise via top -H (comando para visualizar threads individualmente)
![IMAGEMREADME](https://github.com/user-attachments/assets/cbf87f37-063d-40f8-82b4-0fa7df5b3273)

No print acima, o comando top -H foi utilizado para monitorar as threads individualmente dentro de um processo. Cada linha representa uma thread criada pelo programa, com detalhes como PID, uso de CPU, uso de memória e tempo de execução.

#### 🔍 Observação importante: %Cpu(s): 99.8 id
No topo da saída, o valor %Cpu(s): 99.8 id indica que cerca de 99.8% do tempo o processador está ocioso, ou seja, as threads passam a maior parte do tempo aguardando recursos para continuar sua execução. Isso é esperado em aplicações com sincronização baseada em semáforos, onde as chamadas sem_wait() fazem com que as threads bloqueiem até que uma condição seja satisfeita.


