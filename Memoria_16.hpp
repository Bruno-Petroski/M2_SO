#ifndef MEMORIA_16_HPP
#define MEMORIA_16_HPP
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int temporizador = 1;
const int TAM_TLB = 16;
const int TAM_TP = 32;

// TLB
struct TLB {
    int pagina, frame, tempo = temporizador++;
};

TLB tlb[TAM_TLB] = {
    {0, 5, 0},
    {1, 8, 0},
    {2, 3, 0},
    {3, 12, 0},
    {4, 1, 0},
    {5, 7, 0},
    {6, 6, 0},
    {7, 15, 0},
    {8, 2, 0},
    {9, 14, 0},
    {10, 9, 0},
    {11, 4, 0},
    {12, 10, 0},
    {13, 13, 0},
    {-1, -1, -1},
    {-1, -1, -1}    
};

int procuraTLB(int pag, bool &isTLBHIT){
    for (int i = 0; i < TAM_TLB; i++) {
        if (tlb[i].pagina == pag) {
            tlb[i].tempo = temporizador++;
            isTLBHIT = true;
            return tlb[i].frame;
        }
    }
    isTLBHIT = false;
    return -1;
}

void insereTLB(int pag, int frame) {
    // Procura se tem slot vazio na TLB
    for (int i = 0; i < TAM_TLB; i++) {
        if (tlb[i].pagina == -1) {
            tlb[i].pagina = pag;
            tlb[i].frame = frame;
            tlb[i].tempo = temporizador++;
            return;
        }
    }

    int menorT = 0;
    for (int i = 1; i < TAM_TLB; i++) {
        if (tlb[i].tempo < tlb[menorT].tempo) {
            menorT = i;
        }
    }
    tlb[menorT].pagina = pag;
    tlb[menorT].frame = frame;
    tlb[menorT].tempo = temporizador++;
}

void imprimeTLB(){
    cout << "----------------- TLB -----------------" << endl;
    for(int i = 0; i < TAM_TLB; i++){
        cout << "Pagina: " << tlb[i].pagina << ", Frame: " << tlb[i].frame << ", Tempo: " << tlb[i].tempo << endl;
    }
}

// Tabela de Paginas
struct TabelaPaginas {
    bool valid = false, accessed = false;
    int pagina = -1, frame = -1;
};

TabelaPaginas TP[TAM_TP] = {
    {true, true, 14, 0},
    {true, false, 15, 1},
    {true, true, 16, 2},
    {true, false, 17, 3},
    {true, true, 18, 4},
    {true, false, 19, 5},
    {true, true, 20, 6},
    {true, false, 21, 7},
    {true, true, 22, 8},
    {true, false, 23, 9},
    {true, true, 24, 10},
    {true, false, 25, 11},
    {true, true, 26, 12},
    {true, false, 27, 13},
    {true, true, 28, 14},
    {true, false, 29, 15},
    {true, true, 30, 16},
    {true, false, 31, 17},
    {true, true, 32, 18},
    {true, false, 33, 19},
    {true, true, 34, 20},
    {true, false, 35, 21},
    {true, true, 36, 22},
    {true, false, 37, 23},
    {true, true, 38, 24},
    {true, false, 39, 25},
    {true, true, 40, 26},
    {true, false, 41, 27},
    {true, true, 42, 28},
    {true, false, 43, 29},
    {true, true, 44, 30},
    {false, false, -1, -1}
};

void procuraTP(int pag, int &pagTP, int &frameTP, bool &isPageHIT){
    for (int i = 0; i < TAM_TP; i++) {
        if (TP[i].pagina == pag and TP[i].valid) {
            isPageHIT = true;
            pagTP = TP[i].pagina;
            frameTP = TP[i].frame;
            return;
        }
    }
    isPageHIT = false;
    return;
}

void SwapPageFault(int pag, int frameBS) {
    for (int i = 0; i < TAM_TP; i++) {
        if (!TP[i].valid) {
            TP[i].pagina = pag;
            TP[i].frame = frameBS;
            TP[i].valid = true;
            TP[i].accessed = true;
            return;
        }
    }

    int index = -1;
    for (int i = 0; i < TAM_TP; i++) {
        if (!TP[i].accessed) {
            index = i;
            break;
        }
    }

    // Se todas estão com accessed = true, escolhe o primeiro e define todos os accessed para false
    if (index == -1){
        for (int i = 0; i < TAM_TP; i++) {
            TP[i].accessed = false;
        }
        index = 0;
    } 

    TP[index].pagina = pag;
    TP[index].frame = frameBS;
    TP[index].valid = true;
    TP[index].accessed = true;
}


void imprimeTP(){
    cout << "----------------------- TP -----------------------" << endl;
    for(int i = 0; i < TAM_TP; i++){
        cout << "Pagina: " << TP[i].pagina << ", Frame: " << TP[i].frame << ", Valid: " << TP[i].valid << ", Accessed: " << TP[i].accessed << endl;
    }
}

// Backing Store
int lerBackingStore(int paginaBuscada) {
    ifstream arquivo("backing_store.txt");
    string linha;

    while (getline(arquivo, linha)) {
        stringstream ss(linha);
        string paginaStr, frameStr;

        if (getline(ss, paginaStr, ',') and getline(ss, frameStr)) {
            int pag = stoi(paginaStr);
            int frame = stoi(frameStr);
    
            if (pag == paginaBuscada) {
                return frame;
            }
        }
    }

    return -1;
}

#endif