#include "MovimentoSimples.h"
//largura do pou 80
//altura do pou 70


using namespace std;

MovimentoSimples::MovimentoSimples(){
    velocidade = -24; //V0y 
}

void MovimentoSimples::modificaPosicao(){
    yPou += velocidade;
}

void MovimentoSimples::modificaVelocidade(){
    if(verificaColisao()){
        velocidade = -24; //V0y no pulo
    } else {
        velocidade += 2; // gravidade = 2;
    }
}

bool MovimentoSimples::verificaColisao(){
    for(int i = 0; i < blocos.size(); i++){
        if((abs(yPou - blocos[i].posicaoY)) < 4 && (xPou >= blocos[i].posicaoX) && xPou <= blocos[i].posicaoX + blocos[i].tamanhoX){
            return true;
        }
    }

    return false;    
}