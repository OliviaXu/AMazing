//
//  UserControl.h
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//


#ifndef CS248_Final_Project_UserControl_h
#define CS248_Final_Project_UserControl_h

#include "Framework.h"

class UserControl {
public:
    UserControl();
    void setWindow(sf::Window* window);
    void getAngleUpdate(float &dAngleNS, float &dAngleEW);
    Keyorientation getCamDirUpdate();
	void handleInput();//this handle general input???
private:
    sf::Window* window;
    bool buttonPressed;//? no need?
	const float angSpeed;//to be changed as the speed of change
	float angNS,angEW;
	Keyorientation camdir;//enum Keyorientation{UP,RIGHT,DOWN,LEFT};
};

#endif
