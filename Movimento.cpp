#include "Movimento.h"

using namespace std;

Movimento::Movimento(){
    //velocidadeInicial = -10;
    inicio = true;
    velocidadeAtual = 0;
}

int Movimento::velocidade(){
    return velocidadeAtual += gravidade;
}

void Movimento::pular(){
    velocidadeAtual = velocidadeInicial;
}

int Movimento::movimentoY(){
    if(velocidadeAtual >= 0){
        subindo = false;
    }
    if(!subindo){
        if(alturaAtual >= alturaMinima){//mudar o parametro para ver se houve colisao
            pular();
            subindo = 1;
        }
    }
    alturaAtual += velocidadeAtual;
    velocidade();

    return alturaAtual;
}