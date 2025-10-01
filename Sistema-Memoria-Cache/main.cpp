#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "cache.h" 

using namespace std;

class Student {
    int id;
    int value;
    string data; 

public:
    static const string class_name;

    Student() = default; //Constructor por defecto, para map

    Student(int _key, int _value, const string& _data) : id(_key), value(_value), data(_data){}

    void print() {
        cout << "Student Object: " << id << ", " << value << ", " << data << endl;
    }
    friend ostream& operator<<(ostream& os, const Student& student) {//sobrecarga de << para escribir en archivo
        os << student.id << "," << student.value << "," << student.data;
        return os;
    }


    friend istream& operator>>(istream& is, Student& student) {//sobrecarga de >> para leer del archivo
        char comma;
        string line;
        if (getline(is, line)) {
            stringstream ss(line);
            if (ss >> student.id >> comma >> student.value >> comma) {
                ss >> student.data;
            }
        }
        return is;
    }
};

const string Student::class_name = "StudentClass";

int main() {

    CacheManager<Student> my_cache(3, "disco.txt");
    
    cout << "--- Prueba completa de CacheManager ---" << endl;

    // Prueba insercion  
    cout << "\nLlenando la cache (capacidad 3)..." << endl;
    my_cache.insert("0", Student(0, 22, "student1"));
    my_cache.insert("1", Student(1, 23, "student2"));
    my_cache.insert("2", Student(2, 25, "student3"));
    my_cache.show_cache();

    //Prueba el metodo get y actualiza el MRU
    cout << "\nAccediendo a '0' para que sea el mas reciente..." << endl;
    try {
        Student retrieved = my_cache.get("0");
        cout << "Elemento '0' recuperado: ";
        retrieved.print();
    } catch (const runtime_error& e) {
        cout << e.what() << endl;
    }
    my_cache.show_cache();

    // Prueba insercion cuando la cache esta llena 
    cout << "\nInsertando '3' (elimina '1' por ser el menos reciente)..." << endl;
    my_cache.insert("3", Student(3, 29, "student4"));
    my_cache.show_cache();

    // Prueba get de un elemento que esta en el archivo pero no en la cache
    cout << "\nAccediendo a '1' (deberia ser recuperado del archivo)..." << endl;
    try {
        Student retrievedFromFile = my_cache.get("1");
        cout << "Elemento '1' recuperado del archivo: ";
        retrievedFromFile.print();
    } catch (const runtime_error& e) {
        cout << e.what() << endl;
    }
    my_cache.show_cache();

    // Pruba get de un elemento que no existe 
    cout << "\nIntentando acceder a '99' (no existe)..." << endl;
    try {
        my_cache.get("99");
    } catch (const runtime_error& e) {
        cout << "Error esperado: " << e.what() << endl;
    }

    // Prueba actualizar un elemento existente en la cache 
    cout << "\nActualizando '0' con nuevos datos..." << endl;
    my_cache.insert("0", Student(0, 50, "student5"));
    my_cache.show_cache();
    
    cout << "\n--- Fin de la prueba ---" << endl;

    return 0;
}
