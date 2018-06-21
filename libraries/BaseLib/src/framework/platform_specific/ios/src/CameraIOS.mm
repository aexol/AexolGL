//
//  CameraIOS.cpp
//  AexolGL
//
//  Created by Dariusz Murawski on 18/03/15.
//

#include <stdio.h>
#include <sstream>
#include <cstdlib>
#include <vector>

#include <SDL.h>
#include <SDL_syswm.h>

#include <aex/Logger.hpp>
#include <aex/Camera.hpp>
#include <aex/Texture.hpp>

#include "CameraImplIOS_Y_UV.hpp"
#import "../include/CameraController.hh"
#import "../include/IOSUtils.hh"

std::vector<CameraController*> controllers;

namespace aex
{
    namespace cam
    {

        bool Parameters::isSupported(int param) const
        {
            int code = [controllers[m_camID] isParamSupported : param];

            if (code == [controllers[m_camID] codeSuccess])
            {
                return true;
            } else
            {
                return false; // TODO maybe an exception if something more is wrong in code
            }
        }

        void Parameters::sendAntibanding(int param) throw (camera_parameters_exception)
        {
            int code = [controllers[m_camID] setParam : param];

            if (code == [controllers[m_camID] codeFailure])
            {
                throw camera_parameters_exception(std::string("Parameter is not supported, pls use method \"isSupported\" before setting it"), m_camID, 0, "antibanding param");
            }
        }

        void Parameters::sendColorEffect(int param) throw (camera_parameters_exception)
        {
            int code = [controllers[m_camID] setParam : param];

            if (code == [controllers[m_camID] codeFailure])
            {
                throw camera_parameters_exception(std::string("Parameter is not supported, pls use method \"isSupported\" before setting it"), m_camID, 0, "colorEffect param");
            }
        }

        void Parameters::sendWhiteBalance(int param) throw (camera_parameters_exception)
        {
            int code = [controllers[m_camID] setParam : param];

            if (code == [controllers[m_camID] codeFailure])
            {
                throw camera_parameters_exception(std::string("Parameter is not supported, pls use method \"isSupported\" before setting it"), m_camID, 0, "whiteBalance param");
            }
        }

        void Parameters::sendFlashMode(int param) throw (camera_parameters_exception)
        {
            int code = [controllers[m_camID] setParam : param];

            if (code == [controllers[m_camID] codeFailure])
            {
                throw camera_parameters_exception(std::string("Parameter is not supported, pls use method \"isSupported\" before setting it"), m_camID, 0, "flashMode param");
            }
        }

        void Parameters::sendSceneMode(int param) throw (camera_parameters_exception)
        {
            int code = [controllers[m_camID] setParam : param];

            if (code == [controllers[m_camID] codeFailure])
            {
                throw camera_parameters_exception(std::string("Parameter is not supported, pls use method \"isSupported\" before setting it"), m_camID, 0, "sceneMode param");
            }
        }

        void Parameters::sendFocusMode(int param) throw (camera_parameters_exception)
        {
            int code = [controllers[m_camID] setParam : param];

            if (code == [controllers[m_camID] codeFailure])
            {
                throw camera_parameters_exception(std::string("Parameter is not supported, pls use method \"isSupported\" before setting it"), m_camID, 0, "focusMode param");
            }
        }

        void Parameters::sendTorchMode(int param) throw (camera_parameters_exception)
        {
            int code = [controllers[m_camID] setParam : param];

            if (code == [controllers[m_camID] codeFailure])
            {
                throw camera_parameters_exception(std::string("Parameter is not supported, pls use method \"isSupported\" before setting it"), m_camID, 0, "torchMode param");
            }
        }

        void Parameters::sendExposureMode(int param) throw (camera_parameters_exception)
        {
            int code = [controllers[m_camID] setParam : param];

            if (code == [controllers[m_camID] codeFailure])
            {
                throw camera_parameters_exception(std::string("Parameter is not supported, pls use method \"isSupported\" before setting it"), m_camID, 0, "exposureMode param");
            }
        }

        void Parameters::sendZoom(float zoom) throw (camera_parameters_exception)
        {
            [controllers[m_camID] setZoomFactor : zoom];
        }

        void Parameters::sendZoomSmoothly(float zoom) throw (camera_parameters_exception)
        {
            [controllers[m_camID] setZoomFactorSmoothly : zoom : smoothZoomRate];
        }

        void Parameters::sendExposureCompensation(float ev) throw (camera_parameters_exception)
        {
            int code = [controllers[m_camID] setExposureCompensation : ev];

            if (code == [controllers[m_camID] codeFailure])
            {
                throw camera_parameters_exception("Bad exposure value provided!", -1, 0, "nothing");
            }
        }

        void Parameters::sendISO(float iso) throw (camera_parameters_exception)
        {
            int code = [controllers [m_camID] setISO : iso];
            if (code == [controllers [m_camID] codeFailure])
            {
                throw camera_parameters_exception("Bad iso value provided!", -1, 0, "nothing");
            }
        }

        void Parameters::sendExposureDuration(float ed) throw (camera_parameters_exception)
        {
            int code = [controllers[m_camID] setExposureDuration : ed];
            if (code == [controllers[m_camID] codeFailure])
            {
                throw camera_parameters_exception("Bad exposureDuration value provided!", -1, 0, "nothing");
            }
        }

        void Parameters::sendISOAndED(float iso, float ed) throw (camera_parameters_exception)
        {
            int code = [controllers[m_camID] setISOAndED : iso : ed ];
            if (code == [controllers[m_camID] codeFailure])
            {
                throw camera_parameters_exception("Bad iso or ed value provided!", -1, 0, "nothing");
            }
        }

        void Parameters::sendFocusAreas(const std::vector<Area>& areas) throw (camera_parameters_exception)
        {

            if (areas.size() > 0)
            {
                FPoint2D center = areas[0].getCenter();
                int code = [controllers[m_camID] setFocusPointOfInterest : center.x() : center.y()];

                if (code != [controllers[m_camID] codeSuccess])
                {
                    throw camera_parameters_exception("Wrong areas in setExposureAreas provided!", m_camID, 0, "nothing");
                }
            } else
            {
                if (maxFocusAreas > 0)
                {
                    [controllers[m_camID] setExposurePointOfInterest : 0.5f : 0.5f]; // default
                }
            }
        }

        void Parameters::sendPreviewFPSRange(Range range) throw (camera_parameters_exception)
        {
            int code = [controllers[m_camID] setFPSRange : range.getMin() : range.getMax()];

            if (code == [controllers[m_camID] codeFailure])
            {
                throw camera_parameters_exception("Wrong fps range in setPreviewFpsRange provided!", m_camID, 0, "nothing");
            }
        }

        void Parameters::sendExposureAreas(const std::vector<Area>& areas) throw (camera_parameters_exception)
        {
            if (areas.size() > 0)
            {
                FPoint2D center = areas[0].getCenter();
                int code = [controllers[m_camID] setExposurePointOfInterest : center.x() : center.y()];

                if (code != [controllers[m_camID] codeSuccess])
                {
                    throw camera_parameters_exception("Wrong areas in setExposureAreas provided!", m_camID, 0, "nothing");
                }
            } else
            {
                if (maxExpAreas > 0)
                {
                    [controllers[m_camID] setExposurePointOfInterest : 0.5f : 0.5f]; // default
                }
            }
        }

        void Parameters::sendLensPositionWithFocusModeFixed(float position) throw (camera_parameters_exception)
        {
            int code = [controllers[m_camID] setLensPositionWithFixedFocusMode : position];

            if (code == [controllers[m_camID] codeFailure])
            {
                throw camera_parameters_exception("Focus mode fixed not supported in setting lensPosition", m_camID, 0, "nothing");
            }
        }

        void Parameters::sendRotation(int degrees) throw (camera_exception, camera_parameters_exception)
        {
            //degrees = (degrees + 90) % 360;
            int code = [controllers[m_camID] setRotation : degrees];

            if (code != [controllers[m_camID] codeSuccess])
            {
                throw camera_parameters_exception("Bad rotation parameters provided, use 0, 90, 180 or 270", m_camID, 0, "rotation");
            }
        }

        void Parameters::sendAutoFocus() throw (camera_parameters_exception)
        {

            int code = [controllers[m_camID] autoFocus];

            if (code != [controllers[m_camID] codeSuccess])
            {
                throw camera_parameters_exception("Problem with autoFocus", m_camID, 0, "nothing");
            }
        }

        bool Parameters::sendConfigurationDevice() throw (camera_parameters_exception)
        {
            if (size.field() > 0)
            {
                [controllers[m_camID] setFormatAt : deviceConfigID]; // it will set AVCaptureDeviceFormat which have needed preview size
            } else
            {
                throw camera_parameters_exception("Strange error, preview size field == 0 !", -1, 0, "nothing");
            }

            return true;
        }

        bool isOpened(int camID)
        {
            if (controllers.size() <= camID)
            {
                return false;
            } else
            {
                return [controllers[camID] isOpened : camID];
            }
        }

        float Parameters::loadZoom() const
        {
            if (!isOpened(m_camID))
            {
                return zoom;
            }

            float zoom = [controllers[m_camID] getZoom];
            return zoom;
        }

        float Parameters::loadExposureCompensation() const
        {
            if (!isOpened(m_camID))
            {
                return exposureComp;
            }

            float exposureComp = [controllers[m_camID] getExposureCompensation];
            return exposureComp;
        }

        std::vector<Area> Parameters::loadExposureAreas() const
        {
            if (!isOpened(m_camID))
            {
                return expAreas;
            }

            if ([controllers[m_camID] isSupportedExposurePointOfInterest] == [controllers[m_camID] codeSuccess])
            {
                float x = [controllers[m_camID] getExposurePointOfInterestX];
                float y = [controllers[m_camID] getExposurePointOfInterestY];
                return std::vector<Area>{Area(FPoint2D(x, y))};
            } else
            {
                return std::vector<Area>{Area(FPoint2D(0.5f, 0.5f))};
            }
        }

        std::vector<Area> Parameters::loadFocusAreas() const
        {
            if (!isOpened(m_camID))
            {
                return focusAreas;
            }

            if ([controllers[m_camID] isSupportedFocusPointOfInterest] == [controllers[m_camID] codeSuccess])
            {
                float x = [controllers[m_camID] getFocusPointOfInterestX];
                float y = [controllers[m_camID] getFocusPointOfInterestY];
                return std::vector<Area>{Area(FPoint2D(x, y))};
            } else
            {
                return std::vector<Area>{Area(FPoint2D(0.5f, 0.5f))};
            }
        }

        std::vector<float> Parameters::loadFocusDistances() const
        {
            if (!isOpened(m_camID))
            {
                return focusDistances;
            }

            float near = -1, optimal = -1, far = -1;
            return std::vector<float>{near, optimal, far};
        }

        float Parameters::loadFocalLength() const
        {
            if (!isOpened(m_camID))
            {
                return focalLength;
            }

            return [controllers[m_camID] getFocalLength];
        }

        float Parameters::loadHorizontalViewAngle() const
        {
            if (!isOpened(m_camID))
            {
                return hViewAngle;
            }

            return [controllers[m_camID] getHorizontalViewAngle];
        }

        float Parameters::loadVerticalViewAngle() const
        {
            if (!isOpened(m_camID))
            {
                return vViewAngle;
            }

            return [controllers[m_camID] getVerticalViewAngle];
        }

        float Parameters::loadLensAperture() const
        {
            if (!isOpened(m_camID))
            {
                return lensAperture;
            }

            return [controllers[m_camID] getLensAperture];
        }

        float Parameters::loadLensPosition() const
        {
            if (!isOpened(m_camID))
            {
                return lensPosition;
            }

            return [controllers[m_camID] getLensPosition];
        }

        float Parameters::loadFOV() const
        {
            if (!isOpened(m_camID))
            {
                return fov;
            }

            return [controllers[m_camID] getFOVForFormatAt : deviceConfigID];
        }

        float Parameters::loadISO() const
        {
            if (!isOpened(m_camID))
            {
                return iso;
            }

            return [controllers[m_camID] getISO];
        }

        float Parameters::loadExposureDuration() const
        {
            if (!isOpened(m_camID))
            {
                return exposureDuration;
            }

            return [controllers[m_camID] getExposureDuration];
        }

        /* Picture class */
        bool Picture::sendIsSupportedPictureFormat(FORMAT format, int camID)
        {
            bool supp = [controllers[camID] isSupportedPictureFormat : (int) format];
            return supp;
        }

        void Picture::sendOpenAlbumInGallery(const std::string& albumName)
        {
            // std::function<void()> cbOnGalleryQuit = [&window](){
            //    //SDL_RestoreWindow((SDL_Window*)window->getSDLWindow());
            //    LOG_DEBUG("cb on quit gallery invoked.");
            // };
            // [controllers[camID] openAlbumInGallery : albumName : nil : cbOnGalleryQuit];

            [IOSUtils openAlbumInGallery : albumName];

            // Opening new app instance, pausing this.
            // UIApplication *ourApplication = [UIApplication sharedApplication];
            // NSString *ourPath = @"photos-redirect://";
            // NSURL *ourURL = [NSURL URLWithString:ourPath];
            // [ourApplication openURL:ourURL];
        }

        void Camera::loadConfigurations()
        {
            // 420v: need kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange param - Y component only uses the byte values from 16 to 235
            // 420f: need kCVPixelFormatType_420YpCbCr8BiPlanarFullRange param - Y component uses bytes from 0 to 255 - WE WANT THIS !!!
            int formatsNum = [controllers[m_camID] getFormatsNum];

            std::vector<Size> allPreviewSizes;
            std::vector<std::vector<Size>*> suppSizesVectors;
            for (int i = 0; i < formatsNum; i++)
            {
                if ([controllers[m_camID] isYColorFullRangeFormat420fAt : i]) // if 420f format
                {
                    //LOG("----+++--CHOSEN INDEX", i);
                    Parameters p(m_camID);
                    p.deviceConfigID = i;

                    // zoom settings
                    p.zoom = [controllers[m_camID] getZoom];
                    p.maxZoom = [controllers[m_camID] getZoomMaxForFormatAt : i];
                    p.size = Size([controllers[m_camID] getPreviewWidhForFormatAt : i], [controllers[m_camID] getPreviewHeightForFormatAt : i]);
                    p.maxOpticalZoom = [controllers[m_camID] getZoomUpscaleTresholdForFormatAt : i];
                    p.smoothlyZoomSupported = [controllers[m_camID] isSmoothlyZoomSupported];
                    p.focalLength = [controllers[m_camID] getFocalLength];

                    // exposure settings
                    p.exposureCompStep = [controllers[m_camID] getExposureCompensationStep];
                    p.exposureComp = [controllers[m_camID] getExposureCompensation];
                    p.minExposureComp = [controllers[m_camID] getMinExposureCompensation];
                    p.maxExposureComp = [controllers[m_camID] getMaxExposureCompensation];

                    p.minISO = [controllers [m_camID] getMinISOForFormatAt : i];
                    p.maxISO = [controllers [m_camID] getMaxISOForFormatAt : i];
                    p.iso = [controllers [m_camID] getISO];

                    p.minExposureDuration = [controllers [m_camID] getMinExposureDurationForFormatAt : i];
                    p.maxExposureDuration = [controllers [m_camID] getMaxExposureDurationForFormatAt : i];
                    p.exposureDuration = [controllers [m_camID] getExposureDuration];

                    if ([controllers[m_camID] isSupportedExposurePointOfInterest] == [controllers[m_camID] codeSuccess])
                    {
                        p.maxExpAreas = 1;
                        float x = [controllers[m_camID] getExposurePointOfInterestX];
                        float y = [controllers[m_camID] getExposurePointOfInterestY];
                        p.expAreas = std::vector<Area>{Area(FPoint2D(x, y))};
                    } else
                    {
                        p.maxExpAreas = 0;
                    }


                    if ([controllers[m_camID] isSupportedFocusPointOfInterest] == [controllers[m_camID] codeSuccess])
                    {
                        p.maxFocusAreas = 1;
                        float x = [controllers[m_camID] getFocusPointOfInterestX];
                        float y = [controllers[m_camID] getFocusPointOfInterestY];
                        p.focusAreas = std::vector<Area>{Area(FPoint2D(x, y))};
                    } else
                    {
                        p.maxFocusAreas = 0;
                    }

                    // not supported on iOS so all are -1
                    float near = -1, optimal = -1, far = -1;
                    p.focusDistances = std::vector<float>{near, optimal, far};

                    std::vector<float> rvec = [controllers[m_camID] getFPSRangeForFormatAt : i];
                    p.fpsRange = Range(rvec[0], rvec[1]);

                    std::vector<float> suppRVec = [controllers[m_camID] getSupportedFPSRangesForFormatAt : i];
                    p.supportedFpsRanges = p.floatVectorToRanges(suppRVec);

                    p.hViewAngle = [controllers[m_camID] getHorizontalViewAngle];
                    p.vViewAngle = [controllers[m_camID] getVerticalViewAngle];

                    p.lensAperture = [controllers[m_camID] getLensAperture];
                    p.lensPosition = [controllers[m_camID] getLensPosition];

                    p.fov = [controllers[m_camID] getFOVForFormatAt : i];

                    p.supportedPictureSizes.push_back(p.size); // only one supp pic size (equal preview size) for configuration
                    allPreviewSizes.push_back(p.size);
                    suppSizesVectors.push_back(&p.supportedVideoSizes);

                    configurations.push_back(p); // next possible cinfiguration
                }
                //LOG(configurations[i]);
            }

            for (int i = 0; i < configurations.size(); i++)
            {
                configurations[i].supportedVideoSizes = allPreviewSizes;
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

            if ((degrees % 90) != 0)
            {
                throw camera_parameters_exception("Cant set that display degree, pls use 0, 90, 180 or 270", m_camID, degrees, "nothing");
            } else
            {
                this->degrees = degrees;
                this->degreesChanged = true;
                return true;
            }
        }

        void Camera::step()
        {
            if (!m_opened)
            {
                return;
            }

            updateTransformMatrix();

            hardwareTasks.invokeAll(); // in loop, thread safe
        }

        void Camera::updateTexture()
        {
            if (!isPreviewing())
                return;

            // if ([controllers[m_camID] isFrameAvaible]) // no needed, is on callback
            // {
                LumaChromaTexIds lc = [controllers[m_camID] getLumaChromaTextureID]; // lock between this method and captureOutptut in CameraController could be smaller? Critical section.
                CameraImplIOS_Y_UV* camImpl = (CameraImplIOS_Y_UV*) m_camImpl.get();
                camImpl->nextCameraFrame(lc.luma, lc.chroma);
                invokeSignalNewFrame();
          //  }
        }

        void Camera::updateTransformMatrix()
        {
            if (!isPreviewing())
                return;

            if (degreesChanged)
            {
                degreesChanged = false;

                bool bfrontCam = (m_camID == Camera::FRONT) ? true : false;

                RotationMatrix rotMat = RotationMatrix((degrees) % 360, bfrontCam);
                invokeSignalNewTransformMatrix(rotMat);
                m_newPreviewViewCreated = false; // sendend once, not need to send again now
            }

            if(m_newPreviewViewCreated){
                invokeSignalNewTransformMatrix(m_rotationMatrix);
                m_newPreviewViewCreated = false;
            }
        }

        void Camera::sendStartPreview() throw (camera_exception)
        {
            if (!m_opened)
            {
                throw camera_exception("Camera is not opened yet", m_camID);
            }

            [controllers[m_camID] startPreview];
            m_previewing = true;

        }

        void Camera::sendStopPreview()
        {
            [controllers[m_camID] stopPreview];
            m_previewing = false;
        }

        void Camera::sendOpen() throw (camera_permissions_exception, camera_opening_exception, camera_no_device_exception)
        {
            if (m_opened)
            {
                return;
            }

            if (m_camID >= controllers.size())
            {
                controllers.resize(m_camID + 1, nullptr);
            }

            if(controllers[m_camID] == nullptr )
                controllers[m_camID] = [[CameraController alloc] init];

                int code = [controllers[m_camID] openCamera : m_camID : m_withAudio];

            if (code == [controllers[m_camID] codeSuccess])
            {
                m_opened = true;
                loadConfigurations();
                CameraImplIOS_Y_UV* camImpl = new CameraImplIOS_Y_UV(GL_TEXTURE_2D);
                m_camImpl.reset(camImpl);

                camImpl->setNextSettedTextureCode(CameraImplIOS_Y_UV::SET_Y_TEXTURE_CODE);
                textureY.reset(new Texture(m_camImpl.get()));

                camImpl->setNextSettedTextureCode(CameraImplIOS_Y_UV::SET_UV_TEXTURE_CODE);
                textureUV.reset(new Texture(m_camImpl.get()));

                [controllers[m_camID] setCamDevActiveTexUnit : 0]; // Unneded here, but it documments, which TexUnit is using in CamDev Ios API for Y UV tex creating

                afterSuccessOpenedCamera();
            } else
            {
                if(code == [controllers[m_camID] codeFailurePermissions]){
                    throw camera_permissions_exception("Camera permissions exception ", m_camID);
                }else{
                    throw camera_opening_exception("Camera IOS opening exception ", m_camID);
                }
            }
        }

        void Camera::sendRelease()
        {
            if (!m_opened)
                return;
            //textureCamDevTexUnitLock->unbind();  // TODO if camera is preview works good, remove it later
            CameraImpl* camImpl = (CameraImpl*) m_camImpl.get();
            camImpl->release();
            [controllers[m_camID] releaseCamera];

            // TODO - removed because reopening 4 times dataOutput cases crash without log (ios err ?, to comment out in new iOS version)
//            [controllers[m_camID] release]; // it is delete
//            controllers[m_camID] = NULL;

            // check if any opened, if not, clear static vars (for app closing)
//            bool anyOpened = false;
//            for (int i = 0; i < controllers.size(); i++)
//            {
//                if (controllers[i])
//                {
//                    anyOpened = true;
//                }
//            }
//            if (!anyOpened)
//            {
//                controllers.clear();
//            }
        }

        void Camera::sendInit()
        {
            if(!isInitialized()){
                // if(!textureCamDevTexUnitLock)  // TODO if camera is preview works good, remove it later
                // {
                //     int emptyData[4];
                //     textureCamDevTexUnitLock.reset(new Texture());
                //     textureCamDevTexUnitLock->setData(emptyData, 1, 1);
                //     textureCamDevTexUnitLock->bind(); // should bind TexUnit 0 for all app work time
                // }
            }
        }

        int Camera::getNumberOfCameras()
        {
            return [CameraController getCamerasNumber];
        }


        bool Camera::isLowLightBoostSupported() const throw(camera_exception)
        {
            assertCameraOpened();
            return [controllers[m_camID] isLowLightBoostSupported];
        }

        void Camera::setLowLightBoost(bool enabled) throw(camera_exception)
        {
            assertCameraOpened();
            [controllers[m_camID] setLowLightBoost:enabled];
        }

        void Camera::sendCallbackExposureCompensation(const std::function<void()>& cb)
        {
            [controllers[m_camID] setCallbackExposureCompensation : cb];
        }

        void Camera::sendCallbackLensPosition(const std::function<void()>& cb)
        {
            [controllers[m_camID] setCallbackLensPosition : cb];
        }

        void Camera::sendCallbackSmoothZoom(const std::function<void()>& cb)
        {
            [controllers [m_camID] setCallbackSmoothZoom : cb];
        }

        void Camera::sendCallbackAutoFocus(const std::function<void(bool)>& cb)
        {
            [controllers[m_camID] setCallbackAutoFocus : cb];
        }

        void Camera::sendCallbackTakePicture(const std::function<void(int) >& cb)
        {
            [controllers[m_camID] setCallbackTakePicture : cb];
        }

        void Camera::sendCallbackISO(const std::function<void()>& cb)
        {
            [controllers[m_camID] setCallbackISO : cb];
        }

        void Camera::sendCallbackExposureDuration(const std::function<void()>& cb)
        {
            [controllers[m_camID] setCallbackExposureDuration : cb];
        }

        void Camera::sendCallbackISOAndED(const std::function<void()>& cb)
        {
            [controllers[m_camID] setCallbackISOAndED : cb];
        }

        void Camera::sendCallbackStopVideo(const std::function<void(int)>& cb)
        {
            [controllers[m_camID] setCallbackStopVideo : cb];
        }

        void Camera::sendTakePicture(const Picture& picture) throw (camera_parameters_exception)
        {
            int width = picture.getSize().getWidth();
            int height = picture.getSize().getHeight();
            int format = static_cast<int> (picture.getFormat());
            int jpgQual = picture.getJPGQuality();
            int filter = static_cast<int> (picture.getFilter());

            int code;

            if (picture.saveInAlbum())
            {
                // only provide album name, pic name is generated by iOS code automatically in album
                code = [controllers [m_camID] takePicture : picture.getFullFileName() : picture.getAlbumName() : width : height : format : jpgQual : filter : true];
            } else
            {
                code = [controllers [m_camID] takePicture : picture.getFullFileName() : picture.getAlbumName() : width : height : format : jpgQual : filter : false];
            }
            if (code != [controllers[m_camID] codeSuccess])
            {
                throw camera_parameters_exception("Bad parameters provided for taking picture", m_camID, 0, "take picture parameters");
            }
        }

        void Camera::sendStartVideo(const Video& video) throw (camera_parameters_exception)
        {
            LOG_DEBUG("Starting video with size: ", video.getSize().getWidth(), "-", video.getSize().getHeight());
            [controllers[getCamID()] startVideo : video.getFullFileName() : video.getAlbumName() : video.needSaveInAlbum() : video.getSize().getWidth() : video.getSize().getHeight()];
        }

        void Camera::sendStopVideo()
        {
            [controllers[getCamID()] stopVideo];
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

            [controllers[m_camID] setCallbackOnNewFrame : cb];
        }

        std::string Camera::getFlatten() const
        {
            std::string flatten;
            // TODO implement it in CameraController. Now is not too much important
            return flatten;
        }

        Directory Directory::getUserDataDirectory()
        {
            NSString *dir = NSHomeDirectory();
            std::string retPath([dir UTF8String]);

            return Directory(retPath);
        }

        Directory Directory::getCacheDirectory()
        {
            NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
            NSString *dir = [paths objectAtIndex : 0];
            std::string retPath([dir UTF8String]);

            return Directory(retPath);
        }

        Directory Directory::getExternalStorageDirectory()
        {
            NSArray *paths = NSSearchPathForDirectoriesInDomains(NSSharedPublicDirectory, NSUserDomainMask, YES);
            NSString *dir = [paths objectAtIndex : 0];
            std::string retPath([dir UTF8String]);

            return Directory(retPath);
        }

        Directory Directory::getDirectory(int type)
        {
            NSArray *paths;
            NSString *dir;
            switch (type)
            {
                case Directory::ALARMS:
                    dir = NSHomeDirectory();
                    break;
                case Directory::DCIM:
                    dir = NSHomeDirectory();
                    dir = [dir stringByAppendingPathComponent : @"DCIM"];
                    break;
                case Directory::DOWNLOADS:
                    paths = NSSearchPathForDirectoriesInDomains(NSDownloadsDirectory, NSUserDomainMask, YES);
                    dir = [paths objectAtIndex : 0];
                    break;
                case Directory::MOVIES:
                    paths = NSSearchPathForDirectoriesInDomains(NSMoviesDirectory, NSUserDomainMask, YES);
                    dir = [paths objectAtIndex : 0];
                    break;
                case Directory::MUSIC:
                    paths = NSSearchPathForDirectoriesInDomains(NSMusicDirectory, NSUserDomainMask, YES);
                    dir = [paths objectAtIndex : 0];
                    break;
                case Directory::NOTIFICATIONS:
                    dir = NSHomeDirectory();
                    break;
                case Directory::PICTURES:
                    paths = NSSearchPathForDirectoriesInDomains(NSPicturesDirectory, NSUserDomainMask, YES);
                    dir = [paths objectAtIndex : 0];
                    break;
                case Directory::PODCAST:
                    dir = NSHomeDirectory();
                    break;
                case Directory::RINGTONES:
                    dir = NSHomeDirectory();
                    break;
                case Directory::DOCUMENTS:
                    paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
                    dir = [paths objectAtIndex : 0];
                    break;

                default:
                    // paths = NSSearchPathForDirectoriesInDomains(NSUserDirectory, NSUserDomainMask, YES);
                    dir = NSHomeDirectory();
                    break;
            }


            std::string retPath([dir UTF8String]);

            return Directory(retPath);
        }

        Directory Directory::mkdir(const std::string& name) throw (directory_exception)
        {
            std::stringstream ss;
            ss << getPath() << "/" << name;
            std::string fullPath = ss.str();
            std::string albumNameCpp;

            NSString* filePath = [NSString stringWithUTF8String : getPath().c_str()];
            NSString* directoryName = [NSString stringWithUTF8String : name.c_str()];
            NSString *newDir = [filePath stringByAppendingPathComponent : directoryName];

            BOOL isDir;
            BOOL exists = [[NSFileManager defaultManager] fileExistsAtPath : newDir isDirectory : &isDir];
            if (exists)
            {
                if (isDir)
                {
                    return Directory(ss.str());
                } else
                    throw directory_exception(std::string("Cannot create directory: \"" + ss.str() + " because there is an a file of that name"));
            }

            NSError *error;
            if (![[NSFileManager defaultManager] createDirectoryAtPath : newDir
                withIntermediateDirectories : YES
                attributes : nil
                error : &error])
            {
                std::string errorStr = [[error description] UTF8String];
                throw directory_exception(std::string("Cannot create directory: \"" + ss.str() + "\"" + errorStr));
            }

            return Directory(fullPath);
        }



    } // end of namespace cam
} // end of namespace aex
