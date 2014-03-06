#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <stdlib.h>

//Enrique Marroquín Artezán
//Estructuras de Datos
//Noviembre 26, 2013
 
using namespace std;
 
template <class T>
class NodoArbol
{ public:
    T info;
    int reps;
    NodoArbol<T> *izq, *der;
    NodoArbol() { izq = der = NULL; reps = 1;}
    NodoArbol(T dato) { info = dato; izq = der = NULL; reps =1;}
};
 
template <class T>
class ABB
{
private:
    NodoArbol<T> *raiz;
public:
    ABB() { raiz = NULL;}
    void inserta (T dato);
    int cuenta () { return contar(raiz); }
    void getMayores(ofstream& archSalida);
    NodoArbol<T>* getRaiz() {return raiz; }
    ~ABB() { libera(raiz); }
};
 
template <class T>
class ComparaFrecuencia
{ //compara frecuencia de dos nodos, regresa 1 si el primer nodo es mayor y 0 si es el segundo
public:
    bool operator()(NodoArbol<T> *nodo1, NodoArbol<T> *nodo2)
    {
        return nodo1->reps > nodo2->reps;
    }
};
 
template <class T>
void ABB<T>::getMayores(ofstream& archSalida){
    int cantPalabras = cuenta();  // que numero de palabras se quieren sacar. Ej. las 3 mas repetidas, las 5, etc;
    vector<string> vectorMayores(cantPalabras);
    priority_queue<NodoArbol<T> *, vector<NodoArbol<T> *>, ComparaFrecuencia<T> > pq; // uso una PQ de NodoArbol<T> * donde se guarde en un vector
    checaMayores(raiz, cantPalabras, pq);                                              //solo es necesario para poner la 3era opcion
                                                                                    // ComparaFrecuencia es comparador de NodoArbol<T> *

   
    int numPalabras = pq.size();
    for(int i=0; !pq.empty(); i++)
    {
        vectorMayores[numPalabras - 1 - i] = pq.top()->info;
        pq.pop();
    }
    archSalida.open("wordcloooud.html");
    archSalida << "<!DOCTYPE html> <html> <body>";
    archSalida << "<table>";

    int randNum=rand() % cantPalabras, cont =0; // Aqui se crea el documento html, creando una tabla
    for(int i=0; i<9; i++){
        archSalida << "<tr>";
        for(int j=0; j<6; j++){
            if(randNum<=3){
                archSalida << "<td><font face = \"symbol\" color=green size=20>" << vectorMayores[randNum] << "</font></td>";
            }
            else if(randNum>=4 && randNum <=6){
                archSalida << "<td><font face = \"braggadocio\" color=blue size=12>" << vectorMayores[randNum] << "</font></td>";
            }
            else if(randNum>=7 && randNum<=11){
                archSalida << "<td><font face = \"symbol\" color=red size=10>" << vectorMayores[randNum] << "</font></td>";
            }
            else if(randNum>=12 && randNum<=50){
                archSalida << "<td><font color=grey size=8>" << vectorMayores[randNum] << "</font></td>";
            }
            else if(randNum>=51 && randNum<=120){
                archSalida << "<td><font face = \"desdemona\" color=brown size=6>" << vectorMayores[randNum] << "</font></td>";
            }
            else if(randNum>=121 && randNum<=270){
                archSalida << "<td><font face = \"arial\" color=orange size=5>" << vectorMayores[randNum] << "</font></td>";
            }
            else{
                archSalida << "<td><font face = \"desdemona\" color=purple size=4>" << vectorMayores[randNum] << "</font></td>";
            }
            randNum= rand() % cantPalabras;
            cont++;
            if(cont>3){
                randNum= rand() % 6;
                cont=0;
            }
        }
        archSalida << "</tr>";
    }
    archSalida << "</table></body></html>";
    archSalida.close();
}
 
template <class T>
void checaMayores(NodoArbol<T> *raiz, int limite, priority_queue<NodoArbol<T> *, vector<NodoArbol<T> *>, ComparaFrecuencia<T> > &pq)
{
    if (raiz != NULL) //se va metiendo a la PQ si el nodo va siendo menor del limite puesto 
    {                   // el limite (o tope) es para que saque una palabra repetida, y luego saque la siguiente mas repetida
        if(pq.size() < limite) 
            pq.push(raiz);
        else if(pq.top()->reps < raiz->reps)
        {
            pq.pop();
            pq.push(raiz);
        }
        checaMayores(raiz->izq, limite, pq);
        checaMayores(raiz->der, limite, pq);
    }
}
 
template <class T>
void libera (NodoArbol<T>* raiz) //libera los nodos del arbol al terminar el programa
{
    if (raiz != NULL)
    {
        libera(raiz->izq);
        libera(raiz->der);
        delete(raiz);
    }
}
 
template <class T>
int contar (NodoArbol<T>* raiz) //util para saber cuantas palabras se han insertado en el arbol
{
    if (raiz == NULL)
        return 0;
    return(1+contar(raiz->izq)+contar(raiz->der));
}
 
template <class T>
void ABB<T>::inserta (T valor) // compara strings y las sortea alfabeticamente
{      
    NodoArbol<T> *NuevoNodo = new NodoArbol<T>(valor);
    NodoArbol<T> *actual = raiz, *anterior = NULL;
    int i =0; //contador de caracter para ver donde para la comparacion entre caracteres
    while(actual != NULL && valor!=actual->info)
    {               // se va a mover a traves del arbol hasta encontrarse a si mismo o encontrar su lugar alfabeticamente
        anterior = actual;
        while(valor[i]==actual->info[i]) //si las primeras letras de los dos strings comparados son iguales, se mueve hasta encontrar una diferente
            i++;
        actual = (valor[i] <= actual->info[i] ?  actual->izq : actual->der);
        //si el nuevo string es primero alfabeticamente, se va a la izq, si no, a la der

    }
    if(anterior==NULL){
        if(raiz!=NULL && valor == actual->info){
            actual->reps=actual->reps+1;}
        else
            raiz = NuevoNodo;
    }
    else{  
        if(actual!=NULL){ // si se salio del loop de moverse a traves del arbol por ser igual al actual, actual seguira no sera null
            if(actual->info == valor)
                actual->reps++;
        }
        else{ //el string que va antes alfabeticamente va a la izquierda
            (valor[i] <= anterior->info[i] ? anterior->izq = NuevoNodo : anterior->der = NuevoNodo);
        }
    }
}
 
int main()
{
    ABB<string> arbol;
    ifstream arch;
    ofstream archSalida;
    string nomarch;
    string palabra;

    cout << "Ingrese el nombre del archivo a cargar: ";
    cin >> nomarch;
    cout << endl;

    arch.open(nomarch.c_str());
    while (!arch.eof())
    {
        arch >> palabra;
        arbol.inserta(palabra);
    }
    arbol.getMayores(archSalida);
    arch.close();

    return 0;
}