/*
* Adapted from: http://w...content-available-to-author-only...s.org/sieve-of-eratosthenes
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/*Aluna: Ana Carolina Medeiros Gonçalves
 *Matricula: 591513
 *
 * Tempo sequencial: 0m4.038s
 *
 * schedule(guided,100)
 * Tempo paralelo: 0m2.534s
 * SpeedUP: 1,593
 *
 * schedule(static,100)
 * Tempo paralelo: 0m2.541s
 * SpeedUP: 1,589
 *
 * schedule(dynamic,100)
 * Tempo paralelo: 0m2.560s
 * SpeedUP: 1,577
*/

int sieveOfEratosthenes(int n)
{
   // Create a boolean array "prime[0..n]" and initialize
   // all entries it as true. A value in prime[i] will
   // finally be false if i is Not a prime, else true.
   int primes = 0; 
   bool *prime = (bool*) malloc((n+1)*sizeof(bool));
   int sqrt_n = sqrt(n);

   memset(prime, true,(n+1)*sizeof(bool));

   for (int p=2; p <= sqrt_n; p++)
   {
       // If prime[p] is not changed, then it is a prime
       if (prime[p] == true)
       {
           // Update all multiples of p
           #pragma omp parallel for
           for (int i=p*2; i<=n; i += p)
           prime[i] = false;
        }
    }

    // count prime numbers
    #pragma omp parallel for reduction (+:primes) schedule(dynamic, 100)
    for (int p=2; p<=n; p++)
       if (prime[p])
         primes++;

    return(primes);
}

int main()
{
   int n = 100000000;
   printf("%d\n",sieveOfEratosthenes(n));
   return 0;
} 
