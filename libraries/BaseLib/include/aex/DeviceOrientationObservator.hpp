/*
 * File:   DeviceOrientationObservator.hpp
 * Author: darek
 *
 * Created on April 28, 2015, 10:07 AM
 */

#ifndef DEVICEORIENTATIONOBSERVATOR_HPP
#define	DEVICEORIENTATIONOBSERVATOR_HPP

#include <aex/AccelEvent.hpp>
#include <aex/Accelerometer.hpp>
#include <aex/ALoopNode.hpp>

namespace aex {
    //   LANDSCAPE_RIGHT
    //      #########
    //      #|    |o#  (HOME BUTTON AT RIGHT)
    //      #########

    enum ORIENTATION {
        LANDSCAPE_RIGHT = 0,
        LANDSCAPE_LEFT = 180,
        PORTRAIT = 90,
        PORTRAIT_UPSIDE_DOWN = 270
    };

    class AEX_PUBLIC_API DeviceOrientationObservator {
    public:
        DeviceOrientationObservator(float interval = 0.5f, float switchPercent = 0.75f);
        virtual ~DeviceOrientationObservator();
        void accelStateChanged(const aex::AccelerometerEventData& e);

        virtual void orientationChanged(ORIENTATION orientation) = 0;

    protected:
        // x, y, z - axis przeciazenia
        ORIENTATION handleOrientation(float x, float y, float z);

        ORIENTATION orientation;
        ORIENTATION tmpOrientation;

        float interval; // in seconds
        float switchPercent; // of max overload in axis, when oritentation will be changed
        double time1, time2;
        int counter;
        bool m_bSendFirstState;
        double timeNow() const;
    };

    class AEX_PUBLIC_API AccelLoopNode : public aex::ALoopNode {
    public:
        AccelLoopNode();
    protected:
        virtual bool init();
        virtual void pause();
        virtual void resume();
        virtual void update();
        virtual void finish();
    private:
        aex::Accelerometer accel;
    };
}

#endif	/* DEVICEORIENTATIONOBSERVATOR_HPP */
