#pragma once
#include <opencv2/opencv.hpp>
#include "Movimento.h"
#include <iostream>
#include "stdlib.h"
#include "time.h"

using namespace std;
using namespace cv;

class Jogo{//tem que fazer mudancas com relacao aos blocos
    public:
        Movimento mov;
        int pontuacao;
        int menu;
        int tempoSelecionado;

        int deltaX;//distancia da borda
        int deltaY;//distancia minima de um bloco para o outro

        //Matrizes de imagens
        Mat chao;
        Mat terreno;
        Mat pou;
        Mat selecaoBase;

        int larguraTela;
        int alturaTela;

        bool comecouJogo;
        bool perdeu;

        Bloco blocoAux;

        string cascadeName;
        VideoCapture capture;//captura da camera
        Mat frame;//matriz geral
        bool tryflip;//inverter a camera
        CascadeClassifier cascade;
        double scale;

        void gerarBlocosIniciais();

        Jogo();//construtor

        void verificaBlocos();
        void geraBlocos();
        void desenhaBlocos(Mat& quadro);
        void desenhaPou(Mat& quadro);
        void desenhaSelecao(Mat& quadro, int tempo, int centroX, int centroY);

        void drawTransparency(Mat frame, Mat transp, int xPos, int yPos);
        void drawTransRect(Mat frame, Scalar color, double alpha, Rect region);
        void desenhaJogo(Mat& img, CascadeClassifier& cascade, double scale, bool tryflip);
        int desenhaMenuInicio(Mat& img, CascadeClassifier& cascade, double scale, bool tryflip);
        void conquistas();
        
        void menuInicio();
        int inicio();

};