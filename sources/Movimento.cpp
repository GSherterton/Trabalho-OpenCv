#include "Movimento.h"

using namespace std;

Movimento::Movimento(){
    //velocidadeInicial = -10;
    inicio = true;
    velocidadeAtual = 0;
    qtdSubiu = 0;
    pontos = 0;
}

void Movimento::deletaBloco(int index){
    bloco.erase(bloco.begin() + index);
}

void Movimento::deletaBlocos(){
    bloco.erase(bloco.begin(), bloco.end());
}

int Movimento::deltaColisao(){
    if(velocidadeAtual > 10){
        return deltaColisaoBase*(1. + (((float)velocidadeAtual-10)/20.));
    }

    return deltaColisaoBase;
}

bool Movimento::colisao(){
    for(int i = 0; i < bloco.size(); i++){//vai ver se ha colisao com todos os blocos
        if((abs(yAtual - bloco[i].posicaoY) <= deltaColisao()) && ((xAtual >= bloco[i].posicaoX) && xAtual <= bloco[i].posicaoX + bloco[i].tamanhoX)){
            //cout << "Foi detectada colisao\n";
            system("mplayer -msglevel all=-1 audios/jump.mp3 &");//tirei para debugar pq fica aparecendo no terminal
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

    if(subindo && (yAtual <= yMaximo)){
        pontos -= velocidadeAtual;
        subir(-velocidadeAtual);
    }else{
        yAtual += velocidadeAtual;
        //cout << "Velocidade Atual: " << velocidadeAtual << endl;
    }
    
    velocidade();

    return yAtual;
}

void Movimento::subir(int distancia){
    qtdSubiu += distancia;
    for(int i = 0; i < bloco.size(); i++){
        bloco[i].posicaoY += distancia;
    }
}

void Movimento::mostrarBlocos(){
    if(bloco.size() == 0){
        cout << "Nao ha nenhum bloco\n";
    }

    for(int i = 0; i < bloco.size(); i++){
        bloco[i].exibe();
    }
}