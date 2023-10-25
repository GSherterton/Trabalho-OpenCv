#pragma once
#include <iostream>

class Bloco{
    public:
        int tamanho;
        int posicaoX;
        int posicaoY;

        Bloco();
        Bloco(int tamanho, int posicaoX, int posicaoY);
        
        void exibe();//fazer um exibe para bloco
};