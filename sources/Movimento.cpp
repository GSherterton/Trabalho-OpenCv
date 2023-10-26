#include "Movimento.h"

using namespace std;

Movimento::Movimento(){
    //velocidadeInicial = -10;
    inicio = true;
    velocidadeAtual = 0;
}

bool Movimento::colisao(){
    for(int i = 0; i < blocos.size(); i++){//vai ver se ha colisao com todos os blocos
        //cout << "distancia na altura para o bloco: " << abs(yAtual - blocos[i].posicaoY) << endl;
        if((abs(yAtual - blocos[i].posicaoY) <= deltaColisao) && ((xAtual >= blocos[i].posicaoX) && xAtual <= blocos[i].posicaoX + blocos[i].tamanho)){
            //cout << "Foi detectada colisao\n";
            return 1;
        }
    }

    return 0;
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
        if(colisao()){
            pular();
            subindo = 1;
        }
    }
    yAtual += velocidadeAtual;
    velocidade();

    return yAtual;
}

void Movimento::mostrarBlocos(){
    if(blocos.size() == 0){
        cout << "Nao ha nenhum bloco\n";
    }

    for(int i = 0; i < blocos.size(); i++){
        blocos[i].exibe();
    }
}