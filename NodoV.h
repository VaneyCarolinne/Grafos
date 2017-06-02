#ifndef Nodo_V
#define Nodo_V
#include <iostream>
using namespace std;
#include "NodoA.h"
template<class T>
class NodoA;
template<class T>// un tipo de dato generico 
class NodoV
{
	private:
		T elemento; //elemento generico 
		NodoV<T> *prox;//apuntador a nodo generico
		NodoA<T> *pri;
		int etiqueta;
	public:	
		//Constructores					
		NodoV():prox(NULL),etiqueta(0){}
		NodoV(T e):prox(NULL),elemento(e),etiqueta(0){}//Con parametro
		NodoV(T e,NodoV<T>* sig, NodoA<T>* firts):elemento(e),prox(sig),pri(firts),etiqueta(0){}//Con parametro
		//Métodos de modificación
		void modificarElemento(T e){elemento=e;}
		void modificarProximo(NodoV<T>* sig){prox=sig;}
		void modificarPri(NodoA<T>* f){pri=f;}
		void modificarEtiqueta(int i){etiqueta=i;}
		//Métodos de Inspección
		NodoV<T>* proximo()const{return(this->prox);}
		NodoA<T>* ObtAdy()const{return(pri);}
		T obt_elemento()const{return(this->elemento);}
		int Etiqueta(){return(etiqueta);}
};

#endif
