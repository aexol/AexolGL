  #include <aex/FingerJoystick.hpp>

namespace aex
{

Finger::Finger(int FingerID, aex::math::Vector2 startingPosition, aex::aexTimeUnit FingerClickTime) :
    m_FingerID(FingerID),
    m_startingPosition(startingPosition),
    m_trackHistory(false),
    m_FingerClickTime(FingerClickTime) { }

Finger::Finger(int FingerID, aex::math::Vector2 startingPosition, bool trackHistory) :
    m_FingerID(FingerID), m_startingPosition(startingPosition),
    m_trackHistory(trackHistory) { }

aex::math::Vector2 Finger::getMoveDelta()
{
    return m_currentPosition - m_startingPosition;
}

void Finger::setFingerID(int FingerID)
{
    m_FingerID = FingerID;
}

int Finger::getFingerID() const
{
    return m_FingerID;
}

void Finger::setHistoryMaxSize(int historyMaxSize)
{
    m_historyMaxSize = historyMaxSize;
}

int Finger::getHistoryMaxSize() const
{
    return m_historyMaxSize;
}

void Finger::setMoveTrack(std::list<aex::math::Vector2> moveTrack)
{
    m_moveTrack = moveTrack;
}

std::list<aex::math::Vector2> Finger::getMoveTrack() const
{
    return m_moveTrack;
}

void Finger::setCurrentPosition(aex::math::Vector2 currentPosition)
{
    if(m_trackHistory)
    {
        m_moveTrack.push_back(currentPosition);
    }

    m_currentPosition = currentPosition;
}

aex::math::Vector2 Finger::getCurrentPosition() const
{
    return m_currentPosition;
}

void Finger::setStartingPosition(aex::math::Vector2 startingPosition)
{
    m_startingPosition = startingPosition;
}

aex::math::Vector2 Finger::getStartingPosition() const
{
    return m_startingPosition;
}

void Finger::setFingerClickTime(aex::aexTimeUnit FingerClickTime)
{
    m_FingerClickTime = FingerClickTime;
}

aex::aexTimeUnit Finger::getFingerClickTime() const
{
    return m_FingerClickTime;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////JoystickButton/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool JoystickButton::checkFinger(Finger_ptr finger)
{
    UNUSED(finger);
    return m_isPressed;
}

bool JoystickButton::setFinger(Finger_ptr)
{
    return true;
}

bool JoystickButton::pressElement()
{
    return true;
}

bool JoystickButton::releaseElement()
{
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////JoystickPivot////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool JoystickPivot::checkFinger(Finger_ptr finger)
{
    UNUSED(finger);
    if(!m_finger)
    {
        //if()
    }
    else
    {

    }
    return true;
}

bool JoystickPivot::setFinger(Finger_ptr)
{
    return true;
}

bool JoystickPivot::pressElement()
{
    return true;
}

bool JoystickPivot::releaseElement()
{
    return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////FingerJoystick///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FingerJoystick::FingerJoystick()
{
}

}
