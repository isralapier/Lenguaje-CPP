#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <string>
#include <sstream>
#include <climits>
#include <vector>
#include <algorithm>


using namespace std;

//clase cache
//-----------------------------------------------------------------------------
template <class T>
class CacheManager{

//privado
int capacity; // capacidad cache
map <string,pair <T,int > > cacheData; // clave, <Objeto,indice de uso>
int mru = 0; //mru cache, contador global
int maxSize;
string fileName; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
bool write_file(string,T ); //check file en la cache
bool objectFile(string key,string fileName); 
T  getObjectFile(string key,string fileName);
void remove();


//metodos publicos
public:

CacheManager(int capacity,string fileName); // recibe la capacidad en el int
~ CacheManager ();

void insert(string key, T objeto);
T get(string key);
void show_cache(); //muestra cache
};

//metodos cache
//-----------------------------------------------------------------------------------------------

//constructor
template <class T>
CacheManager<T> :: CacheManager(int capacity ,string fileName){

this -> capacity = capacity;
this -> fileName = fileName;
this -> maxSize = capacity; //  inicializar maxSize!!!!!!!!!!!!!!!!!!

}


//destructor
template<class T>
CacheManager<T> :: ~CacheManager() { //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

}
//-------------------------------------------------------------------------
//metodo write_file     - carga un objeto T en file
template<class T>
bool CacheManager<T> :: write_file(string key,T obj){

ofstream file(fileName, ios::app); // modo append agrega datos al final del archivo

if(!file.is_open()){

    return false; //el archivo no se abrio correctamente

}else{

    file << key<< ":" << obj << endl;          //meto en archivo
    file.close();
    return true; // el archivo se escribio correctamente

}

}
//-------------------------------------------------------------------------
//metodo objectFile  -  verifica si el objeto en file
template <class T>    
bool CacheManager<T>:: objectFile(string key,string fileName){

   ifstream file(fileName);

   if(!file.is_open()) throw runtime_error ("Error al abrir archivo");  //excepcion 

   string linea;

   while(getline(file,linea)){ //guarda una linea de file en "linea"

        if(linea.find(key + ":") == 0){ //busco la clave

            file.close();
            return true;

        }

   }

   file.close();
    return false;

}

//-----------------------------------------------------------------------------
//metodo getObjectFile   -   toma un objeto desde file y lo devuelve
template <class T>                 
T CacheManager<T>:: getObjectFile(string key, string fileName){//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    ifstream file(fileName);

    if(!file.is_open()) throw runtime_error ("Error al abrir el archivo"); //excepcion

    string linea;

    while(getline(file,linea)){           

        if(linea.find(key + ":") == 0){
            string dato = linea.substr(key.length() + 1);

            T objeto;
            stringstream datos(dato);

            datos >> objeto;

            file.close();
            return objeto;


        }


    }

file.close();
throw runtime_error ("No se encontro la clave");



}
//----------------------------------------------------------------------------
//metodo remove     -    elimina el objeto de la cache menos usado, por RLU 
template <class T>
void CacheManager<T> :: remove(){

    if(cacheData.empty()) return; //verifica si esta vacio
    string lru;
    int lruValue = INT_MAX;

    for (auto cache = cacheData.begin(); cache != cacheData.end(); ++cache ){ // it iterador de map

        if(cache -> second.second < lruValue){ //segundo elemento de map - lru value

           lruValue = cache -> second.second;  //reemplazo lru value
           lru = cache-> first;                //primer elemento de map - reemplazo key

        }



    }

        cacheData.erase(lru); //elimina el elemento de map - cacheData

}

//---------------------------------------------------------------------
//metodo insert     -    inserta un objeto en la cache - map 
template <class T>
void CacheManager<T>:: insert(string key, T objeto){

   auto it = cacheData.find(key); 

   if(it != cacheData.end()){ // Si el elemento ya está en la caché

    write_file(key,objeto); //actualiza archivo
    it->second.first =  objeto; //actualiza  objeto en map - cacheData
    it->second.second = mru++; // actualiza mru, y se lo asigna a la ultima modificacion en cache

    return;

   }


   if(cacheData.size() >= maxSize){ // verifica cache llena
    remove();                //elimina objeto de  cache

   }


    cacheData[key] = make_pair(objeto,mru++); //INserta el nuevo objeto con la clave y valor
    write_file(key,objeto);

}

//----------------------------------------------------------------------------------------
//metodo get         - toma un elemento de la cache y lo devuelve - varios casos
template <class T>
T CacheManager<T> :: get(string key){
    
//busca en cache
auto it = cacheData.find(key);
if (it != cacheData.end()) {
    it->second.second = mru++;
    return it->second.first;
}

// Si no está en la caché, buscar en el archivo
if (objectFile(key, fileName)) {
    if (cacheData.size() >= maxSize) {
        remove();
    }
    T objeto = getObjectFile(key, fileName);
    cacheData[key] = make_pair(objeto, mru++);
    return objeto;
}


// no esta ni en cache ni en file
throw runtime_error("No se encontro elemento");

}


//--------------------------------------------------------------------------
//metodo show cache    -  muestra los objetos de la cache 


template <class T>
void CacheManager<T>::show_cache() {
    cout << "---------CONTENIDO CACHE ---------" << endl;
    cout << "Tamano cache: " << cacheData.size() << "/" << maxSize << endl;

    if (cacheData.empty()) {
        cout << "La cache esta vacia" << endl;
    } else {
        cout << "Elementos (ordenados por clave):" << endl;

        for (const auto& elemento : cacheData) {
            cout << "  Key: \"" << elemento.first
                 << "\" | Valor: " << elemento.second.first
                 << " | MRU: " << elemento.second.second << endl;
        }
    }
    cout << "---------------" << endl;
}
