//
//  EventMgr.h
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CS248_Final_Project_EventMgr_h
#define CS248_Final_Project_EventMgr_h

#include "Framework.h"
#include "Event.h"

class EventMgr {
public:
    EventMgr();
    bool updateEvents(std::vector<GameObject*> &objects, MapLoader* mapLoader, std::vector<Event> events);
};

#endif
