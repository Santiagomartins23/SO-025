# 🧵💡 Problema do Produtor e Consumidor com Pthreads e Semáforos (POSIX)

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

### Buffer

<pre> ```c
#include <stdio.h> int main() { printf("Olá, mundo!\n"); return 0; }
``` 
</pre>


---

## 🔐 Controle de Concorrência

### Semáforos
- `sem_t empty`: Conta quantas posições estão **vazias** no buffer. Inicia com `BUFFER_SIZE`.
- `sem_t full`: Conta quantas posições estão **ocupadas**. Inicia com `0`.

### Exclusão Mútua
- `pthread_mutex_t mutex`: Impede que duas threads modifiquem simultaneamente o buffer.

---


