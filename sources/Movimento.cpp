#include "Movimento.h"

using namespace std;

Movimento::Movimento(){
    //velocidadeInicial = -10;
    inicio = true;
    velocidadeAtual = 0;
}

void Movimento::deletaBloco(int index){
    blocos.erase(blocos.begin() + index);
}

int Movimento::deltaColisao(){
    if(velocidadeAtual > 10){
        return deltaColisaoBase*(1. + (((float)velocidadeAtual-10)/20.));
    }

    return deltaColisaoBase;
}

bool Movimento::colisao(){
    for(int i = 0; i < blocos.size(); i++){//vai ver se ha colisao com todos os blocos
        if(abs(yAtual - blocos[i].posicaoY) < 100){
            cout << "distancia na altura para o bloco " << i << ": " << abs(yAtual - blocos[i].posicaoY) << endl;
        }
        if((abs(yAtual - blocos[i].posicaoY) <= deltaColisao()) && ((xAtual >= blocos[i].posicaoX) && xAtual <= blocos[i].posicaoX + blocos[i].tamanhoX)){
            cout << "Foi detectada colisao\n";
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
        subir(-velocidadeAtual);
    }else{
        yAtual += velocidadeAtual;
        //cout << "Velocidade Atual: " << velocidadeAtual << endl;
    }
    
    velocidade();

    return yAtual;
}

void Movimento::subir(int distancia){
    for(int i = 0; i < blocos.size(); i++){
        blocos[i].posicaoY += distancia;
    }
}

void Movimento::mostrarBlocos(){
    if(blocos.size() == 0){
        cout << "Nao ha nenhum bloco\n";
    }

    for(int i = 0; i < blocos.size(); i++){
        blocos[i].exibe();
    }
}