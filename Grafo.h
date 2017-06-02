#ifndef Grafo_H
#define Grafo_H
#include "NodoV.h"
#include "arco.h"
#include <string>
#include <queue>
#include <list>
#include <vector>
#include <climits>
using std:: list; 
using std:: vector;
using std:: queue;
#define N 100000
/********************************************************/
/*******************************************************/
/***********Definición de la Clase Grafo***************/
/*************Autora: Vanessa Cruz********************/
/****************************************************/
/***************************************************/
template<class T>
class Grafo
{
	private:
		NodoV<T>* g;
		int n;
	private:
		void eliminarAdy(NodoA<T>* p);		
		bool Igual(NodoV<T> *aux,NodoV<T> *aux2,bool &band);
	public:	
		//Constructores:
		Grafo():g(NULL),n(0){}
		Grafo(list<T> V, list<Arco<T> > A);
		Grafo(Grafo<T> &Orig);
		//Métodos de Inspección
		bool esVacio(){return(g==NULL);}
		int Orden(){return(n);}
		list<T> vertices();
		list<Arco<T> > arcos();
		bool existeVertice(T v);
		bool existeArco(T v, T w);
		float CostoArco(T v, T w);
		list<T> predecesores(T v);
		list<T> sucesores(T v);
		list<NodoV<T>* > sucesoresNODOS(T v);	
		int GradoInterior(T v);
		int GradoExterior(T v);
		bool Es_VerticeFuente(T v);
		bool Es_VerticeSumidero(T v);
		int GradoVertice(T v){return(GradoInterior(v)+GradoExterior(v));}
		list<T> ObtenerVerticesFuentes();
		list<NodoV<T>* > ObtenerNodosFuentes();
		list<T> Vecindad_De_Un_Vertice(T v);
		bool Es_aislado(T v){return(Vecindad_De_Un_Vertice(v).empty()||(Vecindad_De_Un_Vertice(v).front()==v&&Vecindad_De_Un_Vertice(v).size()==1));}
		bool alcanzable(T v, T w);
		void alcanzable(T v, T w, bool &band, T original);
		bool Marcado(T v);
		//Métodos de Modificación:
		void agregarVertice(T v);
		void agregarArco(T v, T w, float c);
		void eliminarVertice(T v);
		void eliminarArco(T v, T w);
		void Crear(list<T> V, list<Arco<T> > A);
		void Copiar(Grafo<T> &Orig);
		void eliminarPrede(T v);
		void vaciar();
		void Etiquetar();
		void MarcarVertice(T v);
		//Métodos de Busqueda:
		void BFS(T s, int dist[N], T pred[N]);
		void BFS(NodoV<T>* s, int dist[N], T pred[N]);
		void DFS(T pred[N],int tdesc[N], int tfinal[N]);
		void DFS_Visitar(NodoV<T>* u,T pred[N],int tdesc[N],int tfinal[N],string color[N],int &tiempo);
		void DFS_Visitar(NodoV<T>* u,string color[N],list<T> &ord);
		//Sobrecarga de operadores:
		void operator=(Grafo<T> &G);
		template<class A>
		friend ostream& operator<<(ostream& salida,Grafo<A>& objeto_grafo);
		bool operator ==(const Grafo<T> &G1);
		bool operator !=(const Grafo<T> &G1);
		//Métodos de Ordenamiento:
		list<T> OrdenamientoTopologico();
		//Algoritmos De Caminos:
		void Dijkstra(int &suma, int &cont,list<T> &Camino,list<T> &CaminoOptimo, T origen, T Destino);
		void Dijkstra(T origen, T Destino, list<T> &Camino, int &Costo);
		void Dijkstra(T origen);
		//Único Destructor de la Clase
		~Grafo(){vaciar();}

};
/*****************************/
/********Constructores:******/
/*****************************/
template<class T>
Grafo<T>::Grafo(list<T> V, list<Arco<T> > A)
{
	g=NULL;
	n=0;
	Crear(V,A);
}
template<class T>
Grafo<T>::Grafo(Grafo<T> &Orig)
{
	Copiar(Orig);	
}
template<class T>
void Grafo<T>::Crear(list<T> V, list<Arco<T> > A)
{
	while(!V.empty())
	{
		agregarVertice(V.front());
		V.pop_front();	
	}
	while(!A.empty())
	{
		agregarArco(A.front().getV(),A.front().getW(),A.front().getCosto());
		A.pop_front();	
	}
}
template<class T>
void Grafo<T>::Copiar(Grafo<T>& Orig)
{
	NodoV<T> *vertice;
	NodoA<T> *arco;
	
	vertice=Orig.g;
	while(vertice!=NULL)
	{
		agregarVertice(vertice->obt_elemento());
		vertice=vertice->proximo();	
	}
	vertice=Orig.g;
	while(vertice!=NULL){
		arco=vertice->ObtAdy();	
		while(arco!=NULL&&vertice!=NULL)
		{
			agregarArco(vertice->obt_elemento(),arco->Obt_info()->obt_elemento(),CostoArco(vertice->obt_elemento(),arco->Obt_info()->obt_elemento()));
			arco=arco->proximo();
		}
		vertice=vertice->proximo();
	}	
}
/*****************************/
/****Métodos Modificadores:***/
/*****************************/
template <class T>
void Grafo<T>::agregarVertice(T v)
{
	NodoV<T> *ant, *sig, *nuevo;
	
	nuevo=new NodoV<T>(v,NULL,NULL);
	ant=NULL;
	if(g!=NULL){
		sig=g;
		while(sig!=NULL&&sig->obt_elemento()< v)
		{
			ant=sig;
			sig=sig->proximo();	
		}
	}	
	if(ant==NULL)
	{
		nuevo->modificarProximo(g);
		g=nuevo;		
	}else{
		ant->modificarProximo(nuevo);
		nuevo->modificarProximo(sig);	
	}
	n++;	
}
template <class T>
void Grafo<T>::agregarArco(T v, T w, float c)
{
	NodoV<T> *origen, *buscar;
	NodoA<T> *ant, *sig, *nuevo;
	
	buscar=g;
	while(buscar->obt_elemento()!=w)
	{
		buscar=buscar->proximo();	
	}
	nuevo=new NodoA<T>(buscar,NULL,c);
	origen=g;
	while(origen->obt_elemento() != v)
	{
		origen=origen->proximo();	
	} 
	ant=NULL;
	sig=origen->ObtAdy();
	while(sig!=NULL&& sig->Obt_info()->obt_elemento() < w)
	{
		ant=sig;
		sig=sig->proximo();	
	}
	if(ant==NULL)
	{
		nuevo->modificarProximo(sig);
		origen->modificarPri(nuevo);	
	}else{
		ant->modificarProximo(nuevo);
		nuevo->modificarProximo(sig);	
	}
}
template <class T>
void Grafo<T>::eliminarVertice(T v)
{
		NodoV<T> *ant, *sig, *actual;
		NodoA<T> *p;
		
	if(g!=NULL){	
		ant=g;
		actual=ant->proximo();
		sig=actual->proximo();
		eliminarPrede(v);
		if(ant->obt_elemento() == v)
		{
			g=actual;
			ant->modificarProximo(NULL);
			p=ant->ObtAdy();
			eliminarAdy(p);
			delete(ant);
			ant=NULL;	
		}else{
			while(sig!=NULL && actual->obt_elemento() != v)
			{
				ant=actual;
				actual=ant->proximo();
				sig=actual->proximo();	
			}	
			p=actual->ObtAdy();
			ant->modificarProximo(sig);
			actual->modificarProximo(NULL);
			eliminarAdy(p);
			delete(actual);
			actual=NULL;
		}	
		n--;
	}	
}
template <class T>
void Grafo<T>::eliminarPrede(T v)
{
	NodoV<T> *origen;
	NodoA<T> *arco,*ant;

	origen=g;

	while(origen!=NULL)
	{
	  ant=origen->ObtAdy();
	  if(ant!=NULL){
		arco=ant->proximo();
		if(ant->Obt_info()->obt_elemento() == v)
		{
			origen->modificarPri(arco);
			ant->modificarInfo(NULL);
			ant->modificarProximo(NULL);
			delete(ant);
			ant=NULL;	
		}else{		
			while(arco!=NULL&&arco->Obt_info()->obt_elemento()!=v)
			{
				ant=arco;
				arco=ant->proximo();	
			}
		
			if(arco!=NULL){	
				ant->modificarProximo(arco->proximo());
				arco->modificarProximo(NULL);
				arco->modificarInfo(NULL);
				delete(arco);
				arco=NULL;
				origen->modificarPri(ant);
			}
		}
	  }	
	origen=origen->proximo();	
   }
}
template <class T>
void Grafo<T>::eliminarAdy(NodoA<T>* p)
{
	NodoA<T>* aux;
	
	while(p!=NULL){
		aux=p;
		aux->modificarInfo(NULL);
		delete(aux);
		aux=NULL;
		p=p->proximo();	
	}	
}
template <class T>
void Grafo<T>::eliminarArco(T v, T w)
{
	NodoV<T> *vertice;
	NodoA<T> *arco,*ant;
	
	if(g!=NULL){
		vertice=g;
		while(vertice!=NULL && vertice->obt_elemento() != v)
		{
			vertice=vertice->proximo();	
		}	
		if(vertice!=NULL)
		{
			ant=vertice->ObtAdy();
			arco=ant->proximo();
			if(ant->Obt_info()->obt_elemento() == w)
			{
				vertice->modificarPri(arco);
				ant->modificarInfo(NULL);
				delete(ant);
				ant=NULL;
			}else{
				while(arco!=NULL && arco->Obt_info()->obt_elemento() != w)
				{
					ant=arco;
					arco=arco->proximo();	
				}
				if(arco!=NULL)
				{
					ant->modificarProximo(arco->proximo());
					vertice->modificarPri(ant);
					arco->modificarInfo(NULL);
					delete(arco);
					arco=NULL;
				}
			}		
		}
	}	
}
template <class T>
void Grafo<T>::vaciar()
{
	NodoV<T> *origen,*aux;
	NodoA<T> *p;
	if(g!=NULL){
		origen=g;
		while(origen!=NULL)
		{
			p=origen->ObtAdy();
			eliminarAdy(p);
			aux=origen;
			origen=origen->proximo();
			delete(aux);
			aux=NULL;	
		}
		g=NULL;
		n=0;
	}		
}
/*****************************/
/****Métodos de Inspección:***/
/*****************************/
template <class T>
bool Grafo<T>::existeVertice(T v)
{
	NodoV<T> *vertice=g;	

	while(vertice!=NULL && vertice->obt_elemento() != v)
	{
		vertice=vertice->proximo();	
	}
	if(vertice!=NULL)
	{
		return(true);	
	}else{
		return(false);	
	}
}
template <class T>
bool Grafo<T>::existeArco(T v, T w)
{
	NodoV<T> *vertice=g;	
	NodoA<T> *arco;
	while(vertice!=NULL && vertice->obt_elemento() != v)
	{
		vertice=vertice->proximo();	
	}
	if(vertice!=NULL)
	{
		arco=vertice->ObtAdy();
		while(arco!=NULL && arco->Obt_info()->obt_elemento() != w)
		{
			arco=arco->proximo();	
		}
		if(arco!=NULL)
		{
			return(true);	
		}else{
			return(false);	
		}	
	}else{
		return(false);	
	}	
}
template <class T>
float Grafo<T>::CostoArco(T v, T w)
{
	NodoV<T> *vertice=g;	
	NodoA<T> *arco;
	while(vertice!=NULL && vertice->obt_elemento() != v)
	{
		vertice=vertice->proximo();	
	}
	if(vertice!=NULL)
	{
		arco=vertice->ObtAdy();
		while(arco!=NULL && arco->Obt_info()->obt_elemento() != w)
		{
			arco=arco->proximo();	
		}	
		if(arco!=NULL)
		{
			return(arco->Obt_costo());	
		}else{
			return(-1); //no existe el arco	
		}
	}	
}

template <class T>
list<T> Grafo<T>::predecesores(T v)
{
	NodoV<T> *vertice;
	NodoA<T> *actual;
	list<T> l;
	bool band;
	
	vertice=g;
	while(vertice!=NULL)
	{
		actual=vertice->ObtAdy();
		band=false;
		while(actual!=NULL && !band)
		{
			if(actual->Obt_info()->obt_elemento()==v)
			{
				l.push_back(vertice->obt_elemento());
				band=true;	
			}
			actual=actual->proximo();	
		}
		vertice=vertice->proximo();	
	}
	return(l);
}
template <class T>
list<T> Grafo<T>::sucesores(T v)
{
	NodoV<T> *vertice;
	NodoA<T> *arco;	
	list<T> l;
	
	vertice=g;
	while(vertice!=NULL && vertice->obt_elemento() != v)
	{
		vertice=vertice->proximo();	
	}
	if(vertice!=NULL)
	{
		arco=vertice->ObtAdy();
		while(arco!=NULL)
		{
			l.push_back(arco->Obt_info()->obt_elemento());
			arco=arco->proximo();
		}	
	}
	return(l);
}	
template <class T>
int Grafo<T>::GradoInterior(T v)
{
	NodoV<T> *vertice;
	NodoA<T> *actual;
	int cont=0;
	bool band;
	
	vertice=g;
	while(vertice!=NULL)
	{
		actual=vertice->ObtAdy();
		band=false;
		while(actual!=NULL && !band)
		{
			if(actual->Obt_info()->obt_elemento()==v)
			{
				cont++;
				band=true;	
			}
			actual=actual->proximo();	
		}
		vertice=vertice->proximo();	
	}
	return(cont);	
}
template <class T>
int Grafo<T>::GradoExterior(T v)
{
	NodoV<T> *vertice;
	NodoA<T> *arco;	
	int cont=0;
	
	vertice=g;
	while(vertice!=NULL && vertice->obt_elemento() != v)
	{
		vertice=vertice->proximo();	
	}
	if(vertice!=NULL)
	{
		arco=vertice->ObtAdy();
		while(arco!=NULL)
		{
			cont++;
			arco=arco->proximo();
		}	
	}
	return(cont);	
}
template <class T>
list<Arco<T> > Grafo<T>::arcos()
{
	NodoV<T> *vertice;
	NodoA<T> *arco;
	Arco<T> punto;
	list<Arco<T> > l;
	
	vertice=g;
	while(vertice!=NULL)
	{
		arco=vertice->ObtAdy();
		while(arco!=NULL)
		{
			punto.setV(vertice->obt_elemento());
			punto.setW(arco->Obt_info()->obt_elemento());
			l.push_back(punto);
			arco=arco->proximo();
		}
		vertice=vertice->proximo();		
	}
	return(l);
}
template <class T>
bool Grafo<T>::Es_VerticeFuente(T v)
{
	NodoV<T> *vertice;
	NodoA<T> *arco;
	bool band=true;
		
	vertice=g;
	while(vertice!=NULL&&band)
	{
		arco=vertice->ObtAdy();
		while(arco!=NULL&&band){
			if(arco->Obt_info()->obt_elemento()==v)
			{
				band=false;	
			}	
			arco=arco->proximo();
		}
		vertice=vertice->proximo();	
	}	
	return(band);	
}
template <class T>
bool Grafo<T>::Es_VerticeSumidero(T v)
{
	NodoV<T> *origen;
	bool band=true;
	if(g!=NULL){
		origen=g;
		while(origen!=NULL&&origen->obt_elemento()!=v)
		{
				origen=origen->proximo();
		}
		if(origen!=NULL){
			if(origen->obt_elemento()==v){
				if(origen->ObtAdy()!=NULL)
					band=false;
			}		
		}else{
			band=false;	
		}
	}	
		return(band);	
}
template <class T>
list<T> Grafo<T>::ObtenerVerticesFuentes()
{
	NodoV<T> *origen;
	list<T> l;
	
	if(g!=NULL)
	{
		origen=g;
		while(origen!=NULL)
		{
			if(Es_VerticeFuente(origen->obt_elemento()))
			{
				l.push_back(origen->obt_elemento());	
			}
			origen=origen->proximo();		
		}	
	}
	return(l);	
}
template <class T>
list<T> Grafo<T>::Vecindad_De_Un_Vertice(T v)
{
	NodoV<T> *origen;
	list<T> l;
	
	origen=g;
	while(origen!=NULL)
	{
		if(existeArco(v,origen->obt_elemento())||existeArco(origen->obt_elemento(),v))
		{
			l.push_back(origen->obt_elemento());	
		}	
		origen=origen->proximo();
	}
	return(l);	
}
template <class T>
list<T> Grafo<T>::vertices()
{
	NodoV<T> *origen;
	list<T> l;
	if(g!=NULL)
	{
		origen=g;
		while(origen!=NULL)
		{
			l.push_back(origen->obt_elemento());
			origen=origen->proximo();	
		}	
	}
	return(l);	
}
template <class T>
bool Grafo<T>::alcanzable(T v, T w)
{
	bool band=false;
	T original=v;
	alcanzable(v,w,band,original);
	return(band);
}
template <class T>
void Grafo<T>::alcanzable(T v, T w, bool &band, T original)
{
	T u;
	list<T> S;
	S=sucesores(v);
	if(original==w){
		band=true;
	}else{
		while(!S.empty()&&!band)
		{
			u=S.front();
			if(u==original){
				S.pop_front();
				u=S.front();
			}else{
				S.pop_front();
			}
			if(u==w)
			{
				band=true;	
			}else{
					alcanzable(u,w,band,original);	
			}	
		}
	}
}
/*****************************/
/**Sobrecarga de Operadores:**/
/*****************************/
template <class T>
void Grafo<T>::operator=(Grafo<T> &G)
{	
	if(this!=&G){
		this->Copiar(G);
	}	
}
/*****************************/
/****Métodos de Busqueda:*****/
/*****************************/
template <class T>
void Grafo<T>::BFS(T s, int dist[N], T pred[N])
{
	Etiquetar();
	NodoV<T> *origen=g;
	while(origen->obt_elemento()!=s&&origen!=NULL)
	{
			origen=origen->proximo();
	}	
	if(origen!=NULL)
	{
		BFS(origen,dist,pred);	
	}else{
		cerr << "El Vertice NO EXISTE" <<endl;	
	}
}
template <class T>
void Grafo<T>::BFS(NodoV<T>* s, int dist[N], T pred[N])
{
	int i;
	std::string color[N];
	queue<NodoV<T>*> C; 
	NodoV<T> *u,*v;
	list<NodoV<T>*> l;
	for (i=1; i<=n; i++)
	{
		color[i]="blanco";
		dist[i]=INT_MAX;
		pred[i]=0;	
	}
	color[s->Etiqueta()]="gris";
	dist[s->Etiqueta()]=0;
	C.push(s);
	while(!C.empty())
	{
		u=C.front();
		C.pop();
		l=sucesoresNODOS(u->obt_elemento());
		while(!l.empty())
		{
			v=l.front();
			l.pop_front();
			if(color[v->Etiqueta()]=="blanco")
			{
				color[v->Etiqueta()]="gris";
				dist[v->Etiqueta()]=dist[u->Etiqueta()]+1;
				pred[v->Etiqueta()]=u->obt_elemento();
				C.push(v);	
			}	
		}
		color[u->Etiqueta()]="negro";
	}
}
template <class T>
void Grafo<T>::DFS(T pred[N],int tdesc[N], int tfinal[N])
{
	Etiquetar();
	NodoV<T>* u=g;
	int tiempo,i;
	string color[N];
	
	for(i=1;i<=n;i++)
	{
		color[i]="blanco";
		pred[i]=0;	
	}
	tiempo=0;
	while(u!=NULL)
	{
		if(color[u->Etiqueta()]=="blanco")
		{
			DFS_Visitar(u,pred,tdesc,tfinal,color,tiempo);	
		}
		u=u->proximo();	
	}
}
template <class T>
void Grafo<T>::DFS_Visitar(NodoV<T>* u,T pred[N],int tdesc[N],int tfinal[N],string color[N],int &tiempo)
{
		list<NodoV<T>* > l;
		NodoV<T> *v;
		
		color[u->Etiqueta()]="gris";
		tiempo++;
		tdesc[u->Etiqueta()]=tiempo;
		l=sucesoresNODOS(u->obt_elemento());
		while(!l.empty())
		{
			v=l.front();
			l.pop_front();
			if(color[v->Etiqueta()]=="blanco")
			{
				pred[v->Etiqueta()]=u->obt_elemento();
				DFS_Visitar(v,pred,tdesc,tfinal,color,tiempo);	
			}	
		}
		color[u->Etiqueta()]="negro";
		tiempo++;
		tfinal[u->Etiqueta()]=tiempo;
}
template<class T>
void Grafo<T>::DFS_Visitar(NodoV<T>* u,string color[N],list<T> &ord)
{
	list<NodoV<T>* > l;
	NodoV<T>* v;
		
	color[u->Etiqueta()]="gris";
	l=sucesoresNODOS(u->obt_elemento());
	while(!l.empty())
	{
		v=l.front();
		l.pop_front();
		if(color[v->Etiqueta()]=="blanco")
		{
			DFS_Visitar(v,color,ord);	
		}	
	}
	color[u->Etiqueta()]="negro";
	ord.push_front(u->obt_elemento());	
}
/*******************************/
/***Métodos de Ordenamiento:***/
/*****************************/
template <class T>
list<T> Grafo<T>::OrdenamientoTopologico()
{
	NodoV<T>* u;
	string color[N];
	list<T> ord;
	list<NodoV<T>* > fuentes;
	int ui;
	Etiquetar();
	fuentes=ObtenerNodosFuentes();
	for(ui=1;ui<=n;ui++)
	{
		color[ui]="blanco";	
	}
	while(!fuentes.empty())
	{
		u=fuentes.front();
		fuentes.pop_front();
		if(color[u->Etiqueta()]=="blanco")
		{
			DFS_Visitar(u,color,ord);	
		}
	}
	return(ord);
}
template<class T>
void Grafo<T>::Etiquetar()
{
	NodoV<T> *origen=g;
	int i=1;
	while(origen!=NULL)
	{
		origen->modificarEtiqueta(i);
		i++;
		origen=origen->proximo();	
	}
}
template <class T>
list<NodoV<T>* > Grafo<T>::sucesoresNODOS(T v)
{
	NodoV<T> *vertice;
	NodoA<T> *arco;	
	list<NodoV<T>* > l;
	
	vertice=g;
	while(vertice!=NULL && vertice->obt_elemento() != v)
	{
		vertice=vertice->proximo();	
	}
	if(vertice!=NULL)
	{
		arco=vertice->ObtAdy();
		while(arco!=NULL)
		{
			l.push_back(arco->Obt_info());
			arco=arco->proximo();
		}	
	}
	return(l);
}	
template <class T>
list<NodoV<T>* > Grafo<T>::ObtenerNodosFuentes()
{
	NodoV<T> *origen;
	list<NodoV<T>* > l;
	
	if(g!=NULL)
	{
		origen=g;
		while(origen!=NULL)
		{
			if(Es_VerticeFuente(origen->obt_elemento()))
			{
				l.push_back(origen);	
			}
			origen=origen->proximo();		
		}	
	}
	return(l);	
}
template<class T>
void Grafo<T>::Dijkstra(T origen)
{
	list<T> Camino;
	int Costo;
	NodoV<T> *p=g;	
	while(p!=NULL)
	{
		if(p->obt_elemento()!=origen&&alcanzable(origen,p->obt_elemento())){
			Dijkstra(origen,p->obt_elemento(),Camino,Costo);
		}		
		p=p->proximo();
	}
}
template<class T>
void Grafo<T>::Dijkstra(T origen, T Destino, list<T> &Camino, int &Costo)
{
	list<T> aux;
	int suma=0,i;
	NodoV<T> *O=g;
	
	Costo=INT_MAX;
	aux.push_back(origen);
	Dijkstra(suma,Costo,aux,Camino,origen,Destino);	
	cout << "Camino: ";
	while(!Camino.empty())
	{
		cout << Camino.front() << " ";
		Camino.pop_front();	
	}
	cout << endl << "Costo: " << Costo << endl; 
}
template <class T>
void Grafo<T>::Dijkstra(int &suma, int &cont,list<T> &Camino,list<T> &CaminoOptimo, T origen, T Destino)
{
	T u;
	list<T> S;
	S=sucesores(origen);
	while(!S.empty())
	{
		u=S.front();
		S.pop_front();
		if(alcanzable(u,Destino))
		{
			suma=suma+CostoArco(origen,u);
			Camino.push_back(u);
			if(u!=Destino)
			{
				Dijkstra(suma,cont,Camino,CaminoOptimo,u,Destino);	
			}else
			{
				if(suma<cont)
				{
					cont=suma;
					CaminoOptimo=Camino;	
				}	
				suma=suma-CostoArco(origen,u);
				Camino.pop_back();
			}	
		}	
	}
}
template<class T>
void Grafo<T>::MarcarVertice(T v)
{
	NodoV<T> *buscar=g;
		
	while(buscar->obt_elemento()!=v&&buscar!=NULL)
	{
		buscar=buscar->proximo();	
	}
	if(buscar!=NULL)
	{
		buscar->modificarEtiqueta(-1);	
	}else{
		cout << "El vertice no EXISTE en el grafo" << endl;	
	}	
}
template <class T>
bool Grafo<T>::Marcado(T v)
{
	NodoV<T> *origen=g;
	while(origen->obt_elemento()!=v&&origen!=NULL)
	{
		origen=origen->proximo();	
	}	
	if(origen!=NULL)
	{
		if(origen->Etiqueta()==-1)
		{
			return(true);	
		}else{
			return(false);	
		}	
	}
}

template<class T>
ostream& operator << (ostream& salida,Grafo<T>& objeto_grafo)
{
	
	NodoV<T> *ver;
	NodoA<T> *nodo;
	
	ver = objeto_grafo.g;

	if(ver == NULL)
	{
			salida<<"\n matriz no generada \n";
	}
	else
	{
			salida<<"\n\n\t MATRIZ \n\n";
		
		while(ver!=NULL)
		{
				
				salida<<"   "<<ver->obt_elemento()<< ":";
				
			nodo=ver->ObtAdy();
			while(nodo!=NULL)
			{
				if(nodo->Obt_info()!=NULL)
				{
					salida<<"   "<<nodo->Obt_info()->obt_elemento();
				}		
				nodo=nodo->proximo();
			}
			salida<<"\n";
			ver=ver->proximo();
		}
	}
	
	return(salida);
	
}
template <class T>
bool Grafo<T>::Igual(NodoV<T> *aux,NodoV<T> *aux2,bool &band)
{
	NodoA<T> *aux3,*aux4;
	NodoV<T> *ver,*ver2;
	bool band2=true;
	
	aux3=aux->ObtAdy();
	aux4=aux2->ObtAdy();

	while(band2)
	{
		if((aux3==NULL)&&(aux4!=NULL))
		{
			band2=false;
			band=false;
		}		
		else
		{
			if((aux3!=NULL)&&(aux4==NULL))
			{
				band2=false;
				band=false;		
			}
			else
			{
				if((aux3==NULL)&&(aux4==NULL))
					band2=false;
				else
				{
					ver=aux3->Obt_info();
					ver2= aux4->Obt_info();
						if(ver->obt_elemento()!=ver2->obt_elemento())
						{
							band=false;
							band2=false;						
						}
				}			
			}
		}
		
		if(band2)
		{
			aux3=aux3->proximo();
			aux4=aux4->proximo();
		}
	}

	return band;
}
template <class T>
bool Grafo<T>::operator ==(const Grafo<T> &G1)
{
	NodoV<T> *aux,*aux2;
	bool band=true,band2=true;
	aux=G1.g;
	aux2=g;

	while(band2)
	{
		if((aux!=NULL)&&(aux2==NULL))
		{
			band2=false;
			band=false;
		}
		else
		{
			if((aux==NULL)&&(aux2!=NULL))
			{
				band2=false;
				band=false;
			}
			else
			{
				if((aux==NULL)&&(aux2==NULL))
					band2=false;
				else
				{
					if(aux->obt_elemento()==aux2->obt_elemento())
						band2=Igual(aux,aux2,band);					
					else
					{
						band=false;
						band2=false;					
					}
				}
			}
		}

		if(band2)
		{
			aux=aux->proximo();
			aux2=aux2->proximo();
		}
	}
	return band;
}

template <class T>
bool Grafo<T>::operator !=(const Grafo<T> &G1)
{
	return !(*this==G1);
}

#endif
