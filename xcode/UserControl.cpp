#include "UserControl.h"
using namespace std;

UserControl::UserControl() : angSpeed(2000){
	angNS=0.;
	angEW=0.;
	camdir=(Keyorientation)0;//or UP
	camM=NO;//no camera movement at first start
	lightP=Vec3(1.,1.,0.);
}

void UserControl::setWindow(sf::Window* _window) {
    this->window = _window;
}

void UserControl::handleInput(){
	camM=NO;//debug... making the camera movement available only once a frame
	sf::Event evt;
    while (window->GetEvent(evt)) {
        switch (evt.Type) {
        case sf::Event::Closed: 
            // Close the window.  This will cause the game loop to exit,
            // because the IsOpened() function will no longer return true.
            window->Close(); 
            break;
        case sf::Event::Resized: 
            // If the window is resized, then we need to change the perspective
            // transformation and viewport
            //glViewport(0, 0, evt.Size.Width, evt.Size.Height);
            break;
		case sf::Event::MouseButtonPressed:
			break;
		case sf::Event::MouseButtonReleased:
			break;
		case sf::Event::MouseMoved:
			break;
		case sf::Event::KeyPressed:
                cout << window->GetFrameTime() << endl;
			if(evt.Key.Code==sf::Key::Left){
				angEW-=angSpeed*window->GetFrameTime(); 
				//TODO: this is problematic should be changed according to the direction of ball movcement
			}
			else if(evt.Key.Code==sf::Key::Right){
				angEW+=angSpeed*window->GetFrameTime(); 
			}
			else if(evt.Key.Code==sf::Key::Up){
				angNS+=angSpeed*window->GetFrameTime(); 
			}
			else if(evt.Key.Code==sf::Key::Down){
				angNS-=angSpeed*window->GetFrameTime(); 
			}else if(evt.Key.Code=='a' || evt.Key.Code=='A' ){
				camdir=LEFT;
			}
			else if(evt.Key.Code=='w' || evt.Key.Code=='W' ){
				camdir=UP;
			}else if(evt.Key.Code=='D' || evt.Key.Code=='d' ){
				camdir=RIGHT;
			}else if(evt.Key.Code=='s' || evt.Key.Code=='S' ){
				camdir=DOWN;
			}else if(evt.Key.Code=='1'){
				camM=U;
			}else if(evt.Key.Code=='2'){
				camM=D;
			}else if(evt.Key.Code=='3'){
				camM=L;
			}else if(evt.Key.Code=='4'){
				camM=R;
			}else if(evt.Key.Code=='5'){
				camM=F;
			}else if(evt.Key.Code=='6'){
				camM=B;
			}else if(evt.Key.Code=='7'){
				lightP.x++;
			}else if(evt.Key.Code=='8'){
				lightP.x--;
			}else if(evt.Key.Code=='9'){
				lightP.z++;
			}else if(evt.Key.Code=='0'){
				lightP.z--;
			}
			break;
        default: 
            break;

        }

		if(DEBUG_OUTPUT)
            cout << "light " << lightP.x << "," << lightP.y <<"," << lightP.z<<endl;
    }

}

struct Vec3 UserControl::lightPos(){
	return lightP;
}



void UserControl::getAngleUpdate(float &dAngleNS, float &dAngleEW){
	//it shouldn't be done here
	dAngleNS=angNS;
	dAngleEW=angEW;
	//printf("test angle %f %f ",dAngleNS,dAngleEW);
}

Keyorientation UserControl::getCamDirUpdate(){
	//printf("test camera direction %d ",camdir);
	return camdir;
}


CamMorientation UserControl::getCamM(){
	return camM;
}