#ifndef Nodo_A
#define Nodo_A
#include <iostream>
using namespace std;
#include "NodoV.h"
template<class T>
class NodoV;
template<class T>// un tipo de dato generico 
class NodoA
{
	private: 
		NodoV<T> *info;//apuntador a nodo generico
		NodoA<T> *prox;
		float costo;
	public:	
		//Constructores					
		NodoA():prox(NULL),costo(0){}
		NodoA(NodoV<T>* p):info(p),prox(NULL),costo(0){}//Con parametro
		NodoA(NodoV<T>* p,NodoA<T>* sig, float c):info(p),prox(sig),costo(c){}//Con parametro
		//Métodos de modificación
		void modificarInfo(NodoV<T>* e){info=e;}
		void modificarProximo(NodoA<T>* sig){prox=sig;}
		void modificarCosto(float money){costo=money;}
		//Métodos de Inspección
		NodoA<T>* proximo()const{return(prox);}
		NodoV<T>* Obt_info()const{return(info);}
		float Obt_costo(){return(costo);}
};

#endif
