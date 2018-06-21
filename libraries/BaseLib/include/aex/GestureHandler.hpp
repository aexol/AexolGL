/* 
 * File:   GestureHandler.h
 * Author: darek
 *
 * Created on May 18, 2015, 1:09 PM
 */

#ifndef GESTUREHANDLER_H
#define	GESTUREHANDLER_H

#include <aex/Common.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <functional>

template <typename T> 
struct AEX_PUBLIC_API P2D{
    T x,y;
};

class AEX_PUBLIC_API Finger{
public:
    const static float LINE_DISTANCE_TRESHOLD;
    Finger() : id(-1) {}
    Finger(int id) ;
   
    void setDown(float x, float y);
    void setTmp(float x, float y);
    void setUp(float x, float y);
    
    bool operator == (const Finger& second);
    
    int id;
    P2D<float> down;
    P2D<float> tmp;
    P2D<float> up;
};

template <typename T>
static float distance(P2D<T> p1, P2D<T> p2);

class AEX_PUBLIC_API GestureHandler {
public:
    GestureHandler();
    virtual ~GestureHandler();
    
    virtual void touchDownCallback(float x, float y, int fingerId) = 0;
    virtual void touchMotionCallback(float x, float y, float dx, float dy, int fingerId) = 0;
    virtual void touchUpCallback(float x, float y, int fingerId) = 0;
    virtual void lineCallback(float x1, float y1, float x2, float y2, int fingerId) = 0;
    virtual void clickCallback(float x, float y, int fingerId) = 0;
    virtual void scaleCallback(float scale) = 0;
    
    void touchDownEvent(float x, float y, int fingerId);
    void touchMotionEvent(float x, float y, float dx, float dy, int fingerId);
    void touchUpEvent(float x, float y, int fingerId);
    
private:

    std::vector<Finger> fingers; // try put more if u can   
    Finger* getFinger(int id);
    bool addFingerIfNotExistsWith(int id);
    void removeFingerWith(int id);
    
    float last2FingersDistance;
    uint32_t maxFingersInTouchingSession;
};

#endif	/* GESTUREHANDLER_H */

