<h1 align="center"> Sky Jump </h1>

</h1>
Projeto final criado para a disciplina de Linguagem de Programação I (2023.1), com o desenvolvimento de um jogo básico, utilizando a biblioteca OpenCV, feito para avaliar, na prática, os conhecimentos ensinados sobre Programação Orientada a Objetos e o uso do algoritmo da OpenCV de detecção de objetos, em C++.

<p>&nbsp;</p>

Professor: Derzu Omaia.

<p>&nbsp;</p>

## Especificações gerais

-  Fazer a modelagem das classes que serão utilizadas no sistema utilizando diagrama UML de classe.

- O programa deve rodar indefinidamente, até que o usuário escolha sair. Exiba um menu (modo texto ou tela).

-  Exibir pontuação final, e salvar em arquivo a maior pontuação (record).

- Usar a biblioteca OpenCV, Yolo ou OpenPose para detectar algum objeto ou jogador.

- Tocar um som quando uma pontuação for feita.

<p>&nbsp;</p>

## Diagrama de classes

![Diagrama](https://github.com/GSherterton/Trabalho-OpenCv/blob/main/UML%20OpenCv.jpg?raw=true)

<p>&nbsp;</p>

## Objetivo do jogo
- O usuário deve mexer o rosto para pular de uma plataforma para outra. As plataformas aparecerão de maneira aleatória na tela.

- O objetivo principal é bater o recorde de maior altura alcançada.

- O jogo acaba quando o usuário cair das plataformas.

<p>&nbsp;</p>

## Como rodar o jogo?
- As bibliotecas [OpenCV](https://sites.google.com/site/derzuomaia/tutoriais/instala%C3%A7%C3%A3o-opencv-no-linux-ubuntu), [Chrono](https://api.projectchrono.org/tutorial_install_chrono.html) e [MPlayer](https://howtoinstall.co/package/mplayer) devem ser instaladas antes de rodar o jogo.

- Rodando no Linux Ubuntu:
    - A linha de comando para compilar o código é: ```g++ sources/*.cpp `pkg-config --cflags opencv4` `pkg-config --libs --static opencv4` -I headers -o teste` ```
    - Depois de compilado, basta rodar o arquivo executável. (ex: ```./teste```)

<p>&nbsp;</p>

## Contribuidores

| <a href="https://www.linkedin.com/in/gabrielcformiga/" target="_blank">**Gabriel Campelo Formiga**</a> | <a href="https://www.linkedin.com/in/gsherterton/" target="_blank">**Gabriel Sherterton**</a> | 
| :---------------------------------------------------------------------------------------: | :-----------------------------------------------------------------------------------------: | 
| <img src="https://avatars.githubusercontent.com/u/130695548?v=4" width="200px">          | <img src="https://avatars.githubusercontent.com/u/129867791?v=4" width="200px">          | 
| <a href="https://github.com/GabrielCFormiga" target="_blank">`github.com/GabrielCFormiga`</a>  | <a href="https://github.com/GSherterton" target="_blank">`github.com/GSherterton`</a> | 
