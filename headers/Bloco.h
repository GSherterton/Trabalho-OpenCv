#pragma once
#include <iostream>
#include <string>

class Bloco{
    public:
        int tamanhoX;
        int tamanhoY;
        int posicaoX;
        int posicaoY;
        std::string nome;

        Bloco();
        Bloco(int tamanhoX, int tamanhoY, int posicaoX, int posicaoY);
        Bloco(int tamanhoX, int tamanhoY, int posicaoX, int posicaoY, std::string nome);
        
        void exibe();
};