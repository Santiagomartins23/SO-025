
# Gerenciamento de Memória

 ###Foram criados 3 programas na linguagem de programação C, cada um desses programas utiliza uma primitiva de chamada de sistema disponível no Linux, as escolhidas para a categoria gerenciamento de memória foram BRK, MMAP e MPROTECT.
<p>&nbsp;</p>

 ## Arquivo BRK.c - Nesse arquivo, foi utilizada a função sbrk() para manipular diretamente a memória heap de um programa. Nele, é alocada memória para a heap, expandida e por fim reduzida. A função sbrk() utilizada no arquivo é uma forma de interação com a primitiva de chamada de sistema brk(). Ao compilarmos e rodarmos o programa é nos dado o seguinte output:


 ![image](https://github.com/user-attachments/assets/be68c1bd-0565-4d21-9c69-ef12cd472deb)
 
 <p>&nbsp;</p>
 
 Usando o comando " strace -c ./BRK ", vamos conseguir estatísticas relacionadas ao uso das chamadas ao sistema e o tempo gasto por elas: 


![image](https://github.com/user-attachments/assets/949c795b-79ee-4c3d-99c0-47bdb62f6828)

<p>&nbsp;</p>
<p>&nbsp;</p>

 ## Arquivo mmap.c - A função mmap() em C é utilizada para mapear arquivos ou alocar memória diretamente, sem depender da heap tradicional ou do sistema de gerenciamento de memória padrão. Neste arquivo, a memória é alocada com mmap() usando a flag MAP_ANONYMOUS, o que significa que a memória não está associada a nenhum arquivo, e a flag MAP_PRIVATE, que cria um mapeamento privado da memória. Após a alocação, a memória é preenchida com valores inteiros, e os primeiros 10 valores são exibidos na tela. Ao compilarmos e rodarmos o programa é nos dado o seguinte output:


![image](https://github.com/user-attachments/assets/1155a2bc-c22f-48c9-bccb-6d285224a294)

<p>&nbsp;</p>

 Usando o comando " strace -c ./mmap ", vamos conseguir estatísticas relacionadas ao uso das chamadas ao sistema e o tempo gasto por elas: 
 
![image](https://github.com/user-attachments/assets/f277f17f-2bcb-4fdf-b88e-988d31a96262)

<p>&nbsp;</p>
<p>&nbsp;</p>

 ## Arquivo mprotect.c - Neste arquivo, foi utilizada a função mprotect() para controlar as permissões de acesso a uma área de memória já alocada. A memória é alocada com mmap() e, em seguida, as permissões de acesso são modificadas para tornar a memória somente para leitura, impedindo a escrita nela. Quando a memória é acessada para algo que não seja leitura, como escrita, ocorre um "segmentation fault".

 ![image](https://github.com/user-attachments/assets/e9dad229-843f-43d5-aeee-cc0d276f310e)

 <p>&nbsp;</p>

 Usando o comando " strace -c ./mprotect ", vamos conseguir estatísticas relacionadas ao uso das chamadas ao sistema e o tempo gasto por elas: 

![image](https://github.com/user-attachments/assets/6f771361-f1da-4065-a55a-eb5b60e285fb)



