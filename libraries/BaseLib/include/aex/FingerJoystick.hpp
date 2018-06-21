/*
 * File:   Joystick.hpp
 * Author: mritke
 *
 * Created on August 28, 2014, 3:25 PM
 */

#ifndef JOYSTICK_HPP
#define	JOYSTICK_HPP

#include <aex/AexolGL.hpp>

namespace aex
{

class AEX_PUBLIC_API Finger
{
protected:
    int m_FingerID;
    aex::math::Vector2 m_startingPosition;
    aex::math::Vector2 m_currentPosition;
    std::list<aex::math::Vector2> m_moveTrack;
    bool m_trackHistory;
    int m_historyMaxSize = 20;
    aex::aexTimeUnit m_FingerClickTime = 0.0;
    bool m_longClick =false;

public:

    Finger(int FingerID, aex::math::Vector2 startingPosition, aex::aexTimeUnit FingerClickTime);
    Finger(int FingerID, aex::math::Vector2 startingPosition, bool trackHistory);

    aex::math::Vector2 getMoveDelta();
    void setFingerID(int FingerID);
    int getFingerID() const;
    void setHistoryMaxSize(int historyMaxSize);
    int getHistoryMaxSize() const;
    void setMoveTrack(std::list<aex::math::Vector2> moveTrack);
    std::list<aex::math::Vector2> getMoveTrack() const;
    void setCurrentPosition(aex::math::Vector2 currentPosition);
    aex::math::Vector2 getCurrentPosition() const;
    void setStartingPosition(aex::math::Vector2 startingPosition);
    aex::math::Vector2 getStartingPosition() const;
    void setFingerClickTime(aex::aexTimeUnit FingerClickTime);
    aex::aexTimeUnit getFingerClickTime() const;

};

typedef aex::shared_ptr<Finger> Finger_ptr;

class AEX_PUBLIC_API IJoystickElement
{
protected:
    Finger_ptr m_finger;
    aex::math::Vector2 m_size;
    aex::math::Vector2 m_position;

public:
    virtual bool checkFinger(Finger_ptr finger) = 0;
    virtual bool setFinger(Finger_ptr) = 0;
    virtual bool pressElement() = 0;
    virtual bool releaseElement() = 0;
};

typedef aex::shared_ptr<IJoystickElement> JoystickElement_ptr;

class AEX_PUBLIC_API JoystickButton : public IJoystickElement
{
private:


    bool m_isPressed = false;

protected:

public:
    bool checkFinger(Finger_ptr finger);
    bool setFinger(Finger_ptr);
    bool pressElement();
    bool releaseElement();
};

class AEX_PUBLIC_API JoystickPivot : public IJoystickElement
{
private:
    aex::math::Vector2 m_pivotCenter;
    aex::math::Vector2 m_deflection;
protected:

public:
    bool checkFinger(Finger_ptr finger);
    bool setFinger(Finger_ptr);
    bool pressElement();
    bool releaseElement();

};

typedef aex::shared_ptr<JoystickButton> JoystickButton_ptr;

class AEX_PUBLIC_API FingerJoystickVisual
{
private:

protected:
    aex::Visual_ptr m_visual;
  
public:
    FingerJoystickVisual();
    void initVisual(aex::DrawNode_ptr root);

};

class AEX_PUBLIC_API FingerJoystick
{
private:

    std::vector<Finger_ptr> m_fingers;
    std::vector<JoystickElement_ptr> m_controlls;

protected:

public:
    FingerJoystick();
    void TrackFinger();


    bool init();
    void update();
    void pause();
    void resume();



};


}


#endif	/* JOYSTICK_HPP */

