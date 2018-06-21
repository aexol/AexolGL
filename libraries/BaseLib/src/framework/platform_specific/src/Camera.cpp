#include <aex/Camera.hpp>
#include <aex/Logger.hpp>
#include <aex/Texture.hpp>
#include <sstream>
#include <cmath>
#include <thread>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

#include "../include/CameraImpl.hpp"
#include "../include/CamPrevView.hpp"

#ifndef ABS
#define ABS(x) ( (x)<0 ? -(x) : (x))
#endif

namespace aex
{
    namespace cam
    {
        // trim from start

        static inline std::string ltrim(const std::string &ss)
        {
            std::string s = ss;
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
            return s;
        }

        // trim from end

        static inline std::string rtrim(const std::string &ss)
        {
            std::string s = ss;
            s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
            return s;
        }

        // trim from both ends

        static inline std::string trim(const std::string &ss)
        {
            return ltrim(rtrim(ss));
        }

        camera_exception::camera_exception(const std::string& str, int camID) : runtime_error(str), cameraID(camID)
        {
        }

        camera_exception::camera_exception(const char* str, int camID) : runtime_error(str), cameraID(camID)
        {
        }

        camera_opening_exception::camera_opening_exception(const std::string& str, int camID) : camera_exception(str, camID)
        {
        }

        camera_opening_exception::camera_opening_exception(const char* str, int camID) : camera_exception(str, camID)
        {
        }

        camera_permissions_exception::camera_permissions_exception(const std::string& str, int camID) : camera_exception(str, camID)
        {
        }

        camera_permissions_exception::camera_permissions_exception(const char* str, int camID) : camera_exception(str, camID)
        {
        }

        camera_no_device_exception::camera_no_device_exception(const std::string& str, int camID) : camera_exception(str, camID)
        {
        }

        camera_no_device_exception::camera_no_device_exception(const char* str, int camID) : camera_exception(str, camID)
        {
        }

        camera_parameters_exception::camera_parameters_exception(const std::string& str, int camID, int badDeg, std::string noSupPar) :
        camera_exception(str, camID),
        m_badDegrees(badDeg),
        noSupportedParam(noSupPar)
        {
        }

        camera_parameters_exception::camera_parameters_exception(const char* str, int camID, int badDeg, std::string noSupPar) :
        camera_exception(str, camID),
        m_badDegrees(badDeg),
        noSupportedParam(noSupPar)
        {
        }

        int camera_parameters_exception::getBadDegree() const
        {
            return m_badDegrees;
        }

        std::string camera_parameters_exception::getNoSupportedParam() const
        {
            return noSupportedParam;
        }

        const double Ratio::EPSILON = 0.00001;

        Ratio::Ratio(double w, double h) :
        m_w(w),
        m_h(h)
        {
            calculateMinIntValues();
        }

        double Ratio::difference(const Ratio& r2) const
        {
            return ABS(this->aspect() - r2.aspect());
        }

        Size Ratio::createSizeWidh(unsigned width) const
        {
            if (m_w > 0)
            {
                return Size(width, round((width * m_h) / m_w));
            } else
            {
                return Size(1, 1);
            }
        }

        Size Ratio::createSizeHeight(unsigned height) const
        {
            if (m_h > 0)
            {
                return Size(round((m_w * height) / m_h), height);
            } else
            {
                return Size(1, 1);
            }
        }

        unsigned Ratio::getW() const
        {
            return m_minIntW;
        };

        unsigned Ratio::getH() const
        {
            return m_minIntH;
        };

        bool operator==(const Ratio& ratio, const Ratio& ratio2)
        {
            if (ratio.difference(ratio2) < Ratio::EPSILON)
            {
                return true;
            } else
            {
                return false;
            }
        }

        bool operator==(const Ratio& ratio, const Size& size)
        {
            return ratio == size.getRatio();
        }

        bool operator==(const Size& size, const Ratio& ratio)
        {
            return ratio == size.getRatio();
        }

        std::ostream& operator<<(std::ostream& out, const Ratio& r)
        {
            return out << r.getW() << ":" << r.getH();
        }

        double Ratio::aspect() const
        {
            return m_w / m_h;
        }

        void Ratio::calculateMinIntValues()
        {
            bool canDivideMore = true;
            unsigned tmpMinW = m_w;
            unsigned tmpMinH = m_h;

            while (canDivideMore)
            {
                if ((tmpMinW % 2) != 0)
                {
                    canDivideMore = false;
                }

                if ((tmpMinH % 2) != 0)
                {
                    canDivideMore = false;
                }

                if (canDivideMore == false)
                {
                    break;
                }

                tmpMinW /= 2;
                tmpMinH /= 2;
            }

            m_minIntW = tmpMinW;
            m_minIntH = tmpMinH;
        }

        /* Size class */
        Size::Size() :
        m_width(0),
        m_height(0)
        {
        }

        Size::Size(unsigned width, unsigned height)
        {
            this->m_width = width;
            this->m_height = height;
        }

        Size Size::getRotated90() const
        {
            return Size(m_height, m_width);
        }

        int Size::getWidth() const
        {
            return m_width;
        }

        int Size::getHeight() const
        {
            return m_height;
        }

        long long int Size::field() const
        {
            return (long long int) m_width * (long long int) m_height;
        }

        Ratio Size::getRatio() const
        {
            return Ratio(m_width, m_height);
        }

        Size Size::createInternalSizeFor(const Ratio& r2) const
        {
            if (this->getRatio().aspect() > r2.aspect())
            {
                return r2.createSizeHeight(getHeight());
            } else
            {
                return r2.createSizeWidh(getWidth());
            }
        }

        Size Size::createFullFillSizeFor(const Ratio& r2) const
        {
            if (this->getRatio().aspect() < r2.aspect())
            {
                return r2.createSizeHeight(getHeight());
            } else
            {
                return r2.createSizeWidh(getWidth());
            }
        }

        bool Size::operator==(const Size& second) const
        {
            if ((m_width == second.m_width) && (m_height == second.m_height))
            {
                return true;
            }

            return false;
        }

        bool Size::operator!=(const Size& second) const
        {
            return !(*this == second);
        }

        bool Size::operator>(const Size& second) const
        {
            if (this->field() > second.field())
            {
                return true;
            }

            return false;
        }

        bool Size::operator<(const Size& second) const
        {
            return !((*this) > second);
        }

        bool Size::operator>=(const Size& second) const
        {
            if ((*this) == second)
            {
                return true;
            }

            return (*this) > second;
        }

        bool Size::operator<=(const Size& second) const
        {
            if ((*this) == second)
            {
                return true;
            }

            return (*this) < second;
        }

        std::ostream& operator<<(std::ostream& out, const Size& size)
        {
            return out << size.m_width << ":" << size.m_height;
        }

        Area::Area() :
        center(FPoint2D(0.5f, 0.5f)),
        rLeft(0.05f),
        rTop(0.05f),
        rRight(0.05f),
        rBottom(0.05f),
        weight(0.5f)
        {
        }

        Area::Area(FPoint2D center, float xDiameter, float yDiameter, float weight) :
        center(center),
        rLeft(xDiameter / 2),
        rTop(yDiameter / 2),
        rRight(xDiameter / 2),
        rBottom(yDiameter / 2),
        weight(weight)
        {
        }

        Area::Area(FPoint2D center, float radius, float weight) :
        center(center),
        rLeft(radius),
        rTop(radius),
        rRight(radius),
        rBottom(radius),
        weight(weight)
        {
        }

        Area::Area(FPoint2D center) :
        center(center),
        rLeft(0.05f),
        rTop(0.05f),
        rRight(0.05f),
        rBottom(0.05f),
        weight(0.5f)
        {
        }

        float Area::getLeft() const
        {
            return center.x() - rLeft;
        }

        float Area::getTop() const
        {
            return center.y() - rTop;
        }

        float Area::getRight() const
        {
            return center.x() + rRight;
        }

        float Area::getBottom() const
        {
            return center.y() + rBottom;
        }

        FPoint2D Area::getLeftTop() const
        {
            return FPoint2D(getLeft(), getTop());
        }

        FPoint2D Area::getRightBottom() const
        {
            return FPoint2D(getRight(), getBottom());
        }

        FPoint2D Area::getCenter() const
        {
            return center;
        }

        float Area::getWeight() const
        {
            return weight;
        }

        bool Area::operator==(const Area& second) const
        {
            if (center == second.center)
            {
                if (rLeft == second.rLeft && rTop == second.rTop && rRight == second.rRight && rBottom == second.rBottom)
                {
                    return true;
                }
            }

            return false;
        }

        bool Area::operator!=(const Area& second) const
        {
            return !(*this == second);
        }

        std::ostream& operator<<(std::ostream& out, const FPoint2D& p)
        {
            out << "(" << p.x() << "," << p.y() << ")";
            return out;
        }

        std::ostream& operator<<(std::ostream& out, const Area& a)
        {
            out << "center: " << a.center << ", lrtb:(" << a.getLeft() << "," << a.getRight() << "," << a.getTop() << "," << a.getBottom() << "), weight: " << a.weight;
            //out << "leftTop:" << a.getLeftTop() << ", rightBottom:" << a.getRightBottom() << ", center:" << a.center << ", weight: " << a.weight;
            return out;
        }

        Range::Range() :
        min(0),
        max(1)
        {
        }

        Range::Range(float min, float max) :
        min(min),
        max(max)
        {
        }

        float Range::getMin() const
        {
            return min;
        }

        float Range::getMax() const
        {
            return max;
        }

        bool Range::contains(const Range& range) const
        {
            if (min <= range.min)
                if (max >= range.max)
                {
                    return true;
                }

            return false;
        }

        float Range::cutToRange(float value) const
        {
            if (value < min)
                value = min;
            else
                if (value > max)
                value = max;
            return value;
        }

        bool Range::operator==(const Range& r2)
        {
            if (min == r2.min)
                if (max == r2.max)
                {
                    return true;
                }

            return false;
        }

        bool Range::operator!=(const Range& r2)
        {
            return !(*this == r2);
        }

        std::ostream& operator<<(std::ostream& out, const Range& r)
        {
            out << "(" << r.min << "," << r.max << ")";
            return out;
        }

        /* RotationMatrix */
        RotationMatrix::RotationMatrix()
        {
            matrix = createRotationMatrix0().getMatrixArray();
            degrees = 0;
        }

        RotationMatrix RotationMatrix::createRotationMatrix0()
        {
            std::array<float, 16> matrix = std::array<float, 16>{{
                1, 0, 0, 0,
                0, -1, 0, 1,
                0, 0, 1, 0,
                0, 0, 0, 1
            }};


            return RotationMatrix(matrix, 0);
        }

        RotationMatrix RotationMatrix::createRotationMatrix90()
        {
            std::array<float, 16> matrix = std::array<float, 16>{{
                0, -1, 0, 1,
                -1, 0, 0, 1,
                0, 0, 1, 0,
                0, 0, 0, 1
            }};

            return RotationMatrix(matrix, 90);
        }

        RotationMatrix RotationMatrix::createRotationMatrix180()
        {
            std::array<float, 16> matrix = std::array<float, 16>{{
                -1, 0, 0, 1,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            }};

            return RotationMatrix(matrix, 180);
        }

        RotationMatrix RotationMatrix::createRotationMatrix270()
        {
            std::array<float, 16> matrix = std::array<float, 16>{{
                0, 1, 0, 0,
                1, 0, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            }};

            return RotationMatrix(matrix, 270);
        }

        RotationMatrix RotationMatrix::createRotationMatrix0FrontCam()
        {
            std::array<float, 16> matrix = std::array<float, 16>{{
                -1, 0, 0, 1,
                0, -1, 0, 1,
                0, 0, 1, 0,
                0, 0, 0, 1
            }};


            return RotationMatrix(matrix, 0);
        }

        RotationMatrix RotationMatrix::createRotationMatrix90FrontCam()
        {
            std::array<float, 16> matrix = std::array<float, 16>{{
                0, 1, 0, 0,
                -1, 0, 0, 1,
                0, 0, 1, 0,
                0, 0, 0, 1
            }};

            return RotationMatrix(matrix, 90);
        }

        RotationMatrix RotationMatrix::createRotationMatrix180FrontCam()
        {
            std::array<float, 16> matrix = std::array<float, 16>{{
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            }};

            return RotationMatrix(matrix, 180);
        }

        RotationMatrix RotationMatrix::createRotationMatrix270FrontCam()
        {
            std::array<float, 16> matrix = std::array<float, 16>{{
                0, 1, 0, 0,
                -1, 0, 0, 1,
                0, 0, 1, 0,
                0, 0, 0, 1
            }};

            return RotationMatrix(matrix, 270);
        }

        std::ostream& operator<<(std::ostream& out, const RotationMatrix& rmat)
        {

            std::array<float, 16> arr = rmat.getMatrixArray();

            for (unsigned i = 0; i < 16; i += 4)
            {
                out << arr[i] << " " << arr[i + 1] << " " << arr[i + 2] << " " << arr[i + 3] << std::endl;
            }

            return out;
        }

        RotationMatrix RotationMatrix::getRotatedRight() const
        {
            return RotationMatrix((degrees + 90) % 360);
        }

        RotationMatrix RotationMatrix::getRotatedLeft() const
        {
            return RotationMatrix((360 + degrees - 90) % 360);
        }

        std::array<float, 16> RotationMatrix::getMatrixArray() const
        {
            return matrix;
        }

        unsigned RotationMatrix::getDegrees() const
        {
            return degrees;
        }

        RotationMatrix::RotationMatrix(std::array<float, 16> matrix, unsigned degrees)
        {
            this->matrix = matrix;
            this->degrees = degrees;
        }

        RotationMatrix::RotationMatrix(unsigned degrees, bool frontCam)
        {
            RotationMatrix rotMat;

            switch (degrees)
            {
                case 0:
                    if (!frontCam)
                        rotMat = createRotationMatrix0();
                    else
                        rotMat = createRotationMatrix0FrontCam();
                    break;

                case 90:
                    if (!frontCam)
                        rotMat = createRotationMatrix90();
                    else
                        rotMat = createRotationMatrix90FrontCam();
                    break;

                case 180:
                    if (!frontCam)
                        rotMat = createRotationMatrix180();
                    else
                        rotMat = createRotationMatrix180FrontCam();
                    break;

                case 270:
                    if (!frontCam)
                        rotMat = createRotationMatrix270();
                    else
                        rotMat = createRotationMatrix270FrontCam();
                    break;

                default:
                    if (!frontCam)
                        rotMat = createRotationMatrix0();
                    else
                        rotMat = createRotationMatrix0FrontCam();
                    break;
            }

            this->matrix = rotMat.getMatrixArray();
            this->degrees = rotMat.getDegrees();
        }

        /* Parameters class*/

        const int Parameters::RANGE;

        const int Parameters::ANTIBANDING_DEFAULT;
        const int Parameters::ANTIBANDING_AUTO;
        const int Parameters::ANTIBANDING_50HZ;
        const int Parameters::ANTIBANDING_60HZ;
        const int Parameters::ANTIBANDING_OFF;

        // Values for white balance settings.
        const int Parameters::WHITE_BALANCE_DEFAULT;
        const int Parameters::WHITE_BALANCE_AUTO;
        const int Parameters::WHITE_BALANCE_INCANDESCENT;
        const int Parameters::WHITE_BALANCE_FLUORESCENT;
        const int Parameters::WHITE_BALANCE_WARM_FLUORESCENT;
        const int Parameters::WHITE_BALANCE_DAYLIGHT;
        const int Parameters::WHITE_BALANCE_CLOUDY_DAYLIGHT;
        const int Parameters::WHITE_BALANCE_TWILIGHT;
        const int Parameters::WHITE_BALANCE_SHADE;
        const int Parameters::WHITE_BALANCE_LOCKED;

        // Values for color effect settings.
        const int Parameters::COLOR_EFFECT_DEFAULT;
        const int Parameters::COLOR_EFFECT_NONE;
        const int Parameters::COLOR_EFFECT_MONO;
        const int Parameters::COLOR_EFFECT_NEGATIVE;
        const int Parameters::COLOR_EFFECT_SOLARIZE;
        const int Parameters::COLOR_EFFECT_SEPIA;
        const int Parameters::COLOR_EFFECT_POSTERIZE;
        const int Parameters::COLOR_EFFECT_WHITEBOARD;
        const int Parameters::COLOR_EFFECT_BLACKBOARD;
        const int Parameters::COLOR_EFFECT_AQUA;

        // Values flash mode
        const int Parameters::FLASH_MODE_DEFAULT;
        const int Parameters::FLASH_MODE_OFF;
        const int Parameters::FLASH_MODE_AUTO;
        const int Parameters::FLASH_MODE_ON;
        const int Parameters::FLASH_MODE_RED_EYE;

        // Keys scene mode
        const int Parameters::SCENE_MODE_DEFAULT;
        const int Parameters::SCENE_MODE_AUTO;
        const int Parameters::SCENE_MODE_ACTION;
        const int Parameters::SCENE_MODE_PORTRAIT;
        const int Parameters::SCENE_MODE_LANDSCAPE;
        const int Parameters::SCENE_MODE_NIGHT;
        const int Parameters::SCENE_MODE_NIGHT_PORTRAIT;
        const int Parameters::SCENE_MODE_THEATRE;
        const int Parameters::SCENE_MODE_BEACH;
        const int Parameters::SCENE_MODE_SNOW;
        const int Parameters::SCENE_MODE_SUNSET;
        const int Parameters::SCENE_MODE_STEADYPHOTO;
        const int Parameters::SCENE_MODE_FIREWORKS;
        const int Parameters::SCENE_MODE_SPORTS;
        const int Parameters::SCENE_MODE_PARTY;
        const int Parameters::SCENE_MODE_CANDLELIGHT;
        const int Parameters::SCENE_MODE_BARCODE;
        const int Parameters::SCENE_MODE_HDR;

        // Focus values
        const int Parameters::FOCUS_MODE_DEFAULT;
        const int Parameters::FOCUS_MODE_AUTO;
        const int Parameters::FOCUS_MODE_INFINITY;
        const int Parameters::FOCUS_MODE_MACRO;
        const int Parameters::FOCUS_MODE_FIXED;
        const int Parameters::FOCUS_MODE_EDOF;
        const int Parameters::FOCUS_MODE_CONTINUOUS_VIDEO;
        const int Parameters::FOCUS_MODE_CONTINUOUS_PICTURE;

        const int Parameters::TORCH_MODE_DEFAULT;
        const int Parameters::TORCH_MODE_OFF;
        const int Parameters::TORCH_MODE_ON;
        const int Parameters::TORCH_MODE_AUTO;

        Parameters::Parameters(int camID) :
        antibanding(ANTIBANDING_DEFAULT),
        colorEffect(COLOR_EFFECT_DEFAULT),
        whiteBalance(WHITE_BALANCE_DEFAULT),
        flashMode(FLASH_MODE_DEFAULT),
        sceneMode(SCENE_MODE_DEFAULT),
        focusMode(FOCUS_MODE_DEFAULT),
        torchMode(TORCH_MODE_DEFAULT),
        exposureMode(EXPOSURE_MODE_DEFAULT),
        m_camID(camID),
        size(Size()),
        zoom(0),
        maxZoom(0),
        smoothlyZoom(false),
        smoothlyZoomSupported(false),
        maxOpticalZoom(0),
        smoothZoomRate(1.0f),
        focalLength(0.0f),
        exposureCompStep(0),
        exposureComp(0),
        maxExposureComp(0),
        minExposureComp(0),
        iso(0),
        minISO(0),
        maxISO(0),
        exposureDuration(0),
        minExposureDuration(0),
        maxExposureDuration(0),
        maxExpAreas(0),
        expAreas(std::vector<Area>()),
        maxFocusAreas(0),
        focusAreas(std::vector<Area>()),
        focusDistances(std::vector<float>{
            -1, -1, -1
        }),
        fpsRange(Range()),
        supportedFpsRanges(std::vector<Range>()),
        hViewAngle(0),
        vViewAngle(0),
        lensAperture(0),
        lensPosition(0),
        fov(0),
        rotation(0),
        deviceConfigID(0)
        {
        }

        inline bool isType(int param, int first)
        {
            first = ((int) first / (int) Parameters::RANGE) * Parameters::RANGE;
            //LOG("Param base type: ", first);
            return ((param >= first) && (param < (first + Parameters::RANGE))) ? true : false;
        }

        void Parameters::setAntibanding(int param) throw (camera_parameters_exception)
        {
            if (isType(param, ANTIBANDING_DEFAULT))
            {
                this->sendAntibanding(param);
                antibanding = param;
            } else
            {
                throw camera_parameters_exception(std::string("Not antibanding param provided"), m_camID, 0, "antibanding");
            }
        }

        void Parameters::setColorEffect(int param) throw (camera_parameters_exception)
        {
            if (isType(param, COLOR_EFFECT_DEFAULT))
            {
                this->sendColorEffect(param);
                colorEffect = param;
            } else
            {
                throw camera_parameters_exception(std::string("Not color effect param provided"), m_camID, 0, "color effect");
            }
        }

        void Parameters::setWhiteBalance(int param) throw (camera_parameters_exception)
        {
            if (isType(param, WHITE_BALANCE_DEFAULT))
            {
                this->sendWhiteBalance(param);
                whiteBalance = param;
            } else
            {
                throw camera_parameters_exception(std::string("Not white balance provided"), m_camID, 0, "white balance");
            }
        }

        void Parameters::setFlashMode(int param) throw (camera_parameters_exception)
        {
            if (isType(param, FLASH_MODE_DEFAULT))
            {
                this->sendFlashMode(param);
                flashMode = param;
            } else
            {
                throw camera_parameters_exception(std::string("Not flash mode param provided"), m_camID, 0, "flash mode");
            }
        }

        void Parameters::setSceneMode(int param) throw (camera_parameters_exception)
        {
            if (isType(param, SCENE_MODE_DEFAULT))
            {
                this->sendSceneMode(param);
                sceneMode = param;
            } else
            {
                throw camera_parameters_exception(std::string("Not scene mode param provided"), m_camID, 0, "scene mode");
            }
        }

        void Parameters::setFocusMode(int param) throw (camera_parameters_exception)
        {
            if (isType(param, FOCUS_MODE_DEFAULT))
            {
                this->sendFocusMode(param);
                focusMode = param;
            } else
            {
                throw camera_parameters_exception(std::string("Not focus mode param provided"), m_camID, 0, "focus mode");
            }
        }

        void Parameters::setTorchMode(int param) throw (camera_parameters_exception)
        {
            if (isType(param, TORCH_MODE_DEFAULT))
            {
                this->sendTorchMode(param);
                torchMode = param;
            } else
            {
                throw camera_parameters_exception(std::string("Not torch mode param provided"), m_camID, 0, "torch mode");
            }
        }

        void Parameters::setExposureMode(int param) throw (camera_parameters_exception)
        {
            if (isType(param, EXPOSURE_MODE_DEFAULT))
            {
                this->sendExposureMode(param);
                exposureMode = param;
            } else
            {
                throw camera_parameters_exception(std::string("Not exposure mode param provided"), m_camID, 0, "exposure mode");
            }
        }

        int Parameters::getAntibanding() const
        {
            return antibanding;
        }

        int Parameters::getColorEffect() const
        {
            return colorEffect;
        }

        int Parameters::getWhiteBalance() const
        {
            return whiteBalance;
        }

        int Parameters::getFlashMode() const
        {
            return flashMode;
        }

        int Parameters::getSceneMode() const
        {
            return sceneMode;
        }

        int Parameters::getFocusMode() const
        {
            return focusMode;
        }

        int Parameters::getTorchMode() const
        {
            return torchMode;
        }

        int Parameters::getExposureMode() const
        {
            return exposureMode;
        }

        Size Parameters::getPreviewSize() const
        {
            return size;
        }

        Size Parameters::getPreviewSize(int deviceOrientationDegrees) const
        {
            if(deviceOrientationDegrees == PreviewSource::PORTRAIT || deviceOrientationDegrees == PreviewSource::PORTRAIT_UPSIDE_DOWN)
            {
                return size.getRotated90();
            }
            return size;
        }

        bool Parameters::isSmoothlyZoomSupported() const
        {
            return smoothlyZoomSupported;
        }

        float Parameters::getZoom() const
        {
            return loadZoom(); // from device
        }

        float Parameters::getMaxOpticalZoom() const
        {
            return maxOpticalZoom;
        }

        float Parameters::getMaxZoom() const
        {
            return maxZoom;
        }

        float Parameters::getMinZoom() const
        {
            return 1.0f;
        }

        float Parameters::getFocalLengthMilimeters() const
        {
            return loadFocalLength(); // from device
        }

        int Parameters::getMaxExposureAreas() const
        {
            return maxExpAreas;
        }

        std::vector<Area> Parameters::getExposureAreas() const
        {
            return loadExposureAreas(); // from device
        }

        void Parameters::setExposureAreas(const std::vector<Area>& areas) throw (camera_parameters_exception)
        {
            checkAreas(areas, getMaxExposureAreas());
            this->sendExposureAreas(areas);
            expAreas = areas; // everything is ok, remember it
        }

        int Parameters::getMaxFocusAreas() const
        {
            return maxFocusAreas;
        }

        std::vector<Area> Parameters::getFocusAreas() const
        {
            return loadFocusAreas();
        }

        void Parameters::setFocusAreas(const std::vector<Area>& areas) throw (camera_parameters_exception)
        {
            checkAreas(areas, getMaxFocusAreas());
            this->sendFocusAreas(areas);
            focusAreas = areas;
        }

        std::vector<float> Parameters::getFocusDistances() const
        {
            return loadFocusDistances(); // from device
        }

        Range Parameters::getFPSRange() const
        {
            return fpsRange;
        }

        std::vector<Range> Parameters::getSupportedFPSRanges() const
        {
            return supportedFpsRanges;
        }

        void Parameters::setPreviewFPSRange(Range range) throw (camera_parameters_exception)
        {
            bool isSupp = false;

            for (Range& r : supportedFpsRanges)
            {
                if (r.contains(range))
                {
                    isSupp = true;
                    break;
                }
            }

            if (isSupp)
            {
                this->sendPreviewFPSRange(range);
                fpsRange = range;
            } else
            {
                throw camera_parameters_exception("Provided range is not supported, check in getSupportedFPSRanges", m_camID, 0, "nothing");
            }

        }

        float Parameters::getHorizontalViewAngle() const
        {
            return loadHorizontalViewAngle(); // from device
        }

        float Parameters::getVerticalViewAngle() const
        {
            return loadVerticalViewAngle(); // from device
        }

        float Parameters::getLensAperture() const
        {
            return loadLensAperture();
        }

        float Parameters::getLensPosition() const
        {
            return loadLensPosition();
        }

        float Parameters::getMinLensPosition() const
        {
            return 0.0; // iOS doc
        }

        float Parameters::getMaxLensPosition() const
        {
            return 1.0f; // iOS doc (1.0 is auto focus, infinity foc)
        }

        int Parameters::getRotation() const
        {
            return rotation;
        }

        void Parameters::setLensPositionWithFocusModeFixed(float position) throw (camera_parameters_exception)
        {
            if (position < getMinLensPosition() || position > getMaxLensPosition())
            {
                throw camera_parameters_exception("Bad lens position provided or method not supported, use get<Min,Max>LensPosition methods before set", m_camID, 0, "nothing");
            }

            this->sendLensPositionWithFocusModeFixed(position);
            lensPosition = position;
        }

        void Parameters::setRotation(int degrees) throw (camera_exception, camera_parameters_exception)
        {
            if (degrees < 0)
                degrees = 0;

            degrees = degrees % 360;
            sendRotation(degrees);
            this->rotation = degrees;
        }

        void Parameters::autoFocus() throw (camera_exception, camera_parameters_exception)
        {
            sendAutoFocus();
        }

        float Parameters::getFOV() const
        {
            return loadFOV();
        }

        const std::vector<Size>& Parameters::getSupportedPictureSizes() const
        {
            return supportedPictureSizes;
        }

        const std::vector<Size>& Parameters::getSupportedVideoSizes() const
        {
            return supportedVideoSizes;
        }

        void Parameters::checkAreas(const std::vector<Area>& areas, int maxPossible) throw (camera_parameters_exception)
        {
            if (areas.size() > (unsigned)maxPossible)
            {
                throw camera_parameters_exception("Too much areas provided, use method getMax<WHAT_PARAM>Areas before set it", m_camID, 0, "nothing");
            }

            for (const Area& a : areas)
            {
                if (a.getLeft() < 0 || a.getTop() < 0 || a.getRight() > 1 || a.getBottom() > 1)
                {
                    throw camera_parameters_exception("Bad area provided, must be in clampf values (0,0 to 1,0 points)", m_camID, 0, "nothing");
                }

                if (a.getWeight() <= 0 || a.getWeight() > 1)
                {
                    throw camera_parameters_exception("Bad weight in area provided, must be in clampf value (0 to 1>", m_camID, 0, "nothing");
                }
            }
        }

        std::ostream& Parameters::longDescription(std::ostream& out) const
        {
            std::string myendl = "\n";
            out << "CamID: " << m_camID << myendl;
            out << "Preview Size: " << getPreviewSize() << myendl;
            out << "Antibanding: " << getAntibanding() << myendl;
            out << "White Balance: " << getWhiteBalance() << myendl;
            out << "Color Effect: " << getColorEffect() << myendl;
            out << "Flash Mode: " << getFlashMode() << myendl;
            out << "Scene Mode: " << getSceneMode() << myendl;
            out << "Focus Mode: " << getFocusMode() << myendl;
            out << "Torch Mode: " << getTorchMode() << myendl;
            out << "Exposure Mode: " << getExposureMode() << myendl;

            out << "Zoom: " << getZoom() << myendl;
            out << "Max zoom: " << getMaxZoom() << myendl;
            out << "SmoothZoomSupp: " << isSmoothlyZoomSupported() << myendl;
            out << "Max optical zoom: " << getMaxOpticalZoom() << myendl;
            out << "Smooth zoom rate: " << getSmoothZoomRatePerSecond() << myendl;
            out << "Focal length: " << getFocalLengthMilimeters() << myendl;

            out << "EV step: " << getExposureCompensationStep() << myendl;
            out << "EV: " << getExposureCompensation() << myendl;
            out << "Min EV: " << getMinExposureCompensation() << myendl;
            out << "Max EV: " << getMaxExposureCompensation() << myendl;
            out << "Max Exp Areas: " << getMaxExposureAreas() << myendl;
            out << "Exposure Areas:" << myendl;

            std::vector<Area> tmpExpAreas = getExposureAreas();

            for (const Area& a : tmpExpAreas)
            {
                out << "[" << a << "], ";
            }

            out << myendl;

            out << "Max Foc Areas: " << getMaxFocusAreas() << myendl;
            out << "Focus Areas:" << myendl;

            std::vector<Area> tmpFocAreas = getFocusAreas();

            for (const Area& a : tmpFocAreas)
            {
                out << "[" << a << "], ";
            }

            out << myendl;

            out << "Focus distances: " << myendl;

            std::vector<float> focDists = getFocusDistances();

            for (float f : focDists)
            {
                out << "[" << f << "], ";
            }

            out << myendl;

            out << "FPS Range: " << getFPSRange() << myendl;
            out << "Supported fps ranges:" << myendl;

            for (unsigned i = 0; i < getSupportedFPSRanges().size(); i++)
            {
                out << "[" << getSupportedFPSRanges()[i] << "], ";
            }

            out << myendl;

            out << "h,v angles: " << getHorizontalViewAngle() << ", " << getVerticalViewAngle() << myendl;
            out << "lens aperture: " << getLensAperture() << myendl;
            out << "lens position: " << getLensPosition() << myendl;
            out << "fov:" << getFOV() << myendl;

            out << "Supp pic sizes" << myendl;
            for (unsigned i = 0; i < getSupportedPictureSizes().size(); i++)
            {
                out << "[" << getSupportedPictureSizes()[i] << "], ";
            }

            return out;
        }

        std::ostream& Parameters::shortDescription(std::ostream& out) const
        {
            std::string myendl = " | ";
            out << "id[" << m_camID << "]" << myendl;
            out << "s[" << getPreviewSize() << "]" << myendl;
            out << "a[" << getAntibanding() << "]" << myendl;
            out << "wb[" << getWhiteBalance() << "]" << myendl;
            out << "ce[" << getColorEffect() << "]" << myendl;
            out << "fm[" << getFlashMode() << "]" << myendl;
            out << "sm[" << getSceneMode() << "]" << myendl;
            out << "fom[" << getFocusMode() << "]" << myendl;
            out << "tm[" << getTorchMode() << "]" << myendl;
            out << "em[" << getExposureMode() << "]" << myendl;

            out << "z[" << getZoom() << "]" << myendl;
            out << "mz[" << getMaxZoom() << "]" << myendl;
            out << "sms[" << isSmoothlyZoomSupported() << "]" << myendl;
            out << "moz[" << getMaxOpticalZoom() << "]" << myendl;
            out << "szr[" << getSmoothZoomRatePerSecond() << "]" << myendl;
            out << "fl[" << getFocalLengthMilimeters() << "]" << myendl;

            out << "evs[" << getExposureCompensationStep() << "]" << myendl;
            out << "ev[" << getExposureCompensation() << "]" << myendl;
            out << "minev[" << getMinExposureCompensation() << "]" << myendl;
            out << "maxev[" << getMaxExposureCompensation() << "]" << myendl;

            out << "iso[" << getISO() << "]" << myendl;
            out << "minIso[" << getMinISO() << "]" << myendl;
            out << "maxIso[" << getMaxISO() << "]" << myendl;

            out << "ed[" << getExposureDuration() << "]" << myendl;
            out << "minED[" << getMinExposureDuration() << "]" << myendl;
            out << "maxED[" << getMaxExposureDuration() << "]" << myendl;

            out << "mea[" << getMaxExposureAreas() << "]" << myendl;
            out << "ea[";

            std::vector<Area> tmpExpAreas = getExposureAreas();

            for (const Area& a : tmpExpAreas)
            {
                out << "[" << a << "], ";
            }

            out << "]" << myendl;

            out << "mfa[" << getMaxFocusAreas() << "]" << myendl;
            out << "fa[";

            std::vector<Area> tmpFocAreas = getFocusAreas();

            for (const Area& a : tmpFocAreas)
            {
                out << "[" << a << "], ";
            }

            out << "]" << myendl;

            out << "fd[";

            std::vector<float> focDists = getFocusDistances();

            for (float f : focDists)
            {
                out << "[" << f << "], ";
            }

            out << "]" << myendl;

            out << "fpsr[" << getFPSRange() << "]" << myendl;
            out << "sfpsr[";

            for (unsigned i = 0; i < getSupportedFPSRanges().size(); i++)
            {
                out << "[" << getSupportedFPSRanges()[i] << "], ";
            }

            out << "]" << myendl;

            out << "hva[" << getHorizontalViewAngle() << ", " << getVerticalViewAngle() << "]" << myendl;
            out << "la[" << getLensAperture() << "]" << myendl;
            out << "lp[" << getLensPosition() << "]" << myendl;
            out << "fov:" << getFOV() << myendl;

            out << "sPicS[" << myendl;
            for (unsigned i = 0; i < getSupportedPictureSizes().size(); i++)
            {
                out << "[" << getSupportedPictureSizes()[i] << "], ";
            }

            out << "]" << myendl;

            out << "sVidS[" << myendl;
            for (unsigned i = 0; i < getSupportedVideoSizes().size(); i++)
            {
                out << "[" << getSupportedVideoSizes()[i] << "], ";
            }
            out << "]" << myendl;

            return out;
        }

        void Parameters::setPreviewResolution(Size sizep)
        {
            this->size = sizep;
        }

        std::vector<float> Parameters::areasToFloatVector(const std::vector<Area>& areas)
        {
            std::vector<float> areasFloats;

            for (unsigned i = 0; i < areas.size(); i++)
            {
                //LOG(expAreas[i]);
                areasFloats.push_back(areas[i].getLeft());
                areasFloats.push_back(areas[i].getTop());
                areasFloats.push_back(areas[i].getRight());
                areasFloats.push_back(areas[i].getBottom());
                areasFloats.push_back(areas[i].getWeight());
            }

            return areasFloats;
        }

        std::vector<Area> Parameters::floatVectorToAreas(const std::vector<float>& vec)
        {
            std::vector<Area> areas;

            for (unsigned i = 0; i < vec.size(); i += 5)
            {
                float left = vec[i];
                float top = vec[i + 1];
                float right = vec[i + 2];
                float bottom = vec[i + 3];
                float weight = vec[i + 4];

                float xDiameter = right - left;
                float yDiameter = bottom - top;
                FPoint2D center(left + (xDiameter / 2), top + (yDiameter / 2));
                areas.push_back(Area(center, xDiameter, yDiameter, weight));
            }

            return areas;
        }

        std::vector<Range> Parameters::floatVectorToRanges(const std::vector<float>& vec)
        {
            std::vector<Range> ranges;

            for (unsigned i = 0; i < vec.size(); i += 2)
            {
                ranges.push_back(Range(vec[i], vec[i + 1]));
            }

            return ranges;
        }

        std::vector<Size> Parameters::intVectorToSizes(const std::vector<int>& vec)
        {
            std::vector<Size> sizes;
            for (unsigned i = 0; i < vec.size(); i += 2)
            {
                sizes.push_back(Size(vec[i], vec[i + 1]));
            }
            return sizes;
        }

        bool Parameters::checkIfAreasChanged(const std::vector<Area> tmpAreas, const std::vector<Area> lastAreas) const
        {
            if (lastAreas.size() != tmpAreas.size())
            {
                return true;
            }

            for (unsigned i = 0; i < tmpAreas.size(); i++)
            {
                if (lastAreas[i] != tmpAreas[i])
                {
                    return true;
                }
            }

            return false;
        }

        void Parameters::setZoom(float zoom) throw (camera_parameters_exception)
        {
            if (zoom >= getMinZoom() && zoom <= maxZoom)
            {
                sendZoom(zoom);
                this->zoom = zoom;
                this->smoothlyZoom = false;
            } else
            {
                throw camera_parameters_exception("Wrong zoom factor provided, use 1.0 to getZoomMax() value", m_camID, 0, "nothing");
            }
        }

        void Parameters::setZoomSmoothly(float zoom) throw (camera_parameters_exception)
        {
            if (!smoothlyZoomSupported)
            {
                throw camera_parameters_exception("Smoothly zoom not supported !!!, check it using isSmoothlyZoomSupported() method", m_camID, 0, "nothing");
            }

            if (zoom >= getMinZoom() && zoom <= maxZoom)
            {
                sendZoomSmoothly(zoom);
                this->zoom = zoom;
                this->smoothlyZoom = true;
            } else
            {
                throw camera_parameters_exception("Wrong zoom factor provided, use 1.0 to getZoomMax() value", m_camID, 0, "nothing");
            }
        }

        void Parameters::setSmoothZoomRatePerSecond(float rate) throw (camera_parameters_exception)
        {
            if (rate <= 0)
            {
                throw camera_parameters_exception("Smooth rate per second must be a positive value", m_camID, 0, "nothing");
            }

            smoothZoomRate = rate;
        }

        float Parameters::getSmoothZoomRatePerSecond() const
        {
            return smoothZoomRate;
        }

        float Parameters::getExposureCompensationStep() const
        {
            return exposureCompStep;
        }

        float Parameters::getExposureCompensation() const
        {
            return loadExposureCompensation();
        }

        float Parameters::getMinExposureCompensation() const
        {
            return minExposureComp;
        }

        float Parameters::getMaxExposureCompensation() const
        {
            return maxExposureComp;
        }

        float Parameters::getMinISO() const
        {
            return minISO;
        }

        float Parameters::getMaxISO() const
        {
            return maxISO;
        }

        float Parameters::getISO() const
        {
            return loadISO();
        }

        float Parameters::getMinExposureDuration() const
        {
            return minExposureDuration + 0.0000001;
        }

        float Parameters::getMaxExposureDuration() const
        {
            return maxExposureDuration;
        }

        float Parameters::getExposureDuration() const
        {
            return loadExposureDuration();
        }

        void Parameters::setExposureCompensation(float ev) throw (camera_parameters_exception)
        {
            if ((ev < getMinExposureCompensation()) || (ev > getMaxExposureCompensation()))
            {
                throw camera_parameters_exception("Invalid EV value provided to setExposure method. Pls use getMinExposure and getMaxExposure methods", m_camID, 0, "nothing");
            }

            this->sendExposureCompensation(ev);
            exposureComp = ev;
        }

        void Parameters::setISO(float iso) throw (camera_parameters_exception)
        {
            if (iso < getMinISO() || iso > getMaxISO())
            {
                throw camera_parameters_exception("Invalid iso value provided. Pls use getMinISO and getMaxISO methods", m_camID, 0, "nothing");
            }
            sendISO(iso);
            this->iso = iso;
        }

        void Parameters::setExposureDuration(float ed) throw (camera_parameters_exception)
        {
            if (ed < getMinExposureDuration() || ed > getMaxExposureDuration())
            {
                throw camera_parameters_exception("Invalid exposureDuration value. Pls use getMinExposureDuration and getMaxExposureDuration methods", m_camID, 0, "nothing");
            }
            sendExposureDuration(ed);
            exposureDuration = ed;
        }

        void Parameters::setISOAndED(float iso, float ed) throw (camera_parameters_exception)
        {
            if (iso < getMinISO() || iso > getMaxISO())
            {
                throw camera_parameters_exception("Invalid iso value provided. Pls use getMinISO and getMaxISO methods", m_camID, 0, "nothing");
            }
            if (ed < getMinExposureDuration() || ed > getMaxExposureDuration())
            {
                throw camera_parameters_exception("Invalid exposureDuration value. Pls use getMinExposureDuration and getMaxExposureDuration methods", m_camID, 0, "nothing");
            }
            sendISOAndED(iso, ed);
            this->iso = iso;
            this->exposureDuration = ed;
        }

        bool Parameters::operator==(const Parameters& second) const
        {
            return getPreviewSize() == second.getPreviewSize();
        }

        bool Parameters::operator>(const Parameters& second) const
        {
            return getPreviewSize() > second.getPreviewSize();
        }

        bool Parameters::operator<(const Parameters& second) const
        {
            return getPreviewSize() < second.getPreviewSize();
        }

        bool Parameters::operator>=(const Parameters& second) const
        {
            return getPreviewSize() >= second.getPreviewSize();
        }

        bool Parameters::operator<=(const Parameters& second) const
        {
            return getPreviewSize() <= second.getPreviewSize();
        }

        std::ostream& operator<<(std::ostream& out, const Parameters& params)
        {
            //return params.longDescription(out);
            return params.shortDescription(out);
        }

        /* Directory */

        directory_exception::directory_exception(const std::string& str) : runtime_error(str)
        {
        }

        directory_exception::directory_exception(const char* str) : runtime_error(str)
        {
        }

        Directory::Directory() : path("./")
        {
        }

        Directory::Directory(const std::string& path) : path(path)
        {
        }

        const std::string& Directory::getPath() const
        {
            return path;
        }

        std::ostream& operator<<(std::ostream& out, const Directory& dir)
        {
            return out << dir.getPath();
        }

        File::File(const Directory& dir, const std::string& fileName) :
        dir(dir),
        fileName(fileName)
        {
        }

        const Directory& File::getDir() const
        {
            return dir;
        }

        const std::string& File::getFileName() const
        {
            return fileName;
        }

        std::string File::getFullPath() const
        {
            std::stringstream ss;
            ss << dir.getPath() << "/" << getFileName();
            return ss.str();
        }

        std::ostream& operator<<(std::ostream& out, const File& file)
        {
            return out << file.getFullPath();
        }

        Picture::Picture(Camera& cam, const File& file, Size s, FORMAT f) throw (camera_exception, camera_parameters_exception) :
        cam(cam),
        file(file),
        albumName(""),
        JPGQuality(100),
        inAlbum(false),
        filter(FILTER::NONE)
        {
            if (!cam.isOpened())
            {
                throw camera_exception("Camera is not opened while constructing Picture class object!!!", cam.getCamID());
            }


            if (s.getWidth() == 0 || s.getHeight() == 0)
            {
                const std::vector<Size>& sizes = cam.getParameters().getSupportedPictureSizes();
                size = *std::max_element(sizes.begin(), sizes.end());
            } else
            {
                setSize(s);
            }

            setFormat(f); // can throw exception camera_parameters_exception


            std::string extension;
            switch (f)
            {
                case FORMAT::JPG:
                    extension = ".jpg";
                    break;
                default:
                    throw camera_parameters_exception("Unsupported picture format provided", cam.getCamID(), 0, "FORMAT");
            }
            pictureFilePath = file.getFullPath() + extension;
        }

        Picture::Picture(Camera& cam, const std::string& albumName, Size s, FORMAT f) throw (camera_exception, camera_parameters_exception) :
        cam(cam),
        file(File(Directory(), "")),
        albumName(trim(albumName)),
        JPGQuality(100),
        inAlbum(true),
        filter(FILTER::NONE)
        {
            if (!cam.isOpened())
            {
                throw camera_exception("Camera is not opened while constructing Picture class object!!!", cam.getCamID());
            }

            if (s.getWidth() == 0 || s.getHeight() == 0)
            {
                const std::vector<Size>& sizes = cam.getParameters().getSupportedPictureSizes();
                size = *std::max_element(sizes.begin(), sizes.end());
            } else
            {
                setSize(s);
            }

            setFormat(f); // can throw exception camera_parameters_exception

            std::string extension;
            switch (f)
            {
                case FORMAT::JPG:
                    extension = ".jpg";
                    break;
                default:
                    throw camera_parameters_exception("Unsupported picture format provided", cam.getCamID(), 0, "FORMAT");
            }

            auto duration = std::chrono::system_clock::now().time_since_epoch();
            auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            std::stringstream ss;
            ss << "IMG_" << millis << extension;
            pictureFilePath = ss.str();
        }

        bool Picture::isSupported(FORMAT format, const Camera& cam)
        {
            return sendIsSupportedPictureFormat(format, cam.getCamID());
        }

        void Picture::setFormat(FORMAT format)
        {
            this->format = format; // while trying takePicture will shout when is not supported
        }

        FORMAT Picture::getFormat() const
        {
            return format;
        }

        void Picture::setSize(Size s) throw (camera_parameters_exception)
        {
            const std::vector<Size>& sizes = cam.getParameters().getSupportedPictureSizes();
            bool isSupp = false;
            for (const Size& s2 : sizes)
            {
                if (s2 == s)
                {
                    isSupp = true;
                    break;
                }
            }

            if (isSupp)
            {
                size = s;
            } else
            {
                throw camera_parameters_exception("Provided picture size not supported, pls use Parameters::getSupportedPictureSizes() method", cam.getCamID(), 0, "picture size");
            }
        }

        Size Picture::getSize() const
        {
            return size;
        }

        int Picture::getJPGQuality() const
        {
            return JPGQuality;
        }

        void Picture::setJPGQuality(int percent)
        {
            if ((percent >= getMinJPGQuality()) && (percent <= getMaxJPGQuality()))
                JPGQuality = percent;
        }

        int Picture::getMaxJPGQuality() const
        {
            return 100;
        }

        int Picture::getMinJPGQuality() const
        {
            return 1;
        }

        const std::string& Picture::getFullFileName() const
        {
            return pictureFilePath;
        };

        bool Picture::saveInAlbum() const
        {
            return inAlbum;
        }

        const std::string& Picture::getAlbumName() const
        {
            return albumName;
        }

        FILTER Picture::getFilter() const
        {
            return filter;
        }

        void Picture::setFilter(FILTER filter)
        {
            this->filter = filter;
        }

        void Picture::openAlbumInGallery(const std::string& albumName)
        {
            Picture::sendOpenAlbumInGallery(albumName);
        }

        const File& Picture::getFile() const
        {
            return file;
        }

        /* Video */
        Video::Video(Camera& cam, const std::string& albumName, Size s) throw (camera_exception, camera_parameters_exception) :
        inAlbum(true),
        albumName(albumName)
        {
            if (!cam.isOpened())
            {
                throw camera_exception("Camera is not opened while constructing Picture class object!!!", cam.getCamID());
            }

            if (s.getWidth() == 0 || s.getHeight() == 0)
            {
                // if not provided, set to max possible
                const std::vector<Size>& sizes = cam.getParameters().getSupportedVideoSizes();
                size = *std::max_element(sizes.begin(), sizes.end());
            } else
            {
                size = s;
            }

            std::string extension = ".mp4";
            auto duration = std::chrono::system_clock::now().time_since_epoch();
            auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            std::stringstream ss;
            ss << "VIDEO_" << millis << extension;
            videoFilePath = ss.str();
        }

        bool Video::needSaveInAlbum() const
        {
            return inAlbum;
        }

        const std::string& Video::getAlbumName() const
        {
            return albumName;
        }

        const std::string& Video::getFullFileName() const
        {
            return videoFilePath;
        }

        Size Video::getSize() const
        {
            return size;
        }

        /* ThreadSafeTasks class */
        void ThreadSafeTasks::transferNewTasks()
        {
            std::lock_guard<std::mutex> lg(newTaskMutex);
            while (!newTasks.empty())
            {
                tasks.push(newTasks.front());
                newTasks.pop();
            }
        }

        void ThreadSafeTasks::push(const std::function<void()>& t)
        {
            //LOG(std::this_thread::get_id());
            std::lock_guard<std::mutex> lg(newTaskMutex);
            newTasks.push(t);
        }

        void ThreadSafeTasks::invokeAll()
        {
            std::lock_guard<std::mutex> lg(m);
            transferNewTasks();

            while (!tasks.empty())
            {
                tasks.front() (); // invocation
                tasks.pop();
            }
        }

        int ThreadSafeTasks::size() const
        {
            return tasks.size();
        }

        void ThreadSafeTasks::clear()
        {
            std::lock_guard<std::mutex> lg(m);
            std::lock_guard<std::mutex> lg2(newTaskMutex);
            newTasks = std::queue<std::function<void()> >();
            tasks = std::queue<std::function<void()> >();
        }

        PixelBuffer::PixelBuffer(uint32_t width, uint32_t height, uint32_t channels) :
        m_width(width),
        m_height(height),
        m_channels(channels)
        {
            m_data = new u_char[width * height * channels];
        }

        PixelBuffer::~PixelBuffer()
        {
            delete[] m_data;
        }

        u_char PixelBuffer::getR(int row, int col) const
        {
            return m_data[getDataIndex(row,col)+0];
        }

        u_char PixelBuffer::getG(int row, int col) const
        {
            return m_data[getDataIndex(row,col)+1];
        }

        u_char PixelBuffer::getB(int row, int col) const
        {
            return m_data[getDataIndex(row,col)+2];
        }

        u_char PixelBuffer::getA(int row, int col) const
        {
            return m_data[getDataIndex(row,col)+3];
        }
         //                    :  0xFF|FF|FF|FF
         // in memory positions:    RR|GG|BB|AA ]
        uint32_t PixelBuffer::getRGBA(int row, int col) const
        {
            // TODO add testing little_endian - big_endian, if big_endian -> getSwapped need to be invoked
            return getSwapped( *( (uint32_t*)&m_data[getDataIndex(row,col)] ) ); // ptr from 1 to 4 bytes data value
        }

        u_char* PixelBuffer::getRawData()
        {
            return m_data;
        }

        // window screen coordinates indexing, rows from up to bottom (0->height), cols from left to right (0->width)
        uint32_t PixelBuffer::getDataIndex(int row, int col) const
        {
            return ((m_height-1-row)*m_width*m_channels) + (col*m_channels);
        }

        uint32_t PixelBuffer::getSwapped(uint32_t ui)
        {
            return   (ui >> 24) |
                    ((ui << 8)  & 0x00FF0000) |
                    ((ui >> 8)  & 0x0000FF00) |
                     (ui << 24);
        }

        uint32_t PixelBuffer::getWidth() const
        {
            return m_width;
        }

        uint32_t PixelBuffer::getHeight() const
        {
            return m_height;
        }

        uint32_t PixelBuffer::getRows() const
        {
            return getHeight();
        }

        uint32_t PixelBuffer::getCols() const
        {
            return getWidth();
        }

        uint32_t PixelBuffer::getChannels() const
        {
            return m_channels;
        }

        u_char PixelBuffer::rgb2gray_lightness(u_char r, u_char g, u_char b)
        {
            return ( std::max(std::max(r, g), b) + std::min(std::min(r, g), b)) / 2.0f;
        }

        u_char PixelBuffer::rgb2gray_average(u_char r, u_char g, u_char b)
        {
            return (r + g + b) / 3.0f;
        }

        u_char PixelBuffer::rgb2gray_luminosity(u_char r, u_char g, u_char b)
        {
            return (0.21f * r) + (0.72f * g) + (0.07f * b);
        }

        u_char PixelBuffer::getR(uint32_t rgba)
        {
            return (u_char) ((rgba >> 24) & 0xFF);
        }

        u_char PixelBuffer::getG(uint32_t rgba)
        {
            return (u_char) ((rgba >> 16) & 0xFF);
        }

        u_char PixelBuffer::getB(uint32_t rgba)
        {
            return (u_char) ((rgba >> 8) & 0xFF);
        }

        u_char PixelBuffer::getA(uint32_t rgba)
        {
            return (u_char) (rgba & 0xFF);
        }

        /* PreviewSource and PreviewView class */
        void PreviewSource::invokeSignalNewFrame()
        {
            m_onNewFrameSignal(*this);
        }

        void PreviewSource::invokeSignalNewTransformMatrix(const RotationMatrix& rm)
        {
            m_onTransformMatrixSignal(rm);
        }

        PreviewSource::~PreviewSource()
        {
            _sig(); // on die
        }

        void PreviewSource::releaseSource()
        {
            _sig();
            _sig.disconnect_all_slots();
            m_onTransformMatrixSignal.disconnect_all_slots();
            m_onNewFrameSignal.disconnect_all_slots();
        }

        boost::signals2::connection PreviewSource::addOnNewPreviewTransformMatrixSlot(sig_ontm_type::slot_type slot)
        {
            return m_onTransformMatrixSignal.connect(slot);
        }

        boost::signals2::connection PreviewSource::addOnNewPreviewFrameSlot(sig_onf_type::slot_type slot)
        {
            return m_onNewFrameSignal.connect(slot);
        }

        boost::signals2::connection PreviewSource::addOnDieSlot(sig_on_die::slot_type f)
        {
            return _sig.connect(f);
        }

        Size PreviewView::getPreviewSize() const
        {
            return _parent->getPreviewSize();
        }

        void PreviewView::set(PreviewSource& c)
        {
            _parent = &c;
            _dieConn = c.addOnDieSlot(boost::bind(&PreviewView::reset_parent_callback, this));
            _TransMatrixConn = c.addOnNewPreviewTransformMatrixSlot(boost::bind(&PreviewView::newUVMatrixCallback, this, _1));
            _newFrameConn = c.addOnNewPreviewFrameSlot(boost::bind(&PreviewView::newPreviewFrameCallback, this, _1));
        }

        PreviewView::PreviewView(PreviewSource& ps)
        {
            set(ps);
            lastPosX = 0;
            lastPosY = 0;
            m_bIsFBOTextureRenderer = false;
        }

        void PreviewView::reset_parent_callback()
        {
            _parent = nullptr;
        }

        bool PreviewView::isValid() const
        {
            return (_parent == nullptr) ? false : true;
        }

        void PreviewView::setIsFBOTextureRenderer(bool isRenderer)
        {
            m_bIsFBOTextureRenderer = isRenderer;
        }

        std::vector<float> PreviewView::genVertexData(const Size& windowSizeParam, const Size& hudElemSize, int posX, int posY, bool fullFill) const
        {
            if (!isValid())
            {
                return std::vector<float>(12);
            }
            Size windowSize = windowSizeParam;
            Size cameraPreviewSize = _parent->getPreviewSize();

             // Because we are rotating preview texture, if we want fill most of possible place // should be handled just in camera class
//           if (windowSize.getWidth() < windowSize.getHeight()) {
//               cameraPreviewSize = cameraPreviewSize.getRotated90(); // 3200 : 2400 swap to 2400 : 3200 because texture is rotated if portrait or upside down
//           }

            Size viewSize;
            if (!fullFill)
            {
                viewSize = hudElemSize.createInternalSizeFor(cameraPreviewSize.getRatio()); // on screen
            } else
            {
                viewSize = hudElemSize.createFullFillSizeFor(cameraPreviewSize.getRatio()); // on screen
            }

            float widthDelta = (float) viewSize.getWidth() / windowSize.getWidth();
            float heightDelta = (float) viewSize.getHeight() / windowSize.getHeight();

            float hudElemCenterX = ((float)posX + (float)hudElemSize.getWidth() / 2.0f) / (float)windowSizeParam.getWidth();
            float hudElemCenterY = ((float)posY + (float)hudElemSize.getHeight() / 2.0f) / (float)windowSizeParam.getHeight();

            float posXDelta = (hudElemCenterX - 0.5f)*2;
            float posYDelta = (hudElemCenterY - 0.5f)*2;

//            L OG_DEBUG("windowSizeParam: ",windowSizeParam);
//            L OG_DEBUG("hudElemSize: ", hudElemSize);
//            L OG_DEBUG("windowSize: ", windowSize);
//            L OG_DEBUG("viewSize: ", viewSize);
//            L OG_DEBUG("widthDelta posxXDelta, heightDelta, posYDelta: ", widthDelta, " ", posXDelta, " ", heightDelta, " ", posYDelta);

            // HudElem have 0.0 to 1.0 axes (so y is inverted). in opengl y grows to up so -posYDelta
            std::vector<float> vertexData{
                -widthDelta + posXDelta, heightDelta - posYDelta, 0.0f, // top left
                -widthDelta + posXDelta, -heightDelta - posYDelta, 0.0f, // bottom left
                widthDelta + posXDelta, -heightDelta - posYDelta, 0.0f, // bottom right
                widthDelta + posXDelta, heightDelta - posYDelta, 0.0f // top right
            };

            return vertexData;
        }

        /* Camera class */
        const int Camera::BACK;
        const int Camera::FRONT;
        bool Camera::s_bInitialized = false;
        //std::unique_ptr<Texture> Camera::textureCamDevTexUnitLock;  // TODO if camera is preview works good, remove it later

        const std::vector<Parameters>& Camera::getConfigurations() const
        {
            return configurations;
        }

        const Parameters& Camera::getParameters() const throw (camera_exception)
        {
        assertCameraOpened();
            return parameters;
        }

        void Camera::setParameters(const Parameters& params) throw (camera_exception, camera_parameters_exception)
        {
                assertCameraOpened();
            if (params.m_camID == m_camID)
            {
                Parameters tmpParams = params;
                tmpParams.sendConfigurationDevice();
                this->parameters = tmpParams;

                // set default configuration values
                setAntibanding(tmpParams.getAntibanding());
                setColorEffect(tmpParams.getColorEffect());
                setWhiteBalance(tmpParams.getWhiteBalance());
                setFlashMode(tmpParams.getFlashMode());

                try{
                    setSceneMode(tmpParams.getSceneMode());
                }
                catch(std::exception& e){
                    LOG_ERR(e.what());
                }

                setFocusMode(tmpParams.getFocusMode());
                setTorchMode(tmpParams.getTorchMode());
                setExposureMode(tmpParams.getExposureMode());

                setZoom(tmpParams.getZoom());
                setExposureCompensation(tmpParams.getExposureCompensation());

                if (tmpParams.getMaxFocusAreas() > 0)
                {
                    setFocusAreas(tmpParams.getFocusAreas());
                }

                setPreviewFPSRange(tmpParams.getFPSRange());

                if (tmpParams.getMaxExposureAreas() > 0)
                {
                    setExposureAreas(tmpParams.getExposureAreas());
                }

                try
                {
                    if (tmpParams.getLensPosition() >= 0)
                    {
                        setLensPositionWithFocusModeFixed(tmpParams.getLensPosition());
                    }
                } catch (std::exception& e)
                {
                    LOG_ERR(e.what());
                }

            }
        }

        std::unique_ptr<PreviewView> Camera::getPreviewView(FILTER filter) throw (camera_exception)
        {
            assertCameraOpened();
            m_newPreviewViewCreated = true;
            std::unique_ptr<PreviewView> previewView;
            // previewView.reset(new FilterPreviewView(*this, FilterPreviewView::kernelSharpSample()));

            switch (filter)
            {
                case FILTER::NONE:
                    previewView.reset(new SimplePreviewView(*this));
                    break;
                case FILTER::NOCTOVISION:
                    previewView.reset(new NoctovisionPreviewView(*this));
                    break;
                case FILTER::HRM:
                    previewView.reset(new HRMPreviewView(*this));
                    break;
                case FILTER::MOVEMENT:
                    previewView.reset(new MovementPreviewView(*this));
                    break;
                case FILTER::SHARP1:
                    previewView.reset(new FilterPreviewView(*this, std::array<float, 9>{{
                        0.0f, -2.0f, 0.0f,
                       -2.0f, 11.0f, -2.0f,
                        0.0f, -2.0f, 0.0f
                     }}));
                    break;
                case FILTER::SHARP2:
                    previewView.reset(new FilterPreviewView(*this, std::array<float, 9> {{
                        -1.0f, -1.0f, -1.0f,
                        -1.0f,  9.0f, -1.0f,
                        -1.0f, -1.0f, -1.0f
                     }}));
                    break;
                case FILTER::FBOTEST: {
                    std::unique_ptr<FBOTextureSource> fboTexSrc(new FBOTextureSource(std::unique_ptr<SimplePreviewView>(new NoctovisionPreviewView(*this))));
                    FBOTextureChain* fboTexChain = new FBOTextureChain(std::move(fboTexSrc));
                    std::unique_ptr<FBOTextureChainElement> fboTexChainElem1(new FBOTextureSimpleChainElement(*this));
                    std::unique_ptr<FBOTextureChainElement> fboTexChainElem2(new FBOTextureSimpleChainElement(*this));
                    //std::unique_ptr<FBOTextureChainElement> fboTexChainElem3(new FBOTextureSimpleChainElement(*this));
                    fboTexChain->addChainElement(std::move(fboTexChainElem1));
                    //fboTexChain->addChainElement(std::move(fboTexChainElem2));
                    //fboTexChain->addChainElement(std::move(fboTexChainElem3));
                    previewView.reset(fboTexChain);
                    break;
                }
                default:
                    previewView.reset(new SimplePreviewView(*this));
                    break;
            }

            return previewView;
        }

        std::unique_ptr<PreviewView> Camera::getPixelBufferPV(const Size& fboSize, FILTER filter) throw (camera_exception)
        {
            assertCameraOpened();
            std::unique_ptr<PreviewView> pv;

            if(fboSize.getWidth() == 0 || fboSize.getHeight() == 0)
                pv.reset(new PixelBufferPreviewView(*this, getPreviewView(filter)));
            else
                pv.reset(new PixelBufferPreviewView(*this, getPreviewView(filter), fboSize));
            m_newPreviewViewCreated = true;
            return pv;
        }

        unsigned Camera::getDisplayOrientation() const
        {
            return degrees;
        }

        void Camera::initOnce()
        {
            sendInit();
            s_bInitialized = true;
        }

        void Camera::open(bool withAudio) throw (camera_permissions_exception, camera_opening_exception, camera_no_device_exception)
        {
            m_withAudio = withAudio;
            sendOpen();
        }

        void Camera::release()
        {
            if (m_opened)
            {
                stopVideo(); // if any
                stopPreview();
                sendRelease();
                this->releaseSource();
                configurations.clear();
                m_opened = false;
                videoLock.clear();
                this->takePicLock.clear();
                this->autoFocusLock.clear();
                this->hardwareTasks.clear();
                LOG_DEBUG("RELEASING CAMERA: ", m_camID);
            }
        }

        bool Camera::canRelease()
        {
            if (takePicLock.test_and_set())
            {
                return false; // was setted before, cant release
            } else
            {
                takePicLock.clear(); // wasnt setted, clear
            }

            if (autoFocusLock.test_and_set())
            { // return previous flag value ( true - was setted)
                return false;
            } else
            {
                autoFocusLock.clear(); // wasnt setted, clear
            }

            return true;
        }

        bool Camera::isOpened() const
        {
            return m_opened;
        }

        bool Camera::isPreviewing() const
        {
            return m_previewing;
        }

        Camera::Camera(int camID) :
        m_camID(camID),
        degrees(0),
        degreesChanged(true),
        m_previewing(false),
        m_opened(false),
        m_withAudio(true),
        parameters(Parameters(camID)),
        m_newPreviewViewCreated(false)
        {
            takePicLock.clear();
            autoFocusLock.clear();

            if(!isInitialized()) // static
                initOnce();
        }

        Camera::~Camera()
        {
            release();
        }

        int Camera::getCamID() const
        {
            return m_camID;
        }

    void Camera::assertCameraOpened() const
        {
            if (!m_opened)
            {
                throw camera_exception("Camera is not opened yet", m_camID);
            }
    }

    void Camera::startPreview() throw (camera_exception)
    {
        assertCameraOpened();
            sendStartPreview();
            takePicLock.clear(); // if startet previewing, there is no taking picture now
        }

        void Camera::stopPreview()
        {
            sendStopPreview();
        }

        void Camera::setAntibanding(int param) throw (camera_exception, camera_parameters_exception)
        {
        assertCameraOpened();
            parameters.setAntibanding(param);
        }

        void Camera::setColorEffect(int param) throw (camera_exception, camera_parameters_exception)
        {
        assertCameraOpened();
            parameters.setColorEffect(param);
        }

        void Camera::setWhiteBalance(int param) throw (camera_exception, camera_parameters_exception)
        {
        assertCameraOpened();
            parameters.setWhiteBalance(param);
        }

        void Camera::setFlashMode(int param) throw (camera_exception, camera_parameters_exception)
        {
        assertCameraOpened();
            parameters.setFlashMode(param);
        }

        void Camera::setSceneMode(int param) throw (camera_exception, camera_parameters_exception)
        {
        assertCameraOpened();
            parameters.setSceneMode(param);
        }

        void Camera::setFocusMode(int param) throw (camera_exception, camera_parameters_exception)
        {
        assertCameraOpened();
            parameters.setFocusMode(param);
        }

        void Camera::setTorchMode(int param) throw (camera_exception, camera_parameters_exception)
        {
        assertCameraOpened();
            parameters.setTorchMode(param);
        }

        void Camera::setExposureMode(int param) throw (camera_exception, camera_parameters_exception)
        {
        assertCameraOpened();
            parameters.setExposureMode(param);
        }

        void Camera::setZoom(float zoom) throw (camera_exception, camera_parameters_exception)
        {
        assertCameraOpened();
            parameters.setZoom(zoom);
        }

        void Camera::setZoomSmoothly(float zoom, const std::function<void(Camera&)>& cb) throw (camera_exception, camera_parameters_exception)
        {
        assertCameraOpened();
            Camera& thisCam = *this;
            sendCallbackSmoothZoom([&thisCam, cb]()
            {
                if (cb)
                {
                    cb(thisCam);
                }
            });

            parameters.setZoomSmoothly(zoom);
        }

        void Camera::setSmoothZoomRatePerSecond(float rate) throw (camera_exception, camera_parameters_exception)
        {
        assertCameraOpened();
            parameters.setSmoothZoomRatePerSecond(rate);
        }

        void Camera::setExposureCompensation(float ev, const std::function<void(Camera&)>& cb) throw (camera_exception, camera_parameters_exception)
        {
        assertCameraOpened();
            Camera& thisCam = *this;
            sendCallbackExposureCompensation([&thisCam, cb]()
            {
                if (cb)
                {
                    cb(thisCam);
                }
            });
            parameters.setExposureCompensation(ev);
        }

        void Camera::setISO(float iso, const std::function<void(Camera&)>& cb) throw (camera_exception, camera_parameters_exception)
        {
        assertCameraOpened();
            Camera& thisCam = *this;
            sendCallbackISO([&thisCam, cb]()
            {
                if (cb)
                {
                    cb(thisCam);
                }
            });
            parameters.setISO(iso);
        }

        void Camera::setExposureDuration(float ed, const std::function<void(Camera&)>& cb) throw (camera_exception, camera_parameters_exception)
        {
        assertCameraOpened();
            Camera& thisCam = *this;
            sendCallbackExposureDuration([&thisCam, cb]()
            {
                if (cb)
                {
                    cb(thisCam);
                }
            });
            parameters.setExposureDuration(ed);
        }

        void Camera::setISOAndED(float iso, float ed, const std::function<void(Camera&)>& cb) throw (camera_exception, camera_parameters_exception)
        {
        assertCameraOpened();
            Camera& thisCam = *this;
            sendCallbackISOAndED([&thisCam, cb]
            {
                if (cb)
                {
                    cb(thisCam);
                }
            });
            parameters.setISOAndED(iso, ed);
        }

        void Camera::setFocusAreas(const std::vector<Area>& areas) throw (camera_exception, camera_parameters_exception)
        {
        assertCameraOpened();
            parameters.setFocusAreas(areas);
        }

        void Camera::setPreviewFPSRange(Range range) throw (camera_exception, camera_parameters_exception)
        {
        assertCameraOpened();
            parameters.setPreviewFPSRange(range);
        }

        void Camera::setExposureAreas(const std::vector<Area>& areas) throw (camera_exception, camera_parameters_exception)
        {
        assertCameraOpened();
            parameters.setExposureAreas(areas);
        }

        void Camera::setLensPositionWithFocusModeFixed(float position, const std::function<void(Camera&)>& cb) throw (camera_exception, camera_parameters_exception)
        {
        assertCameraOpened();
            Camera& thisCam = *this;
            sendCallbackLensPosition([&thisCam, cb]()
            {
                if (cb)
                {
                    cb(thisCam);
                }
            });
            parameters.setLensPositionWithFocusModeFixed(position);
        }

        void Camera::setRotation(int degrees) throw (camera_exception, camera_parameters_exception)
        {
        assertCameraOpened();
            parameters.setRotation(degrees);
        }

        void Camera::autoFocus(const std::function<void(bool, Camera&)>& cb) throw (camera_exception, camera_parameters_exception)
        {
        assertCameraOpened();
            if (autoFocusLock.test_and_set())
            {
                throw camera_exception("Can't auto focus when previous is still focusing ", getCamID());
            }

            Camera& thisCam = *this;
            sendCallbackAutoFocus([&thisCam, cb](bool success) //cb must be a copy
            {
                thisCam.autoFocusLock.clear();
                if (cb)
                {
                    cb(success, thisCam); // on focused callback
                }
            });

            try
            {
                parameters.autoFocus();
            } catch (camera_exception& exc)
            {
                autoFocusLock.clear();
                throw exc;
            }
        }

        void Camera::takePicture(const Picture& picture, const std::function<void(int, Camera&)>& cb) throw (camera_exception, camera_parameters_exception)
        {
            if (!isPreviewing())
            {
                throw camera_exception("Cant take picture when camera is not previewing", getCamID());
            }
            if (takePicLock.test_and_set())
            {
                throw camera_exception("Cant take picture when previous is still taking ", getCamID());
            }

            Camera& thisCam = *this;
            sendCallbackTakePicture([&thisCam, cb](int result)
            {
                thisCam.afterPictureTaken();
                if (cb)
                {
                    cb(result, thisCam); // result callback
                }
            });

            try
            {
                sendTakePicture(picture);
            } catch (camera_exception& exc)
            {
                afterPictureTaken();
                throw exc;
            }
        }

        void Camera::startVideo(const Video& video, const std::function<void(int, Camera&)>& cb)
        {
            if (!isPreviewing())
            {
                throw camera_exception("Cant start video when camera is not previewing", getCamID());
            }
            if (videoLock.test_and_set())
            {
                throw camera_exception("Cant start video when previous is still taking ", getCamID());
            }

            Camera& thisCam = *this;
            sendCallbackStopVideo([&thisCam, cb](int result)
            {
                if (cb)
                {
                    cb(result, thisCam);
                }
            });

            try
            {
                sendStartVideo(video);
            } catch (camera_exception& exc)
            {
                afterVideoStopped();
                throw exc;
            }
        }

        void Camera::stopVideo()
        {
            if (isTakingVideo())
            {
                sendStopVideo();
                afterVideoStopped();
            }
        }

        bool Camera::isTakingVideo()
        {
            if (videoLock.test_and_set())
            {
                return true;
            } else
            {
                videoLock.clear();
                return false;
            }
        }

        void Camera::afterPictureTaken()
        {
            takePicLock.clear();
        }

        void Camera::afterVideoStopped()
        {
            videoLock.clear();
        }

        void Camera::afterSuccessOpenedCamera()
        {
            sendCallbackOnNewFrame();
            videoLock.clear();
            takePicLock.clear();
            autoFocusLock.clear();
        }

        void Camera::pushTask(const std::function<void()>& task) throw (camera_exception)
        {
            assertCameraOpened();
            hardwareTasks.push(task);
        }

        bool Camera::init()
        {
            return true; // user have to open camera by himself
        }

        void Camera::pause()
        {
            stopPreview(); // or release
        }

        void Camera::resume()
        {
            startPreview(); // or open (after release))
        }

        void Camera::update()
        {
            assertCameraOpened();
            step();
        }

        void Camera::finish()
        {
            // user manage camera releasing (or destructor invocation)
            m_done = true;
        }

        Texture* Camera::getTextureOES()
        {
            return textureOES.get();
        }

        Texture* Camera::getTextureY()
        {
            return textureY.get();
        }

        Texture* Camera::getTextureUV()
        {
            return textureUV.get();
        }

        Size Camera::getPreviewSize() const
        {
            if(getDisplayOrientation() == 90 || getDisplayOrientation() == 270 )
                return getParameters().getPreviewSize().getRotated90();
            return getParameters().getPreviewSize();
        }

        Size Camera::getPreviewSize(int deviceOrientationDegrees) const
        {
            if(getDisplayOrientation() == 90 || getDisplayOrientation() == 270 )
                return getParameters().getPreviewSize(deviceOrientationDegrees).getRotated90();
            return getParameters().getPreviewSize(deviceOrientationDegrees);
        }

        std::ostream& operator<<(std::ostream& out, const Camera& cam)
        {
            return out << cam.getFlatten();
        }

    } // end of namespace cam
} // end of namespace aex
