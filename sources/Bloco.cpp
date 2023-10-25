#include "Bloco.h"

using namespace std;

Bloco::Bloco(){}

Bloco::Bloco(int tamanho, int posicaoX, int posicaoY){
    this->tamanho = tamanho;
    this->posicaoX = posicaoX;
    this->posicaoY = posicaoY;
}

void Bloco::exibe(){
    cout << "Bloco na posicao (" << posicaoX << "," << posicaoY << ") de tamanho " << tamanho << endl;
}