#pragma once
#include <vector>
#include <stdlib.h>
#include <iostream>
#include "Bloco.h"

//aviso com relaca ao delta da colisao, acredito que de um pouco de
//erro se o pou estiver um pouco em cima e acabar passando pelo bloco devido a velocidade alta

class Movimento{
    public:
        std::vector<Bloco> blocos;

        int tamanhoX;
        int tamanhoY;

        int yAtual;
        int yMaximo;//com relacao a yAtual
        int yMinimo;//depois vou tirar isso para colocar inicialmente o chao como o minimo no inicio
        int xAtual;
        int deltaColisao;
        int deltaDeslocamento;

        int velocidadeInicial;//sempre que ele pular vai iniciar com essa velocidade
        int velocidadeAtual;
        int gravidade;//"aceleracao" do pou

        bool subindo;
        bool inicio;

        Movimento();

        bool colisao();
        int velocidade();
        void pular();
        int movimentoY();

        void mostrarBlocos();
};