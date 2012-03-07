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

class EventMgr {
public:
    EventMgr();
    bool updateEvents(vector<GameObject> &objects, MapLoader &mapLoader, vector<Event> events);
};

#endif
