/**********************************************************************
 * Algoritmo de treinamento back-propagation para redes multicamadas
**********************************************************************/

/************************* BIBLIOTECAS *******************************/
#include <iostream>
#include <omp.h>

#include "redeNeural.c"

using namespace std;

/************************* DEFINICOES ********************************/
#define MAXCAM              100             // N�mero m�ximo de camadas
#define MAXNEUIN            1000            // N�mero m�ximo de neur�nios de entrada.
#define MAXNEUOUT           1000            // N�mero m�ximo de neur�nios de saida.

/****************** PROGRAMA PRINCIPAL *****************************/
int main(void)
{
  int Numero_Neuronio_Camada_Inicial;     // Número de neurônios da Camada Inicial.
  int Numero_Camadas_Escondidas;          // Número de camadas escondidas da rede.
  int Numero_Neuronio_Camadas_Escodidas; // Número de neurônios da Camada Escondida.
  int Numero_Neuronio_Saida;              // Número de neurônios da Camada de Saída.
  int i, j;

  double Entrada[MAXNEUIN];
  double Saida[MAXNEUOUT];

  char Continua = 'r';
  RedeNeural *R;
  srand(time(NULL));

  //omp_set_nested(1);
  omp_set_num_threads(4);

  while(Continua != 'n')
  {

    if(Continua == 'r')
    {
      cout << "\n\nDigite o numero de Neurônios da primeira camada da Rede Neural: ";
      cin >> Numero_Neuronio_Camada_Inicial;  

      cout << "\n\nDigite o numero de camadas internas da Rede Neural: ";
      cin >> Numero_Camadas_Escondidas;

      cout << "\n\nDigite o numero de Neurônios da(s) camada(s) internas da Rede Neural: ";
      cin >> Numero_Neuronio_Camadas_Escodidas;

      cout << "\n\nDigite o numero de Neurônios da camada de saída da Rede Neural: ";
      cin >> Numero_Neuronio_Saida;

      R = RNA_CriarRedeNeural(Numero_Camadas_Escondidas, Numero_Neuronio_Camada_Inicial, Numero_Neuronio_Camadas_Escodidas, Numero_Neuronio_Saida);
    }

    //cout << "\n\nDigite as entradas da rede:\n";
    for(j=0; j<10000; j++){

      for(i=0; i < Numero_Neuronio_Camada_Inicial; i++)
      {
        //cout << "\nEntrada " << i << " : ";
        //cin >> Entrada[i];
        Entrada[i] = i*rand()%1000;
      }
    


      //for(i=0; i <= Numero_Neuronio_Saida-1;i++)
      //{
        //cout << "\nSaida " << i << " : " << Saida[i];
      //}

      RNA_CopiarParaEntrada(R, Entrada);     /// Enviando informações para a rede neural.
      RNA_CalcularSaida(R);                  /// Calculando a decisão da rede.
      RNA_CopiarDaSaida(R, Saida);           /// Extraindo a decisão para vetor Saída.
		}

    cout << "\n\nContinua ? (s/n/r)\n";
    cin >> Continua;
  }


  RNA_SalvarRede(R, "RedeNeural.bin");
  R = RNA_DestruirRedeNeural(R);

  return 0;
}



