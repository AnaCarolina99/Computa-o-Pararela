/**
* Pontifícia Universidade Católica de Minas Gerais
* Computação Paralela - Tarefa27
* Parallelized by:
* @author Ana Carolina Medeiros Gonçalves
* @author Arthur Gabriel Mathias Marques 
* @author Igor Machado Seixas
* @author Vinicius Francisco da Silva
* @version 0.01
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define TAG 1

void main(int argc, char* argv[]) {
  int i, rank, val, numProcs;
  MPI_Status status;

  MPI_Init(&argc, &argv) ;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
  if (rank == 0) {
    val = 51;
		
		for(i=1; i<numProcs; i++){
    	// enviar o valor para todos os processos
    	printf("Process %d sends a message to %d\n",rank,i);
    	MPI_Send(&val, 1, MPI_INT, i, TAG, MPI_COMM_WORLD);
    }
  } else {
    // receber o valor enviado
    MPI_Recv(&val, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);
    printf("Process %d receives a message from %d with value %d\n",rank,0,val);
  }
  MPI_Finalize();
}
