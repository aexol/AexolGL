#import "../include/iOSHWSensor.hh"

namespace aex
{
    MotionManager::MotionManager()
    {
        static dispatch_once_t onceToken; //Unneeded?
        dispatch_once(&onceToken, ^
        {
            m_manager = [[CMMotionManager alloc] init];
        });
    }
    CMMotionManager*
    MotionManager::sharedManager()
    {
        return m_manager;
    }
}
