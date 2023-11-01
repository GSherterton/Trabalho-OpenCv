#pragma once
#include <opencv2/opencv.hpp>
#include "Movimento.h"
#include <iostream>
#include "stdlib.h"
#include "time.h"

class Jogo{
    public:
        Movimento mov;

        int deltaX;//distancia da borda
        int deltaY;//distancia minima de um bloco para o outro

        Mat chao;
        Mat Terreno;
        Mat pou;

        void inicio();
        
}