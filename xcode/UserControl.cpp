#include "UserControl.h"
using namespace std;

UserControl::UserControl() : angSpeed(0.01){

}

void UserControl::setWindow(sf::Window _window){
	window=_window;
}

void UserControl::getAngleUpdate(float &dAngleNS, float &dAngleEW){
	//it shouldn't be done here
	    sf::Event evt;
    while (window.GetEvent(evt)) {
        switch (evt.Type) {
        case sf::Event::Closed: 
            // Close the window.  This will cause the game loop to exit,
            // because the IsOpened() function will no longer return true.
            window.Close(); 
            break;
        case sf::Event::Resized: 
            // If the window is resized, then we need to change the perspective
            // transformation and viewport
            glViewport(0, 0, evt.Size.Width, evt.Size.Height);
            break;
		
		case sf::Event::MouseButtonPressed:
			break;
		case sf::Event::MouseButtonReleased:

			break;
		case sf::Event::MouseMoved:
			break;
		case sf::Event::KeyPressed:
			if(evt.Key.Code=='a' || evt.Key.Code=='A' ){
			}


			//TODO: near far
			//if(evt.Key.Code==sf::Key::Left){
			else if(evt.Key.Code==sf::Key::Left){
			}
			break;
        default: 
            break;
        }
    }
}