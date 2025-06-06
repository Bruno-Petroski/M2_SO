#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <locale.h>
#include "Memoria_32.hpp"

using namespace std;

void separaBits256B(bitset<32> end_Binario, int &pag, int &des){
    bitset<24> pag_Binario;
    bitset<8> des_Binario;

    for(int i = 0; i < 8; i++){
        des_Binario[i] = end_Binario[i];
    }

    for(int i = 0; i < 24; i++){
        pag_Binario[i] = end_Binario[i+8];
    }
    
    des = (int)des_Binario.to_ulong();
    pag = (int)pag_Binario.to_ulong();
}

void separaBits1KB(bitset<32> end_Binario, int &pag, int &des) {
    bitset<22> pag_Binario;  
    bitset<10> des_Binario; 

    for (int i = 0; i < 10; i++) {
        des_Binario[i] = end_Binario[i];
    }

    for (int i = 0; i < 22; i++) {
        pag_Binario[i] = end_Binario[i + 10];
    }

    des = (int)des_Binario.to_ulong();
    pag = (int)pag_Binario.to_ulong();
}

void separaBits4KB(bitset<32> end_Binario, int &pag, int &pag1, int &pag2, int &des) {
    bitset<10> pag1_Binario, pag2_Binario;
    bitset<12> offsetBits;
    bitset<20> pag_Binario;


    for (int i = 0; i < 12; i++) {
        offsetBits[i] = end_Binario[i];
    }

    for (int i = 0; i < 10; i++) {
        pag2_Binario[i] = end_Binario[i + 12];
    }

    for (int i = 0; i < 10; i++) {
        pag1_Binario[i] = end_Binario[i + 22];
    }

    for (int i = 0; i < 20; i++) {
        pag_Binario[i] = end_Binario[i + 12];
    }

    des = offsetBits.to_ulong();
    pag2 = pag2_Binario.to_ulong();
    pag1 = pag1_Binario.to_ulong();
    pag = pag_Binario.to_ulong();

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
    bitset<32> end_Binario;
    unsigned long end;
    string end_String;
    bool isValido = false;

    bool isDebug = true; // Debug do código para ativar em aula caso necessario

    do{
        if(isDebug){ 
            imprimeTP_4K();
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

        if(op1 == 1){
            end = stol(end_String);
        }else{
            end = stol(end_String, nullptr, 16);
        }

        isValido = false;
        if (end >= 0 and end <= 4294967295){
            end_Binario = end; // Converte automaticamente inteiro para binario
            isValido = true;
        }

        if (isValido) {
            if (!isDebug) system("clear");
            cout<<"Qual o tamanho da página?"<< endl;
            cout << "1 - 256B" << endl;
            cout << "2 - 1KB" << endl;
            cout << "3 - 4KB" << endl;

            cout << "Escolha: ";
            cin >> op2;

            int pag, pag1, pag2, des, tam_pagina;
            switch(op2){
                // 256B
                case 1:{
                    // 24 Bits utilizados para páginas mais significativos
                    // 8 Bits utilizados para o deslocamento menos significativos
                    separaBits256B(end_Binario, pag, des);
                    tam_pagina = 256;
                    break;
                }
                // 1KB
                case 2:{
                    // 22 Bits utilizados para páginas mais significativos
                    // 10 Bits utilizados para o deslocamento menos significativos
                    separaBits1KB(end_Binario, pag, des);
                    tam_pagina = 1024;
                    break;
                }
                // 4KB
                case 3:{
                    // 20 Bits utilizados para páginas mais significativos
                    // 12 Bits utilizados para o deslocamento menos significativos
                    separaBits4KB(end_Binario, pag, pag1, pag2, des);
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

                if(tam_pagina == 4096){

                    procuraTP_4K(pag1, pag2, frameTP, isPageHIT);
                    if(isPageHIT){
                        acao += ", Page Hit";
                        insereTLB(pag, frameTP);

                        endArq = frameTP*tam_pagina + des;
                        valor = lerMemoriaFisica(endArq);

                    }else{
                        acao += ", Page Fault";

                        int frameBS = lerBackingStore(pag);
                        
                        if (frameBS == -1) {
                            acao += ", não encontrado na backing store";
                        }else{
                            acao += ", carregado da backing store";

                            SwapPageFault_4K(pag1, pag2, frameBS);
                            insereTLB(pag, frameBS);

                            endArq = frameBS * tam_pagina + des;
                            valor = lerMemoriaFisica(endArq);
                        }
                    }
                }else{

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
            }

            if (!isDebug) system("clear");
            cout << "Endereço virtual: " << end_String << endl;
            cout << "Página: " << pag << endl;
            cout << "Deslocamento: " << des << endl;
            cout << "Ação tomada: " << acao << endl;
            cout << "Valor lido: " << valor << endl << endl;

            cout << "Deseja inserir outro endereço?"<<endl;
            cout << "1 - Sim" << endl;
            cout << "2 - Não" << endl;

            cout << "Escolha: ";
            cin >> op3;

        } else {
            cout << "Endereço inválido para 32 bits." << endl;
        }

    } while(op3 != 2);

    return 0;
}
