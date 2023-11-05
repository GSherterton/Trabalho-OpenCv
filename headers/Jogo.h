#pragma once
#include <opencv2/opencv.hpp>
#include "Movimento.h"
#include <iostream>
#include <fstream>
#include <string>
#include "stdlib.h"
#include "time.h"


using namespace std;
using namespace cv;

class Jogo{//tem que fazer mudancas com relacao aos blocos
    public:
        Movimento mov;
        int pontuacao;
        int highScore;
        int menu;
        int tempoSelecionado;
        int tempoBase;//tempo base para a selecao de algo
        int selecao;
        int selecaoAnterior;

        vector<Bloco> botoesMenu;
        vector<Bloco> botoesPausa;

        int deltaX;//distancia da borda
        int deltaY;//distancia minima de um bloco para o outro

        //Matrizes de imagens
        Mat chao;
        Mat terreno;
        Mat pou;
        Mat selecaoBase;
        Mat botaoJogar;
        Mat botaoPausa;
        Mat botaoContinuar;
        Mat botaoSair;
        Mat melhorPontuacao;
        Mat matPontuacao;
        Mat numero;
        Mat zero;
        Mat um;
        Mat dois;
        Mat tres;
        Mat quatro;
        Mat cinco;
        Mat seis;
        Mat sete;
        Mat oito;
        Mat nove;

        int larguraTela;
        int alturaTela;

        bool carregouJogo;
        bool comecouJogo;
        bool perdeu;

        Bloco blocoAux;

        string cascadeName;
        VideoCapture capture;//captura da camera
        Mat frame;//matriz geral
        bool tryflip;//inverter a camera
        CascadeClassifier cascade;
        double scale;

        void carregarHighScore();
        void salvarHighScore();

        void carregarBotoesMenu();
        void carregarBotoesPausa();

        Jogo();//construtor

        void gerarBlocosIniciais();

        void verificaBlocos();
        void geraBlocos();
        void desenhaBlocos(Mat& quadro);
        void desenhaPou(Mat& quadro);

        Mat matNumero(char numero);
        Mat desenhaNumero(int valor);

        Mat matBotao(Bloco botao);

        int selecionado(int tamanhoQuadrado, int posicaoX, int posicaoY, vector<Bloco> botao);
        void desenhaBotao(vector<Bloco> botao, Mat& quadro);
        void desenhaSelecao(Mat& quadro, int centroX, int centroY);

        void drawTransparency(Mat frame, Mat transp, int xPos, int yPos);
        void drawTransRect(Mat frame, Scalar color, double alpha, Rect region);
        void desenhaJogo(Mat& img, CascadeClassifier& cascade, double scale, bool tryflip);
        void desenhaMenuInicio(Mat& img, CascadeClassifier& cascade, double scale, bool tryflip);
        void desenhaMenuPausa(Mat& img, CascadeClassifier& cascade, double scale, bool tryflip);
        void conquistas();
        
        void menuInicio();
        int inicio();

};