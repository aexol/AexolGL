#include <aex/Camera.hpp>
#include <aex/os/OSUtils.hpp>
#include "../../include/CameraImpl.hpp"

namespace aex
{
    namespace cam
    {

        /* Parameters class */
        bool Parameters::isSupported(int param) const
        {
            return true;
        }

        void Parameters::sendAntibanding(int param) throw (camera_parameters_exception)
        {
        }

        void Parameters::sendColorEffect(int param) throw (camera_parameters_exception)
        {
        }

        void Parameters::sendWhiteBalance(int param) throw (camera_parameters_exception)
        {
        }

        void Parameters::sendFlashMode(int param) throw (camera_parameters_exception)
        {
        }

        void Parameters::sendSceneMode(int param) throw (camera_parameters_exception)
        {
        }

        void Parameters::sendFocusMode(int param) throw (camera_parameters_exception)
        {
        }

        void Parameters::sendTorchMode(int param) throw (camera_parameters_exception)
        {
        }

        void Parameters::sendExposureMode(int param) throw (camera_parameters_exception)
        {
        }

        void Parameters::sendZoom(float zoom) throw (camera_parameters_exception)
        {
        }

        void Parameters::sendZoomSmoothly(float zoom) throw (camera_parameters_exception)
        {
        }

        void Parameters::sendExposureCompensation(float ev) throw (camera_parameters_exception)
        {
        }

        void Parameters::sendISO(float iso) throw (camera_parameters_exception)
        {
        }

        void Parameters::sendExposureDuration(float ed) throw (camera_parameters_exception)
        {
        }

        void Parameters::sendISOAndED(float iso, float ed) throw (camera_parameters_exception)
        {
        }

        void Parameters::sendFocusAreas(const std::vector<Area>& areas) throw (camera_parameters_exception)
        {
        }

        void Parameters::sendPreviewFPSRange(Range range) throw (camera_parameters_exception)
        {
        }

        void Parameters::sendExposureAreas(const std::vector<Area>& areas) throw (camera_parameters_exception)
        {
        }

        void Parameters::sendLensPositionWithFocusModeFixed(float position) throw (camera_parameters_exception)
        {
        }

        void Parameters::sendRotation(int degrees) throw (camera_exception, camera_parameters_exception)
        {
        }

        void Parameters::sendAutoFocus() throw (camera_parameters_exception)
        {
        }

        bool Parameters::sendConfigurationDevice() throw (camera_parameters_exception)
        {
            return true;
        }

        float Parameters::loadZoom() const
        {
            return 1;
        }

        float Parameters::loadExposureCompensation() const
        {
            return 1;
        }

        std::vector<Area> Parameters::loadExposureAreas() const
        {
            return std::vector<Area>();
        }

        std::vector<Area> Parameters::loadFocusAreas() const
        {
            return std::vector<Area>();
        }

        std::vector<float> Parameters::loadFocusDistances() const
        {
            return std::vector<float> ();
        }

        float Parameters::loadFocalLength() const
        {
            return 1;
        }

        float Parameters::loadHorizontalViewAngle() const
        {
            return 1;
        }

        float Parameters::loadVerticalViewAngle() const
        {
            return 1;
        }

        float Parameters::loadLensAperture() const
        {
            return 1;
        }

        float Parameters::loadLensPosition() const
        {
            return 1;
        }

        float Parameters::loadFOV() const
        {
            return 1;
        }

        float Parameters::loadISO() const
        {
            return 1;
        }

        float Parameters::loadExposureDuration() const
        {
            return 0.1;
        }

        /* Picture class */
        bool Picture::sendIsSupportedPictureFormat(FORMAT format, int camID) 
        {
            return true;
        }

        void Camera::loadConfigurations()
        {
        }

        void Camera::delete_camera_impl::operator()(void* p)
        {
            delete static_cast<CameraImpl*> (p);
        }

        bool Camera::setDisplayOrientation(unsigned degrees) throw (camera_exception, camera_parameters_exception)
        {
            return true;
        }

        void Camera::step()
        {
        }

        void Camera::updateTexture()
        {
        }

        void Camera::updateTransformMatrix()
        {
        }

        void Camera::sendStartPreview() throw (camera_exception)
        {
        }

        void Camera::sendStopPreview()
        {
        }

        /*void Camera::open(bool withAudio) throw (camera_permissions_exception, camera_opening_exception, camera_no_device_exception)
        {
            this->m_opened = false;
        }*/
        
        void Camera::sendInit()
        {
            
        }

        void Camera::sendOpen() throw (camera_permissions_exception,camera_opening_exception, camera_no_device_exception)
        {
             throw camera_no_device_exception("Camera device no exists", m_camID);
        }
        
        void Camera::sendRelease()
        {
        }

        int Camera::getNumberOfCameras()
        {
            return 0;
        }
        
                
        bool Camera::isLowLightBoostSupported() const throw(camera_exception)
        {
            return false;
        }
         
        void Camera::setLowLightBoost(bool enabled) throw(camera_exception)
        {
            // nothing
        }
        
        void Camera::sendCallbackExposureCompensation(const std::function<void()>& cb)
        {
        }

        void Camera::sendCallbackLensPosition(const std::function<void()>& cb)
        {
        }

        void Camera::sendCallbackSmoothZoom(const std::function<void()>& cb)
        {
        }

        void Camera::sendCallbackAutoFocus(const std::function<void(bool)>& cb)
        {
        }

        void Camera::sendCallbackTakePicture(const std::function<void(int) >& cb)
        {
        }

        void Camera::sendCallbackISO(const std::function<void()>& cb)
        {
        }

        void Camera::sendCallbackExposureDuration(const std::function<void()>& cb)
        {
        }

        void Camera::sendCallbackISOAndED(const std::function<void()>& cb)
        {
        }
        
        void Camera::sendCallbackStopVideo(const std::function<void(int)>& cb)
        {
        }

        void Camera::sendTakePicture(const Picture& picture) throw (camera_parameters_exception)
        {
        }

        void Camera::sendCallbackOnNewFrame()
        {
        }

        std::string Camera::getFlatten() const
        {
            return "";
        }

        void Camera::sendStopVideo()
        {
        }

        void Camera::sendStartVideo(const aex::cam::Video& video) throw (camera_parameters_exception)
        {
        }

        void Picture::sendOpenAlbumInGallery(const std::string& nane)
        {
        }

        Directory Directory::getUserDataDirectory()
        {
            return Directory("./");
        }

        Directory Directory::getCacheDirectory()
        {
            return Directory("/tmp/");
        }

        Directory Directory::getExternalStorageDirectory()
        {
            return Directory("./");
        }

        Directory Directory::getDirectory(int type)
        {
            return Directory("./");
        }

        Directory Directory::mkdir(const std::string& name) throw (directory_exception)
        {
            return Directory("./");
        }
        

    } // namespace cam
    
    /*namespace os
    {
        Locale::LANGUAGE os::Locale::getLanguage()
        {
            return os::Locale::LANGUAGE::ENGLISH;
        }
        
        void MoreApps::openMoreApps(const std::string& developerName)
        {
            UNUSED(developerName);
        }
        
        void AppSettings::setSleepMode(bool value)
        {
            UNUSED(value);
        }
        
    }*/
    
} // namespace aex
