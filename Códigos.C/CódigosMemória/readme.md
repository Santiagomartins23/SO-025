
## Gerenciamento de Memória

 #### Foram criados 3 programas na linguagem de programação C, cada um desses programas utiliza uma primitiva de chamada de sistema disponível no Linux, as escolhidas para a categoria gerenciamento de memória foram BRK, MMAP e MPROTECT. Nessa mesma pasta, estão todos os códigos descritos ao longo deste documento.

-----

<p>&nbsp;</p>

 ### Arquivo brk.c 

   Nesse arquivo, foi utilizada a função sbrk() para manipular diretamente a memória heap de um programa. Nele, é alocada memória para a heap, expandida e por fim reduzida. A função sbrk() utilizada no arquivo é uma forma de interação com a primitiva de chamada de sistema brk(). Ao compilarmos e rodarmos o programa é nos dado o seguinte output:


 ![image](https://github.com/user-attachments/assets/be68c1bd-0565-4d21-9c69-ef12cd472deb)
 
 <p>&nbsp;</p>
 
 ##### Usando o comando " strace -c ./BRK ", vamos conseguir estatísticas relacionadas ao uso das chamadas ao sistema e o tempo gasto por elas: 


![image](https://github.com/user-attachments/assets/949c795b-79ee-4c3d-99c0-47bdb62f6828)




## Observações

Principais chamadas de sistema:

execve (48.47%): Indica que quase metade do tempo de execução foi dedicado à chamada de sistema execve, responsável por carregar e executar programas.

mmap (16.10%): Representa o segundo maior consumo de tempo, referente à função que mapeia arquivos ou dispositivos na memória, essencial para a alocação eficiente de recursos.

write (7.12%): Relaciona-se ao custo de escrever dados em arquivos ou dispositivos, impactando o desempenho em operações de entrada e saída.

mprotect (5.50%): Refere-se à proteção de regiões da memória, uma operação necessária para garantir segurança e controle de acesso.

##### brk (5.50%): Envolve a manipulação do segmento de dados de um programa, ajustando seus limites na memória.


<p>&nbsp;</p>
<p>&nbsp;</p>

 ### Arquivo mmap.c 
 
  A função mmap() em C é utilizada para mapear arquivos ou alocar memória diretamente, sem depender da heap tradicional ou do sistema de gerenciamento de memória padrão. Neste arquivo, a memória é alocada com mmap() usando a flag MAP_ANONYMOUS, o que significa que a memória não está associada a nenhum arquivo, e a flag MAP_PRIVATE, que cria um mapeamento privado da memória. Após a alocação, a memória é preenchida com valores inteiros, e os primeiros 10 valores são exibidos na tela. Ao compilarmos e rodarmos o programa é nos dado o seguinte output:


![image](https://github.com/user-attachments/assets/1155a2bc-c22f-48c9-bccb-6d285224a294)

<p>&nbsp;</p>

 ##### Usando o comando " strace -c ./mmap ", vamos conseguir estatísticas relacionadas ao uso das chamadas ao sistema e o tempo gasto por elas: 
 
![image](https://github.com/user-attachments/assets/f277f17f-2bcb-4fdf-b88e-988d31a96262)


## Observações

Principais chamadas de sistema:

write (38.65%): É a chamada que consumiu a maior parte do tempo, dedicando-se à escrita de dados em arquivos ou dispositivos.

##### mmap (23.54%): É usada para mapear arquivos ou dispositivos diretamente na memória virtual de um processo. Esse mapeamento cria uma correspondência entre o espaço de endereços do processo e o conteúdo do arquivo ou dispositivo.

munmap (9.89%): É responsável por liberar regiões de memória previamente mapeadas, removendo a associação entre o espaço de endereçamento virtual do processo e os recursos alocados.

mprotect (8.90%): Envolve a modificação de permissões em regiões da memória, essencial para segurança.

brk (4.52%): Envolve a manipulação do segmento de dados de um programa, ajustando seus limites na memória.


<p>&nbsp;</p>
<p>&nbsp;</p>

 ### Arquivo mprotect.c
 
  Neste arquivo, foi utilizada a função mprotect() para controlar as permissões de acesso a uma área de memória já alocada. A memória é alocada com mmap() e, em seguida, as permissões de acesso são modificadas para tornar a memória somente para leitura, impedindo a escrita nela. Quando a memória é acessada para algo que não seja leitura, como escrita, ocorre um "segmentation fault". Ao compilarmos e rodarmos o programa é nos dado o seguinte output:

 ![image](https://github.com/user-attachments/assets/e9dad229-843f-43d5-aeee-cc0d276f310e)

 <p>&nbsp;</p>

 ##### Usando o comando " strace -c ./mprotect ", vamos conseguir estatísticas relacionadas ao uso das chamadas ao sistema e o tempo gasto por elas: 

![image](https://github.com/user-attachments/assets/6f771361-f1da-4065-a55a-eb5b60e285fb)

## Observações

Principais chamadas de sistema:

