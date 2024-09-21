//
// Created by NATALIA WATSON on 20/09/2024.
//

#include <iostream>
#include <cmath>
#include <random>
#include <string>
#include <stdexcept>
#define MAX_SIZE 50
using namespace std;

template <typename T, typename V>
struct Entry{
    T key;
    V value;
    bool isDeleted;
    bool isOccupied;

    Entry() : isDeleted(false), isOccupied(false) {}

};

template <typename T, typename V>
class TablaHash{

    int m = MAX_SIZE;
    Entry<T, V>* table;
    int n_elements;
    int P = 97;
    int maxCol = 3;
    float fillfactor;
    int a, b;

public:
    TablaHash(): n_elements(0){
        table = new Entry<T, V>[m]();
        a =  getRandomNumber(1, P-1);
        b = getRandomNumber(0, P-1);
    }

    int getRandomNumber(int lower, int P){
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(lower, P-1);
        return distrib(gen);

    }

    int newKey(T key){

        int nkey = 0;
        if constexpr (is_same_v<T, string>){
            for (char c: key){
                nkey = nkey*31 + c;
            }
        }
        else if constexpr (is_same_v<T, char> or is_same_v<T, int>){
                nkey = static_cast<int>(key);

        }
        else if constexpr (is_same_v<T, float>){
            nkey = static_cast<int>(key*1000);
        }
        else{
            throw invalid_argument("esta variable no esta siendo manejada");
        }
        return nkey;
}

    int hashingfunction(T key){
        int nkey = newKey(key);
        return ((a*nkey + b) % P) % m;
    }

    void reHash(){
        int new_size = m*2;
        Entry<T, V>* new_table = new Entry<T, V>[new_size]();

        Entry<T, V>* old_table = table;
        int old_size = m;

        m = new_size;
        table = new_table;

        a = getRandomNumber(1, P-1);
        b = getRandomNumber(0, P-1);

        for (int i = 0; i < old_size; ++i){
            if (old_table[i].isOccupied and !old_table[i].isDeleted){
                int new_index = hashingfunction(old_table[i].key);
                int original_new_index = new_index;
                int j = 0;
                while (table[new_index].isOccupied){
                    new_index = (original_new_index + ++j) % m;
                }
                table[new_index] = old_table[i];
            }
        }

        delete[] old_table;
    }




    void insert(T key, V value){
        int index = hashingfunction(key);
        int orginal_index = index;
        int i = 0;

        //manejar colisiones
        while (table[index].isOccupied and !table[index].isDeleted){
            if (table[index]. key == key){
                table[index].value = value;
                return;
            }

            index =  (orginal_index + ++i) % m;
        }

        table[index].key = key;
        table[index].value = value;
        table[index].isOccupied = true;
        table[index].isDeleted = false;
        n_elements++;

        fillfactor = static_cast<float>(n_elements) / m;
        if (fillfactor > 0.5){
            reHash();
        }

    }

    V search(T  key){
        int index = hashingfunction(key);
        int original_index = index;
        int i = 0;

        while (table[index].isOccupied){
            if (table[index].key == key and !table[index].isDeleted){
                return table[index].value;
            }

            index = (original_index + ++i) % m;
        }
        return V();

    }

    void Delete(T key){

        int index = hashingfunction(key);
        int original_index = index;
        int i = 0;

        while (table[index].isOccupied){
            if (table[index].key == key and !table[index].isDeleted){
                table[index].isDeleted = true;
                n_elements--;
                return;

            }

            index = (original_index + ++i) % m;
        }
    }

    bool empty(){
        return n_elements == 0;
    }

    int size(){
        return n_elements;
    }

    void clear(){
        delete[] table;
        table = new Entry<T, V>[m]();
        n_elements = 0;
    }




    ~TablaHash(){
        delete[] table;
    }


};


int main() {

    TablaHash<string, int> hashTable;

    cout << "Insertando elementos:" << endl;
    hashTable.insert("clave1", 100);
    hashTable.insert("clave2", 200);
    hashTable.insert("clave3", 300);
    hashTable.insert("clave4", 400);

    cout <<  "Buscando elementos insertados:" << endl;
    cout << "Valor de clave1: " << hashTable.search("clave1") << endl;
    cout << "Valor de clave2: " << hashTable.search("clave2") << endl;
    cout << "Valor de clave3: " << hashTable.search("clave3") << endl;
    cout << "Valor de clave4: " << hashTable.search("clave4") << endl;

    cout << "Buscando una clave que no existe (clave5): " << hashTable.search("clave5") << endl;

    cout << "Eliminando clave2q" << endl;
    hashTable.Delete("clave2");

    cout << "Buscando clave2 despues de eliminarla: " << hashTable.search("clave2") << endl;

    cout << "Esta vacia la tabla? " << (hashTable.empty() ? "si" : "no") << endl;

    cout << "Tamano de la tabla: " << hashTable.size() << endl;

    cout << "Limpiando la tabla" << endl;
    hashTable.clear();

    cout << "la tabla esta vacia despues de limpiar? " << (hashTable.empty() ? "si" : "no") << endl;


}