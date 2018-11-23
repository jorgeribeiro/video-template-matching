#include "ofMain.h"
#include "tempMatch.h"
#include "ofAppGlutWindow.h"

int main()
{

	ofAppGlutWindow window;
	ofSetupOpenGL(&window, camWidth * 2, camHeight + 30, OF_WINDOW);   /* carrega a janela com o tamanho definido */

	ofRunApp(new tempMatch());

}
