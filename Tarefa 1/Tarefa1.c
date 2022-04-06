#include <stdio.h>
#include <omp.h>

/*Aluna: Ana Carolina Medeiros Gonçalves
 *Matricula: 591513
*/

int main()
{
    int i;

    #pragma omp parallel num_threads(2) // seta o número de threads em 2 
    {
        int tid = omp_get_thread_num(); // lê o identificador da thread 
        
        
        /* Adicionado "#pragma omp for ordered" no laço for abaixo: 
         * Para cada iteração ser realizada em diferentes threads(for) 
         * Indicar uma ordem a ser seguida(Ordered) 
        */
        #pragma omp for ordered
        for(i = 1; i <= 3; i++) 
        {
           
           // Bloco criado para o printf para indicar a ordem a ser seguida        
           #pragma omp ordered
           {
           printf("[PRINT1] T%d = %d \n",tid,i);   
           printf("[PRINT2] T%d = %d \n",tid,i);
           }
        }
    }
}
