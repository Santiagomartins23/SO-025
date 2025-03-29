## Gerenciamento de Processos

Foram desenvolvidos 3 programas em C que utilizam primitivas de chamadas de sistema do Linux para gerenciamento de processos. As primitivas escolhidas para esta categoria foram FORK, ESCALONADOR DE PROCESSOS E TRATAMENTO DE SINAIS. Todos os códigos referentes a esta seção estão disponíveis nesta pasta.

---

### Arquivo fork.c
Neste arquivo, foi utilizada a função `fork()` para criar novos processos.

**Funcionalidades principais:**
- Criação de processos filhos
- Execução concorrente entre processos pai e filho
- Uso de `wait()` para sincronização

##### Usando o comando " strace -c ./fork ", vamos conseguir estatísticas relacionadas ao uso das chamadas ao sistema e o tempo gasto por elas: 
<p>&nbsp;</p>







![capturaFork](https://github.com/user-attachments/assets/c20a6304-d2ed-4070-bd30-1ea86e795d57)
<p>&nbsp;</p>


## Observações:

Principais chamadas de sistema:

wait4 (73.14% do tempo): Indica que o processo pai passa a maior parte do tempo aguardando a finalização dos filhos, caracterizando espera bloqueante.

clone (25.34%): Reflete o custo da criação de processos com fork().


### Arquivo scheduling.c
Neste arquivo, foram implementados algoritmos de escalonamento utilizando chamadas de sistema para controle de processos. O programa demonstra:
- **FIFO**: Execução sequencial de processos
- **Round-Robin**: Alternância entre processos com quantum fixo
- **Prioridades**: Ordenação por níveis de prioridade

**Técnicas utilizadas:**
- `fork()` para criar múltiplos processos
- `waitpid()` para controle preciso de espera
- `sleep()` para simular tempo de execução

##### Usando o comando " strace -c ./scheduling ", vamos conseguir estatísticas relacionadas ao uso das chamadas ao sistema e o tempo gasto por elas: 
![capturaEscalonamento](https://github.com/user-attachments/assets/942cd56d-e4e2-401a-9aee-53e48e7ae87b)
<p>&nbsp;</p>


## Observações:

wait4 (67.25%): Confirma que o escalonador gasta mais tempo gerenciando processos do que executando tarefas.

clone (20.63%): Custo proporcional ao número de processos criados.

execve (5.11%): Presente se houver substituição de imagem (ex: uso de exec()).

### Arquivo treatsignal.c

Neste arquivo, foi implementado o tratamento de sinais entre processos utilizando primitivas do sistema Linux. O programa demonstra:

**Funcionalidades principais:**
- Registro de handlers personalizados com `signal()`
- Envio de sinais entre processos usando `kill()`
- Espera assíncrona por sinais com `pause()`



##### Usando o comando " strace -c ./treatsignal.c ", vamos conseguir estatísticas relacionadas ao uso das chamadas ao sistema e o tempo gasto por elas: 
![capturaTratamentoDeSinal](https://github.com/user-attachments/assets/5c50d952-e9d0-419f-b5ec-763d7892233c)
<p>&nbsp;</p>

## Observações:

kill (5.33%): Tempo gasto no envio de sinais entre processos.

clock_nanosleep (5.31%): Corresponde ao pause() para espera por sinais.

execve (34.73%): Valor anormalmente alto - verificar se há chamadas não intencionais de exec.

Padrão de comunicação:

Eficiência na entrega de sinais (baixo tempo em kill).

wait4 (37.69%) indica sincronização após tratamento de sinais.

