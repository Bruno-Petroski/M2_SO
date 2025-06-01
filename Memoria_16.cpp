#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <locale.h>
#include "Memoria.hpp"

using namespace std;

void separaBits256B(bitset<16> end_Binario, int &pag, int &des){
    bitset<8> pag_Binario, des_Binario;

    for(int i = 0; i < 8; i++){
        des_Binario[i] = end_Binario[i];
        pag_Binario[i] = end_Binario[i+8];
    }

    des = (int)des_Binario.to_ulong();
    pag = (int)pag_Binario.to_ulong();
}

void separaBits1KB(std::bitset<16> end_Binario, int &pag, int &des) {
    bitset<6> pag_Binario;  
    bitset<10> des_Binario; 

    for (int i = 0; i < 10; i++) {
        des_Binario[i] = end_Binario[i];
    }

    for (int i = 0; i < 6; i++) {
        pag_Binario[i] = end_Binario[i + 10];
    }

    des = (int)des_Binario.to_ulong();
    pag = (int)pag_Binario.to_ulong();
}

void separaBits4KB(bitset<16> end_Binario, int &pag, int &des) {

    bitset<4> pag_Binario;
    bitset<12> des_Binario;

    for (int i = 0; i < 12; i++) {
        des_Binario[i] = end_Binario[i];
    }

    for (int i = 0; i < 4; i++) {
        pag_Binario[i] = end_Binario[i + 12];
    }

    des = (int)des_Binario.to_ulong();
    pag = (int)pag_Binario.to_ulong();
}

string lerMemoriaFisica(unsigned long endereco) {
    string linha;
    ifstream arquivo("data_memory.txt");

    for (int i = 0; i < endereco; ++i) {
        getline(arquivo, linha);
    }

    return linha;
}

int main(){

    setlocale(LC_ALL, "Portuguese");
    int op1 = 0, op2 = 0, op3 = 0;
    bitset<16> end_Binario;
    unsigned long end;
    string end_String;
    bool isValido = false;

    bool isDebug = true; // Debug do código para ativar em aula caso necessario

    do{
        if isDebug { 
            imprimeTLB();
            imprimeTP();
        }
        if (!isDebug) system("clear");

        op1 = 0;
        op2 = 0;
        op3 = 0;
        
        cout << "Em qual formato você deseja inserir o endereço de memoria?" << endl;
        cout << "1 - Decimal" << endl;
        cout << "2 - Hexadecimal" << endl;
        cout << "3 - Sair" << endl;

        cout << "Escolha: ";
        cin >> op1;

        if(op1 == 3){
            return 0;
        }

        cout << "Insira o endereço: ";
        cin >> end_String;

        // Convertendo as strings em decimal
        if(op1 == 1){
            end = stol(end_String);
        }else{
            end = stol(end_String, nullptr, 16);
        }

        isValido = false;
        if (end >= 0 and end <= 65535){
            end_Binario = end; // Converte automaticamente inteiro para binario
            isValido = true;
        }

        if(isValido){
            if (!isDebug) system("clear");
            cout<<"Qual o tamanho da página?"<< endl;
            cout << "1 - 256B" << endl;
            cout << "2 - 1KB" << endl;
            cout << "3 - 4KB" << endl;

            cout << "Escolha: ";
            cin >> op2;

            int pag, des, tam_pagina;
            switch(op2){
                // 256B
                case 1:{
                    // 8 Bits utilizados para páginas mais significativos
                    // 8 Bits utilizados para o deslocamento menos significativos
                    separaBits256B(end_Binario, pag, des);
                    tam_pagina = 256;
                    break;
                }
                // 1KB
                case 2:{
                    //  6 Bits utilizados para páginas mais significativos
                    // 10 Bits utilizados para o deslocamento menos significativos
                    separaBits1KB(end_Binario, pag, des);
                    tam_pagina = 1024;
                    break;
                }
                // 4KB
                case 3:{
                    //  4 Bits utilizados para páginas mais significativos
                    // 12 Bits utilizados para o deslocamento menos significativos
                    separaBits4KB(end_Binario, pag, des);
                    tam_pagina = 4096;
                    break;
                }
            }

            unsigned long endArq;
            string valor = "-", acao;
            bool isTLBHIT;
            int frame = procuraTLB(pag, isTLBHIT);

            if(isTLBHIT){
                acao += "TLB Hit";

                endArq = frame*tam_pagina + des;
                valor = lerMemoriaFisica(endArq);            
                
            }else{
                acao += "TLB Miss";

                int pagTP, frameTP;
                bool isPageHIT;

                procuraTP(pag, pagTP, frameTP, isPageHIT);
                if(isPageHIT){
                    acao += ", Page Hit";

                    insereTLB(pagTP, frameTP);

                    endArq = frameTP*tam_pagina + des;
                    valor = lerMemoriaFisica(endArq);
                    
                }else{
                    acao += ", Page Fault";

                    int frameBS = lerBackingStore(pag);

                    if (frameBS == -1) {
                        acao += ", não encontrado na backing store";
                    }else{
                        acao += ", carregado da backing store";

                        SwapPageFault(pag, frameBS);
                        insereTLB(pag, frameBS);

                        endArq = frameBS * tam_pagina + des;
                        valor = lerMemoriaFisica(endArq);
                    }
                }
            }
            if (!isDebug) system("clear");
            cout << "Endereço virtual: " << end_String << endl;
            cout << "Página: " << pag << endl;
            cout << "Deslocamento: " << des << endl;
            cout << "Ação Tomada: " << acao << endl;
            cout << "Valor lido: " << valor << endl << endl;

            cout << "Deseja inserir outro endereço?"<<endl;
            cout << "1 - Sim" << endl;
            cout << "2 - Não" << endl;

            cout << "Escolha: ";
            cin >> op3;

        }else{
            cout<<"Endereço invalido para uma memoria de 16 bits"<<endl;
        }
    }while(op3 != 2);
    return 0;
}
