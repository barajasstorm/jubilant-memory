//
//  main.cpp
//  HelloWorld
//
//  Created by Juan Barajas on 10/31/16.
//  Copyright © 2016 Juan Barajas. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <unistd.h>


using namespace std;

// function prototypes
bool searchuser(string salesname0); // search for a user in inventory and return true or false
double search(string nombre0); // search for item in inventory and return 0.1 for not found, i < 0 for inactive, i == 0 || i > 0.1 for active
void save(); // save data to file
void modificar(); // modify individual items
void consultar(); // consult specific item
void mostrar(); // show inventory
void load(); // reading data from file
void bajas(); // delete itemes physically and logically
void altas(); // creating data
void newuser(); // create new user
void users(); // admin sales users menu
void admin(); // admin menu
void salesday(int userid); // create sales
void salesuser(int userid); // sales user menu
void ventas(); // sales menu
void viewsales(int userid); // view sales
void edituser(); // edit user
void showusers(); // show users
void cortesdia(); // closing sales for the day

// global variables
vector<int> existencias, stockmin, borrado, dia, mes, anio, salesdayperuser, cortes_dia_user;
vector<string> nombre, salesname, salespassword;
vector<double> preciocompra, precioventa;

// corte variables
vector<string> corte_user;
vector<double> corte_total, cortes_dia;
vector<unsigned long> corte_num;

// ticket variables
vector<double> ticket_corte_total;
vector<unsigned long> ticket_corte_ventanum, ticket_corte_cortenum, ticket_by_one;

// transaction variables
vector<int> transaction_cant, transaction_year, transaction_month, transaction_day;
vector<string> transaction_desc, transaction_salesname;
vector<double> transaction_price, transaction_importe, transaction_total;
vector<unsigned long> transaction_salesticketnum, transaction_corte, transaction_ticket_count;


// main menu
int main() {
    load();
    int op, password;
    cout << "\n\nBienvenidos\n\n";
    do{
        cout << "Menu Principal\n";
        cout << "1. Administrador\n";
        cout << "2. Ventas\n";
        cout << "3. Salir \n";
        cout << "Opcion: ";
        cin >> op;
        cout << endl;
        
        switch(op){
            case 1:
                cout << "Ingresar contraseña de administrador (1234): ";
                cin >> password;
                if (password==1234){
                    admin();
                }
                else {
                    cout << "\nLa contraseña es incorrecta. Por favor intente de nuevo.";
                }
                break;
            case 2:
                ventas();
                break;
            case 3:
                mostrar();
                cortesdia();
                
                cout << "Sesion terminada.\n\n";
                break;
            default:
                cout << "La opcion que elegiste no es valida.";
                break;
        }
    }while(op!=3);
    return 0;
}

// admin menu
void admin(){
    int op;
    do{
        cout << "\nMenu de Administracion: \n";
        cout << "1. Altas\n";
        cout << "2. Bajas\n";
        cout << "3. Modificaciones\n";
        cout << "4. Mostrar Inventario\n";
        cout << "5. Consultar\n";
        cout << "6. Usuarios De Ventas\n";
        cout << "7. Salir\n";
        cout << "Opcion: ";
        cin >> op;
        cout << endl;
        
        switch(op){
            case 1:
                altas();
                break;
            case 2:
                bajas();
                break;
            case 3:
                modificar();
                break;
            case 4:
                mostrar();
                break;
            case 5:
                consultar();
                break;
            case 6:
                users();
                break;
            case 7:
                break;
            default:
                cout << "La opcion seleccionada no es valida" << endl;
                cin.clear();
        }
    }while(op!=7);
}

// creating data
void altas(){
    cout << endl;
    string nombre0;
    int existencias0, stockmin0, borrado0, respuesta, respuesta2, respuesta3 = 0;
    double preciocompra0, precioventa0, found;
    ofstream outfile;
    do{
        outfile.open("inventory.txt", ios::app);
        cin.ignore();
        cout << "\nIngresar nombre de producto: ";
        getline(cin, nombre0);
        found = search(nombre0);
        if (found == 0.1){
            nombre.push_back (nombre0);
            cout << "\nIngrese el precio de compra: ";
            cin >> preciocompra0;
            preciocompra.push_back (preciocompra0);
            do{
                cout << "\nIngrese el precio de venta: ";
                cin >> precioventa0;
                if(precioventa0 <= preciocompra0){
                    cout << "El precio de venta debe ser mayor al precio de compra. Intentar de nuevo? (1 = Si, 2 = No): ";
                    cin >> respuesta3;
                }
                else {
                    precioventa.push_back (precioventa0);
                    respuesta3=2;
                }
            }while(respuesta3!=2);
            cout << "\nIngrese la cantidad en existencias: ";
            cin >> existencias0;
            existencias.push_back (existencias0);
            cout << "\nIngrese el stock minimo requerido: ";
            cin >> stockmin0;
            stockmin.push_back (stockmin0);
            cin.ignore();
            borrado0 = 0;
            borrado.push_back (borrado0);
            outfile << endl << nombre0 << endl << preciocompra0 << endl << precioventa0 << endl << existencias0 << endl << stockmin0 << endl << borrado0;
            outfile.close();
            cout << "\nProducto agregado exitosamente. Agregar otro? (1 = SI, 2 = NO): ";
        } else {
            if (found < 0){
                cout << "\nEl articulo '" << nombre0 << "' ya existe pero esta desactivado. Desea activarlo? (1 = SI, 2 = NO): ";
                cin >> respuesta2;
                if (respuesta2 == 1){
                    borrado[-found]=0;
                    cout << "\nProducto " << nombre0 << " activado exitosamente. Desea agregar otro producto? (1 = SI, 2 = NO): ";
                    save();
                } else {
                    cout << "\nDesea agregar otro producto? (1 = SI, 2 = NO): ";
                }
            }
            if (found == 0 || found > 0.1) cout << "\nEl producto ya existe. Desea agregar otro producto? (1 = SI, 2 = NO): ";
        }
        cin >> respuesta;
    }while(respuesta!=2);
}

// delete itemes physically and logically
void bajas(){
    int respuesta = 0, respuesta2, respuesta3;
    double found;
    string nombre0;
    cin.ignore();
    do{
        cout << "\nQue producto desea dar de baja? ";
        getline(cin, nombre0);
        found = search(nombre0);
        if (found == 0.1){
            cout << "\nEl producto no existe.";
        } else {
            if (found < 0){
                cout << "\nEl producto se encuentra desactivado. Desea borrarlo permanentemente? (1 = SI, 2 = NO): ";
                cin >> respuesta2;
                if (respuesta2 == 1){
                    nombre.erase (nombre.begin()+(-found--));
                    preciocompra.erase (preciocompra.begin()+(-found--));
                    precioventa.erase (precioventa.begin()+(-found--));
                    existencias.erase (existencias.begin()+(-found--));
                    stockmin.erase (stockmin.begin()+(-found--));
                    borrado.erase (borrado.begin()+(-found--));
                    cout << "\nBorrado exitosamente";
                }
            }
            if (found == 0 || found > 0.1){
                cout << "\nDesea borrar '" << nombre0 << "' permanente o solamente desactivarlo? (1 = Borrarlo, 2 = Desactivarlo, 3 = Cancelar): ";
                cin >> respuesta3;
                if (respuesta3 == 1){
                    nombre.erase (nombre.begin()+(found--));
                    preciocompra.erase (preciocompra.begin()+(found--));
                    precioventa.erase (precioventa.begin()+(found--));
                    existencias.erase (existencias.begin()+(found--));
                    stockmin.erase (stockmin.begin()+(found--));
                    borrado.erase (borrado.begin()+(found--));
                    cout << "\nBorrado exitosamente" << endl;
                } else {
                    if (respuesta3 == 2){
                        borrado[found]=1;
                        cout << "\nDesactivado exitosamente" << endl;
                    }
                }
            }
        }
        cout << "\nDesea dar de baja otro producto? (1 = SI, 2 = NO): ";
        cin >> respuesta;
        cin.ignore();
    }while(respuesta!=2);
    save();
}

// modify individual items in loop
void modificar(){
    string nombre0, newvalue;
    double found = 0, newvalue2;
    int respuesta = 0, respuesta2, respuesta3, respuesta4, newvalue3;
    do{
        cin.ignore();
        cout << "Que producto desea modificar? ";
        getline(cin, nombre0);
        found = search(nombre0);
        if (found == 0 || found > 0.1){
            cout << endl;
            cout << "Producto       " << "$Compra  " << "$Venta  " << "Existencias  " << "Stock" << endl;
            cout << "___________________________________________________" << endl;
            cout << nombre[found] << "\t\t\t" << preciocompra[found] << "\t\t" << precioventa[found] << "\t\t" << existencias[found] << "\t\t\t " << stockmin[found] << endl;
            cout << endl;
            cout << "1. Nombre" << endl;
            cout << "2. $Compra" << endl;
            cout << "3. $Venta" << endl;
            cout << "4. Existencias" << endl;
            cout << "5. Stock" << endl;
            cout << endl;
            cout << "Ingrese el numero correspondiente para modificar el campo deseado: ";
            cin >> respuesta3;
            cout << endl;
            switch(respuesta3){
                case 1:
                    cout << "Ingresar el nuevo nombre para el producto '" << nombre[found] << "': ";
                    cin >> newvalue;
                    cout << endl;
                    cout << "Confirmar el cambio de '" << nombre[found] << "' a '" << newvalue << "' (1 = Confirmar, 2 = Cancelar): ";
                    cin >> respuesta4;
                    cout << endl;
                    if (respuesta4==1){
                        nombre[found] = newvalue;
                        cout << "Cambio exitoso." << endl;
                        cout << endl;
                    }
                    else if (respuesta4==2){
                        cout << "Cancelado" << endl;
                        cout << endl;
                    }
                    break;
                case 2:
                    cout << "Ingresar el nuevo precio de $Compra para el producto '" << nombre[found] << "': ";
                    cin >> newvalue2;
                    cout << endl;
                    cout << "Confirmar el cambio de '" << preciocompra[found] << "' a '" << newvalue2 << "' (1 = Confirmar, 2 = Cancelar): ";
                    cin >> respuesta4;
                    cout << endl;
                    if (respuesta4==1){
                        preciocompra[found] = newvalue2;
                        cout << "Cambio exitoso." << endl;
                        cout << endl;
                    }
                    else if (respuesta4==2){
                        cout << "Cancelado" << endl;
                        cout << endl;
                    }
                    break;
                case 3:
                    cout << "Ingresar el nuevo precio de $Venta para el producto '" << nombre[found] << "': ";
                    cin >> newvalue2;
                    cout << endl;
                    cout << "Confirmar el cambio de '" << precioventa[found] << "' a '" << newvalue2 << "' (1 = Confirmar, 2 = Cancelar): ";
                    cin >> respuesta4;
                    cout << endl;
                    if (respuesta4==1){
                        precioventa[found] = newvalue2;
                        cout << "Cambio exitoso." << endl;
                        cout << endl;
                    }
                    else if (respuesta4==2){
                        cout << "Cancelado" << endl;
                        cout << endl;
                    }
                    break;
                case 4:
                    cout << "Ingresar el nuevo numero de Existencias para el producto '" << nombre[found] << "': ";
                    cin >> newvalue3;
                    cout << endl;
                    cout << "Confirmar el cambio de '" << existencias[found] << "' a '" << newvalue3 << "' (1 = Confirmar, 2 = Cancelar): ";
                    cin >> respuesta4;
                    cout << endl;
                    if (respuesta4==1){
                        existencias[found] = newvalue3;
                        cout << "Cambio exitoso." << endl;
                        cout << endl;
                    }
                    else if (respuesta4==2){
                        cout << "Cancelado" << endl;
                        cout << endl;
                    }
                    break;
                case 5:
                    cout << "Ingresar el nuevo numero de Stock Minimo para el producto '" << nombre[found] << "': ";
                    cin >> newvalue3;
                    cout << endl;
                    cout << "Confirmar el cambio de '" << stockmin[found] << "' a '" << newvalue3 << "' (1 = Confirmar, 2 = Cancelar): ";
                    cin >> respuesta4;
                    cout << endl;
                    if (respuesta4==1){
                        stockmin[found] = newvalue3;
                        cout << "Cambio exitoso." << endl;
                        cout << endl;
                    }
                    else if (respuesta4==2){
                        cout << "Cancelado" << endl;
                        cout << endl;
                    }
                    break;
                default:
                    cout << "La opcion no es validad. ";
                    break;
            }
            
        } else if(found < 0){
            cout << "El producto se encuentra desactivado. Aun asi desea modificarlo? (1 = SI, 2 = NO): ";
            cin >> respuesta2;
            if (respuesta2==1){
                cout << endl;
                cout << "         El producto se encuentra inactivo!     " << endl << endl;
                cout << "Producto       " << "$Compra  " << "$Venta  " << "Existencias  " << "Stock" << endl;
                cout << "___________________________________________________" << endl;
                cout << nombre[-found] << "\t\t\t" << preciocompra[-found] << "\t\t" << precioventa[-found] << "\t\t" << existencias[-found] << "\t\t\t " << stockmin[-found] << endl;
                cout << endl;
                cout << "1. Nombre" << endl;
                cout << "2. $Compra" << endl;
                cout << "3. $Venta" << endl;
                cout << "4. Existencias" << endl;
                cout << "5. Stock" << endl;
                cout << "6. Activarlo" << endl;
                cout << "Ingrese el numero correspondiente para modificar el campo deseado: ";
                cin >> respuesta3;
                switch(respuesta3){
                    case 1:
                        cout << "Ingresar el nuevo nombre para el producto '" << nombre[-found] << "': ";
                        cin >> newvalue;
                        cout << "Confirmar el cambio de '" << nombre[-found] << "' a '" << newvalue << "' (1 = Confirmar, 2 = Cancelar): ";
                        cin >> respuesta4;
                        if (respuesta4==1){
                            nombre[-found] = newvalue;
                            cout << "Cambio exitoso." << endl;
                            cout << endl;
                        }
                        else if (respuesta4==2){
                            cout << "Cancelado" << endl;
                            cout << endl;
                        }
                        break;
                    case 2:
                        cout << "Ingresar el nuevo precio de $Compra para el producto '" << nombre[-found] << "': ";
                        cin >> newvalue2;
                        cout << "Confirmar el cambio de '" << preciocompra[-found] << "' a '" << newvalue2 << "' (1 = Confirmar, 2 = Cancelar): ";
                        cin >> respuesta4;
                        if (respuesta4==1){
                            preciocompra[-found] = newvalue2;
                            cout << "Cambio exitoso." << endl;
                            cout << endl;
                        }
                        else if (respuesta4==2){
                            cout << "Cancelado" << endl;
                            cout << endl;
                        }
                        break;
                    case 3:
                        cout << "Ingresar el nuevo precio de $Venta para el producto '" << nombre[-found] << "': ";
                        cin >> newvalue2;
                        cout << "Confirmar el cambio de '" << precioventa[-found] << "' a '" << newvalue2 << "' (1 = Confirmar, 2 = Cancelar): ";
                        cin >> respuesta4;
                        if (respuesta4==1){
                            precioventa[-found] = newvalue2;
                            cout << "Cambio exitoso." << endl;
                            cout << endl;
                        }
                        else if (respuesta4==2){
                            cout << "Cancelado" << endl;
                            cout << endl;
                        }
                        break;
                    case 4:
                        cout << "Ingresar el nuevo numero de Existencias para el producto '" << nombre[-found] << "': ";
                        cin >> newvalue3;
                        cout << "Confirmar el cambio de '" << existencias[-found] << "' a '" << newvalue3 << "' (1 = Confirmar, 2 = Cancelar): ";
                        cin >> respuesta4;
                        if (respuesta4==1){
                            existencias[-found] = newvalue3;
                            cout << "Cambio exitoso." << endl;
                            cout << endl;
                        }
                        else if (respuesta4==2){
                            cout << "Cancelado" << endl;
                            cout << endl;
                        }
                        break;
                    case 5:
                        cout << "Ingresar el nuevo numero de Stock Minimo para el producto '" << nombre[-found] << "': ";
                        cin >> newvalue3;
                        cout << "Confirmar el cambio de '" << stockmin[-found] << "' a '" << newvalue3 << "' (1 = Confirmar, 2 = Cancelar): ";
                        cin >> respuesta4;
                        if (respuesta4==1){
                            stockmin[-found] = newvalue3;
                            cout << "Cambio exitoso." << endl;
                            cout << endl;
                        }
                        else if (respuesta4==2){
                            cout << "Cancelado" << endl;
                            cout << endl;
                        }
                        break;
                    case 6:
                        cout << "Dease activar el producto '" << nombre[-found] << "'? (1 = Confirmar, 2 = Cancelar): ";
                        cin >> respuesta4;
                        if (respuesta4==1){
                            borrado[-found] = 0;
                            cout << "Cambio exitoso." << endl;
                            cout << endl;
                        }
                        else if (respuesta4==2){
                            cout << "Cancelado" << endl;
                            cout << endl;
                        }
                        break;
                    default:
                        cout << "La opcion no es validad. ";
                        break;
                }
            }
            
        } else {
            cout << "El producto no existe. ";
        }
        save();
        cout << "Desea modificar otro producto? (1 = SI, 2 = NO): ";
        cin >> respuesta;
        cout << endl;
    }while(respuesta!=2);
    save();
}

// show inventory
void mostrar(){
    cout << endl;
    cout << "Producto       " << "$Compra  " << "$Venta  " << "Existencias  " << "Stock" << endl;
    cout << "___________________________________________________" << endl;
    for (int i = 0; i < nombre.size(); i++){
        if(borrado[i] == 0) cout << nombre[i] << " \t\t\t" << preciocompra[i] << "\t\t" << precioventa[i] << "\t\t" << existencias[i] << "\t\t\t " << stockmin[i];
        if(existencias[i] < stockmin[i]){
            cout << " *" << endl;
        } else {
            cout << endl;
        }
    }
    cout << endl;
}

// consult specific item
void consultar(){
    string nombre0;
    int respuesta = 0;
    double found;
    do{
        cin.ignore();
        cout << "Que producto deseas consultar? ";
        getline(cin, nombre0);
        found = search(nombre0);
        if (found == 0.1){
            cout << "El producto no existe. ";
        }
        if (found < 0){
            cout << endl;
            cout << "         El producto se encuentra inactivo!     " << endl << endl;
            cout << "Producto       " << "$Compra  " << "$Venta  " << "Existencias  " << "Stock" << endl;
            cout << "___________________________________________________" << endl;
            cout << nombre[-found] << "\t\t\t" << preciocompra[-found] << "\t\t" << precioventa[-found] << "\t\t" << existencias[-found] << "\t\t\t " << stockmin[-found] << endl;
            cout << endl;
        }
        if (found == 0 || found > 0.1){
            cout << endl;
            cout << "Producto       " << "$Compra  " << "$Venta  " << "Existencias  " << "Stock" << endl;
            cout << "___________________________________________________" << endl;
            cout << nombre[found] << "\t\t\t" << preciocompra[found] << "\t\t" << precioventa[found] << "\t\t" << existencias[found] << "\t\t\t " << stockmin[found] << endl;
            cout << endl;
        }
        cout << "Deseas consultar otro producto? (1 = SI, 2 = NO): ";
        cin >> respuesta;
    }while(respuesta!=2);
}

// search for item in inventory and return 0.1 for not found, i < 0 for inactive, i == 0 || i > 0.1 for active
double search(string nombre0){
    int i = 0;
    double found = 0;
    while( i < nombre.size()){
        if (nombre0 == nombre[i]){
            if (borrado[i]==0){
                found = i;
                break;
            } else {
                found = -i;
                break;
            }
        } else {
            found = 0.1;
        }
        i++;
    }
    return found;
}

// admin sales users menu
void users(){
    int op;
    do{
        cout << "\nMenu de Usuarios\n";
        cout << "1. Crear Usuario Nuevo\n";
        cout << "2. Modificar Usuario\n";
        cout << "3. Mostrar Todos\n";
        cout << "4. Salir\n";
        cout << "Opcion: ";
        cin >> op;
        cout << endl;
        
        switch(op){
            case 1:
                if (salesname.size() < 5){
                    newuser();
                } else {
                    cout << "Solo se permite un maximo de 5 usuarios.\n";
                }
                break;
            case 2:
                edituser();
                break;
            case 3:
                showusers();
                break;
            case 4:
                break;
            default:
                cout << "Opcion seleccionada no es valida." << endl;
                break;
        }
    }while(op!=4);
}
// show all users
void showusers(){
    cout << "Usuarios de Ventas" << endl;
    cout << "___________________" << endl;
    for (int i = 0; i < salesname.size(); i++){
        cout << i+1;
        cout << ":" << "\t\t" << salesname[i] << endl;
    }
}


// edit user
void edituser(){
    int respuesta = 0, respuesta2 = 0;
    int userid, modtype;
    string nombre0, password0;
    cin.ignore();
    do{
        cout << "Escribe # de Usuario" << endl;
        cout << "    A Modificar       " << endl;
        cout << "___________________" << endl;
        for (int i = 0; i < salesname.size(); i++){
            cout << i+1;
            cout << ":" << "\t\t" << salesname[i] << endl;
        }
        cout << endl << "Usuario: ";
        cin >> userid;
        cout << endl;
        cout << "Selecciona opcion a modificar:" << endl;
        cout << "1. Nombre" << endl;
        cout << "2. Contraseña" << endl;
        cout << "3. Remover Usuario" << endl;
        cout << "Opcion: ";
        cin >> modtype;
        if(modtype==1){
            cout << "\nIngresar nuevo nombre para el usuario '" << salesname[userid-1] << "': ";
            cin.ignore();
            getline(cin, nombre0);
            salesname[userid-1] = nombre0;
            cout << "\nCambio exitoso!" << endl << endl;
        }
        if(modtype==2){
            cout << "\nIngresar nueva contraseña para el usuario '" << salesname[userid-1] << "': ";
            cin.ignore();
            getline(cin, password0);
            salespassword[userid-1] = password0;
            cout << "\nCambio exitoso!" << endl << endl;
        }
        if(modtype==3){
            cout << "\nEsta seguro que desea borrar el usuario '" << salesname[userid-1] << "' permanentemente? (1 = SI, 2 = NO): " << endl;
            cin >> respuesta2;
            if(respuesta2==1){
                salesname.erase (salesname.begin()+(userid-1));
                salespassword.erase (salespassword.begin()+(userid-1));
                cout << "\nUsuario fue dado de baja exitosamente!" << endl;}
            
        }
        cout << "Deseas modificar otro usuario? (1 = SI, 2 = NO): ";
        cin >> respuesta;
        cout << endl;
        
        ofstream outfile;
        outfile.open("users.txt");
        for(int s = 0; s <= salesname.size()-1; s++){
            outfile << endl << salesname[s] << endl << salespassword[s];
        }
        outfile.close();
    }while(respuesta!=2);
}

// create new user
void newuser(){
    int respuesta = 0;
    string salesname0, salespassword0;
    ofstream outfile;
    bool found;
    do{
        outfile.open("users.txt", ios::app);
        cin.ignore();
        cout << "Creando usuario..." << endl;
        cout << "Ingresar nombre de usuario: ";
        getline(cin, salesname0);
        found = searchuser(salesname0);
        if (found==false){
            salesname.push_back (salesname0);
            cout << endl;
            cout << "Ingresar contrasena: ";
            getline(cin, salespassword0);
            salespassword.push_back (salespassword0);
            cout << endl << "Usuario '" << salesname0 << "' creado exitosamente.\n" << endl;
            cout << "Desea crear otro usuario? (1 = SI, 2 = NO): ";
            cin >> respuesta;
            cout << endl;
            outfile << endl << salesname0 << endl << salespassword0;
            outfile.close();
            if (respuesta==1 && salesname.size()==5) cout << "Solo se permite un maximo de 5 usuarios.\n";
        } else{
            cout << "\nEl usario ya existe. Desea intentar de nuevo? (1 = SI, 2 = NO): ";
            cin >> respuesta;
            cout << endl;
        }
    }while(respuesta!=2 && salesname.size() < 5);
}

// search for a user in user list and return true or false
bool searchuser(string salesname0){
    int i = 0;
    double found = 0;
    while( i < salesname.size()){
        if (salesname0 == salesname[i]){
            found = true;
        } else {
            found = false;
        }
        i++;
    }
    return found;
}

// sales menu
void ventas(){
    int userid, respuesta;
    string salespassword0;
    do{
        cout << "Seleccionar Usuario:" << endl;
        cout << "_____________________" << endl;
        for (int i = 0; i < salesname.size(); i++){
            cout << i+1;
            cout << ":" << "\t\t" << salesname[i] << endl;
        }
        cout << endl << "Usuario: ";
        cin >> userid;
        cout << "Favor de ingresar contraseña: ";
        cin >> salespassword0;
        cout << endl;
        if (salespassword0 == salespassword[userid-1]){
            salesuser(userid-1);
            respuesta=2;
        } else {
            cout << "La contraseña es incorrecta. Desea intentar de nuevo (1 = SI, 2 = NO): ";
            cin >> respuesta;
            cout << endl;
        }
    }while(respuesta!=2);
    
}
// view sales
void viewsales(int userid){
    int d = 0;
    cout << endl << "Fecha    \tCorte#\tTicket#\tCant  \tImporte \tProducto   " << endl;
    cout << "_______________________________________________________________" << endl;
    for(int z = 0; z < transaction_salesticketnum.size(); z++){
        if(salesname[userid]==transaction_salesname[z]){
            cout << transaction_day[z] << "-" << transaction_month[z] << "-" << transaction_year[z] << "\t" << transaction_corte[z] << "\t\t#" << transaction_salesticketnum[z] << "\t\t " << transaction_cant[z] << "\t\t$" <<  transaction_importe[z] << "  \t  " <<  transaction_desc[z] << endl;
            d++;
        }
    }
    cout << endl;
    cout << "Total de transacciones: " << d << endl;
}


// sales per user
void salesuser(int userid){
    int opcion;
    cout << "Hola " << salesname[userid] << "!" << endl;
    while(opcion!=3){
        cout << "\nSeleccionar opcion:" << endl;
        cout << "1. Iniciar Dia de Ventas" << endl;
        cout << "2. Visualizar Ventas" << endl;
        cout << "3. Salir" << endl;
        cout << "Opecion: ";
        cin >> opcion;
        cout << endl;
        
        switch(opcion){
            case 1:
                salesday(userid);
                break;
            case 2:
                viewsales(userid);
                break;
            case 3:
                break;
            default:
                break;
        }
    }
}

// sales day
void salesday(int userid){
    int respuesta = 2, respuesta2 = 2, respuesta3 = 0, cantidad, trans_per_ticket = 0;
    string nombre0;
    double found, transaction_total0 = 0, corte_total0 = 0, ticket_total0 = 0.0, corte_dia00 = 0.0;
    unsigned long venta_num = 0, corte = 0, ticket_count0 = 0;
    time_t t = time(0);
    struct tm * now = localtime( & t );
    anio.push_back (now->tm_year + 1900);
    mes.push_back (now->tm_mon + 1);
    dia.push_back (now->tm_mday);
    
    do{
        unsigned long i;
        if(respuesta==2){
            corte_user.push_back (salesname[userid]);
            corte = corte_user.size();
            corte_num.push_back (corte);
        }
        venta_num = transaction_salesticketnum.back() + 1;
        cout << "Ticket #" << venta_num << "            Fecha: " << anio[0] << "-" << mes[0] << "-" << dia[0] << endl;
        do{
            ofstream outfile;
            outfile.open("transactions.txt", ios::app);
            
            cin.ignore();
            cout << "\nNombre de producto a vender: ";
            getline(cin, nombre0);
            found = search(nombre0);
            if (found==0 || found>0.1){
                do{
                    cout << "Cantidad a vender, tiene " << existencias[found] << " en existencias: ";
                    cin >> cantidad;
                    cout << endl;
                    if (cantidad <= existencias[found]){
                        existencias[found] = existencias[found] - cantidad;
                        trans_per_ticket++;
                        cout << "************************************************" << endl;
                        cout << endl << "Ticket de Venta #" << venta_num << "\t\t Fecha: " << anio[0] << "-" << mes[0] << "-" << dia[0] << endl;
                        cout << endl << "Cant \tDescripcion      \tPrecio    \tImporte" << endl;
                        cout << "_____________________________________________________" << endl;
                        cout << cantidad  << "\t\t" << nombre[found] << "\t\t\t$ " << precioventa[found] << "\t\t$ " << cantidad * precioventa[found] << endl;
                        respuesta3=2;
                        transaction_salesticketnum.push_back (venta_num), transaction_year.push_back (anio[0]), transaction_month.push_back (mes[0]), transaction_day.push_back (dia[0]), transaction_cant.push_back (cantidad), transaction_desc.push_back (nombre[found]), transaction_price.push_back (precioventa[found]), transaction_importe.push_back (cantidad * precioventa[found]), transaction_ticket_count.push_back (ticket_count0);
                        
                        transaction_total0 = cantidad * precioventa[found];
                        corte_total0 = corte_total0 + transaction_total0;
                        ticket_total0 = ticket_total0 + transaction_total0;
                        
                        transaction_total.push_back (transaction_total0);
                        transaction_salesname.push_back (salesname[userid]);
                        transaction_corte.push_back (corte);
                        
                        i = transaction_salesticketnum.size() - 1;
                        outfile <<  endl << transaction_salesname[i] << endl <<  transaction_corte[i] << endl << transaction_salesticketnum[i] << endl << transaction_year[i] << endl << transaction_month[i] << endl << transaction_day[i] << endl << transaction_cant[i] << endl <<  transaction_desc[i] << endl <<  transaction_price[i] << endl <<  transaction_importe[i] << endl << transaction_total[i];
                        outfile.close();
                        save();
                    }
                    if (cantidad > existencias[found]){
                        cout << "No se cuenta con suficiente cantidad en existencias. Desea ingresar otra cantidad? (1 = SI, 2 = NO): ";
                        cin >> respuesta3;
                    }
                    
                }while(respuesta3!=2);
            } else {
                cout << "\nEl producto no existe o esta desactivado." << endl;
            }
            cout << "\n1. Vender Otro Producto";
            cout << "\n2. Cerrar Ticket";
            cout << "\nOpcion: ";
            cin >> respuesta2;
            cout << endl;
            if(respuesta2==2){
                ticket_count0++;
                ticket_by_one.push_back (ticket_count0);
                unsigned long t = ticket_by_one.size()-1;
                cout << endl;
                cout << "*****************************************************" << endl;
                cout << "Vendedor: " << salesname[userid] << endl;
                cout << "Ticket de Venta #" << venta_num << "\t\t Fecha: " << transaction_year[t] << "-" << transaction_month[t] << "-" << transaction_day[t] << endl;
                cout << endl << "Cant \tDescripcion      \tPrecio      \tImporte" << endl;
                cout << "______________________________________________________" << endl;
                
                for(t = transaction_total.size()-1; t > transaction_total.size()-trans_per_ticket-1; t--){
                    cout << transaction_cant[t]  << "\t\t" << transaction_desc[t] << "\t\t\t$ " << transaction_price[t] << "\t\t$ " << transaction_importe[t] << endl;
                }
                cout << endl;
                cout << "                                  Total: $" << ticket_total0 << endl;
                cout << "*****************************************************" << endl;
                ticket_corte_total.push_back (ticket_total0);
                ticket_corte_ventanum.push_back (venta_num);
                ticket_corte_cortenum.push_back (corte);
                ticket_total0 = 0.0;
                trans_per_ticket = 0;
            }
        }while(respuesta2!=2);
        cout << "\n1. Iniciar Nueva Venta";
        cout << "\n2. Realizar Corte";
        cout << "\nOpcion: ";
        cin >> respuesta;
        if(respuesta == 2){
            ofstream outfile;
            outfile.open("cortes.txt", ios::app);
            corte_total.push_back (corte_total0);
            cout << corte << endl;
            outfile << endl << corte_num[corte-1] << endl << corte_user[corte-1] << endl << corte_total[corte-1];
            outfile.close();
            
            cout << endl;
            cout << "*****************************************************" << endl;
            cout << "Vendedor: " << salesname[userid] << endl;
            cout << "Numero de Corte: " << corte_num[corte-1] << "\t\t Fecha: " << transaction_year[corte-1] << "-" << transaction_month[corte-1] << "-" << transaction_day[corte-1] << endl;
            cout << endl << "Venta           \t\tTotal" << endl;
            cout << "____________________________________" << endl;
            cout << endl;
            for(int o = 0; o < ticket_corte_ventanum.size(); o++){
                if(ticket_corte_cortenum[o] == corte){
                    cout << "Venta " << ticket_corte_ventanum[o] << "         \t Total: $" << ticket_corte_total[o] << endl;
                }
            }
            cout << "*****************************************************" << endl;
            cout << "Vendedor: " << salesname[userid] << "                     Total: " << corte_total[corte_total.size()-1] << endl;
            corte_dia00 = corte_total[corte_total.size()-1];
            cortes_dia.push_back (corte_dia00);
            cortes_dia_user.push_back (userid);
            
        }
        cout << endl;
    }while(respuesta!=2);
}

// Show end of day
void cortesdia(){
    for(int h = 0; h < cortes_dia.size(); h++){
        cout << "Ventas de " << salesname[cortes_dia_user[h]] << ": ___________ " "Total: " << cortes_dia[h] << endl << endl;
    }
    
    
}

// save inventory to file
void save(){
    int i = 0;
    ofstream outfile;
    outfile.open("inventory.txt");
    while (i < nombre.size() - 1){
        outfile << nombre[i] << endl << preciocompra[i] << endl << precioventa[i] << endl << existencias[i] << endl << stockmin[i] << endl << borrado[i] << endl;
        i++;
    }
    outfile << nombre[i] << endl << preciocompra[i] << endl << precioventa[i] << endl << existencias[i] << endl << stockmin[i] << endl << borrado[i];
    outfile.close();
}

// reading data from file
void load(){
    string nombre0, salesname0, salespassword0, corte_user0, transaction_salesname0, transaction_desc0;
    int existencias0, stockmin0, borrado0, corte_num0, transaction_corte0, transaction_salesticketnum0, transaction_year0, transaction_month0, transaction_day0, transaction_cant0;
    double preciocompra0, precioventa0, corte_total0, transaction_price0, transaction_importe0, transaction_total0;
    
    
    ifstream infile;
    infile.open("inventory.txt");
    if (infile.fail()){
        cout << "Error en el archivo inventory.txt. " << endl;
    }
    if (!infile.fail()){
        cout << "Inventario cargado exitosamente.";
    }
    while (!infile.eof()) {
        infile >> nombre0;
        nombre.push_back (nombre0);
        infile >> preciocompra0;
        preciocompra.push_back (preciocompra0);
        infile >> precioventa0;
        precioventa.push_back (precioventa0);
        infile >> existencias0;
        existencias.push_back (existencias0);
        infile >> stockmin0;
        stockmin.push_back (stockmin0);
        infile >> borrado0;
        borrado.push_back (borrado0);
    }
    infile.close();
    
    infile.open("users.txt");
    if (infile.fail()){
        cout << "\nError al abrir el archivo users.txt." << endl;
    }
    if (!infile.fail()){
        cout << "\nUsuarios cargados exitosamente.";
    }
    while (!infile.eof()) {
        infile >> salesname0;
        salesname.push_back (salesname0);
        infile >> salespassword0;
        salespassword.push_back (salespassword0);
    }
    infile.close();
    
    infile.open("cortes.txt");
    if (infile.fail()){
        cout << "\nError al abrir el archivo cortes.txt." << endl;
    }
    if (!infile.fail()){
        cout << "\nCortes cargados exitosamente.";
    }
    while (!infile.eof()) {
        infile >> corte_num0;
        corte_num.push_back (corte_num0);
        infile >> corte_user0;
        corte_user.push_back (corte_user0);
        infile >> corte_total0;
        corte_total.push_back (corte_total0);
    }
    infile.close();
    
    infile.open("transactions.txt");
    if (infile.fail()){
        cout << "\nError al abrir el archivo transactions.txt." << endl;
    }
    if (!infile.fail()){
        cout << "\nTransacciones cargadas exitosamente.";
    }
    while (!infile.eof()) {
        infile >> transaction_salesname0;
        transaction_salesname.push_back (transaction_salesname0);
        infile >> transaction_corte0;
        transaction_corte.push_back (transaction_corte0);
        infile >> transaction_salesticketnum0;
        transaction_salesticketnum.push_back (transaction_salesticketnum0);
        infile >> transaction_year0;
        transaction_year.push_back (transaction_year0);
        infile >> transaction_month0;
        transaction_month.push_back (transaction_month0);
        infile >> transaction_day0;
        transaction_day.push_back (transaction_day0);
        infile >> transaction_cant0;
        transaction_cant.push_back (transaction_cant0);
        infile >> transaction_desc0;
        transaction_desc.push_back (transaction_desc0);
        infile >> transaction_price0;
        transaction_price.push_back (transaction_price0);
        infile >> transaction_importe0;
        transaction_importe.push_back (transaction_importe0);
        infile >> transaction_total0;
        transaction_total.push_back (transaction_total0);
    }
    infile.close();
}


