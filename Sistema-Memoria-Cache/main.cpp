#include <iostream>
#include <string>
#include "cache.h"

using namespace std;

class Canciones {
    private:
        int id; // Identificador único de la canción
        string nombre;
        string artista;
    
    public:
        // Constructor por defecto
        Canciones() : id(0), nombre(""), artista("") {}
        
        // Constructor con parámetros
        Canciones(int _id, string _nombre, string _artista) 
            : id(_id), nombre(_nombre), artista(_artista) {}
        
        // Getters
        int getId() const { return id; }
        string getNombre() const { return nombre; }
        string getArtista() const { return artista; }
        

        void print() const {
            cout << "Cancion{ID: " << id << ", Nombre: " << nombre 
                 << ", Artista: " << artista << "}";
        }
        
        // Operador << para escribir a archivo y mostrar
        friend ostream& operator<<(ostream& os, const Canciones& c) {
            os << c.id << "," << c.nombre << "," << c.artista;
            return os;
        }
        
        // Operador >> para leer desde archivo
        friend istream& operator>>(istream& is, Canciones& c) {
            char comma;
            is >> c.id >> comma;
            getline(is, c.nombre, ',');
            getline(is, c.artista);
            return is;
        }
};


int main() {
    cout << "=== PRUEBAS DEL CACHE MANAGER ===" << endl;
    
    // Crear cache con capacidad de 3 elementos
    // El file que se crea como disco es el segundo parametro (se crea automaticamante el txt)
    CacheManager<Canciones> cache_canciones(3, "canciones_cache.txt");
    
    cout << "\n1. Cache inicial (vacio):" << endl;
    cache_canciones.show_cache();
    
    // Insertar canciones
    cout << "\n2. Insertando canciones..." << endl;
    cache_canciones.insert("cancion1", Canciones(1, "Bohemian_Rhapsody", "Queen"));
    cache_canciones.insert("cancion2", Canciones(2, "Hotel_California", "Eagles"));
    cache_canciones.insert("cancion3", Canciones(3, "Imagine", "John_Lennon"));
    
    cache_canciones.show_cache();
    
    // Insertar cuarta canción (debería eliminar LRU)
    cout << "\n3. Insertando cuarta cancion (cache lleno - elimina LRU):" << endl;
    cache_canciones.insert("cancion4", Canciones(4, "Stairway_to_Heaven", "Led_Zeppelin"));
    
    cache_canciones.show_cache();
    
    // Acceder a una canción existente (actualiza MRU)
    // Lo hice en bloques try/catch pq throwee los errores en el cache manager como runtime exceptions
    cout << "\n4. Accediendo a 'cancion2' (actualiza MRU):" << endl;
    try {
        Canciones cancion = cache_canciones.get("cancion2");
        cout << "Cancion encontrada: ";
        cancion.print();
        cout << endl;
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    
    cache_canciones.show_cache();
    
    // Actualizar una canción existente
    cout << "\n5. Actualizando 'cancion3' con nueva informacion:" << endl;
    cache_canciones.insert("cancion3", Canciones(3, "Imagine_Remastered", "John_Lennon"));
    
    cache_canciones.show_cache();
    
    // Intentar acceder a una canción que no existe
    cout << "\n6. Intentando acceder a cancion inexistente:" << endl;
    
    try {
        Canciones cancion = cache_canciones.get("cancion_inexistente");
        cancion.print();
    } catch (const exception& e) {
        cout << "Error esperado: " << e.what() << endl;
    }
    
    // Llenar el cache y probar más eliminaciones LRU
    cout << "\n7. Insertando mas canciones para probar LRU:" << endl;
    cache_canciones.insert("cancion5", Canciones(5, "Sweet_Child_O_Mine", "Guns_N_Roses"));
    cache_canciones.insert("cancion6", Canciones(6, "Purple_Haze", "Jimi_Hendrix"));
    
    cache_canciones.show_cache();
    
    cout << "\n=== PRUEBAS COMPLETADAS ===" << endl;
    
    return 0;
}