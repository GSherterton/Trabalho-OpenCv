#pragma once
#include <vector>
#include <stdlib.h>
#include <iostream>
#include "Bloco.h"

class Movimento{
    public:
        std::vector<Bloco> bloco;
        int tamanhoX;
        int tamanhoY;

        int pontos;
        int qtdSubiu;
        int yAtual;
        int yMaximo;
        int xAtual;
        int deltaColisaoBase;
        int deltaDeslocamento;

        int velocidadeInicial;//sempre que ele pular vai iniciar com essa velocidade
        int velocidadeAtual;
        int gravidade;//"aceleracao" do pou

        bool subindo;
        bool inicio;

        Movimento();

        void deletaBloco(int index);
        void deletaBlocos();
        int deltaColisao();
        bool colisao();
        int velocidade();
        void pular();
        int movimentoY();
        void subir(int distancia);

        void mostrarBlocos();
};