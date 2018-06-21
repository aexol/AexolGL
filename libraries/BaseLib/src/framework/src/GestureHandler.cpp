/* 
 * File:   GestureHandler.cpp
 * Author: darek
 * 
 * Created on May 18, 2015, 1:09 PM
 */

#include "aex/GestureHandler.hpp"
#include <aex/Logger.hpp>

const float Finger::LINE_DISTANCE_TRESHOLD = 0.06f;
 
Finger::Finger(int id) :
id(id)
{
}

void Finger::setDown(float x, float y){
    down.x = x;
    down.y = y;
    setTmp(x, y);
}

void Finger::setTmp(float x, float y){
    tmp.x = x;
    tmp.y = y;
}

void Finger::setUp(float x, float y){
    up.x = x;
    up.y = y;
}

bool Finger::operator == (const Finger& second){
    if(id == second.id){
        return true;
    }else{
        return false;
    }
}

template <typename T>
static float distance(P2D<T> p1, P2D<T> p2){
    return sqrt( pow(p2.x-p1.x, 2) + pow(p2.y - p1.y, 2) );
}


GestureHandler::GestureHandler() :
maxFingersInTouchingSession(0)
{
}

GestureHandler::~GestureHandler(){
}

void GestureHandler::touchDownEvent(float x, float y, int fingerId){
    if(addFingerIfNotExistsWith(fingerId) == true){
        Finger* f = getFinger(fingerId);
        f->setDown(x, y);
    }
    if(maxFingersInTouchingSession < fingers.size()){
        maxFingersInTouchingSession = fingers.size();
    }
    
    touchDownCallback(x, y, fingerId);
}

void GestureHandler::touchMotionEvent(float x, float y, float dx, float dy, int fingerId){
    Finger& f = *getFinger(fingerId);
    f.setTmp(x, y);
    
    //FingerMotionEvent event(e.x, e.y, e.dx, e.dy);
    //m_EventHandler->handleEvent(&event);
    touchMotionCallback(x, y, dx, dy, fingerId);
    
    if(fingers.size() == 2){
        Finger& f1 = fingers[0];
        Finger& f2 = fingers[1];
        
        if(last2FingersDistance < 0){
            last2FingersDistance = distance(f1.tmp, f2.tmp);
        }else{
            float tmpDist = distance(f1.tmp, f2.tmp);
            float scale = tmpDist - last2FingersDistance;
            
            last2FingersDistance = tmpDist;
            
            scaleCallback(scale);
        }
    }else{
        last2FingersDistance = -1;
    }
}

void GestureHandler::touchUpEvent(float x, float y, int fingerId){
    Finger& f = *getFinger(fingerId);
    f.setUp(x, y);
    
    if( distance(f.down, f.up) > Finger::LINE_DISTANCE_TRESHOLD){
        if(maxFingersInTouchingSession == 1){
            lineCallback(f.down.x, f.down.y, f.up.x, f.up.y, fingerId);
        }
    }else{
        if(maxFingersInTouchingSession == 1){
            clickCallback(f.up.x, f.up.y, fingerId);
        }
    }
    
    touchUpCallback(x, y, fingerId);
    
    last2FingersDistance = -1; // for proper handling new puting down of figers for scale event
    
    // now we can remove this finger
    removeFingerWith(fingerId);
    
    if(fingers.empty()){
        maxFingersInTouchingSession = 0;
    }
}

Finger* GestureHandler::getFinger(int id){
    for(uint32_t i = 0 ; i < fingers.size() ; i++){
        if(fingers[i].id == id){
            return &fingers[i];
        }
    }
    
    return nullptr;
}

bool GestureHandler::addFingerIfNotExistsWith(int id){
    bool exists = false;
    for(uint32_t i = 0 ; i < fingers.size() ; i++){
        if(fingers[i].id == id){
            exists = true;
            break;
        }
    }
    
    if( ! exists){
        fingers.push_back(Finger(id));
        return true;
    }
    
    return false;
}
    
void GestureHandler::removeFingerWith(int id){
    for(uint32_t i = 0 ; i < fingers.size() ; i++){
        if(fingers[i].id == id){
            fingers.erase(fingers.begin()+i);
            i--;
        }
    }
}
