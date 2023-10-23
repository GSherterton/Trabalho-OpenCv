#pragma once

class Movimento{
    public:
        int alturaAtual;
        int alturaMaxima;
        int alturaMinima;

        int velocidadeInicial;
        int velocidadeAtual;
        int gravidade;

        bool subindo;
        bool inicio;

        Movimento();

        int velocidade();
        void pular();
        int movimentoY();
};