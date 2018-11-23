#include "tempMatch.h"

typedef struct _mV_ 
{
	double value;
	struct _mV_ *next;
} mV;

mV *createList(double v)
{
	mV *tmp = NULL;
	tmp = (mV*)malloc(sizeof(mV));
	if(tmp != NULL)
	{
		tmp->value = v;
		tmp->next = NULL;
		return tmp;
	}
	return NULL;
}

void insertFinal(mV **head, double v)
{
	mV *elm = createList(v);
	mV *tmp = *head;
	if(*head == NULL)
		*head = elm;
	else
	{
		while(tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = elm;
	}
}

double searchMinVal(mV *head)
{
	mV *tmp = head;
	double minV = 0;
	if(tmp != NULL)
	{
		minV = tmp->value;
		while(tmp->next != NULL)
		{
			if(tmp->value < minV)
				minV = tmp->value;
			tmp = tmp->next;
		}
		return minV;
	}
	return minV;
}

void printList(mV *head)
{
	mV *tmp = head;
	if(tmp == NULL)
		cout << "Lista vazia!\n";
	else
	{
		while(tmp != NULL)
		{
			cout << tmp->value;
			cout << "\n";
			tmp = tmp->next;
		}		
	}
}

void clearList(mV **head)
{
	mV *tmp = *head;
	mV *aux;
	while(tmp != NULL)
	{
		aux = tmp->next;
		free(tmp);
		tmp = aux;
	}
	*head = NULL;
}

mV *minValList = NULL;

void tempMatch::setup()
{
	webCam.setVerbose(true); 
	webCam.initGrabber(camWidth,camHeight);  /* inicia a captura da webcam */
	imgWebCam.allocate(camWidth,camHeight);  /* aloca memória para a imagem da webcam */
	cout <<"Webcam iniciada!\n";
	
	templateDefined = false; /* imagem de busca definida = false */
	imgCaptured = false; 
	count = 1;
}

void tempMatch::update()
{
	webCam.update(); /* atualiza a imagem da webcam */
	if(webCam.isFrameNew())  /* carrega o frame da webcam e exibe */
	{
		imgWebCam.setFromPixels(webCam.getPixels(), camWidth, camHeight);
		if(templateDefined) /* se true, inicia a busca do template nos frames do vídeo */
		{
			IplImage *result = cvCreateImage(cvSize((camWidth - imgCapture.width + 1), (camHeight - imgCapture.height + 1)), 32, 1);
			cvMatchTemplate(imgWebCam.getCvImage(), imgCapture.getCvImage(), result, CV_TM_SQDIFF); /* função que faz o rastreamento da imagem capturada */
			double minVal, maxVal;
			CvPoint minLoc, maxLoc;
			cvMinMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, 0); /* define o minLoc e a partir dele se define o resultado do template matching */
			objectLocation.x = minLoc.x;
			objectLocation.y = minLoc.y;
			insertFinal(&minValList, minVal); /* insere os valores de minVal na lista */
		}
	}
}

void tempMatch::draw()
{
	ofBackground(64,64,64);
	ofSetHexColor(0xffffff);
	imgWebCam.draw(5,5);

	if(templateDefined)  /* se true, cria um retângulo que exibe o resultado do template matching */
	{
		ofSetHexColor(0xffffff);
		ofDrawBitmapString("Imagem Buscada", camWidth + 30, 15);
		imgCapture.draw(camWidth + 20, 20);
		ofNoFill();
		ofSetHexColor(0xff0000);
		ofRect(objectLocation.x, objectLocation.y, templateFrame.width, templateFrame.height); /* localização e tamanho do retângulo */
	}
	if(imgCaptured) /* se true, aparece a caixa de seleção do template */
	{
		ofEnableAlphaBlending();
		ofSetColor(255, 255, 255, 125);
		ofRect(templateFrame);
		ofDisableAlphaBlending();
	}	
}

void tempMatch::mousePressed(int x, int y, int button)
{
	templateDefined = false;
	templateFrame.x = x;
	templateFrame.y = y;
	templateFrame.width = 0;
	templateFrame.height = 0;
	imgCaptured = true;
}

void tempMatch::mouseDragged(int x, int y, int button)
{
	templateFrame.width = x - templateFrame.x;
	templateFrame.height = y - templateFrame.y;
}

void tempMatch::mouseReleased(int x, int y, int button)
{
	double minValueTotal;
	if(templateFrame.width < 0)
	{
		templateFrame.x += templateFrame.width;
		templateFrame.width *= -1;
	}

	if(templateFrame.height < 0)
	{
		templateFrame.y += templateFrame.height;
		templateFrame.height *= -1;
	}
	imgCaptured = false;
	objectLocation.x = templateFrame.x;
	objectLocation.y = templateFrame.y;
	
	imgCapture.allocate(templateFrame.width, templateFrame.height);	/* aloca memória pra imagem de busca */
	imgWebCam.setROI(templateFrame); /* setROI = set Region of Interest da webcam a partir da caixa de seleção */
	imgCapture = imgWebCam; /* seta a imagem de busca a partir do ROI */
	imgTemplate.grabScreen(objectLocation.x, objectLocation.y, imgCapture.width, imgCapture.height);
	imgTemplate.saveImage("template_matching_"+ofToString(count)+".png"); /* salva a imagem no arquivo */
	count++;
	imgWebCam.resetROI(); /* reinicia o ROI para não dar problemas futuros */
	templateDefined = true; /* template matching definido = true */
	cout << "Menor minVal do ultimo template: \n";
	minValueTotal = searchMinVal(minValList);
	cout << minValueTotal;
	cout << "\n";
	clearList(&minValList);
}

void tempMatch::keyPressed(int key)
{
	switch (key)
	{
		case ' ':
			break;
	}
}

void tempMatch::keyReleased(int key)
{

}

void tempMatch::mouseMoved(int x, int y )
{

}

void tempMatch::windowResized(int w, int h)
{

}

void tempMatch::gotMessage(ofMessage msg)
{

}

void tempMatch::dragEvent(ofDragInfo dragInfo)
{ 

}