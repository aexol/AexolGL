#include <sstream>
#include <cstdlib>
#include <SDL.h>

#include <random>
#include <sstream>
#include <string>

#include <aex/Logger.hpp>
#include <aex/Camera.hpp>
#include <aex/Texture.hpp>

#include "CameraMediator.hpp"
#include "CameraImplAndroid.hpp"

namespace aex
{
    namespace cam
    {
        // have singeton state
        CameraMediator cm;

        JNIEnv* getJNIEnv()
        {
            return static_cast<JNIEnv*> (SDL_AndroidGetJNIEnv());
        }

        /* Parameters class */
        bool Parameters::isSupported(int param) const
        {
            bool retVal;
            cm.isSupported(getJNIEnv(), param, &retVal, m_camID);
            return retVal;
        }

        void Parameters::sendAntibanding(int param) throw (camera_parameters_exception)
        {
            int code = cm.setParameter(getJNIEnv(), param, m_camID);

            if (code == cm.IS_NOT_SUPPORTED_CODE())
            {
                throw camera_parameters_exception(std::string("Parameter is not supported, pls use method \"isSupported\" before setting it"), m_camID, 0, "antibanding");
            }
        }

        void Parameters::sendColorEffect(int param) throw (camera_parameters_exception)
        {
            int code = cm.setParameter(getJNIEnv(), param, m_camID);

            if (code == cm.IS_NOT_SUPPORTED_CODE())
            {
                throw camera_parameters_exception(std::string("Parameter is not supported, pls use method \"isSupported\" before setting it"), m_camID, 0, "colorEffect");
            }
        }

        void Parameters::sendWhiteBalance(int param) throw (camera_parameters_exception)
        {
            int code = cm.setParameter(getJNIEnv(), param, m_camID);

            if (code == cm.IS_NOT_SUPPORTED_CODE())
            {
                throw camera_parameters_exception(std::string("Parameter is not supported, pls use method \"isSupported\" before setting it"), m_camID, 0, "whiteBalance");
            }
        }

        void Parameters::sendFlashMode(int param) throw (camera_parameters_exception)
        {
            int code = cm.setParameter(getJNIEnv(), param, m_camID);

            if (code == cm.IS_NOT_SUPPORTED_CODE())
            {
                throw camera_parameters_exception(std::string("Parameter is not supported, pls use method \"isSupported\" before setting it"), m_camID, 0, "flashMode");
            }
        }

        void Parameters::sendSceneMode(int param) throw (camera_parameters_exception)
        {
            int code = cm.setParameter(getJNIEnv(), param, m_camID);

            if (code == cm.IS_NOT_SUPPORTED_CODE())
            {
                throw camera_parameters_exception(std::string("Parameter is not supported, pls use method \"isSupported\" before setting it"), m_camID, 0, "sceneMode");
            }
        }

        void Parameters::sendFocusMode(int param) throw (camera_parameters_exception)
        {
            int code = cm.setParameter(getJNIEnv(), param, m_camID);

            if (code == cm.IS_NOT_SUPPORTED_CODE())
            {
                throw camera_parameters_exception(std::string("Parameter  is not supported, pls use method \"isSupported\" before setting it"), m_camID, 0, "focusMode");
            }
        }

        void Parameters::sendTorchMode(int param) throw (camera_parameters_exception)
        {
            cm.setParameter(getJNIEnv(), Parameters::FLASH_MODE_OFF, m_camID); // first need to set flash mode to off (torch mode is together with flash mode on iOS)
            int code = cm.setParameter(getJNIEnv(), param, m_camID);

            if (code == cm.IS_NOT_SUPPORTED_CODE())
            {
                throw camera_parameters_exception(std::string("Parameter  is not supported, pls use method \"isSupported\" before setting it"), m_camID, 0, "torchMode");
            }
        }

        void Parameters::sendExposureMode(int param) throw (camera_parameters_exception)
        {
            int code = cm.setParameter(getJNIEnv(), param, m_camID);

            if (code == cm.IS_NOT_SUPPORTED_CODE())
            {
                throw camera_parameters_exception(std::string("Parameter  is not supported, pls use method \"isSupported\" before setting it"), m_camID, 0, "exposureMode");
            }
        }

        void Parameters::sendZoom(float zoom) throw (camera_parameters_exception)
        {
            cm.setZoom(getJNIEnv(), zoom, m_camID);
        }

        void Parameters::sendZoomSmoothly(float zoom) throw (camera_parameters_exception)
        {
            int code = cm.setZoomSmooth(getJNIEnv(), zoom, m_camID);

            if (code != cm.SUCCESS_CODE())
            {
                throw camera_parameters_exception(std::string("Error while trying to set zoom smoothly"), m_camID, 0, "zoom");
            }
        }

        void Parameters::sendExposureCompensation(float ev) throw (camera_parameters_exception)
        {
            int code = cm.setExposureCompensation(getJNIEnv(), ev, m_camID);

            if (code == cm.IS_NOT_SUPPORTED_CODE())
            {
                throw camera_parameters_exception("Bad exposure value provided!", m_camID, 0, "nothing");
            }
        }

        void Parameters::sendISO(float iso) throw (camera_parameters_exception)
        {
            //int code = cm.set TODO
            // if code ==
            // throw
            //}
        }

        void Parameters::sendExposureDuration(float ed) throw (camera_parameters_exception)
        {
            //int code = cm.set TODO
            // if code ==
            // throw
            //}
        }

        void Parameters::sendISOAndED(float iso, float ed) throw (camera_parameters_exception)
        {
            //int code = cm.set TODO
            // if code ==
            // throw
            //}
        }

        void Parameters::sendFocusAreas(const std::vector<Area>& areas) throw (camera_parameters_exception)
        {
            std::vector<float> areasFloats = areasToFloatVector(areas);
            int code = cm.setFocusAreas(getJNIEnv(), areasFloats, m_camID);

            if (code != cm.SUCCESS_CODE())
            {
                throw camera_parameters_exception("Wrong areas in setFocusAreas provided!", m_camID, 0, "nothing");
            }
        }

        void Parameters::sendPreviewFPSRange(Range range) throw (camera_parameters_exception)
        {
            Range closestRange = range;
            int minDelta = std::numeric_limits<int>::max();
            for (Range& r : supportedFpsRanges)
            {
                if (r.contains(range))
                {
                    int tmpDelta = (range.getMin() - r.getMin()) + (r.getMax() - range.getMax());
                    if(tmpDelta < minDelta)
                    {
                      minDelta = tmpDelta;
                      closestRange = r;
                    }
                }
            }

            if(minDelta != 0)
            {
              LOG_ERR("Provided range: ", range, " is not supported. Setted closest range: ", closestRange);
            }
            int code = cm.setPreviewFPSRange(getJNIEnv(), closestRange.getMin(), closestRange.getMax(), m_camID);
            if (code != cm.SUCCESS_CODE())
            {
                throw camera_parameters_exception("Wrong fps range in setPreviewFpsRange provided!", m_camID, 0, "nothing");
            }
        }

        void Parameters::sendExposureAreas(const std::vector<Area>& areas) throw (camera_parameters_exception)
        {
            std::vector<float> areasFloats = areasToFloatVector(areas);
            int code = cm.setExposureAreas(getJNIEnv(), areasFloats, m_camID);

            if (code != cm.SUCCESS_CODE())
            {
                throw camera_parameters_exception("Wrong areas in setExposureAreas provided!", m_camID, 0, "nothing");
            }
        }

        void Parameters::sendLensPositionWithFocusModeFixed(float position) throw (camera_parameters_exception)
        {
            int code = cm.setLensPositionWithFixedFocusMode(getJNIEnv(), position, m_camID);

            if (code != cm.SUCCESS_CODE())
            {
                throw camera_parameters_exception("Focus mode fixed not supported in setting lensPosition", m_camID, 0, "nothing");
            }
        }

        void Parameters::sendRotation(int degrees) throw (camera_exception, camera_parameters_exception)
        {
            if (m_camID == Camera::FRONT)
            {
                switch (degrees)
                {
                    case 0:
                        break;
                    case 90:
                        degrees += 180;
                        break;
                    case 180:
                        break;
                    case 270:
                        degrees += 180;
                        break;
                }
            }

            int code = cm.setRotation(getJNIEnv(), degrees % 360, m_camID);

            if (code != cm.SUCCESS_CODE())
            {
                throw camera_parameters_exception("Bad rotation parameters provided, use 0, 90, 180 or 270", m_camID, 0, "rotation");
            }
        }

        void Parameters::sendAutoFocus() throw (camera_parameters_exception)
        {
            int code = cm.autoFocus(getJNIEnv(), m_camID);

            if (code != cm.SUCCESS_CODE())
            {
                throw camera_parameters_exception("Problem with autoFocus", m_camID, 0, "nothing");
            }
        }

        bool Parameters::sendConfigurationDevice() throw (camera_parameters_exception)
        {
            if (size.field() > 0)
            {
                int code = cm.setPreviewSize(getJNIEnv(), size.getWidth(), size.getHeight(), m_camID);

                if (code == cm.FAILURE_CODE())
                {
                    throw camera_parameters_exception("Cant set provided preview size, pls use one of supported sizes. Use method \"getSupportedPreviewSizes\"", -1, 0, "nothing");
                }
            } else
            {
                throw camera_parameters_exception("Strange error, size field == 0 !", m_camID, 0, "nothing");
            }


            return true;
        }

        float Parameters::loadZoom() const
        {
            if (!cm.isOpened(m_camID))
            {
                return zoom;
            }

            float retZoom;
            cm.getZoom(getJNIEnv(), &retZoom, m_camID);
            return retZoom;
        }

        float Parameters::loadExposureCompensation() const
        {
            if (!cm.isOpened(m_camID))
            {
                return exposureComp;
            }

            float retExpComp;
            cm.getExposureCompensation(getJNIEnv(), &retExpComp, m_camID);
            return retExpComp;
        }

        std::vector<Area> Parameters::loadExposureAreas() const
        {
            if (!cm.isOpened(m_camID))
            {
                return expAreas;
            }

            std::vector<float> areasFloats;
            cm.getExposureAreas(getJNIEnv(), &areasFloats, m_camID);
            return floatVectorToAreas(areasFloats);
        }

        std::vector<Area> Parameters::loadFocusAreas() const
        {
            if (!cm.isOpened(m_camID))
            {
                return focusAreas;
            }

            std::vector<float> focAreasFloats;
            cm.getFocusAreas(getJNIEnv(), &focAreasFloats, m_camID);
            return floatVectorToAreas(focAreasFloats);
        }

        std::vector<float> Parameters::loadFocusDistances() const
        {
            if (!cm.isOpened(m_camID))
            {
                return focusDistances;
            }

            float near, optimal, far;
            cm.getFocusDistances(getJNIEnv(), &near, &optimal, &far, m_camID);
            return std::vector<float>{near, optimal, far};
        }

        float Parameters::loadFocalLength() const
        {
            if (!cm.isOpened(m_camID))
            {
                return focalLength;
            }

            float focl;
            cm.getFocalLength(getJNIEnv(), &focl, m_camID);
            return focl;
        }

        float Parameters::loadHorizontalViewAngle() const
        {
            if (!cm.isOpened(m_camID))
            {
                return hViewAngle;
            }

            float hva;
            cm.getHorizontalViewAngle(getJNIEnv(), &hva, m_camID);
            return hva;
        }

        float Parameters::loadVerticalViewAngle() const
        {
            if (!cm.isOpened(m_camID))
            {
                return vViewAngle;
            }

            float vva;
            cm.getVerticalViewAngle(getJNIEnv(), &vva, m_camID);
            return vva;
        }

        float Parameters::loadLensAperture() const
        {
            if (!cm.isOpened(m_camID))
            {
                return lensAperture;
            }

            float val;
            cm.getLensAperture(getJNIEnv(), &val, m_camID);
            return val;
        }

        float Parameters::loadLensPosition() const
        {
            if (!cm.isOpened(m_camID))
            {
                return lensPosition;
            }

            float val;
            cm.getLensPosition(getJNIEnv(), &val, m_camID);
            return val;
        }

        float Parameters::loadFOV() const
        {
            if (!cm.isOpened(m_camID))
            {
                return fov;
            }

            float retfov;
            cm.getFOV(getJNIEnv(), &retfov, m_camID);
            return retfov;
        }

        float Parameters::loadISO() const
        {
            if (!cm.isOpened(m_camID))
            {
                return iso;
            }
            float retISO = 0;
            // cm.getISO bla bla TODO
            return retISO;
        }

        float Parameters::loadExposureDuration() const
        {
            if (!cm.isOpened(m_camID))
            {
                return exposureDuration;
            }
            float retED = 0.0;
            // cm.getExposureDuration... TODO
            return retED;
        }

        /* Picture class */
        bool Picture::sendIsSupportedPictureFormat(FORMAT format, int camID)
        {
            bool supp;
            int formatInt = static_cast<int> (format);
            cm.isSupportedPictureFormat(getJNIEnv(), formatInt, &supp, camID);
            return supp;
        }

        void Picture::sendOpenAlbumInGallery(const std::string& albumName)
        {
            std::stringstream ss;
            Directory dcimDir = Directory::getDirectory(Directory::DCIM); // DCIM is default android camera pic dir, there we will put albums (subdir of DCIM == album)
            ss << dcimDir.getPath() << "/" << albumName << "/";
            LOG("Trying open an album: ", ss.str());
            cm.openGallery(getJNIEnv(), ss.str());
        }

        void Camera::loadConfigurations()
        {
            // loading supported preview sizes (base to create list of possible configurations)
            std::vector<Size> suppSizes;
            std::vector<int> sizes;
            cm.getSupportedPreviewSizes(getJNIEnv(), &sizes, m_camID); // shure is opened now, no need to check return code
            suppSizes = Parameters::intVectorToSizes(sizes);

            std::sort(suppSizes.begin(), suppSizes.end());

            // In Android this parameters are same for every configuration
            float zoom, zoomMax, focalLength;
            bool isSmoothZoomSupp;
            cm.getZoom(getJNIEnv(), &zoom, m_camID);
            cm.getZoomMax(getJNIEnv(), &zoomMax, m_camID);
            cm.isSmoothZoomSupported(getJNIEnv(), &isSmoothZoomSupp, m_camID);
            cm.getFocalLength(getJNIEnv(), &focalLength, m_camID);

            float ev, minEv, maxEv, stepEv;
            cm.getExposureCompensationStep(getJNIEnv(), &stepEv, m_camID);
            cm.getExposureCompensation(getJNIEnv(), &ev, m_camID);
            cm.getMinExposureCompensation(getJNIEnv(), &minEv, m_camID);
            cm.getMaxExposureCompensation(getJNIEnv(), &maxEv, m_camID);

            int maxExpAreas;
            std::vector<float> areasFloats;
            cm.getMaxExposureAreas(getJNIEnv(), &maxExpAreas, m_camID);
            cm.getExposureAreas(getJNIEnv(), &areasFloats, m_camID);

            int maxFocusAreas;
            std::vector<float> focAreasFloats;
            float near, optimal, far;
            cm.getMaxFocusAreas(getJNIEnv(), &maxFocusAreas, m_camID);
            cm.getFocusAreas(getJNIEnv(), &focAreasFloats, m_camID);
            cm.getFocusDistances(getJNIEnv(), &near, &optimal, &far, m_camID);

            float minFPS, maxFPS;
            std::vector<float> suppFPSRangesFloats;
            cm.getPrevievFPSRange(getJNIEnv(), &minFPS, &maxFPS, m_camID);
            cm.getSupportedPreviewFPSRanges(getJNIEnv(), &suppFPSRangesFloats, m_camID);

            float hVA, vVA;
            cm.getHorizontalViewAngle(getJNIEnv(), &hVA, m_camID);
            cm.getVerticalViewAngle(getJNIEnv(), &vVA, m_camID);

            std::vector<int> intVec;
            std::vector<Size> suppPicSizes;

            cm.getSupportedPictureSizes(getJNIEnv(), &intVec, m_camID);
            suppPicSizes = Parameters::intVectorToSizes(intVec);

            std::vector<int> intVecVideoSizes;
            std::vector<Size> suppVideoSizes;
            cm.getSupportedVideoSizes(getJNIEnv(), &intVecVideoSizes, m_camID);
            suppVideoSizes = Parameters::intVectorToSizes(intVecVideoSizes);

            for (int i = 0; i < suppSizes.size(); i++)
            {
                Parameters p(m_camID);
                p.size = suppSizes[i];

                // zoom settings
                p.zoom = zoom;
                p.maxZoom = zoomMax;
                p.smoothlyZoomSupported = isSmoothZoomSupp;
                p.maxOpticalZoom = 1.0f; // above this zoom value, image is upscaling (not optical zoom, but digital)
                p.focalLength = focalLength;

                // exposure settings
                p.exposureCompStep = stepEv;
                p.exposureComp = ev;
                p.minExposureComp = minEv;
                p.maxExposureComp = maxEv;

                p.maxExpAreas = maxExpAreas;
                p.expAreas = p.floatVectorToAreas(areasFloats);

                p.maxFocusAreas = maxFocusAreas;
                p.focusAreas = p.floatVectorToAreas(focAreasFloats);
                p.focusDistances = std::vector<float>{near, optimal, far};

                p.fpsRange = Range(minFPS, maxFPS);
                p.supportedFpsRanges = p.floatVectorToRanges(suppFPSRangesFloats);

                p.hViewAngle = hVA;
                p.vViewAngle = vVA;

                p.lensAperture = p.loadLensAperture();
                p.lensPosition = p.loadLensPosition();

                p.fov = p.loadFOV();

                p.supportedPictureSizes = suppPicSizes;
                p.supportedVideoSizes = suppVideoSizes;

                configurations.push_back(p); // next possible configuration
                //LOG_DEBUG(configurations[i]);
            }

            Parameters pMaxPreview = *std::max_element(configurations.begin(), configurations.end());
            setParameters(pMaxPreview);
        }

        void Camera::delete_camera_impl::operator()(void* p)
        {
            delete static_cast<CameraImpl*> (p);
        }

        bool Camera::setDisplayOrientation(unsigned degrees) throw (camera_exception, camera_parameters_exception)
        {
            if (!m_opened)
            {
                throw camera_exception("Camera is not opened yet", m_camID);
            }

            int code = cm.setDisplayOrientation(getJNIEnv(), degrees, m_camID);

            if (code == cm.FAILURE_CODE())
            {
                throw camera_parameters_exception("Cant set that display degree, pls use 0, 90, 180 or 270", m_camID, degrees, "nothing");
            } else
            {
                this->degrees = degrees;
                return true;
            }
        }

        void Camera::step()
        {
            if (!m_opened)
            {
                return;
            }

            // updateTexture();    // it is adden on callback in hardwareTasks now
            updateTransformMatrix();
            hardwareTasks.invokeAll(); // in loop, thread safe
        }

        void Camera::updateTexture()
        {
            if (!isPreviewing())
                return;

            if (cm.isCameraFrameWaiting(m_camID))
            {
                cm.updateTexture(getJNIEnv(), m_camID); // to java (same tex id - same tex)
                invokeSignalNewFrame();
            }
        }

        void Camera::updateTransformMatrix()
        {
            if (!isPreviewing())
                return;

            if (cm.isNewTransformMatrix(m_camID))
            {
                const float* m = cm.getTransformMatrix(m_camID);
                std::array<float, 16> arr{{
                    m[0], m[4], m[8], m[12],
                    m[1], m[5], m[9], m[13],
                    m[2], m[6], m[10], m[14],
                    m[3], m[7], m[11], m[15]
                }};

                cm.transformMatrixLoaded(m_camID);
                m_rotationMatrix = RotationMatrix(arr, degrees);
                invokeSignalNewTransformMatrix(m_rotationMatrix);
                m_newPreviewViewCreated = false; // sendend once, not need to send again now
            }

            if(m_newPreviewViewCreated){
                invokeSignalNewTransformMatrix(m_rotationMatrix);
                m_newPreviewViewCreated = false;
            }
        }

        void Camera::sendStartPreview() throw (camera_exception)
        {
            cm.start(getJNIEnv(), m_camID);
            m_previewing = true;
            LOG("AGAIN PREVIEWING");
        }

        void Camera::sendStopPreview()
        {
            cm.stop(getJNIEnv(), m_camID);
            m_previewing = false;
        }

        void Camera::sendOpen() throw (camera_permissions_exception, camera_opening_exception, camera_no_device_exception)
        {
            if (m_opened)
            {
                return;
            }

            // open returning texture id of camera preview
            int code = cm.open(getJNIEnv(), m_camID); // TODO add m_withAudio parameter

            if (code == cm.SUCCESS_CODE())
            {
                m_opened = true;
                int texID = cm.getTextureID(getJNIEnv(), m_camID);

                // Creating camera object
                loadConfigurations();
                m_camImpl.reset(new CameraImplAndroid(texID));
                textureOES.reset(new Texture(m_camImpl.get()));
                afterSuccessOpenedCamera();
            } else
            {
                if (code == cm.IS_OPENED_CODE())
                {
                    throw camera_opening_exception("Camera is opened now", m_camID);
                }

                if (code == cm.NO_PERMISSION_CODE())
                {
                    throw camera_opening_exception("No permissions to open camera device in AndroidManifest.xml", m_camID);
                }

                if (code == cm.NO_CAMERA_CODE())
                {
                    throw camera_no_device_exception("No camera device with provided id", m_camID);
                }

                throw camera_opening_exception("Unknown exception !!!", m_camID);
            }
        }


        void Camera::sendRelease()
        {
            if (!m_opened)
                return;

            CameraImpl* camImpl = (CameraImpl*) m_camImpl.get();
            camImpl->release();
            cm.release(getJNIEnv(), m_camID);
        }

        void Camera::sendInit()
        {
            // empty
        }

        int Camera::getNumberOfCameras()
        {
            return cm.getNumberOfCameras(getJNIEnv());
        }

        bool Camera::isLowLightBoostSupported() const throw(camera_exception)
        {
            assertCameraOpened();
            return false;
        }

        void Camera::setLowLightBoost(bool enabled) throw(camera_exception)
        {
            assertCameraOpened();
            // nothing (maeybe in future))
        }

        void Camera::sendCallbackExposureCompensation(const std::function<void()>& cb)
        {
            cm.setCallbackExposureCompensation(cb, m_camID);
        }

        void Camera::sendCallbackLensPosition(const std::function<void()>& cb)
        {
            cm.setCallbackLensPosition(cb, m_camID);
        }

        void Camera::sendCallbackSmoothZoom(const std::function<void()>& cb)
        {
            cm.setCallbackSmoothZoom(cb, m_camID);
        }

        void Camera::sendCallbackAutoFocus(const std::function<void(bool)>& cb)
        {
            cm.setCallbackAutoFocus(cb, m_camID);
        }

        bool androidWorkaround = false;
        void Camera::sendCallbackTakePicture(const std::function<void(int) >& cb)
        {
            bool w = androidWorkaround;
            Camera& thisCam = *this;
            std::function<void(int) > workAroundCalback = [&thisCam, cb] (int result)
            {
                /* workaround*/
                thisCam.pushTask([&thisCam] ()
                {
                    // set preview to setted before take picture (when taking picture, we changing it on smaller to workaround bug)
                    LOG_DEBUG("before send");
                    thisCam.parameters.sendConfigurationDevice();
                    LOG_DEBUG("after send");
                });
                /* workaround end */
                if (cb)
                {
                    cb(result);
                }
            };

            if (w)
            {
                cm.setCallbackTakePicture(workAroundCalback, m_camID);
            } else
            {
                cm.setCallbackTakePicture(cb, m_camID);
            }
        }

        void Camera::sendCallbackISO(const std::function<void()>& cb)
        {
            // cm.setCallback... TODO
        }

        void Camera::sendCallbackExposureDuration(const std::function<void()>& cb)
        {
            // cm.setCallback... TODO
        }

        void Camera::sendCallbackISOAndED(const std::function<void()>& cb)
        {
            // cm.setCallback... TODO
        }

        void Camera::sendCallbackStopVideo(const std::function<void(int) >& cb) {
            cm.setCallbackStopVideo(cb, getCamID());
        }

        void Camera::sendTakePicture(const Picture& picture) throw (camera_parameters_exception)
        {
            // workarounding "old tv" bug after take picture with big preview size */
            if (androidWorkaround)
            {
                const std::vector<Parameters>& configurations = this->getConfigurations();
                Parameters min = *std::min_element(configurations.begin(), configurations.end());
                Size size = min.getPreviewSize();
                LOG_DEBUG("Setting preview size before taking picture: ", size);
                int cod = cm.setPreviewSize(getJNIEnv(), size.getWidth(), size.getHeight(), m_camID);
                if (cod == cm.SUCCESS_CODE())
                {
                    LOG_DEBUG("Change preview size before take picture SUCCESSFULL !!!");
                }
            }
            /*end of workarounding*/

            std::stringstream ss;
            if (picture.saveInAlbum())
            {
                Directory dcimDir = Directory::getDirectory(Directory::DCIM); // DCIM is default android camera pic dir, there we will put albums (subdir of DCIM == album)
                std::string albumName = picture.getAlbumName();
                if (albumName != "")
                {
                    try
                    {
                        dcimDir = dcimDir.mkdir(albumName);
                    } catch (directory_exception& e)
                    {
                        throw camera_parameters_exception("CANNOT CREATE ALBUM in DCIM/ path !!!", m_camID, 0, "DCIM/Album");
                    }
                }
                ss << dcimDir.getPath() << "/" << picture.getFullFileName();
                LOG_DEBUG("FILE IN ALBUM: ", ss.str());


            } else
            {
                ss << picture.getFullFileName(); // save in provided directory and file name
                LOG_DEBUG("FILE IN DIR: ", ss.str());
            }

            int width = picture.getSize().getWidth();
            int height = picture.getSize().getHeight();
            int format = static_cast<int> (picture.getFormat());
            int jpgQual = picture.getJPGQuality();
            int filter = static_cast<int> (picture.getFilter());
            int code = cm.takePicture(getJNIEnv(), ss.str(), width, height, format, jpgQual, filter, getCamID());

            if (code != cm.SUCCESS_CODE())
            {
                // callback not invoked
                if (androidWorkaround)
                {
                    this->parameters.sendConfigurationDevice();
                }
                throw camera_parameters_exception("Bad parameters provided for taking picture", m_camID, 0, "take picture parameters");
            }
        }

        void Camera::sendStartVideo(const Video& video) throw (camera_parameters_exception)
        {

            std::stringstream ss;
            if (video.needSaveInAlbum())
            {
                Directory dcimDir = Directory::getDirectory(Directory::DCIM); // DCIM is default android camera pic dir, there we will put albums (subdir of DCIM == album)
                std::string albumName = video.getAlbumName();
                if (albumName != "")
                {
                    try
                    {
                        dcimDir = dcimDir.mkdir(albumName);
                    } catch (directory_exception& e)
                    {
                        throw camera_parameters_exception("CANNOT CREATE ALBUM in DCIM/ path !!!", m_camID, 0, "DCIM/Album");
                    }
                }
                ss << dcimDir.getPath() << "/" << video.getFullFileName();
                LOG_DEBUG("FILE IN ALBUM: ", ss.str());
            } else
            {
                ss << video.getFullFileName(); // save in provided directory and file name
                throw camera_parameters_exception("NOT IMPLEMENTED YET", m_camID, 0, "ERROR");
            }

            Size size = video.getSize();
            LOG_DEBUG("Taking video with size: ", size);
            int code = cm.startVideo(getJNIEnv(), ss.str(), size.getWidth(), size.getHeight(), getCamID());
            if (code != cm.SUCCESS_CODE())
            {
                throw camera_parameters_exception("Error while starting taking video", m_camID, 0, "video parameters");
            }
        }

        void Camera::sendStopVideo()
        {
            cm.stopVideo(getJNIEnv(), getCamID());
        }

        void Camera::sendCallbackOnNewFrame()
        {
            Camera& cam = *this;

            std::function<void() > cb = [&cam] ()
            {
                cam.pushTask([&cam] ()
                {
                    cam.updateTexture();
                });
            };
            cm.setCallbackOnNewFrame(cb, getCamID());
        }

        std::string Camera::getFlatten() const
        {
            std::string flatten;
            cm.getFlatten(getJNIEnv(), &flatten, getCamID());
            return flatten;
        }

        Directory Directory::getUserDataDirectory()
        {
            std::string path;
            cm.getUserDataDirectory(getJNIEnv(), &path);
            return Directory(path);
        }

        Directory Directory::getCacheDirectory()
        {
            std::string path;
            cm.getDownloadCacheDirectory(getJNIEnv(), &path);
            return Directory(path);
        }

        Directory Directory::getExternalStorageDirectory()
        {
            std::string path;
            cm.getExternalStorageDirectory(getJNIEnv(), &path);
            return Directory(path);
        }

        Directory Directory::getDirectory(int type)
        {
            std::string path;
            cm.getExternalStoragePublicDirectory(getJNIEnv(), type, &path);
            return Directory(path);
        }

        Directory Directory::mkdir(const std::string& name) throw (directory_exception)
        {
            std::stringstream ss;
            ss << getPath() << "/" << name;

            int code = cm.mkdir(getJNIEnv(), ss.str());
            if (code != cm.SUCCESS_CODE())
            {
                throw directory_exception(std::string("Cannot create directory: \"" + ss.str() + "\""));
            }

            return Directory(ss.str());
        }
    } // end of namespace cam
} // end of namespace aex
