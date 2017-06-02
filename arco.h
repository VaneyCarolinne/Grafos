#ifndef Arco_h
#define Arco_h
#include <iostream>
using namespace std;
template <class T>
class Arco
{
	private:
		T v,w; //Atributos 
		float c;
	public:						
		Arco():v(),w(),c(0){}
		Arco(T a, T b,float s):v(a),w(b),c(s){} 
		Arco(const Arco &p):v(p.v),w(p.w),c(p.c){}//Constructor Copia
		T getV()const{return (v);}
		T getW()const{return (w);} 
		float getCosto()const{return (c);}
		void setV(T vi){v=vi;}
		void setW(T wi){w=wi;}
		void setCosto(float s){c=s;}
		template <class A>
		friend std::ostream& operator<<(std::ostream& salida, const Arco<A> &p);
};
template <class T>
std::ostream& operator<<(std::ostream& salida, const Arco<T> &p)
{
	salida << " < " << p.v << " , " << p.w <<" , "<< p.c<<" > " << endl ;
	return(salida);
}
#endif
