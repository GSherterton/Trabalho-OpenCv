#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "Movimento.h"
#include <iostream>

using namespace std;
using namespace cv;

void detectAndDraw( Mat& img, CascadeClassifier& cascade, double scale, bool tryflip);

string cascadeName;

int posicaoX = 450;

Movimento mov;

int main( int argc, const char** argv ){
    VideoCapture capture;
    Mat frame;
    bool tryflip;
    CascadeClassifier cascade;
    double scale;

    cascadeName = "haarcascade_frontalface_default.xml";
    scale = 1; // usar 1, 2, 4.//mudei isso aqui
    if (scale < 1)
        scale = 1;
    tryflip = true;

    if (!cascade.load(cascadeName)) {
        cerr << "ERROR: Could not load classifier cascade: " << cascadeName << endl;
        return -1;
    }

    //if(!capture.open("video.mp4")) // para testar com um video
    if(!capture.open(0)) // para testar com a webcam
    //if(!capture.open("rtsp://10.204.238.71:8080/h264_ulaw.sdp")) // tentar conectar no celular
    {
        cout << "Capture from camera #0 didn't work" << endl;
        return 1;
    }

    if( capture.isOpened() ) {
        cout << "Video capturing has been started ..." << endl;

        while (1)
        {
            capture >> frame;
            if( frame.empty() )
                break;

            detectAndDraw( frame, cascade, scale, tryflip );

            char c = (char)waitKey(10);
            if( c == 27 || c == 'q' )
                break;

            if(c == 81){
                posicaoX -= 10;
            }
            if(c == 83){
                posicaoX += 10;
            }
        }
    }

    return 0;
}

/**
 * @brief Draws a transparent image over a frame Mat.
 * 
 * @param frame the frame where the transparent image will be drawn
 * @param transp the Mat image with transparency, read from a PNG image, with the IMREAD_UNCHANGED flag
 * @param xPos x position of the frame image where the image will start.
 * @param yPos y position of the frame image where the image will start.
 */
void drawTransparency(Mat frame, Mat transp, int xPos, int yPos) {
    Mat mask;
    vector<Mat> layers;

    split(transp, layers); // seperate channels
    Mat rgb[3] = { layers[0],layers[1],layers[2] };
    mask = layers[3]; // png's alpha channel used as mask
    merge(rgb, 3, transp);  // put together the RGB channels, now transp insn't transparent 
    transp.copyTo(frame.rowRange(yPos, yPos + transp.rows).colRange(xPos, xPos + transp.cols), mask);
}

/**
 * @brief Draws a transparent rect over a frame Mat.
 * 
 * @param frame the frame where the transparent image will be drawn
 * @param color the color of the rect
 * @param alpha transparence level. 0 is 100% transparent, 1 is opaque.
 * @param regin rect region where the should be positioned
 */
void drawTransRect(Mat frame, Scalar color, double alpha, Rect region) {
    Mat roi = frame(region);
    Mat rectImg(roi.size(), CV_8UC3, color); 
    addWeighted(rectImg, alpha, roi, 1.0 - alpha , 0, roi); 
}

void detectAndDraw( Mat& img, CascadeClassifier& cascade, double scale, bool tryflip)
{
    Bloco blocoAux;
    double t = 0;
    vector<Rect> faces;
    Mat gray, smallImg;
    Scalar color = Scalar(255,0,0);

    double fx = 1 / scale;
    resize( img, smallImg, Size(), fx, fx, INTER_LINEAR_EXACT );
    if( tryflip )
        flip(smallImg, smallImg, 1);
    cvtColor( smallImg, gray, COLOR_BGR2GRAY );
    equalizeHist( gray, gray );

    t = (double)getTickCount();

    cascade.detectMultiScale( gray, faces,
        1.3, 2, 0
        //|CASCADE_FIND_BIGGEST_OBJECT
        //|CASCADE_DO_ROUGH_SEARCH
        |CASCADE_SCALE_IMAGE,
        Size(40, 40) );
    t = (double)getTickCount() - t;
    //printf( "detection time = %g ms\n", t*1000/getTickFrequency());
    // PERCORRE AS FACES ENCONTRADAS
    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Rect r = faces[i];

        Mat pou2 = cv::imread("imagens/pou.png", IMREAD_UNCHANGED);
        drawTransparency(smallImg, pou2, r.x + r.width/2 - pou2.rows/2, r.y + r.height/2 -pou2.cols/2);

        rectangle( smallImg, Point(cvRound(r.x), cvRound(r.y)),
                    Point(cvRound((r.x + r.width-1)), cvRound((r.y + r.height-1))),
                    color, 3);
    }
    
    // Desenha uma imagem
    Mat chao = cv::imread("imagens/chao.png", IMREAD_UNCHANGED);
    Mat terreno = cv::imread("imagens/plataformaTerra.png", IMREAD_UNCHANGED);
    Mat pou = cv::imread("imagens/pou.png", IMREAD_UNCHANGED);
    //Mat orange = cv::imread("imagens/orange.png", IMREAD_UNCHANGED);

    if(mov.inicio){
    	//mov.alturaAtual = (smallImg.rows - pou.rows);
        mov.tamanhoX = pou.cols;
        mov.tamanhoY = pou.rows;
        mov.yMaximo = (smallImg.rows - ((smallImg.rows*3)/5) + mov.tamanhoY);
        mov.yMinimo = smallImg.rows;
        mov.yAtual = mov.yMaximo;
        mov.gravidade = 2;
        mov.velocidadeInicial = -18;
        mov.inicio = false;

        mov.deltaColisao = 5;
    
        blocoAux = Bloco(smallImg.cols, 0, (smallImg.rows-(smallImg.rows/20)));
        (mov.blocos).push_back(blocoAux);//colocando o chão inicial, lembrar depois de tirar ele
        //cout << "Coloquei o chao\n";
    }

    //drawTransparency(smallImg, terreno, posicaoX, mov.yMaximo);//desenha um terreno
    drawTransparency(smallImg, chao, 0, mov.yMaximo);//desenha o chao
    drawTransparency(smallImg, pou, posicaoX, 0);//desenhando o pou
    //drawTransparency(smallImg, orange, posicaoX, mov.movimentoY());//desenhando a laranja
    //printf("pou::width: %d, height=%d\n", pou.cols, pou.rows);

    // Desenha quadrados com transparencia
    double alpha = 0.3;
    drawTransRect(smallImg, Scalar(0,255,0), alpha, Rect(  0, 0, 200, 200));
    drawTransRect(smallImg, Scalar(255,0,0), alpha, Rect(200, 0, 200, 200));

    // Desenha um texto
    color = Scalar(0,0,255);
    putText	(smallImg, "Placar:", Point(300, 50), FONT_HERSHEY_PLAIN, 2, color); // fonte

    // Desenha o frame na tela
    imshow("result", smallImg );
    //printf("image::width: %d, height=%d\n", smallImg.cols, smallImg.rows );


    /*if(subindo){
    	posicaoY -= 5;
    }else{
    	posicaoY += 5;
    }
    if(posicaoY >= smallImg.rows - 100){
    	subindo = false;
    }else if(posicaoY >= smallImg.rows){
    	subindo = true;
    }*/
    
       
    //mecanismo de mexer a laranja de um lado para o outro 
    /*if(indo){
    	posicaoX+=5;
    }else{
    	posicaoX-=5;
    }
    
    if(posicaoX >= smallImg.cols - orange.cols){
    	indo = false;
    }else if(posicaoX <= 0){
    	indo = true;
    }*/
    
}
