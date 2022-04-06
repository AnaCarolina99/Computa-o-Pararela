/**
* Pontifícia Universidade Católica de Minas Gerais
* Computação Paralela - Projeto01
* Parallelized by:
* Ana Carolina Medeiros Gonçalves
* Arthur Gabriel Mathias Marques 
* Igor Machado Seixas
* Vinicius Francisco da Silva
*/

A estrutura de arquivos está disposta da seguinte maneira:
.
..
--main.cpp 		 // Arquivo que contém código fonte em C++ para chamada e simulação da Rede Neural.
--file.in  		 // Arquivo que contem dados para a execução do main. Explicação deste arquivo virá mais abaixo.
--redeNeuralSequencial.c // Código da Rede Neural em série.
--redeNeural.c 	 	 // Código da Rede Neural em paralelo.

//Após a compilação e execução teremos mais dois arquivos:
--main			// Arquivo derivado da compilação, executável por meio do comando "./main".
--RedeNeural.bin   	// Arquivo derivado da execução do código, contém os binários que constituem a Rede Neural.

Explicação arquivo file.in:
Disposição e explicação das variáveis do arquivo:
"100 - 100 Neurônios na camada inicial.
 10  - 10  Camadas Escondidas.
 100 - 100 Neurônios em cada uma das Camadas Escondidas.
 100 - 100 Neurônios de Saída.
 n   - Indica que a criação e o primeiro cálculo da Rede Neural nessas condições terminaram."

Este arquivo foi montado para que a criação e cálculo da primeira geráção da Rede Neural durasse em torno
de 10 segundos. Testes iniciais indicaram a duração de 10.605s no servidor Parcode.
A ideia inicial é que caso a paralelização da Rede Neural seja efetiva, o tempo de construção, cálculo e gravação
da RedeNeural no arquivo RedeNeural.bin diminua. Está Rede Neural foi idealizada para que o método de aprendizado fosse o
"Random Mutations" que consiste pegar o melhor indivíduo de cada geração, replicá-lo e gerar pequenos ajustes randomicos na esperança de que surja um indivíduo mais bem adaptado. Portando uma junção de um algorítmo genético a Rede Neural.
Como o objetivo é somente a paralelização da Rede o método de aprendizado não foi implementado somente a construção e cálculo de saídas.
Código original em: https://github.com/JVictorDias/Dinossauro-Google

Explicação arquivo RedeNeural.bin:
Este arquivo contem os binários que representam a Rede Neural. Através dele é possível recuperar a rede através da
função "RNA_CarregarRede()". Caso o tamanho deste arquivo se compare e esteja próxima a ordem de grandeza na execução
da Rede Neural em série e paralelo é um indicativo de que a rede esteja funcionando de forma correta.

Como os valores gerados são randomizados podemos ter algumas variações em cada geração da Rede Neural.

Para compilar devemos utilizar o seguinte comando:
g++ main.cpp -o main -fopenmp

Para testar devemos utilizar o seguinte comando:
time ./main < file.in

Tempo sequencial: 10.605s. 

Foram utilizado a paralelização do OpenMp, utilizando o MAP e a estratégia de Schedule Static que é a padrão.
Foi testado diversos outros métodos, como reduction, dynamic, para redimensionamento. Nenhuma delas foi obtido um resultado melhor. Pelo contrário tivemos uma alta redução de eficiência.

As funções paralelizadas foram escolhidas de acordo com o artigo disponibilizado no material "cisci02.pdf". No caso da nossa rede foram:
- void RNA_CalcularSaida() - Como o nome já diz, calcula a sáida da rede das camadas e camadas escondidas.
- void RNA_CriarNeuronio() - Aqui que acontece a atualização dos pesos nas camadas.

A média de tempo de execução para 20 testes utilizando 1, 2, 4 e 8 threads foram as seguintes:
Utilizando 1 thread:
 -13.622s
 Este resultado era esperado, já que em uma aplicação paralela gastamos tempo para criar a thread e sicronizar quando definimos apenas 1 thread. Então aumentamos o resultado de tempo de execução em relação ao sequencial. Portanto não ouve SpeedUp.

Utilizando 2 thread:
-7.450s
 Aqui já vemos uma melhora. Resultado aproximadamente a metade do anterior como previsto. Aqui em relação ao sequencial temos um SpeedUp de 1.42. Gastamos tempo criando e sincronizando as threads porem já temos duas threads diminuindo o tempo de execução.

Utilizando 4 thread:
 -3.915s
 Aqui verificamos a maior melhora. Provavelmente pela máquina possuir 4 núcleos e uma boa distribuição de tarefas entre as threads. O SpeedUp atingido foi de 2.7.

Utilizando 8 thread:
 -13.792s
 Neste caso acontece parecido com o teste com 1 thread. O tempo gasto para criação, organização e sincronização das threads não é compensado pelo uso do paralelismo.
