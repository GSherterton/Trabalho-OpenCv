#pragma once
#include <iostream>
#include <vector>
#include "Bloco.h"

//todas velocidades se referem ao eixo y
//velocidade positiva é para baixo e negativa para cima
class MovimentoSimples {
    public:
        std::vector<Bloco> blocos; 

        int xPou;
        int yPou; 
        int velocidade;
        
        //métodos Pou
        MovimentoSimples();
        void modificaPosicao();
        void modificaVelocidade();
        bool verificaColisao();
};