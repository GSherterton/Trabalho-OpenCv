#include <opencv2/opencv.hpp>
#include "Movimento.h"
#include <iostream>
#include "stdlib.h"
#include "time.h"

using namespace std;
using namespace cv;

void detectAndDraw( Mat& img, CascadeClassifier& cascade, double scale, bool tryflip);

string cascadeName;

Movimento mov;

int deltaX;
int deltaY;
int pontuacao;

//leitura das imagens
Mat chao = cv::imread("imagens/chao.png", IMREAD_UNCHANGED);
Mat terreno = cv::imread("imagens/plataformaTerra.png", IMREAD_UNCHANGED);
Mat pou = cv::imread("imagens/pou.png", IMREAD_UNCHANGED);

int main( int argc, const char** argv ){
    //iniciando uma seed aleatoria
    srand(time(NULL));

    VideoCapture capture;
    Mat frame;
    bool tryflip;
    CascadeClassifier cascade;
    double scale;

    cascadeName = "haarcascade_frontalface_default.xml";
    scale = 1.5; // usar 1, 2, 4.//mudei isso aqui
    tryflip = true;

    if (!cascade.load(cascadeName)) {
        cerr << "ERROR: Could not load classifier cascade: " << cascadeName << endl;
        return -1;
    }

    if(!capture.open("video.mp4")) // para testar com um video
    //if(!capture.open(0)) // para testar com a webcam
    //if(!capture.open("rtsp://10.204.238.71:8080/h264_ulaw.sdp")) // tentar conectar no celular
    {
        cout << "Capture from camera #0 didn't work" << endl;
        return 1;
    }

    if(capture.isOpened()){
        cout << "Video capturing has been started ..." << endl;

        while(1){
            capture >> frame;
            if(frame.empty()) break;

            detectAndDraw(frame, cascade, scale, tryflip);

            // detecta saida do programa
            char c = (char)waitKey(1);
            if(c == 27 || c == 'q'){
                break;
            }
            if(c == 'w'){//subir a tela
                mov.subir(10);
            }
            if(c == 'h'){//andar muito para a esquerda
                mov.xAtual -= 50;
            }
            if(c == 'l'){//andar muito para a direita
                mov.xAtual += 50;
            }
            if(c == 'j'){//andar um pouco para a esquerda
                mov.xAtual -= 30;
            }
            if(c == 'k'){//andar um pouco para a direita
                mov.xAtual += 30;
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
void drawTransparency(Mat frame, Mat transp, int xPos, int yPos){
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
void drawTransRect(Mat frame, Scalar color, double alpha, Rect region){
    Mat roi = frame(region);
    Mat rectImg(roi.size(), CV_8UC3, color); 
    addWeighted(rectImg, alpha, roi, 1.0 - alpha , 0, roi); 
}

void detectAndDraw(Mat& img, CascadeClassifier& cascade, double scale, bool tryflip){
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
    cascade.detectMultiScale(gray, faces, 1.3, 5);
    t = (double)getTickCount() - t;

    for(size_t i = 0; i < faces.size(); i++){
        Rect r = faces[i];

        if(((r.x + (r.width/2)) >= (deltaX + mov.tamanhoX)) && ((r.x + r.width/2) <= (smallImg.cols - deltaX - mov.tamanhoX))){
            //mov.xAtual = (r.x + r.width/2);//fazendo a movimentacao manual devido a falta de camera
        }

        rectangle(smallImg, Point(cvRound(r.x), cvRound(r.y)), Point(cvRound((r.x + r.width-1)), cvRound((r.y + r.height-1))), color, 3);
    }
    

    if(mov.inicio){
        int cria;
        int posicaoX;

    	//mov.alturaAtual = (smallImg.rows - pou.rows);
        mov.tamanhoX = pou.cols;
        mov.tamanhoY = pou.rows;
        
        mov.tamanhoY = pou.rows;

        mov.yMaximo = smallImg.rows/2;
        mov.yMinimo = smallImg.rows;
        mov.yAtual = mov.yMaximo;
        mov.xAtual = smallImg.cols/2;

        mov.gravidade = 2;
        mov.velocidadeInicial = -24;
        mov.inicio = false;

        mov.deltaColisaoBase = 12;
        deltaX = ((smallImg.cols/2) - (chao.cols/2));//esse delta é para a borda
        deltaY = ((smallImg.rows/7));
        pontuacao = (0 - mov.yMaximo);//colocar para que se a pontuacao ficar menor que zero na hora de exibir, colocar 0
    
        //Colocando o chao
        blocoAux = Bloco(chao.cols, chao.rows, deltaX, (smallImg.rows-chao.rows));
        (mov.blocos).push_back(blocoAux);//colocando o chão inicial, lembrar depois de tirar ele

        //Colocando os blocos iniciais
        for(cria = (smallImg.rows - chao.rows - deltaY); cria >= -deltaY; cria--){//colocando parametros manuais | mudar dps
            if(rand() % 2){
                posicaoX = rand() % (chao.cols-terreno.cols) + deltaX;

                blocoAux = Bloco(terreno.cols, terreno.rows, posicaoX, cria);
                (mov.blocos).push_back(blocoAux);

                cria -= deltaY;
            }
        }

        /*//Colocando um terreno
        posicaoX = rand() % (chao.cols-terreno.cols) + delta;

        blocoAux = Bloco(terreno.cols, posicaoX, (mov.blocos[0]).posicaoY-(smallImg.rows/8));
        (mov.blocos).push_back(blocoAux);//colocando um terreno de teste*/
    }

    //cout << "x atual: " << mov.xAtual << endl;
    //mov.mostrarBlocos();

    //drawTransparency(smallImg, terreno, 200, mov.yMaximo);//desenha um terreno
    //drawTransparency(smallImg, chao, 0, (smallImg.rows-chao.rows));//desenha o chao

    //verifica se algum bloco ja desceu
    if((mov.blocos[0]).posicaoY > smallImg.rows){
        mov.deletaBloco(0);
        cout << "Bloco deletado\n";
    }

    //gerando blocos a medida que sobe
    if(mov.qtdSubiu > deltaY){
        if(rand() % 2){
            int posicaoX;

            posicaoX = rand() % (chao.cols-terreno.cols) + deltaX;

            blocoAux = Bloco(terreno.cols, terreno.rows, posicaoX, (mov.blocos[(mov.blocos).size()-1]).posicaoY - mov.qtdSubiu);
            (mov.blocos).push_back(blocoAux);

            mov.qtdSubiu = 0;
            cout << "Gerou bloco\n";
        }
    }

    //Desenha os blocos
    if((mov.blocos).size() == 0){
        cout << "Nao ha blocos!\n";
    }

    Rect crop;
    Mat exemp;

    for(int i = 0; i < (mov.blocos).size(); i++){
        if(((mov.blocos[i]).posicaoY >= 0)){
            if(((mov.blocos[i]).posicaoY <= (smallImg.rows-(mov.blocos[i]).tamanhoY))){//colocando um if primeiro para nao desenhar um bloco que esteja acima da tela
                if((mov.blocos[i]).tamanhoX > 200){
                    drawTransparency(smallImg, chao, (mov.blocos[i]).posicaoX, (mov.blocos[i]).posicaoY);//desenha o chao
                }else{
                    drawTransparency(smallImg, terreno, (mov.blocos[i]).posicaoX, (mov.blocos[i]).posicaoY);//desenha um terreno
                }
            }else if((mov.blocos[i]).posicaoY < smallImg.rows){//recortar o bloco que esta quase descendo
                cout << "Entrei aqui no i = " << i << endl;
                crop = Rect(0, 0, (mov.blocos[i]).tamanhoX, smallImg.rows - (mov.blocos[i]).posicaoY);
                
                if((mov.blocos[i]).tamanhoX > 200){    
                    exemp = chao(crop);
                    drawTransparency(smallImg, exemp, (mov.blocos[i]).posicaoX, (mov.blocos[i]).posicaoY);//desenha o chao
                }else{
                    exemp = terreno(crop);
                    drawTransparency(smallImg, exemp, (mov.blocos[i]).posicaoX, (mov.blocos[i]).posicaoY);//desenha um terreno
                }
            }
        }
    }

    
    //Desenha o pou
    cout << "Altura atual: " << mov.yAtual << endl;
    if(mov.movimentoY() > smallImg.rows){
        cout << "Perdeu!\n";
        return;//perdeu
    }else{
        drawTransparency(smallImg, pou, (mov.xAtual - (pou.cols/2)), (mov.yAtual-pou.rows));//desenhando o pou
    }
    
    
    // Desenha um texto
    //color = Scalar(0,0,255);
    //putText	(smallImg, "Placar:", Point(300, 50), FONT_HERSHEY_PLAIN, 2, color); // fonte

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
