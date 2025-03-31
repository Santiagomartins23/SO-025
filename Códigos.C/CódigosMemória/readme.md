
## Gerenciamento de Memória

 #### Foram criados 3 programas na linguagem de programação C, cada um desses programas utiliza uma primitiva de chamada de sistema disponível no Linux, as escolhidas para a categoria gerenciamento de memória foram BRK, MMAP e MPROTECT. Nessa mesma pasta, estão todos os códigos descritos ao longo deste documento.

---

<p>&nbsp;</p>

 ### Arquivo brk.c 

   Nesse arquivo, foi utilizada a função sbrk() para manipular diretamente a memória heap de um programa. Nele, é alocada memória para a heap, expandida e por fim reduzida. A função sbrk() utilizada no arquivo é uma forma de interação com a primitiva de chamada de sistema brk(). Ao compilarmos e rodarmos o programa é nos dado o seguinte output:

   - **Heap**: O programa utiliza a função sbrk para gerenciar a memória do heap.

```
void *heap_inicio = sbrk(0); 
    printf("\nEndereco inicial da heap: %p\n", heap_inicio);

    size_t tamanho = 4096;

    void *novo_limite = sbrk(tamanho); 

    if (novo_limite == (void *)-1) {
        perror("\nFalha ao expandir heap com sbrk");
        return 1;
    }

    printf("\nHeap expandida. Novo limite: %p\n", novo_limite);

    memset(novo_limite, 0, tamanho);  

```

   - **Gerenciamento de Heap** :

```
char *mensagem = (char *)novo_limite;
    strcpy(mensagem, "Ola, esta string foi armazenada na heap!");  //Armazena string na heap

    printf("String armazenada na heap: %s\n", mensagem);

    if (sbrk(-tamanho) == (void *)-1) {  //Reduz a heap ao tamanho original
        perror("Falha ao liberar heap com sbrk");
        return 1;
    }

```
<p>&nbsp;</p>

##### Ao compilarmos e rodarmos o programa é nos dado o seguinte output:

 ![image](https://github.com/user-attachments/assets/be68c1bd-0565-4d21-9c69-ef12cd472deb)
 
 <p>&nbsp;</p>
 
 ##### Usando o comando " strace -c ./BRK ", vamos conseguir estatísticas relacionadas ao uso das chamadas ao sistema e o tempo gasto por elas: 


![image](https://github.com/user-attachments/assets/949c795b-79ee-4c3d-99c0-47bdb62f6828)




## Observações

Principais chamadas de sistema:

execve (48.47%): Indica que quase metade do tempo de execução foi dedicado à chamada de sistema execve, responsável por carregar e executar programas.

mmap (16.10%): Representa o segundo maior consumo de tempo, referente à chamada que mapeia arquivos ou dispositivos na memória, essencial para a alocação eficiente de recursos.

write (7.12%): É a chamada que escreve dados em arquivos ou dispositivos, impactando o desempenho em operações de entrada e saída.

mprotect (5.50%): Refere-se à proteção de regiões da memória, uma operação necessária para garantir segurança e controle de acesso.

#### brk (5.50%): Envolve a manipulação do segmento de dados de um programa, ajustando seus limites na memória.

---
<p>&nbsp;</p>
<p>&nbsp;</p>

 ### Arquivo mmap.c 
 
  A função mmap() em C é utilizada para mapear arquivos ou alocar memória diretamente, sem depender da heap tradicional ou do sistema de gerenciamento de memória padrão. Neste arquivo, a memória é alocada com mmap() usando a flag MAP_ANONYMOUS, o que significa que a memória não está associada a nenhum arquivo, e a flag MAP_PRIVATE, que cria um mapeamento privado da memória. Após a alocação, a memória é preenchida com valores inteiros, e os primeiros 10 valores são exibidos na tela. Ao compilarmos e rodarmos o programa é nos dado o seguinte output:

  - **Utilização da função mmap para alocação de memória.**
```
  size_t size = 4096;

    int *mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); //Aloca memória com mmap

    if (mem == MAP_FAILED) {
        perror("\nmmap falhou");
        return 1;
    }

    printf("\nMemoria alocada em: %p\n\n", mem);
```
 - **Interação dinâmica com a memória.**
```
   for (int i = 0; i < size / sizeof(int); i++) { //Preenche a memória com valores
        mem[i] = i * 10;
    }
    =
    printf("\nValores armazenados:\n");
    for (int i = 0; i < 10; i++) {
        printf("\nmem[%d] = %d\n", i, mem[i]);
    }

    =
    if (munmap(mem, size) == -1) {
        perror("munmap falhou");
        return 1;
    }
```
<p>&nbsp;</p>

##### Ao compilarmos e rodarmos o programa é nos dado o seguinte output:

![image](https://github.com/user-attachments/assets/1155a2bc-c22f-48c9-bccb-6d285224a294)

<p>&nbsp;</p>

 ##### Usando o comando " strace -c ./mmap ", vamos conseguir estatísticas relacionadas ao uso das chamadas ao sistema e o tempo gasto por elas: 
 
![image](https://github.com/user-attachments/assets/f277f17f-2bcb-4fdf-b88e-988d31a96262)


## Observações

Principais chamadas de sistema:

write (38.65%): É a chamada que consumiu a maior parte do tempo, dedicando-se à escrita de dados em arquivos ou dispositivos.

#### mmap (23.54%): É usada para mapear arquivos ou dispositivos diretamente na memória virtual de um processo. Esse mapeamento cria uma correspondência entre o espaço de endereços do processo e o conteúdo do arquivo ou dispositivo.

munmap (9.89%): É responsável por liberar regiões de memória previamente mapeadas, removendo a associação entre o espaço de endereçamento virtual do processo e os recursos alocados.

mprotect (8.90%): Envolve a modificação de permissões em regiões da memória, essencial para segurança.

brk (4.52%): Envolve a manipulação do segmento de dados de um programa, ajustando seus limites na memória.

---
<p>&nbsp;</p>
<p>&nbsp;</p>

 ### Arquivo mprotect.c
 
  Neste arquivo, foi utilizada a função mprotect() para controlar as permissões de acesso a uma área de memória já alocada. A memória é alocada com mmap() e, em seguida, as permissões de acesso são modificadas para tornar a memória somente para leitura, impedindo a escrita nela. Quando a memória é acessada para algo que não seja leitura, como escrita, ocorre um "segmentation fault". Ao compilarmos e rodarmos o programa é nos dado o seguinte output:

   - **Alternância de permissões de regiões da memória**

```
 if (mprotect(mem, size, PROT_READ) == -1) { //Atualiza memória para somente leitura usando PROT_READ
        perror("\nmprotect falhou ao tornar a memoria somente leitura");
        return 1;
    }
    printf("\nMemoria agora e somente leitura.\n");

    if (mprotect(mem, size, PROT_READ) != -1) {
        printf("\n\nTentando escrever na memoria protegida...\n");
        strcpy(mem, "Tentativa de escrita em memoria somente leitura."); //Ao tentar escrever na memória somente com a permissão para leitura, é retornado um segmentation fold
    } else {
        printf("\nA escrita na memoria esta proibida por conta da protecao.\n");
    }

    printf("Leitura apos protecao: %s\n", (char *)mem);

    if (mprotect(mem, size, PROT_READ | PROT_WRITE) == -1) { //Atualiza memória novamente mas dessa vez para leitura e escrita com PROT_READ e PROT_WRHITE
        perror("\nmprotect falhou ao tornar a memoria gravavel novamente");
        return 1;
    }
```
<p>&nbsp;</p>

##### Ao compilarmos e rodarmos o programa é nos dado o seguinte output:

 ![image](https://github.com/user-attachments/assets/e9dad229-843f-43d5-aeee-cc0d276f310e)

 <p>&nbsp;</p>

 ##### Usando o comando " strace -c ./mprotect ", vamos conseguir estatísticas relacionadas ao uso das chamadas ao sistema e o tempo gasto por elas: 

![image](https://github.com/user-attachments/assets/6f771361-f1da-4065-a55a-eb5b60e285fb)

## Observações

Principais chamadas de sistema:

execve (39.34%): Responsável por carregar e executar programas.

mmap (16.41%): É usada para mapear arquivos ou dispositivos diretamente na memória virtual de um processo. Esse mapeamento cria uma correspondência entre o espaço de endereços do processo e o conteúdo do arquivo ou dispositivo.

write (9.46%): É a chamada que escreve dados em arquivos ou dispositivos, impactando o desempenho em operações de entrada e saída.

#### mprotect (7.28%): A chamada mprotect é usada para alterar as permissões de acesso de uma região de memória já alocada em um processo. Com ela, é possível definir se uma área específica da memória será apenas legível, gravável, executável, ou ainda combinar essas permissões.

munmap (4.59%): É responsável por liberar regiões de memória previamente mapeadas, removendo a associação entre o espaço de endereçamento virtual do processo e os recursos alocados.
