#include "Jogo.h"

using namespace std;
using namespace cv;

void Jogo::mostrarBlocos(){
    if((mov.bloco).size() == 0){
        cout << "Nao ha nenhum bloco\n";
    }

    for(int i = 0; i < (mov.bloco).size(); i++){
        (mov.bloco[i]).exibe();
    }
}

void Jogo::gerarBlocosIniciais(){
    int cria;
    int posicaoX;

    //Colocando o chao
    blocoAux = Bloco(chao.cols, chao.rows, deltaX, (quadro.rows-chao.rows));
    (mov.bloco).push_back(blocoAux);

    //Colocando os blocos iniciais
    for(cria = (quadro.rows - chao.rows - deltaY); cria >= -deltaY; cria--){
        if(rand() % 2){
            posicaoX = rand() % (chao.cols-terreno.cols) + deltaX;

            blocoAux = Bloco(terreno.cols, terreno.rows, posicaoX, cria);
            (mov.bloco).push_back(blocoAux);

            cria -= deltaY;
        }
    } 
}

Jogo::Jogo(){
    Mat chao = cv::imread("imagens/chao.png", IMREAD_UNCHANGED);
    Mat terreno = cv::imread("imagens/plataformaTerra.png", IMREAD_UNCHANGED);
    Mat pou = cv::imread("imagens/pou.png", IMREAD_UNCHANGED);

    perdeu = 0;

    cascadeName = "haarcascade_frontalface_default.xml";
    scale = 1.5;
    tryflip = true;

    mov.tamanhoX = pou.cols;//largura do pou
    mov.tamanhoY = pou.rows;//altura do pou

    mov.yMaximo = quadro.rows/2;//altura maxima que o pou pode chegar
    mov.yAtual = mov.yMaximo;
    mov.xAtual = quadro.cols/2;

    mov.gravidade = 2;
    mov.velocidadeInicial = -24;//negativa devido a orientacao dos eixos

    mov.deltaColisaoBase = 12;//delta para detectar a colisao
    deltaX = ((quadro.cols/2) - (chao.cols/2));
    deltaY = ((quadro.rows/7));

    gerarBlocosIniciais();
}

void Jogo::verificaBlocos(){//mudar a classe blocos e movimento
    if((mov.bloco[0]).posicaoY > quadro.rows){
        mov.deletaBloco(0);
    }
}

void Jogo::geraBlocos(){
    if(mov.qtdSubiu > deltaY){
        if(rand() % 2){
            int posicaoX;

            posicaoX = rand() % (chao.cols-terreno.cols) + deltaX;

            blocoAux = Bloco(terreno.cols, terreno.rows, posicaoX, (mov.bloco[(mov.bloco).size()-1]).posicaoY - mov.qtdSubiu);
            (mov.bloco).push_back(blocoAux);

            mov.qtdSubiu = 0;
        }
    }
}

void Jogo::desenhaBlocos(){
    if((mov.bloco).size() == 0){
        cout << "Nao ha blocos!\n";
    }

    Rect crop;
    Mat exemp;

    for(int i = 0; i < (mov.bloco).size(); i++){
        if(((mov.bloco)[i].posicaoY >= 0)){
            if(((mov.bloco[i]).posicaoY <= (quadro.rows-(mov.bloco[i]).tamanhoY))){
                if((mov.bloco[i]).tamanhoX > 200){
                    drawTransparency(quadro, chao, (mov.bloco[i]).posicaoX, (mov.bloco[i]).posicaoY);//desenha o chao
                }else{
                    drawTransparency(quadro, terreno, (mov.bloco[i]).posicaoX, (mov.bloco[i]).posicaoY);//desenha um terreno
                }
            }else if((mov.bloco[i]).posicaoY < quadro.rows){//recortar o bloco que esta quase descendo
                crop = Rect(0, 0, (mov.bloco[i]).tamanhoX, quadro.rows - (mov.bloco[i]).posicaoY);
                
                if((mov.bloco[i]).tamanhoX > 200){    
                    exemp = chao(crop);
                    drawTransparency(quadro, exemp, (mov.bloco[i]).posicaoX, (mov.bloco[i]).posicaoY);//desenha o chao
                }else{
                    exemp = terreno(crop);
                    drawTransparency(quadro, exemp, (mov.bloco[i]).posicaoX, (mov.bloco[i]).posicaoY);//desenha um terreno
                }
            }
        }
    }
}

void Jogo::desenhaPou(){
    if(mov.movimentoY() > quadro.rows){
        perdeu = 1;
        return;//perdeu
    }else{
        drawTransparency(quadro, pou, (mov.xAtual - (pou.cols/2)), (mov.yAtual-pou.rows));//desenhando o pou
    }
}

void Jogo::drawTransparency(Mat frame, Mat transp, int xPos, int yPos){
    Mat mask;
    vector<Mat> layers;

    split(transp, layers); // seperate channels
    Mat rgb[3] = { layers[0],layers[1],layers[2] };
    mask = layers[3]; // png's alpha channel used as mask
    merge(rgb, 3, transp);  // put together the RGB channels, now transp insn't transparent 
    transp.copyTo(frame.rowRange(yPos, yPos + transp.rows).colRange(xPos, xPos + transp.cols), mask);
}

void Jogo::drawTransRect(Mat frame, Scalar color, double alpha, Rect region){
    Mat roi = frame(region);
    Mat rectImg(roi.size(), CV_8UC3, color); 
    addWeighted(rectImg, alpha, roi, 1.0 - alpha , 0, roi); 
}

void Jogo::detectAndDraw(Mat& img, CascadeClassifier& cascade, double scale, bool tryflip){
    double t = 0;
    vector<Rect> faces;
    Mat gray;
    Scalar color = Scalar(255, 0, 0);

    double fx = 1 / scale;
    resize(img, quadro, Size(), fx, fx, INTER_LINEAR_EXACT );
    if(tryflip)
        flip(quadro, quadro, 1);
    cvtColor(quadro, gray, COLOR_BGR2GRAY );
    equalizeHist(gray, gray);

    t = (double)getTickCount();
    cascade.detectMultiScale(gray, faces, 1.3, 5);
    t = (double)getTickCount() - t;

    for(size_t i = 0; i < faces.size(); i++){
        Rect r = faces[i];

        if(((r.x + (r.width/2)) >= (deltaX + mov.tamanhoX)) && ((r.x + r.width/2) <= (quadro.cols - deltaX - mov.tamanhoX))){
            mov.xAtual = (r.x + r.width/2);
        }

        rectangle(quadro, Point(cvRound(r.x), cvRound(r.y)), Point(cvRound((r.x + r.width-1)), cvRound((r.y + r.height-1))), color, 3);
    }

    verificaBlocos();
    geraBlocos();
    desenhaBlocos();
    desenhaPou();
    if(perdeu){
        perdeu = 0;
        return;
    }

    imshow("result", quadro);
}

void Jogo::jogar(){
    pontuacao = 0;

    while(1){
        capture >> frame;
        if(frame.empty()) break;

        detectAndDraw(frame, cascade, scale, tryflip);

        //teclas
        char c = (char)waitKey(1);
        if(c == 27 || c == 'q'){
            break;
        }
    }
}

int Jogo::inicio(){
    srand(time(NULL));

    if(!cascade.load(cascadeName)) {
        cerr << "ERROR: Could not load classifier cascade: " << cascadeName << endl;
        return -1;
    }

    if(!capture.open(0)){ // para testar com a webcam
        cout << "Capture from camera #0 didn't work" << endl;
        return 1;
    }
    if(capture.isOpened()){
        cout << "Video capturing has been started ..." << endl;

        while(1){
            capture >> frame;
            if(frame.empty()){
                break;
            }

            switch(1){//menu.inicio()){
                case 1://jogar o jogo
                    jogar();
                    break;
                case 2://ver conquistas
                    //conquistas();
                    break;
                case 3://sair do jogo
                    return 0;
            }
        }
    }
}