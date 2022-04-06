/**
* Pontifícia Universidade Católica de Minas Gerais
* Computação Paralela - Tarefa24
* @author Ana Carolina Medeiros Gonçalves
* @author Arthur Gabriel Mathias Marques 
* @author Igor Machado Seixas
* @author Vinicius Francisco da Silva
*/

// Execution Time Serial:                           0m:00.335s      | SpeedUp

// Execution Time Parallel CPU OpenMP     	        0m:00.320s      | ----		

// Execution Time Parallel GPU OpenMP	            0m:01.620s      | ----
    // [CUDA memcpy HtoD]               = 463.39ms
    // sum_cuda(double*, double*, int)  = 7.3908ms

// Execution Time Parallel GPU CUDA                 0m:01.560s      | ----     
    // [CUDA memcpy HtoD]               = 464.81ms
    // sum_cuda(double*, double*, int)  = 21.592msclear

// Execution Time Parallel GPU CUDA __shared__      0m:01.560s      | ----
    // [CUDA memcpy HtoD]               = 464.81ms
    // sum_cuda(double*, double*, int)  = 21.592msclear



/**
 * Foi observado e executado o código paralela para multicore OpenMp, 
 * e paralela para GPU com OpenMp e usando CUDA. Verificamos que para a versão sequencial tivemos o menor dos tempos.
 * isto pode significar que para criar, executar e sincronizar as threads teremos um trabalho elevado.
 * 
 * Em segundo lugar ficou o CPU OpenMP acreditamos que devido a utilização de poucas threads e complexidade de execução e sincronização
 * inferior ele obteve tempos melhores que a GPU.
 * 
 * Por mim não ouve grande diferença entre a GPU OpenMP e GPU CUDA __shared__ porem a CUDA ficou um pouco melhor o que era esperado
 * ja que é uma linguagem desenvolvida para placas NVIDIA e especificamente para este propósito.
 * Não foi implementado a versão sem o uso do __shared__.
 */