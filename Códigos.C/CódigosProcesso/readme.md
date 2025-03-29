## Gerenciamento de Processos

Foram desenvolvidos 3 programas em C que utilizam primitivas de chamadas de sistema do Linux para gerenciamento de processos. As primitivas escolhidas para esta categoria foram FORK, SCHEDULING e SIGNAL HANDLING. Todos os códigos referentes a esta seção estão disponíveis nesta pasta.

---

### Arquivo fork.c
Neste arquivo, foi utilizada a função `fork()` para criar novos processos.

**Funcionalidades principais:**
- Criação de processos filhos
- Execução concorrente entre processos pai e filho
- Uso de `wait()` para sincronização

Ao compilar e executar o programa, obtemos o seguinte output:
<p>&nbsp;</p>







![capturaFork](https://github.com/user-attachments/assets/c20a6304-d2ed-4070-bd30-1ea86e795d57)
<p>&nbsp;</p>




### Arquivo scheduling.c
Neste arquivo, foram implementados algoritmos de escalonamento utilizando chamadas de sistema para controle de processos. O programa demonstra:
- **FIFO**: Execução sequencial de processos
- **Round-Robin**: Alternância entre processos com quantum fixo
- **Prioridades**: Ordenação por níveis de prioridade

**Técnicas utilizadas:**
- `fork()` para criar múltiplos processos
- `waitpid()` para controle preciso de espera
- `sleep()` para simular tempo de execução


![capturaEscalonamento](https://github.com/user-attachments/assets/942cd56d-e4e2-401a-9aee-53e48e7ae87b)
<p>&nbsp;</p>


### Arquivo treatsignal.c

Neste arquivo, foi implementado o tratamento de sinais entre processos utilizando primitivas do sistema Linux. O programa demonstra:

**Funcionalidades principais:**
- Registro de handlers personalizados com `signal()`
- Envio de sinais entre processos usando `kill()`
- Espera assíncrona por sinais com `pause()`




![capturaTratamentoDeSinal](https://github.com/user-attachments/assets/5c50d952-e9d0-419f-b5ec-763d7892233c)


