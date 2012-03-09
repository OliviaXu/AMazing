#include "UserControl.h"
using namespace std;

UserControl::UserControl() : angSpeed(0.01){
	angNS=0.;
	angEW=0.;
	camdir=0;
}

void UserControl::setWindow(sf::Window* _window) {
    this->window = _window;
}

void UserControl::handleInput(){
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
			}
			else if(evt.Key.Code==sf::Key::Down){
				angNS-=angSpeed*window->GetFrameTime(); 
			}else if(evt.Key.Code=='a' || evt.Key.Code=='A' ){
				camdir=2;
			}
			else if(evt.Key.Code=='w' || evt.Key.Code=='W' ){
				camdir=0;
			}else if(evt.Key.Code=='D' || evt.Key.Code=='d' ){
				camdir=3;
			}else if(evt.Key.Code=='s' || evt.Key.Code=='S' ){
				camdir=1;
			}
			break;
        default: 
            break;

        }
    }

}

void UserControl::getAngleUpdate(float &dAngleNS, float &dAngleEW){
	//it shouldn't be done here
	dAngleNS=angNS;
	dAngleEW=angEW;
	//printf("test angle %f %f ",dAngleNS,dAngleEW);
}

int UserControl::getCamDirUpdate(){
	//printf("test camera direction %d ",camdir);
	return camdir;
}