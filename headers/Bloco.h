#pragma once
#include <iostream>

class Bloco{
    public:
        int tamanhoX;
        int tamanhoY;
        int posicaoX;
        int posicaoY;

        Bloco();
        Bloco(int tamanhoX, int tamanhoY, int posicaoX, int posicaoY);
        
        void exibe();//fazer um exibe para bloco
};