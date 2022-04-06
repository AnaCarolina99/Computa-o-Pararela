/**
* Pontifícia Universidade Católica de Minas Gerais
* Computação Paralela - Tarefa09
*/

/*
* Grupo 8: Ana Carolina Medeiros Gonçalves
* Arthur Gabriel Mathias Marques 
* Igor Machado Seixas
* Kaiser Gabriel Silvério Batista dos Santos
*/

// Execution Time Serial: 								0:04.42     | SpeedUp
// Execution Time Parallel (NO STAGGER):  		0:02.94     | 1.50
// Execution Time Parallel (SCHEDULE DYNAMIC):  0:02.75     | 1.60

#include <stdio.h>
#include <stdlib.h>

int main() 
{
   int i, j, n = 30000; 

   // Allocate input, output and position arrays
   int *in = (int*) calloc(n, sizeof(int));
   int *pos = (int*) calloc(n, sizeof(int));   
   int *out = (int*) calloc(n, sizeof(int));   

   // Initialize input array in the reverse order
   for(i=0; i < n; i++)
      in[i] = n-i;  

   // Print input array
   //   for(i=0; i < n; i++) 
   //      printf("%d ",in[i]);
    
	#pragma omp parallel num_threads(2)
	{
   // Silly sort (you have to make this code parallel)
	#pragma omp for private(j) schedule(dynamic,200)
   for(i=0; i < n; i++)
      for(j=0; j < n; j++)
	     if(in[i] > in[j]) 
            pos[i]++;
	}
   
   // Move elements to final position
   for(i=0; i < n; i++) 
      out[pos[i]] = in[i];

   // print output array
   //   for(i=0; i < n; i++) 
   //      printf("%d ",out[i]);

   // Check if answer is correct
   for(i=0; i < n; i++)
      if(i+1 != out[i]) 
      {
         printf("test failed\n");
         exit(0);
      }

   printf("test passed\n");
	 
}  
