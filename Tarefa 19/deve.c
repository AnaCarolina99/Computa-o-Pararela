/**
* Pontifícia Universidade Católica de Minas Gerais
* Computação Paralela - Tarefa19
* Code by:
* @author Ana Carolina Medeiros Gonçalves
* @author Arthur Gabriel Mathias Marques 
* @author Igor Machado Seixas
* @author Vinicius Francisco da Silva
* @version 0.01
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

double pass;

int main(){

	int SIZE = 1000;
  
  int i;
  int *a = (int*) malloc(SIZE*sizeof(int));
  int *b = (int*) malloc(SIZE*sizeof(int));
  int *c = (int*) malloc(SIZE*sizeof(int));

  int sum = 0;
  double x = 0.0;
	int limit = (rand()%SIZE);
   
	pass = 1.0/(double)SIZE;

   for(i=0; i < limit; i+=2){

			a[i] += b[i]+c[i]; //Non-contiguous Memory Accesses
			a[i] = b[i]*b[i]; //Non-contiguous Memory Accesses
			x=(i+0.5)*pass;
   		sum = sum+4.0/(1.0+x*x); // Insecure float point math.
      
      a[SIZE] = a[SIZE+1] - 1; //Data Dependencies  Read-after-write
      limit = (rand()%SIZE)+20;
			
   }
		


   return 0;
}


/**
 * Mensagem de erro após a compilação.

deve.c:23:4: note: not vectorized: number of iterations cannot be computed.
deve.c:23:4: note: bad loop form.
deve.c:13:8: note: not vectorized: not enough data-refs in basic block.
deve.c:8:5: note: not vectorized: not enough data-refs in basic block.
deve.c:8:5: note: not vectorized: not enough data-refs in basic block.
deve.c:25:9: note: not consecutive access _28 = MEM[(int *)a_6 + 4004B];

deve.c:25:9: note: not consecutive access MEM[(int *)a_6 + 4000B] = _29;

deve.c:25:9: note: not consecutive access *_17 = _26;

deve.c:25:9: note: SLP: step doesn't divide the vector-size.
deve.c:25:9: note: Unknown alignment for access: *_19
deve.c:25:9: note: Failed to SLP the basic block.
deve.c:25:9: note: not vectorized: failed to find SLP opportunities in basic block.
deve.c:8:5: note: not vectorized: not enough data-refs in basic block.

deve.c:23:4: note: not vectorized: number of iterations cannot be computed. -> O programa não consegue computar o número de iterações.
deve.c:23:4: note: bad loop form. -> Loop não consegue ser divido de forma a ser vetorizado.
deve.c:13:8: note: not vectorized: not enough data-refs in basic block. -> Exemplo da tarefa.
deve.c:25:9: note: not consecutive access _28 = MEM[(int *)a_6 + 4004B]; -> Acesso não consecutivo a memória.
deve.c:25:9: note: not vectorized: failed to find SLP opportunities in basic block. -> O SLP não conseguiu dividir tarefas de forma homogênea.
deve.c:25:9: note: SLP: step doesn't divide the vector-size. -> O SLP não consegue dividir o tamanho do vetor. 
*/
