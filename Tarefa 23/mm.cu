/**
* Pontifícia Universidade Católica de Minas Gerais
* Computação Paralela - Tarefa23
* Parallelized by:
* @author Ana Carolina Medeiros Gonçalves
* @author Arthur Gabriel Mathias Marques 
* @author Igor Machado Seixas
* @author Vinicius Francisco da Silva
* @version 0.01
*/

// Execution Time Serial:                                       1m:15.600s      | SpeedUp
// Execution Time Parallel CPU     			                        0m:17.415s      | 4.34			
// Execution Time Parallel GPU Distribute	                      1m:50.018s      | ----
// Execution Time Parallel GPU Distribute Parrallel For         0m:20.861s      | 3.62
    // Warps Launched             = 72.
    // Warp Execution Efficiency  = 100%
// Execution Time Parallel GPU Distribute Parrallel For Simd    0m:04.517s      | 16.73     
    // Warps Launched             = 72.
    // Warp Execution Efficiency  = 86.81%
//####NOVO####
// Execution Time Parallel GPU CUDA                             0m:02.041s      | 37.04     **BEST SPEEDUP**
    // Warps Launched             = 125000.
    // Warp Execution Efficiency  = 100.00%



/**
 * Foi observado e executado o código paralela para multicore, 
 * e paralela para GPU com as diretivas "distribute", "distribute parallel for" 
 * e "distribute parallel for simd" e usando CUDA. Verificamos na versão multicore um SpeedUp de 4.34. 
 * Para a primeira experiêcia usando somente o "distribute" não gerou SpeedUp, 
 * devido a isto e a demora para o teste com o  nvprof optamos em não verificar os Warps Launched e 
 * Warp Execution Efficiency.
 * 
 * Para a versão usando "parallel for" obtivemos um SpeedUp de 3.62 ainda inferior ao CPU. Embora verificado que
 * a eficiencia no Warp Execution é maior que usando o SIMD, o melhor SpeedUp ficou usando as diretivas 
 * "distribute parallel for simd". Obtemos um SpeedUp de 16.73  muito superior ao CPU.
 * 
 * ####NOVO####
 * Por último foi implementado a versão usando o CUDA. Verificamos que, como a linguagem CUDA é escrita pela Nvidia
 * e de uso específico para GPU, o SpeedUp e eficiência aumentaram muito. Alcançando 37.04 de SpeedUp com uma
 * eficiência de 100%.
 */

#include <stdio.h>
#include <stdlib.h>

__global__ void mm_cuda(double* a, double* b, double* c, int width) 
{
	//#pragma omp parallel for simd schedule(static, 100)
	//#pragma omp target map(tofrom:c[0:size]) map(to:a[0:size],b[0:size])
	//#pragma omp teams distribute parallel for simd
	int i = blockIdx.x*blockDim.x+threadIdx.x;
	int j = blockIdx.y*blockDim.y+threadIdx.y;

	if(i<width){
		if(j<width){
      double sum = 0;
      for (int k = 0; k < width; k++) {
				double x = a[i * width + k];
				double y = b[k * width + j];
				sum += x * y;
      }
  	c[i * width + j] = sum;
		}
	}
}

int main()
{
  int width = 2000;
  double *a = (double*) malloc (width * width * sizeof(double));
  double *b = (double*) malloc (width * width * sizeof(double));
  double *c = (double*) malloc (width * width * sizeof(double));

  for(int i = 0; i < width; i++) {
    for(int j = 0; j < width; j++) {
      a[i*width+j] = i;
      b[i*width+j] = j;
      c[i*width+j] = 0;
    }
  }
	
	int size = width*width*sizeof(double);
	double *d_a, *d_b, *d_c;

	cudaMalloc((void**) &d_a, size);
	cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
	
	cudaMalloc((void**) &d_b, size);
	cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);

	cudaMalloc((void**) &d_c, size);

	int block_size = 8;
	dim3 dimGrid((width-1)/block_size+1, (width-1)/block_size+1, 1);
	dim3 dimBlock(block_size, block_size, 1);

	mm_cuda<<<dimGrid,dimBlock>>>(d_a, d_b, d_c, width);

	cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);
  	//mm(a,b,c,width);

	//for(int i = 0; i < width; i++) {
    	//for(int j = 0; j < width; j++) {
      		//printf("\n c[%d][%d] = %f",i,j,c[i*width+j]);
    	//}
   	//}

	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);

}
