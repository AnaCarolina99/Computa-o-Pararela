/**
* Pontifícia Universidade Católica de Minas Gerais
* Computação Paralela - Tarefa26
* Parallelized by:
* @author Ana Carolina Medeiros Gonçalves
* @author Arthur Gabriel Mathias Marques 
* @author Igor Machado Seixas
* @author Vinicius Francisco da Silva
* @version 0.01
*/

// Execution Time Serial:                           0m:00.406s      | SpeedUp

// Execution Time Parallel GPU CUDA                 0m:01.960s      | ----     
    // [CUDA memcpy HtoD]                = 469.93ms  (2 chamadas)
    // [CUDA mempcy DtoH]                = 376.38    (2 chamadas)
    // scan_cuda(double*, double*, int)  = 46.740ms
    // add_cuda(double*, double*, int)   = 15.119ms


/**
 * Implementamos a versão paralela GPU para CUDA. Observamos que esta versão possuía um tempo maior de execução.
 * Executamos o nvprof para verificação do que poderia estar acontecendo. Ao final deste arquivo segue resultado completo.
 * Observamos que ele gastou quase 1 segundo para copiar as variáveis entre a CPU e a GPU e atribuimos
 * este fato ao desempenho inferior da versão paralelizada com GPU CUDA.
 */

    #include <stdio.h>
    #include <stdlib.h>
    
    __global__ void scan_cuda(double* a, double *s, int width) {
      // kernel scan
      int t = threadIdx.x;
      int b = blockIdx.x * blockDim.x;
      double x;
    
      // cria vetor na memória local
      __shared__ double p[1024];
    
      // carrega elementos do vetor da memória global para a local
      if(b+t < width){
          p[t] = a[b+t];
      }
        __syncthreads();
    
        for(unsigned int i=1; i < blockDim.x; i <<= 1){
            if(t >= i){ // verifica se a thread ainda participa neste passo
                x = p[t] + p[t-i];
            }
            __syncthreads();
        
    
            if(t >= i){
                p[t] = x; // copia soma em definitivo para o vetor local
            }
            __syncthreads();
        }
    
        if(b+t < width){ // copia da memória local apra a global
            a[b+t] = p[t];
        }
    
        if(t == blockDim.x-1){ // se for a última thread do bloco.
            s[blockIdx.x+1] = a[b+t]; // copia o seu valor para o vertor de saída.
        }
    } 
    
    __global__ void add_cuda(double *a, double *s, int width) {
      // kernel soma
      int t = threadIdx.x;
      int b = blockIdx.x * blockDim.x;
    
      // soma o somatório do último elemento do bloco anterior ao elemento atual
      if(b+t < width){
          a[b+t] += s[blockIdx.x];
      }
    }
    
    int main()
    {
      int width = 40000000;
      int size = width * sizeof(double);
    
      int block_size = 1024;
      int num_blocks = (width-1)/block_size+1;
      int s_size = (num_blocks * sizeof(double));  
     
      double *a = (double*) malloc (size);
      double *s = (double*) malloc (s_size);
    
      for(int i = 0; i < width; i++)
        a[i] = i;
    
      double *d_a, *d_s;
    
      // alocar vetores "a" e "s" no device
      cudaMalloc((void **) &d_a, size);
      cudaMalloc((void **) &d_s, s_size);
    
      // copiar vetor "a" para o device
      cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
    
      // definição do número de blocos e threads (dimGrid e dimBlock)
      dim3 dimGrid(num_blocks, 1, 1);
      dim3 dimBlock(block_size, 1, 1);
    
      // chamada do kernel scan
      scan_cuda<<<dimGrid, dimBlock>>>(d_a, d_s, width);
    
      // copiar vetor "s" para o host
      cudaMemcpy(s, d_s, s_size, cudaMemcpyDeviceToHost);
    
      // scan no host (já implementado)
      s[0] = 0;
      for (int i = 1; i < num_blocks; i++)
        s[i] += s[i-1];
     
      // copiar vetor "s" para o device
      cudaMemcpy(d_s, s, s_size, cudaMemcpyHostToDevice);
    
      // chamada do kernel da soma
      add_cuda<<<dimGrid, dimBlock>>>(d_a, d_s, width);
    
      // copiar o vetor "a" para o host
      cudaMemcpy(a, d_a, size, cudaMemcpyDeviceToHost);
    
      printf("\na[%d] = %f\n",width-1,a[width-1]);
      
      cudaFree(d_a);
      cudaFree(d_s);
    }
    
    /**
    ==2391== Profiling application: ./scan_cuda
    ==2391== Profiling result:
    Time(%)      Time     Calls       Avg       Min       Max  Name
    51.72%  469.39ms         2  234.69ms  452.27us  468.93ms  [CUDA memcpy HtoD]
    41.47%  376.38ms         2  188.19ms  362.28us  376.01ms  [CUDA memcpy DtoH]
    5.15%  46.740ms         1  46.740ms  46.740ms  46.740ms  scan_cuda(double*, double*, int)
    1.67%  15.119ms         1  15.119ms  15.119ms  15.119ms  add_cuda(double*, double*, int)

    ==2391== API calls:
    Time(%)      Time     Calls       Avg       Min       Max  Name
    79.37%  908.82ms         4  227.20ms  87.531us  467.78ms  cudaMemcpy
    20.50%  234.72ms         2  117.36ms  9.7090us  234.71ms  cudaMalloc
    0.06%  731.38us         2  365.69us  44.425us  686.96us  cudaFree
    0.04%  484.25us        90  5.3800us     274ns  206.65us  cuDeviceGetAttribute
    0.01%  107.11us         1  107.11us  107.11us  107.11us  cuDeviceTotalMem
    0.01%  79.656us         2  39.828us  25.777us  53.879us  cudaLaunch
    0.01%  65.786us         1  65.786us  65.786us  65.786us  cuDeviceGetName
    0.00%  10.203us         6  1.7000us     352ns  7.3800us  cudaSetupArgument
    0.00%  4.6610us         2  2.3300us     780ns  3.8810us  cudaConfigureCall
    0.00%  2.8460us         2  1.4230us     971ns  1.8750us  cuDeviceGetCount
    0.00%  1.0690us         2     534ns     495ns     574ns  cuDeviceGet
    */