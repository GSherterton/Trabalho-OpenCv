#pragma once
#include <opencv2/opencv.hpp>
#include "Movimento.h"
#include <iostream>
#include "stdlib.h"
#include "time.h"
#include "Menu.h"

using namespace std;
using namespace cv;

class Jogo{//tem que fazer mudancas com relacao aos blocos
    public:
        Movimento mov;
        Menu menu;
        int pontuacao;

        int deltaX;//distancia da borda
        int deltaY;//distancia minima de um bloco para o outro

        //Matrizes de imagens
        Mat chao;
        Mat terreno;
        Mat pou;

        bool perdeu;

        Bloco blocoAux;

        string cascadeName;
        VideoCapture capture;//captura da camera
        Mat frame;//matriz geral
        Mat quadro;//matriz geral redimensionada
        bool tryflip;//inverter a camera
        CascadeClassifier cascade;
        double scale;

        void mostrarBlocos();

        void gerarBlocosIniciais();

        Jogo();//construtor

        void verificaBlocos();
        void geraBlocos();
        void desenhaBlocos();
        void desenhaPou();

        void drawTransparency(Mat frame, Mat transp, int xPos, int yPos);
        void drawTransRect(Mat frame, Scalar color, double alpha, Rect region);
        void detectAndDraw(Mat& img, CascadeClassifier& cascade, double scale, bool tryflip);

        void jogar();
        int inicio();

};