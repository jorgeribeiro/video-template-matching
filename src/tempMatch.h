#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

const int camWidth = 320;  /* constantes com a resolução da imagem da webcam */
const int camHeight = 240;

class tempMatch : public ofBaseApp  /* carrega as funções e objetos do tempMatch.cpp */
{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofVideoGrabber webCam;  /* imagem da webcam */
		ofxCvColorImage imgWebCam;  /* variável que salva o frame atual */
		ofxCvColorImage imgCapture;  /* imagem a ser buscada pelo Template Matching */
		ofImage imgTemplate; /* variável que cria o arquivo da imagem */
		int count; 
		ofPoint objectLocation; /* variável que salva a coordenada x e y do objeto rastreado */
		ofRectangle templateFrame; /* retangulo desenhado que exibe o objeto rastreado */
		bool templateDefined;
		bool imgCaptured;
};