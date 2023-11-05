#include "Jogo.h"

using namespace std;
using namespace cv;

void Jogo::carregarBotoesMenu(){
    blocoAux = Bloco(botaoJogar.cols, botaoJogar.rows, (larguraTela/2) - (botaoJogar.cols/2), (alturaTela)/2, "jogar");
    botoesMenu.push_back(blocoAux);//primeiro botao eh o de jogar
    blocoAux = Bloco(botaoSair.cols, botaoSair.rows, (larguraTela/2) - (botaoSair.cols/2), (alturaTela*4)/6, "sair");
    botoesMenu.push_back(blocoAux);//segundo eh o de sair
}

void Jogo::carregarBotoesPausa(){//vai ficar para futuras edicoes
    //necessito das imagens
}

void Jogo::carregarHighScore(){
    ifstream fp;

    fp.open("highscore.txt");

    if(!fp.is_open()){
        cout << "Nao foi possivel abrir o arquivo\n";
        return;
    }

    while(1){
        fp >> highScore;

        if(fp.eof()){
            break;
        }
    }

    fp.close();

    cout << "High Score carregado com sucesso!\n";
}

void Jogo::salvarHighScore(){
    ofstream fp;

    fp.open("highscore.txt");

    if(!fp.is_open()){
        cout << "Nao foi possivel abrir o arquivo\n";
        return;
    }

    fp << highScore << endl;

    cout << "High Score salvo com sucesso\n";
}

Jogo::Jogo(){
    chao = imread("imagens/chao.png", IMREAD_UNCHANGED);
    terreno = imread("imagens/plataformaTerra.png", IMREAD_UNCHANGED);
    pou = imread("imagens/pou.png", IMREAD_UNCHANGED);
    selecaoBase = imread("imagens/selecaoMouse1.png", IMREAD_UNCHANGED);
    botaoJogar = imread("imagens/botaoJogar.png", IMREAD_UNCHANGED);
    botaoPausa = imread("imagens/botaoPausa.png", IMREAD_UNCHANGED);
    botaoContinuar = imread("imagens/botaoContinuar.png", IMREAD_UNCHANGED);
    botaoSair = imread("imagens/botaoSair.png", IMREAD_UNCHANGED);
    melhorPontuacao = imread("imagens/melhorPontuacao.png", IMREAD_UNCHANGED);

    perdeu = 0;
    menu = 1;
    carregouJogo = 0;
    comecouJogo = 0;
    tempoSelecionado = 0;
    tempoBase = 3;
    selecaoAnterior = 0;

    cascadeName = "haarcascade_frontalface_default.xml";
    scale = 1.5;
    tryflip = true;

    mov.tamanhoX = pou.cols;//largura do pou
    mov.tamanhoY = pou.rows;//altura do pou

    mov.gravidade = 2;
    mov.velocidadeInicial = -24;//negativa devido a orientacao dos eixos

    mov.deltaColisaoBase = 12;//delta para detectar a colisao
    carregarHighScore();
}

void Jogo::gerarBlocosIniciais(){
    int cria;
    int posicaoX;

    //Colocando o chao
    blocoAux = Bloco(chao.cols, chao.rows, deltaX, (alturaTela-chao.rows));
    (mov.bloco).push_back(blocoAux);

    //Colocando os blocos iniciais
    for(cria = (alturaTela - chao.rows - deltaY); cria >= -deltaY; cria--){
        if(rand() % 2){
            posicaoX = rand() % (chao.cols-terreno.cols) + deltaX;

            blocoAux = Bloco(terreno.cols, terreno.rows, posicaoX, cria);
            
            (mov.bloco).push_back(blocoAux);

            cria -= deltaY;
        }
    }
    //mov.mostrarBlocos();
}

void Jogo::verificaBlocos(){//mudar a classe blocos e movimento
    if((mov.bloco[0]).posicaoY > alturaTela){
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

void Jogo::desenhaBlocos(Mat& quadro){
    if((mov.bloco).size() == 0){
        cout << "Nao ha blocos!\n";
    }

    Rect crop;
    Mat exemp;

    for(int i = 0; i < (mov.bloco).size(); i++){
        if(((mov.bloco[i]).posicaoY >= 0)){
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

void Jogo::desenhaPou(Mat& quadro){
    if(mov.movimentoY() > quadro.rows){
        perdeu = 1;
    }else{
        drawTransparency(quadro, pou, (mov.xAtual - (pou.cols/2)), (mov.yAtual-pou.rows));//desenhando o pou
    }
    pontuacao = mov.pontos;
}

int Jogo::selecionado(int tamanhoQuadrado, int posicaoX, int posicaoY, vector<Bloco> botao){
    Rect aux, recSelecao = Rect(posicaoX, posicaoY, tamanhoQuadrado, tamanhoQuadrado);

    for(int i = 0; i < botao.size(); i++){
        aux = Rect(botao[i].posicaoX, botao[i].posicaoY, botao[i].tamanhoX, botao[i].tamanhoY);
        //cout << "Intersecao das areas: " << (aux & recSelecao).area() << endl;
        if((aux & recSelecao).area() >= 100){
            return i+1;
        }
    }

    return 0;
}

Mat Jogo::matBotao(Bloco botao){//fazer a correspondencia de todos os blocos/botoes
    if((botao.nome).compare("jogar") == 0){
        return botaoJogar;
    }else if((botao.nome).compare("sair") == 0){
        return botaoSair;
    }
}

void Jogo::desenhaBotao(vector<Bloco> botao, Mat& quadro){
    for(int i = 0; i < botao.size(); i++){
        drawTransparency(quadro, matBotao(botao[i]), botao[i].posicaoX, botao[i].posicaoY);
    }
}

void Jogo::desenhaSelecao(Mat& quadro, int centroX, int centroY){
    if(tempoSelecionado == 0*tempoBase){
        selecaoBase = imread("imagens/selecaoMouse1.png", IMREAD_UNCHANGED);
    }else if(tempoSelecionado == 1*tempoBase){
        selecaoBase = imread("imagens/selecaoMouse2.png", IMREAD_UNCHANGED);
    }else if(tempoSelecionado == 2*tempoBase){
        selecaoBase = imread("imagens/selecaoMouse3.png", IMREAD_UNCHANGED);
    }else if(tempoSelecionado == 3*tempoBase){
        selecaoBase = imread("imagens/selecaoMouse4.png", IMREAD_UNCHANGED);
    }else if(tempoSelecionado == 4*tempoBase){
        selecaoBase = imread("imagens/selecaoMouse5.png", IMREAD_UNCHANGED);
    }else if(tempoSelecionado == 5*tempoBase){
        selecaoBase = imread("imagens/selecaoMouse6.png", IMREAD_UNCHANGED);
    }else if(tempoSelecionado == 6*tempoBase){
        selecaoBase = imread("imagens/selecaoMouse7.png", IMREAD_UNCHANGED);
    }else if(tempoSelecionado == 7*tempoBase){
        selecaoBase = imread("imagens/selecaoMouse8.png", IMREAD_UNCHANGED);
    }else if(tempoSelecionado == 8*tempoBase){
        selecaoBase = imread("imagens/selecaoMouse9.png", IMREAD_UNCHANGED);
    }else if(tempoSelecionado == 9*tempoBase){
        selecaoBase = imread("imagens/selecaoMouse10.png", IMREAD_UNCHANGED);
    }else if(tempoSelecionado == 10*tempoBase){
        selecaoBase = imread("imagens/selecaoMouse11.png", IMREAD_UNCHANGED);
    }else if(tempoSelecionado == 11*tempoBase){
        selecaoBase = imread("imagens/selecaoMouse12.png", IMREAD_UNCHANGED);
    }else if(tempoSelecionado == 12*tempoBase){
        selecaoBase = imread("imagens/selecaoMouse13.png", IMREAD_UNCHANGED);
    }else if(tempoSelecionado == 13*tempoBase){
        selecaoBase = imread("imagens/selecaoMouse14.png", IMREAD_UNCHANGED);
    }else if(tempoSelecionado == 14*tempoBase){
        selecaoBase = imread("imagens/selecaoMouse15.png", IMREAD_UNCHANGED);
    }else if(tempoSelecionado == 15*tempoBase){
        selecaoBase = imread("imagens/selecaoMouse16.png", IMREAD_UNCHANGED);
    }else if(tempoSelecionado == 16*tempoBase){
        selecaoBase = imread("imagens/selecaoMouse17.png", IMREAD_UNCHANGED);
        tempoSelecionado = 0;
        menu = selecao+1;

        drawTransparency(quadro, selecaoBase, centroX - (selecaoBase.cols/2) + 1, centroY - (selecaoBase.rows/2) + 1);

        return;
    }

    //ver se o mouse ta batendo em algum botao e em qual
    int auxTamanho = (selecaoBase.rows*7)/10;
    selecao = selecionado(auxTamanho, (centroX - (auxTamanho/2) + 1), (centroY - (auxTamanho/2) + 1), botoesMenu);

    if(selecao && (selecao == selecaoAnterior)){
        tempoSelecionado++;
    }else{
        selecaoAnterior = selecao;
        tempoSelecionado = 0;
    }

    drawTransparency(quadro, selecaoBase, centroX - (selecaoBase.cols/2) + 1, centroY - (selecaoBase.rows/2) + 1);
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

void Jogo::desenhaJogo(Mat& img, CascadeClassifier& cascade, double scale, bool tryflip){
    double t = 0;
    vector<Rect> faces;
    Mat gray, quadro;
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

        rectangle(quadro, Point(cvRound(r.x + (r.width/2) - (r.width/8)), cvRound(r.y + (r.height/2) - (r.height/12))), Point(cvRound(r.x + (r.width/2) + (r.width/8)), cvRound(r.y + (r.height/2) + (r.height/6))), color, 3);
        //rectangle(quadro, Point(cvRound(r.x), cvRound(r.y)), Point(cvRound((r.x + r.width-1)), cvRound((r.y + r.height-1))), color, 3);
    }

    //desenha o botao de pausa
    drawTransparency(quadro, botaoPausa, larguraTela - botaoPausa.cols - larguraTela/10, alturaTela/20);

    verificaBlocos();
    geraBlocos();
    desenhaBlocos(quadro);
    desenhaPou(quadro);
    cout << "Pontuacao: " << pontuacao << endl;
    if(perdeu){
        system("mplayer -msglevel all=-1 audios/roblox-death-sound-effect.mp3 &");
        comecouJogo = 0;
        menu = 1;
        perdeu = 0;
        mov.deletaBlocos();
        mov.qtdSubiu = 0;

        if(pontuacao > highScore){
            highScore = pontuacao/10;
        }

        return;
    }

    imshow("result", quadro);
}

int Jogo::desenhaMenuInicio(Mat& img, CascadeClassifier& cascade, double scale, bool tryflip){
    double t = 0;
    vector<Rect> faces;
    Mat gray, quadro;
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

    if(!carregouJogo){
        larguraTela = quadro.cols;
        alturaTela = quadro.rows;
        mov.yMaximo = quadro.rows/2;//altura maxima que o pou pode chegar
        mov.xAtual = quadro.cols/2;

        deltaX = ((quadro.cols/2) - (chao.cols/2));
        deltaY = ((quadro.rows/7));

        carregouJogo = 1;

        carregarBotoesMenu();
        carregarBotoesPausa();

        //cout << "Jogo carregado\n";
    }
    

    desenhaBotao(botoesMenu, quadro);

    for(size_t i = 0; i < faces.size(); i++){
        Rect r = faces[i];

        rectangle(quadro, Point(cvRound(r.x + (r.width/2) - (r.width/8)), cvRound(r.y + (r.height/2) - (r.height/12))), Point(cvRound(r.x + (r.width/2) + (r.width/8)), cvRound(r.y + (r.height/2) + (r.height/6))), color, 3);
        //rectangle(quadro, Point(cvRound(r.x), cvRound(r.y)), Point(cvRound((r.x + r.width-1)), cvRound((r.y + r.height-1))), color, 3);

        if(((r.x + (r.width/2)) >= (deltaX + mov.tamanhoX)) && ((r.x + r.width/2) <= (quadro.cols - deltaX - mov.tamanhoX))){
            mov.xAtual = (r.x + r.width/2);
            desenhaSelecao(quadro, (r.x + (r.width/2) - 1), (r.y + (r.height/2) - 1));
        }

    }

    imshow("result", quadro);

    return 1;
}

void Jogo::conquistas(){}

void Jogo::menuInicio(){
    while(1){
        capture >> frame;
        if(frame.empty()){
            break;
        }

        switch(menu){
            case 1:
                desenhaMenuInicio(frame, cascade, scale, tryflip);//essa funcao vai alterar a variavel menu
                //menu = 2;

                break;
            case 2:
                if(!comecouJogo){
                    pontuacao = 0;
                    mov.pontos = 0;
                    gerarBlocosIniciais();
                    comecouJogo = 1;
                    mov.yAtual = mov.yMaximo;
                }

                desenhaJogo(frame, cascade, scale, tryflip);

                break;  
            case 3:
                //desenhaMenuPausa(frame, cascade, scale, tryflip);

                break;
            case 4:
                //conquistas();

                break;
            case 5:
                return;      
        }

        // detecta saida do programa
        char c = (char)waitKey(1);
        if(c == 27 || c == 'q'){
            break;
        }
    }
}

int Jogo::inicio(){
    srand(time(NULL));

    if(!cascade.load(cascadeName)){
        cerr << "ERROR: Could not load classifier cascade: " << cascadeName << endl;
        return -1;
    }

    if(!capture.open("video.mp4")){ //para testar com um video
    //if(!capture.open(0)){ //para testar com a webcam
    //if(!capture.open("rtsp://192.168.27.106:8080/h264_ulaw.sdp")){ // tentar conectar no celular
        cout << "Capture from camera #0 didn't work" << endl;
        return 1;
    }
    if(capture.isOpened()){
        cout << "Video capturing has been started ..." << endl;

        menuInicio();
    }

    salvarHighScore();

    return 0;
}