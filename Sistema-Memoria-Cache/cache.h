#include <iostream>
#include <string>
#include <map>
#include <fstream>    
#include <sstream> 
#include <climits>
#include <vector>
#include <algorithm>
#include <vector>
#include <algorithm>

using namespace std;

template <class T>
class CacheManager {

    private:
        int mru = 0;
        int maxSize; // Tamanio maximo del cache
        map <string, pair<T, int>> cacheData; // Mapa para almacenar los elementos en cache
        bool write_file(string, T); // Indica si se escribio el archivo en memoria (y lo hace?) --> Lo hago al final
        void removeLRU();
        string fileName; // TODO: Ver que hago con esto, si lo llamo parametro o si lo hardcodeo
        bool isObjectInFile(string key, string fileName); 
        T getObjectInFile(string key, string fileName);   
    public:

    CacheManager(int, string);
    ~CacheManager();  

    void insert (string key, T obj);
    T get (string key);
    void show_cache(); // Mostrar contenido del cache
};

template <class T>
CacheManager<T>::CacheManager(int maxSize, string fileName) {
    this->maxSize = maxSize;
    this->fileName = fileName;
}

template <class T>
CacheManager<T>::~CacheManager() {
    // Destructor para liberar recursos si es necesario
}

template <class T>
bool CacheManager<T>::write_file(string key, T obj) {
    ofstream file(fileName, ios::app); // Abrir en modo append
    if (!file.is_open()) {
        return false; // No se pudo abrir el archivo
    }
    
    file << key << ":" << obj << endl;
    file.close();
    return true; // Se escribio correctamente
}

template <class T>
bool CacheManager<T>::isObjectInFile(string key, string fileName){
    ifstream file(fileName);

    if(!file.is_open()) throw runtime_error ("No se pudo abrir el archivo");

    string linea;
    while(getline(file,linea)){
        // Busco la linea que arranque con la misma clave
        if(linea.find(key + ":") == 0){ // Porque lo primero que hay en la linea es la clave
            file.close();
            return true;
        } 
    }
    file.close();
    return false;
}

template <class T>
T CacheManager<T>::getObjectInFile(string key, string fileName){
    ifstream file(fileName);

    if(!file.is_open()) throw runtime_error ("No se pudo abrir el archivo");

    string linea;

    while(getline(file,linea)){
         if (linea.find(key + ":") == 0) {
            string valueStr = linea.substr(key.length() + 1);

            T obj;
            stringstream ss(valueStr);
            ss >> obj; 
            
            file.close();
            return obj;
         }
    }
    file.close();
    throw runtime_error("Key '" + key + "' no encontrada en el disco");
}

template <class T>
void CacheManager<T>::removeLRU(){
    if (cacheData.empty()) return; // Proteccion si el cache esta vacio.
    string lruKey; // Variable para almacenar la clave del LRU
    int lruValue = INT_MAX; // Inicializar con el valor máximo posible
        
        // Buscar el elemento con el menor valor MRU (mas antiguo)
        for (auto it = cacheData.begin(); it != cacheData.end(); ++it) {
            if (it->second.second < lruValue) {
                lruValue = it->second.second; // Actualiza el valor LRU mas pequenio
                lruKey = it->first; // Guarda la clave correspondiente
            }
        }
        
        // Elimina el elemento menos recientemente usado (LRU)
        cacheData.erase(lruKey); // Elimina el elemento LRU del cache usando su clave
}

template <class T>
void CacheManager<T>::insert(string key, T obj) {
 // Si existe un elemento con la misma clave, se actualiza su valor (objeto)
  for(auto it =  cacheData.begin();it != cacheData.end(); it++){
    if(it->first == key) {
    write_file(key, obj); // Actualiza el archivo
    it->second.first = obj; // Actualiza el objeto en cache
    it->second.second = mru++;
    return; // Sale de la funcion
    }
}
    // Si no existe, se inserta un nuevo elemento en cache y en el archivo
    if(cacheData.size() >= maxSize) {
        this->removeLRU();
    }
    // Inserta el nuevo elemento en cache y en el archivo
    cacheData[key] = make_pair(obj, mru++); // Inserta el nuevo elemento con su clave y valor
    write_file(key, obj);
}   


template <class T>
T CacheManager<T>::get(string key) {
 
// Si el elemento existe en cache, se retorna el valor y actualiza el MRU
 
for (auto it = cacheData.begin(); it != cacheData.end(); it++){
    if (it->first == key){
        it->second.second = mru++;
        return it->second.first;
    }
}

 // Si existe en disco pero no en cache, lo traigo a disco y lo muestro


if(isObjectInFile(key,fileName)){
    if(cacheData.size() >= maxSize){
        this->removeLRU();
    }
    T obj = getObjectInFile(key,fileName);
    cacheData[key] = make_pair(obj, mru++);
    return obj;
}

 // Si no pasa ninguna de las dos tiro error
throw runtime_error("Clave '" + key + "' no encontrada en cache ni en disco");
}

template <class T>
void CacheManager<T>::show_cache() {
    cout << "=== CONTENIDO CACHE ===" << endl;
    cout << "Tamanio cache: " << cacheData.size() << "/" << maxSize << endl;
    
    if (cacheData.empty()) {
        cout << "La cache esta vacia" << endl;
    } else {
        cout << "Elementos (filtrados por MRU):" << endl;
        
        // Crear vector de pares para ordenar por MRU
        vector<pair<string, pair<T, int>>> sortedElements;
        for (auto it = cacheData.begin(); it != cacheData.end(); ++it) {
            sortedElements.push_back(*it);
        }
        
        // Ordenar por MRU descendente (más reciente primero)
        // El MRU mas grande es el mas recientemente usado
        sort(sortedElements.begin(), sortedElements.end(), 
             [](const pair<string, pair<T, int>>& a, const pair<string, pair<T, int>>& b) {
                 return a.second.second > b.second.second;
             });
        
        // Mostrar elementos ordenados
        for (const auto& element : sortedElements) {
            cout << "  Key: \"" << element.first 
                 << "\" | Valor: " << element.second.first 
                 << " | MRU: " << element.second.second << endl;
        }
    }
    cout << "======================" << endl;
}