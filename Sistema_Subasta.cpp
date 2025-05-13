#include <iostream>
#include <string>
#include <vector>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////
class Persona{ // CLASE PERSONA

private:
    string nombre;          //atributo nombre
public:
    Persona(string nombre);  //constructor con nombre
    string getNombre();     //metodo getNombre


};

///////////////////////////////////////////////////////////////////////////////////

Persona::Persona(string nombre){ //constructor Persona

    this->nombre = nombre;

}

///////////////////////////////////////////////////////////////////////////////////
string Persona::getNombre(){    //devuelve nombre

    return nombre; //puedo usar this->nombre

}

///////////////////////////////////////////////////////////////////////////////////
class Oferta{   //CLASE OFERTA

private:
    float monto;            //atributo
    Persona* persona;         //atributo
public:
    Oferta(float monto,Persona * persona); //constructor
    float getMonto();                   //metodo
    Persona * getPersona();               //metodo

};

///////////////////////////////////////////////////////////////////////////////////
Oferta::Oferta(float monto,Persona * persona){ //constructor monto

    this->monto = monto;
    this->persona = persona;


}
///////////////////////////////////////////////////////////////////////////////////

float Oferta::getMonto(){     //metodo getMonto

    return monto;

}

///////////////////////////////////////////////////////////////////////////////////
Persona * Oferta::getPersona(){

    return persona;

}

////////////////////////////////////////////////////////////////////////////////////
class Lote{      //CLASE LOTE
private:
    int numero;         //atributo
    string nombre;     //atributo
    Oferta * ofertaMaxima;      //atributo
public:
    Lote(int numero,string nombre);         //contructor
    void registraOferta(Oferta* nuevaOferta);   //metodo
    Oferta * getOfertaMaxima();                 //metodo
    string getNombreLote();                     //metodo
    int getNumeroLote();                        //metodo

};

///////////////////////////////////////////////////////////////////////////////////////

int Lote::getNumeroLote(){      //devuelve numero lote

    return numero;


}

/////////////////////////////////////////////////////////////////////////////////////
string Lote::getNombreLote(){   //devuelve nombre lote


        return nombre;

};


////////////////////////////////////////////////////////////////////////////////////////////
Lote::Lote(int numero,string nombre){    //constructor ItemSubasta

        this->nombre = nombre;
        this->numero = numero;
        this->ofertaMaxima = NULL;

};


///////////////////////////////////////////////////////////////////////////////////////////////////
void Lote::registraOferta(Oferta * nuevaOferta){  //metodo registrar oferta////////////////VER

         if(this->ofertaMaxima == NULL  ){


            this->ofertaMaxima = nuevaOferta;


         }else if ((this->ofertaMaxima->getMonto()) < (nuevaOferta->getMonto())){

           this->ofertaMaxima = nuevaOferta;


        };

};

///////////////////////////////////////////////////////////////////////////
Oferta * Lote::getOfertaMaxima(){

        return ofertaMaxima;


}

//////////////////////////////////////////////////////////////////////////////
class Subasta{        //CLASE SUBASTA

public:
    vector<Lote*> lotes;    //vector de subastas

     ~Subasta(){            //destructor

        for(Lote* lote: lotes){

            delete lote;

        }


     }



     void agregarLote(Lote* lote);  //metodo

     void listarLotes(); //metodo

     size_t cantidadLotes();        //metodo

};


/////////////////////////////////////////////////////////////////////////

void Subasta::agregarLote(Lote* lote){  //agrega un lote al vector de lotes


    lotes.push_back(lote);

     };



  ///////////////////////////////////////////////////////////////////////////
 void Subasta::listarLotes(){           //lista cada lote con oferta y ofertante

        cout<< "--LOTES--"<<endl;

    for(Lote* lote: lotes){             //lote con oferta

        cout << "Lote # " << lote->getNumeroLote() << "\nnombre: " << lote->getNombreLote();
        if(lote->getOfertaMaxima()){
        cout << "\noferta Max: $"<< lote->getOfertaMaxima()->getMonto()<<"\nofertante: "<< lote->getOfertaMaxima()->getPersona()->getNombre()<< endl;


        }else {


           cout<<"--Sin Ofertas--"<<endl;   //lote sin oferta

        }
    }

 }



//////////////////////////////////////////////////////////////////////////////////
size_t Subasta::cantidadLotes(){        //cantidad de lote


    return lotes.size();

};

/////////////////////////////////////////////////////////////////////////////

int main(){

//creacion personas
Persona* julieta = new Persona("Julieta");
Persona* clara = new Persona("Clara");

//creacion subasta
Subasta subasta;

//lotes
subasta.agregarLote(new Lote(1,"Auto"));
subasta.agregarLote(new Lote (2,"Notebook"));

//ofertas
Oferta * oferta1 = new Oferta(5000,clara);
Oferta * oferta2 = new Oferta(800,julieta);




subasta.lotes[0]->registraOferta(oferta1);
subasta.lotes[1]->registraOferta(oferta2);

//muestra informacion
subasta.listarLotes();
cout<<"Total de lotes: "<< subasta.cantidadLotes() << endl;

//limpia memoria
delete julieta;
delete clara;



return 0;
}
