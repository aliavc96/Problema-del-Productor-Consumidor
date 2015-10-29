// Problema creado para la práctica 1 de Sistemas Concurrentes Distribuidos de 2º de Ingeniería Informatica en la ETSIIT
// Nombre: Ana Alicia Vílchez Ceballos.


// El problema del productor-consumidor surge cuando se quiere diseñar un programa en el cual un proceso o hebra 
// produce items de datos en memoria que otro proceso consume.
// Este problema es aplicable a una aplicación de reproducción de video.

// Para la compilación del programa se procederá a aplicar las siguientes órdenes:
// g++ -g -c Prod-Cons.cpp
// g++ -o Prod-Cons Prod-Cons.o -lrt -lpthread



#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>

using namespace std ;

// ---------------------------------------------------------------------
// constantes 

  int num_items  = 40;
  int tam_vector = 10; 
  int *vector = new int [tam_vector]; 
  sem_t puede_escribir, puede_leer;  // declaración global de los semáforos  
  

// ---------------------------------------------------------------------

unsigned producir_dato()
{
  static int contador = 0 ;
  cout << "producido: " << contador << endl << flush ;
  return contador++ ;
}
// ---------------------------------------------------------------------

void consumir_dato( int dato )
{
    cout << "dato recibido: " << dato << endl ;
}
// ---------------------------------------------------------------------

void * productor( void * )
{   
  for( unsigned i = 0 ; i < num_items ; i=i+10)
  { 
    for(int j=0; j < tam_vector; j++){
    	sem_wait(&puede_escribir); // espera para poder escribir el dato (producir)
    	int dato = producir_dato() ;
   		vector [j] = dato;
   		sem_post(&puede_leer); // "pone al semáforo puede_leer en verde" de modo que pueda consumir
   	}
   }

  return NULL ;
}
// ---------------------------------------------------------------------

void *consumidor( void * )
{   
  for( unsigned i = 0 ; i < num_items ; i=i+10)
  {   
    int dato ;
    
    for(int j=0; j < tam_vector; j++){
    	sem_wait(&puede_leer);  // Espera al que se escriba el dato para poder leerlo
    	consumir_dato(vector[j]) ;
    	sem_post(&puede_escribir);  //"pone al semáforo puede_escribir en verde" de modo que pueda producir
    }  
  }
  return NULL ;
}
//----------------------------------------------------------------------

int main()
{
   
  pthread_t hebra1, hebra2; // declaración de las hebras: la hebra1 se encargará de la produccion y la hebra2 será la que consuma 
  
  sem_init(&puede_escribir, 0, 1);
  sem_init(&puede_leer, 0, 0);
  
  pthread_create(&hebra1, NULL, productor, NULL); //Procemos a crear la hebra productora
  pthread_join(hebra1, NULL); //Asociamos la hebra a su función 
  
  pthread_create(&hebra2, NULL, consumidor, NULL); // Procedemos a crear la hebra consumidora
  pthread_join(hebra2, NULL);

  sem_destroy(&puede_escribir); // Destrucción de los semaforos
  sem_destroy(&puede_leer);

  delete [] vector; //Destrucción del vector de enteros

  return 0 ; 
}
