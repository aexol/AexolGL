/*
 * File:   DeviceOrientationObservator.cpp
 * Author: darek
 *
 * Created on April 28, 2015, 10:07 AM
 */

#include <aex/DeviceOrientationObservator.hpp>
#include <chrono>
#include <SDL.h>
#include <aex/Logger.hpp>

#ifndef ABS
#define ABS(x) ((x)<0 ? -(x) : (x))
#endif

namespace aex {

    int orientation_change(void *userdata, SDL_Event* e) {
        if (e->type == Event::getAccelerometerEventType()) {
            aex::AccelerometerEventData* eventData = static_cast<AccelerometerEventData*> (e->user.data1);
            ((DeviceOrientationObservator*) userdata)->accelStateChanged(*eventData);
        }
        return 0;
    }

    DeviceOrientationObservator::DeviceOrientationObservator(float interval, float switchPercent) :
    interval(interval),
    switchPercent(switchPercent)
    {
        orientation = ORIENTATION::PORTRAIT_UPSIDE_DOWN;
        tmpOrientation = orientation;

        time1 = timeNow();
        SDL_AddEventWatch(orientation_change, this); // callback for watch an events
        counter = 0;
        // m_bSendFirstState = true;
    }

    DeviceOrientationObservator::~DeviceOrientationObservator() {
        SDL_DelEventWatch(orientation_change, this);
    }

    void DeviceOrientationObservator::accelStateChanged(const aex::AccelerometerEventData& e) {
        time2 = timeNow();

        counter++;

        if ((time2 - time1) >= interval) {
            time1 = time2;
            counter = 0;

            ORIENTATION orient = handleOrientation(e.axes.x, e.axes.y, e.axes.z);

            if (orient != orientation) {
                orientation = orient;
                orientationChanged(orientation);
            }
        }
    }

    ORIENTATION DeviceOrientationObservator::handleOrientation(float x, float y, float z) {

        float divisor = 1;
        float tresh = 4; // sum of overloads
        if ((ABS(x) + ABS(y) + ABS(z)) > tresh) {
            divisor = 10; // android
        } else {
            divisor = 1; // iOS
            x = -x;
            y = -y;
            z = -z;
        }
        x /= divisor;
        y /= divisor;
        z /= divisor;

        //LOG(x, " ", y, " ", z);

        if (ABS(x) >= switchPercent) {
            if (x > 0) {
                return ORIENTATION::LANDSCAPE_RIGHT;
            } else {
                return ORIENTATION::LANDSCAPE_LEFT;
            }
        }
        if (ABS(y) >= switchPercent) {
            if (y > 0) {
                return ORIENTATION::PORTRAIT;
            } else {
                return ORIENTATION::PORTRAIT_UPSIDE_DOWN;
            }
        }

        return orientation; // if
    }

    double DeviceOrientationObservator::timeNow() const {
        auto duration = std::chrono::system_clock::now().time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        return millis / 1000.0;
    }

    AccelLoopNode::AccelLoopNode() {

    }

    bool AccelLoopNode::init() {
        // run an accelerometer
        accel.init();
        accel.startSampling();
        //accel.setSamplingRate(1); // TODO - understand this value in iOS and Android ... it behaves strange (androdi 100 or 50 per sec, on ios 100 or really slow per sec)
        return true;
    }

    void AccelLoopNode::pause() {
        accel.stopSampling();
    }

    void AccelLoopNode::resume() {
        accel.startSampling();
    }

    void AccelLoopNode::update() {
        accel.update();
    }

    void AccelLoopNode::finish() {
        accel.stopSampling();
        m_done = true;
    }

} // end of namespace aex
