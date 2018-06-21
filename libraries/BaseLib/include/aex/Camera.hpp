/*
 * File:   Camera.hpp
 * Author: darek
 *
 * Created on February 26, 2015, 9:38 AM
 */

#ifndef CAMERA_HPP
#define	CAMERA_HPP

#include <exception>
#include <map>
#include <array>
#include <vector>
#include <functional> // for callbacks
#include <atomic>
#include <queue>
#include <memory>
#include <mutex>
#include <string>

#include <aex/ALoopNode.hpp>
#include <aex/Point.hpp>
#include <aex/Common.hpp>
#include <aex/Singleton.hpp>
#include <boost/signals2.hpp>
#include <aex/ShaderDrw.hpp>

namespace aex {
    class Texture;

    namespace cam {
        class Ratio;
        class Size;
        class Area;
        class Range;
        class RotationMatrix;
        class Parameters;
        class PreviewView;
        class Directory;
        class File;
        class Picture;
        class Camera;

        class camera_exception : public std::runtime_error {
        public:
            explicit camera_exception(const std::string& str, int camID);
            explicit camera_exception(const char* str, int camID);
        protected:
            int cameraID;
        };

        class camera_opening_exception : public camera_exception {
        public:
            explicit camera_opening_exception(const std::string& str, int camID);
            explicit camera_opening_exception(const char* str, int camID);
        };

        class camera_permissions_exception : public camera_exception {
        public:
            explicit camera_permissions_exception(const std::string& str, int camID);
            explicit camera_permissions_exception(const char* str, int camID);
        };

        class camera_no_device_exception : public camera_exception {
        public:
            explicit camera_no_device_exception(const std::string& str, int camID);
            explicit camera_no_device_exception(const char* str, int camID);
        };

        class camera_parameters_exception : public camera_exception {
        public:
            explicit camera_parameters_exception(const std::string& str, int camID, int badDeg, std::string noSupPar);
            explicit camera_parameters_exception(const char* str, int camID, int badDeg, std::string noSupPar);
            int getBadDegree() const;
            std::string getNoSupportedParam() const;

        protected:
            int m_badDegrees;
            std::string noSupportedParam;
        };

        class Ratio {
        public:
            static const double EPSILON;

            Ratio(double w, double h);
            double difference(const Ratio& r2) const;
            Size createSizeWidh(unsigned width) const;
            Size createSizeHeight(unsigned height) const;

            unsigned getW() const;
            unsigned getH() const;
            double aspect() const;

            friend bool operator==(const Ratio& ratio, const Ratio& ratio2);
            friend bool operator==(const Ratio& ratio, const Size& size);
            friend bool operator==(const Size& size, const Ratio& ratio);
            friend std::ostream& operator<<(std::ostream& out, const Ratio& r);
        private:

            void calculateMinIntValues();
            unsigned m_minIntW;
            unsigned m_minIntH;
            double m_w;
            double m_h;
        };
        bool operator==(const Ratio& ratio, const Ratio& ratio2);
        bool operator==(const Ratio& ratio, const Size& size);
        bool operator==(const Size& size, const Ratio& ratio);

        class Size {
        public:
            Size();
            Size(unsigned width, unsigned height);

            Size getRotated90() const;
            int getWidth() const;
            int getHeight() const;
            long long int field() const;
            Ratio getRatio() const;
            Size createInternalSizeFor(const Ratio& r2) const;
            Size createFullFillSizeFor(const Ratio& r2) const;

            bool operator==(const Size& second) const;
            bool operator!=(const Size& second) const;
            bool operator>(const Size& second) const;
            bool operator<(const Size& second) const;
            bool operator>=(const Size& second) const;
            bool operator<=(const Size& second) const;

            friend std::ostream& operator<<(std::ostream& out, const Size& size);
            friend bool operator==(const Ratio& ratio, const Size& size);
            friend bool operator==(const Size& size, const Ratio& ratio);
        private:
            unsigned m_width;
            unsigned m_height;

        };
        std::ostream& operator<<(std::ostream& out, const Size& size);

        class Area {
        public:
            Area();
            Area(FPoint2D center, float xDiameter, float yDiameter, float weight);
            Area(FPoint2D center, float radius, float weight);
            Area(FPoint2D center);

            float getLeft() const;
            float getTop() const;
            float getRight() const;
            float getBottom() const;
            FPoint2D getLeftTop() const;
            FPoint2D getRightBottom() const;

            FPoint2D getCenter() const;
            float getWeight() const;

            bool operator==(const Area& second) const;
            bool operator!=(const Area& second) const;

            friend std::ostream& operator<<(std::ostream& out, const Area& p);

        private:
            FPoint2D center;
            float rLeft;
            float rTop;
            float rRight;
            float rBottom;
            float weight;
        };

        class Range {
        public:
            Range();
            Range(float min, float max);

            float getMin() const;
            float getMax() const;
            bool contains(const Range& range) const;
            float cutToRange(float value) const;

            bool operator==(const Range& r2);
            bool operator!=(const Range& r2);

            friend std::ostream& operator<<(std::ostream& out, const Range& r);
        private:
            float min;
            float max;
        };

        class RotationMatrix {
        public:
            RotationMatrix();

            RotationMatrix getRotatedRight() const;
            RotationMatrix getRotatedLeft() const;

            std::array<float, 16> getMatrixArray() const;
            unsigned getDegrees() const;

            friend class Camera;
        private:
            RotationMatrix(std::array<float, 16> matrix, unsigned degrees);
            RotationMatrix(unsigned degrees, bool frontCam = false);

            RotationMatrix createRotationMatrix0();
            RotationMatrix createRotationMatrix90();
            RotationMatrix createRotationMatrix180();
            RotationMatrix createRotationMatrix270();

            RotationMatrix createRotationMatrix0FrontCam();
            RotationMatrix createRotationMatrix90FrontCam();
            RotationMatrix createRotationMatrix180FrontCam();
            RotationMatrix createRotationMatrix270FrontCam();

            friend std::ostream& operator<<(std::ostream& out, const RotationMatrix& rmat);

        private:
            std::array<float, 16> matrix;
            unsigned degrees;
        };

        class Parameters {
        public:

            static const int RANGE = 100;
            // Values for antibanding settings.
            static const int ANTIBANDING_DEFAULT = (RANGE * 1);
            static const int ANTIBANDING_AUTO = (RANGE * 1) + 1;
            static const int ANTIBANDING_50HZ = (RANGE * 1) + 2;
            static const int ANTIBANDING_60HZ = (RANGE * 1) + 3;
            static const int ANTIBANDING_OFF = (RANGE * 1) + 4;

            // Values for white balance settings.
            static const int WHITE_BALANCE_DEFAULT = (RANGE * 2);
            static const int WHITE_BALANCE_AUTO = (RANGE * 2) + 1;
            static const int WHITE_BALANCE_INCANDESCENT = (RANGE * 2) + 2;
            static const int WHITE_BALANCE_FLUORESCENT = (RANGE * 2) + 3;
            static const int WHITE_BALANCE_WARM_FLUORESCENT = (RANGE * 2) + 4;
            static const int WHITE_BALANCE_DAYLIGHT = (RANGE * 2) + 5;
            static const int WHITE_BALANCE_CLOUDY_DAYLIGHT = (RANGE * 2) + 6;
            static const int WHITE_BALANCE_TWILIGHT = (RANGE * 2) + 7;
            static const int WHITE_BALANCE_SHADE = (RANGE * 2) + 8;
            static const int WHITE_BALANCE_LOCKED = (RANGE * 2) + 9;
            static const int WHITE_BALANCE_CONTINUOUS_AUTO = (RANGE * 2) + 10;

            // Values for color effect settings.
            static const int COLOR_EFFECT_DEFAULT = (RANGE * 3);
            static const int COLOR_EFFECT_NONE = (RANGE * 3) + 1;
            static const int COLOR_EFFECT_MONO = (RANGE * 3) + 2;
            static const int COLOR_EFFECT_NEGATIVE = (RANGE * 3) + 3;
            static const int COLOR_EFFECT_SOLARIZE = (RANGE * 3) + 4;
            static const int COLOR_EFFECT_SEPIA = (RANGE * 3) + 5;
            static const int COLOR_EFFECT_POSTERIZE = (RANGE * 3) + 6;
            static const int COLOR_EFFECT_WHITEBOARD = (RANGE * 3) + 7;
            static const int COLOR_EFFECT_BLACKBOARD = (RANGE * 3) + 8;
            static const int COLOR_EFFECT_AQUA = (RANGE * 3) + 9;

            // Values flash mode
            static const int FLASH_MODE_DEFAULT = (RANGE * 4);
            static const int FLASH_MODE_OFF = (RANGE * 4) + 1;
            static const int FLASH_MODE_ON = (RANGE * 4) + 2;
            static const int FLASH_MODE_AUTO = (RANGE * 4) + 3;
            static const int FLASH_MODE_RED_EYE = (RANGE * 4) + 4;

            // Keys scene mode
            static const int SCENE_MODE_DEFAULT = (RANGE * 5);
            static const int SCENE_MODE_AUTO = (RANGE * 5) + 1;
            static const int SCENE_MODE_ACTION = (RANGE * 5) + 2;
            static const int SCENE_MODE_PORTRAIT = (RANGE * 5) + 3;
            static const int SCENE_MODE_LANDSCAPE = (RANGE * 5) + 4;
            static const int SCENE_MODE_NIGHT = (RANGE * 5) + 5;
            static const int SCENE_MODE_NIGHT_PORTRAIT = (RANGE * 5) + 6;
            static const int SCENE_MODE_THEATRE = (RANGE * 5) + 7;
            static const int SCENE_MODE_BEACH = (RANGE * 5) + 8;
            static const int SCENE_MODE_SNOW = (RANGE * 5) + 9;
            static const int SCENE_MODE_SUNSET = (RANGE * 5) + 10;
            static const int SCENE_MODE_STEADYPHOTO = (RANGE * 5) + 11;
            static const int SCENE_MODE_FIREWORKS = (RANGE * 5) + 12;
            static const int SCENE_MODE_SPORTS = (RANGE * 5) + 13;
            static const int SCENE_MODE_PARTY = (RANGE * 5) + 14;
            static const int SCENE_MODE_CANDLELIGHT = (RANGE * 5) + 15;
            static const int SCENE_MODE_BARCODE = (RANGE * 5) + 16;
            static const int SCENE_MODE_HDR = (RANGE * 5) + 17;

            // Focus values
            static const int FOCUS_MODE_DEFAULT = (RANGE * 6);
            static const int FOCUS_MODE_AUTO = (RANGE * 6) + 1;
            static const int FOCUS_MODE_INFINITY = (RANGE * 6) + 2;
            static const int FOCUS_MODE_MACRO = (RANGE * 6) + 3;
            static const int FOCUS_MODE_FIXED = (RANGE * 6) + 4;
            static const int FOCUS_MODE_EDOF = (RANGE * 6) + 5;
            static const int FOCUS_MODE_CONTINUOUS_VIDEO = (RANGE * 6) + 6;
            static const int FOCUS_MODE_CONTINUOUS_PICTURE = (RANGE * 6) + 7;

            // Torch values
            static const int TORCH_MODE_DEFAULT = (RANGE * 7);
            static const int TORCH_MODE_OFF = (RANGE * 7) + 1;
            static const int TORCH_MODE_ON = (RANGE * 7) + 2;
            static const int TORCH_MODE_AUTO = (RANGE * 7) + 3;

            // Exposure values
            static const int EXPOSURE_MODE_DEFAULT = (RANGE * 8);
            static const int EXPOSURE_MODE_AUTO = (RANGE * 8) + 1;
            static const int EXPOSURE_MODE_FIXED = (RANGE * 8) + 2;
            static const int EXPOSURE_MODE_CONTINUOUS_AUTO_EXPOSURE = (RANGE * 8) + 3;
            static const int EXPOSURE_MODE_CUSTOM = (RANGE * 8) + 4;

        public:
            bool isSupported(int param) const;

            int getAntibanding() const;
            int getColorEffect() const;
            int getWhiteBalance() const;
            int getFlashMode() const;
            int getSceneMode() const;
            int getFocusMode() const;
            int getTorchMode() const;
            int getExposureMode() const;

            Size getPreviewSize() const;
            Size getPreviewSize(int deviceOrientationDegrees) const;

            bool isSmoothlyZoomSupported() const;

            float getSmoothZoomRatePerSecond() const;
            float getZoom() const;
            float getMaxOpticalZoom() const;
            float getMaxZoom() const;
            float getMinZoom() const;
            float getFocalLengthMilimeters() const;

            float getExposureCompensationStep() const;
            float getExposureCompensation() const;
            float getMinExposureCompensation() const;
            float getMaxExposureCompensation() const;

            float getMinISO() const;
            float getMaxISO() const;
            float getISO() const;

            float getMinExposureDuration() const;
            float getMaxExposureDuration() const;
            float getExposureDuration() const;

            int getMaxExposureAreas() const;
            std::vector<Area> getExposureAreas() const;

            int getMaxFocusAreas() const;
            std::vector<Area> getFocusAreas() const;

            std::vector<float> getFocusDistances() const; // in meters

            Range getFPSRange() const;
            std::vector<Range> getSupportedFPSRanges() const;

            float getHorizontalViewAngle() const;
            float getVerticalViewAngle() const;

            float getLensAperture() const;
            float getLensPosition() const;
            float getMinLensPosition() const;
            float getMaxLensPosition() const;
            int getRotation() const;

            float getFOV() const;

            const std::vector<Size>& getSupportedPictureSizes() const;
            const std::vector<Size>& getSupportedVideoSizes() const;

            bool operator==(const Parameters& second) const;
            bool operator>(const Parameters& second) const;
            bool operator<(const Parameters& second) const;
            bool operator>=(const Parameters& second) const;
            bool operator<=(const Parameters& second) const;
            friend std::ostream& operator<<(std::ostream& out, const Parameters& params);
            friend class Camera;
        private:

            // sending methods (platform dependend)
            void sendAntibanding(int param) throw (camera_parameters_exception);
            void sendColorEffect(int param) throw (camera_parameters_exception);
            void sendWhiteBalance(int param) throw (camera_parameters_exception);
            void sendFlashMode(int param) throw (camera_parameters_exception);
            void sendSceneMode(int param) throw (camera_parameters_exception);
            void sendFocusMode(int param) throw (camera_parameters_exception);
            void sendTorchMode(int param) throw (camera_parameters_exception);
            void sendExposureMode(int param) throw (camera_parameters_exception);

            void sendZoom(float zoom) throw (camera_parameters_exception);
            void sendZoomSmoothly(float zoom) throw (camera_parameters_exception);
            void sendExposureCompensation(float ev) throw (camera_parameters_exception);
            void sendISO(float iso) throw (camera_parameters_exception);
            void sendExposureDuration(float ed) throw (camera_parameters_exception);
            void sendISOAndED(float iso, float ed) throw (camera_parameters_exception);
            void sendFocusAreas(const std::vector<Area>& areas) throw (camera_parameters_exception);
            void sendPreviewFPSRange(Range range) throw (camera_parameters_exception);
            void sendExposureAreas(const std::vector<Area>& areas) throw (camera_parameters_exception);
            void sendLensPositionWithFocusModeFixed(float position) throw (camera_parameters_exception);
            void sendRotation(int degrees) throw (camera_exception, camera_parameters_exception);

            void sendAutoFocus() throw (camera_parameters_exception);

            bool sendConfigurationDevice() throw (camera_parameters_exception);

            /* loading from devices */
            float loadZoom() const;
            float loadExposureCompensation() const;
            std::vector<Area> loadExposureAreas() const;
            std::vector<Area> loadFocusAreas() const;
            std::vector<float> loadFocusDistances() const;
            float loadFocalLength() const;
            float loadHorizontalViewAngle() const;
            float loadVerticalViewAngle() const;
            float loadLensAperture() const;
            float loadLensPosition() const;
            float loadFOV() const;
            float loadISO() const;
            float loadExposureDuration() const;
            /* end loading devices */

            // multiplatform:
            void setAntibanding(int param) throw (camera_parameters_exception);
            void setColorEffect(int param) throw (camera_parameters_exception);
            void setWhiteBalance(int param) throw (camera_parameters_exception);
            void setFlashMode(int param) throw (camera_parameters_exception);
            void setSceneMode(int param) throw (camera_parameters_exception);
            void setFocusMode(int param) throw (camera_parameters_exception);
            void setTorchMode(int param) throw (camera_parameters_exception);
            void setExposureMode(int param) throw (camera_parameters_exception);

            void setZoom(float zoom) throw (camera_parameters_exception);
            void setZoomSmoothly(float zoom) throw (camera_parameters_exception);
            void setSmoothZoomRatePerSecond(float rate) throw (camera_parameters_exception);
            void setExposureCompensation(float ev) throw (camera_parameters_exception);
            void setISO(float iso) throw (camera_parameters_exception);
            void setExposureDuration(float ed) throw (camera_parameters_exception);
            void setISOAndED(float iso, float ed) throw (camera_parameters_exception);
            void setFocusAreas(const std::vector<Area>& areas) throw (camera_parameters_exception);
            void setPreviewFPSRange(Range range) throw (camera_parameters_exception);
            void setExposureAreas(const std::vector<Area>& areas) throw (camera_parameters_exception);
            void setLensPositionWithFocusModeFixed(float position) throw (camera_parameters_exception);
            void setRotation(int degrees) throw (camera_exception, camera_parameters_exception);

            void autoFocus() throw (camera_exception, camera_parameters_exception); // TODO move to camera class


            void setPreviewResolution(Size sizep);
            static std::vector<float> areasToFloatVector(const std::vector<Area>& areas);
            static std::vector<Area> floatVectorToAreas(const std::vector<float>& vec);
            static std::vector<Range> floatVectorToRanges(const std::vector<float>& vec);
            static std::vector<Size> intVectorToSizes(const std::vector<int>& vec);
            bool checkIfAreasChanged(const std::vector<Area> tmpAreas, const std::vector<Area> lastAreas) const;
            void checkAreas(const std::vector<Area>& areas, int maxPossible) throw (camera_parameters_exception);

            std::ostream& longDescription(std::ostream& out) const;
            std::ostream& shortDescription(std::ostream& out) const;

            Parameters(int camID);
            /* camera video params */
            int antibanding;
            int colorEffect;
            int whiteBalance;
            int flashMode;
            int sceneMode;
            int focusMode;
            int torchMode;
            int exposureMode;

            int m_camID;
            Size size; // preview size

            /* zoom */
            float zoom; // need to send params 2 dev
            float maxZoom;
            bool smoothlyZoom;
            bool smoothlyZoomSupported;
            float maxOpticalZoom; // iosParam
            float smoothZoomRate;
            float focalLength;

            /* exposure */
            float exposureCompStep;
            float exposureComp; // need to send params 2 dev
            float maxExposureComp;
            float minExposureComp;

            float iso;
            float minISO;
            float maxISO;
            float exposureDuration;
            float minExposureDuration;
            float maxExposureDuration;

            int maxExpAreas;
            std::vector<Area> expAreas; // need to send params 2 dev

            /* focus areas */
            int maxFocusAreas;
            std::vector<Area> focusAreas; // need to send params 2 dev
            std::vector<float> focusDistances; // [min, optimal, far] in meters, // need to send params 2 dev

            Range fpsRange;
            std::vector<Range> supportedFpsRanges;

            float hViewAngle;
            float vViewAngle;

            float lensAperture;
            float lensPosition;

            float fov;

            int rotation; // cam device

            std::vector<Size> supportedPictureSizes;
            std::vector<Size> supportedVideoSizes;

            int deviceConfigID; // iOS AVCaptureDeviceFormat index
        };

        class directory_exception : public std::runtime_error {
        public:
            explicit directory_exception(const std::string& str);
            explicit directory_exception(const char* str);
        protected:
            int cameraID;
        };

        class Directory {
        public:
            static const int ALARMS = 0x00;
            static const int DCIM = 0x01;
            static const int DOWNLOADS = 0x03;
            static const int MOVIES = 0x04;
            static const int MUSIC = 0x05;
            static const int NOTIFICATIONS = 0x06;
            static const int PICTURES = 0x07;
            static const int PODCAST = 0x08;
            static const int RINGTONES = 0x09;
            static const int DOCUMENTS = 0x0A;

            Directory();

            static Directory getUserDataDirectory();
            static Directory getCacheDirectory();
            static Directory getExternalStorageDirectory();
            static Directory getDirectory(int type);
            Directory mkdir(const std::string& name) throw (directory_exception);

            const std::string& getPath() const;

            friend std::ostream& operator<<(std::ostream& out, const Directory& dir);
        private:
            Directory(const std::string& path);
            std::string path;
        };

        class File {
        public:
            File(const Directory& dir, const std::string& fileName);

            const Directory& getDir() const;
            const std::string& getFileName() const;
            std::string getFullPath() const;

            friend std::ostream& operator<<(std::ostream& out, const File& file);
        private:
            Directory dir;
            std::string fileName;
        };

        enum FORMAT {
            JPG = 0x01,
        };

        enum RESULT {
            SUCCESS = 0x01,
            FAILURE = 0x00,
            NOT_ENOUGHT_MEMORY = 0x02,
            FILE_NOT_FOUND = 0x03,
            IO_EXCEPTION = 0x04
        };

        /* For Camera.getPreviewView(FILTER filter)*/
        enum FILTER {
            NONE = 0,
            NOCTOVISION,
            HRM,
            MOVEMENT,
            SHARP1,
            SHARP2,
            FBOTEST = 9999
        };

        // capsules
        class FilterData
        {
        public:
            FilterData(FILTER filter) : filter(filter) {}
            FILTER filter;
        };

        class FilterMovementData : public FilterData
        {
        public:
            FilterMovementData(FILTER filter) : FilterData(filter) {
                if(filter != FILTER::MOVEMENT)
                    throw std::runtime_error("This capsule is only for FILTER::MOVEMENT");
            }
            float moveX;
            float moveY;
        };

        class Picture {
        public:
            Picture(Camera& cam, const File& file, Size s = Size(0, 0), FORMAT f = FORMAT::JPG) throw (camera_exception, camera_parameters_exception);
            Picture(Camera& cam, const std::string& albumName = "", Size s = Size(0, 0), FORMAT f = FORMAT::JPG) throw (camera_exception, camera_parameters_exception);

            bool isSupported(FORMAT format, const Camera& cam);
            FORMAT getFormat() const;
            Size getSize() const;
            int getJPGQuality() const;
            void setJPGQuality(int percent);
            int getMaxJPGQuality() const;
            int getMinJPGQuality() const;
            const std::string& getFullFileName() const;
            bool saveInAlbum() const;
            const std::string& getAlbumName() const;
            FILTER getFilter() const;
            void setFilter(FILTER filter);

            static void openAlbumInGallery(const std::string& albumName); // TODO move method to other class or reimplement to not need camID (the best way - create new class)

            friend class Camera;
        private:
            void setSize(Size s) throw (camera_parameters_exception);
            void setFormat(FORMAT format);
            const File& getFile() const;
            bool sendIsSupportedPictureFormat(FORMAT format, int camID);
            static void sendOpenAlbumInGallery(const std::string& albumName);

        private:
            Camera& cam;
            File file;
            std::string albumName;
            FORMAT format;
            Size size;
            int JPGQuality;
            bool inAlbum;
            std::string pictureFilePath;
            FILTER filter;
        };

        /* FORMAT, Quality, Size etc */
        class Video {
        public:
            Video(Camera& cam, const std::string& albumName = "", Size s = Size(0, 0)) throw (camera_exception, camera_parameters_exception);

            bool needSaveInAlbum() const;
            const std::string& getAlbumName() const;
            const std::string& getFullFileName() const;
            Size getSize() const;
        private:
            bool inAlbum;
            std::string videoFilePath;
            std::string albumName;
            Size size;
        };

        class ThreadSafeTasks {
        private:
            std::mutex newTaskMutex;
            std::queue<std::function<void()> > newTasks;

            std::mutex m;
            std::queue<std::function<void()> > tasks;

            void transferNewTasks();

        public:
            void push(const std::function<void()>& t);
            void invokeAll();
            int size() const;
            void clear();
        };

        class PixelBufferPreviewView;
        class PixelBuffer {
        public:
            friend class PixelBufferPreviewView;

            PixelBuffer(uint32_t width, uint32_t height, uint32_t channels = 4);
            ~PixelBuffer();

            u_char getR(int row, int col) const;
            u_char getG(int row, int col) const;
            u_char getB(int row, int col) const;
            u_char getA(int row, int col) const;
            uint32_t getRGBA(int row, int col) const;

            u_char* getRawData();
            uint32_t getWidth() const;
            uint32_t getHeight() const;
            uint32_t getRows() const;
            uint32_t getCols() const;
            uint32_t getChannels() const;

            static u_char rgb2gray_lightness(u_char r, u_char g, u_char b);
            static u_char rgb2gray_average(u_char r, u_char g, u_char b);
            static u_char rgb2gray_luminosity(u_char r, u_char g, u_char b);
            static u_char getR(uint32_t rgba);
            static u_char getG(uint32_t rgba);
            static u_char getB(uint32_t rgba);
            static u_char getA(uint32_t rgba);

            static uint32_t getSwapped(uint32_t value);
        protected:
            uint32_t getDataIndex(int row, int col) const;
        protected:
            uint32_t m_width, m_height;
            uint32_t m_channels;
            u_char* m_data;
        };

        class PreviewSource {
        public:
            static const int LANDSCAPE_RIGHT = 0;
            static const int LANDSCAPE_LEFT = 180;
            static const int PORTRAIT = 90;
            static const int PORTRAIT_UPSIDE_DOWN = 270;
        public:
            typedef boost::signals2::signal<void() > sig_on_die;
            typedef boost::signals2::signal<void (RotationMatrix) > sig_ontm_type;
            typedef boost::signals2::signal<void (PreviewSource&) > sig_onf_type;

            void invokeSignalNewFrame();
            void invokeSignalNewTransformMatrix(const RotationMatrix& rm);

            virtual Texture* getTextureOES() = 0;
            virtual Texture* getTextureY() = 0;
            virtual Texture* getTextureUV() = 0;
            virtual Size getPreviewSize() const = 0;
            virtual Size getPreviewSize(int deviceOrientationDegrees) const = 0;

            boost::signals2::connection addOnNewPreviewTransformMatrixSlot(sig_ontm_type::slot_type slot);
            boost::signals2::connection addOnNewPreviewFrameSlot(sig_onf_type::slot_type slot);
            boost::signals2::connection addOnDieSlot(sig_on_die::slot_type f);

            virtual ~PreviewSource();
            friend class PreviewView;
        protected:
            void releaseSource();

            sig_ontm_type m_onTransformMatrixSignal;
            sig_onf_type m_onNewFrameSignal;
            sig_on_die _sig;
        };

        class PreviewView : public DrawObject {
        public:
            virtual void renderIn(const Size& frameSize, const Size& componetSize, int posX, int posY, bool fullFill = false) = 0;

            virtual void draw(AexRenderer* gl) = 0;
            virtual void finishdraw(AexRenderer* gl) = 0;
            virtual void needsUpdate() = 0;

            virtual ~PreviewView() {
            }
            Size getPreviewSize() const;
            Size getPreviewSize(int deviceOrientationDegrees) const;
            void set(PreviewSource& c);
            virtual void setAction(const std::function<void (aex::cam::PixelBuffer&)>& action) {}
            virtual void setData(FilterData* data) {}
            void setIsFBOTextureRenderer(bool isRenderer);

            friend class Camera;

        protected:
            PreviewView(PreviewSource& ps);
            virtual void newUVMatrixCallback(RotationMatrix m) = 0;
            virtual void newPreviewFrameCallback(PreviewSource& src) = 0;
            void reset_parent_callback();
            bool isValid() const;

            std::vector<float> genVertexData(const Size& windowSizeParam, const Size& hudElemSize, int posX, int posY, bool fullFill = false) const; // gen vertex data in ndc

            Size lastWindowSize;
            Size lastHudElemSize;
            int lastPosX, lastPosY;
            bool m_bIsFBOTextureRenderer;

            RotationMatrix uvMatrix;

            boost::signals2::scoped_connection _dieConn; // needed for unconnecting from source (camera eg), when this object dies
            boost::signals2::scoped_connection _TransMatrixConn;
            boost::signals2::scoped_connection _newFrameConn;

            PreviewSource* _parent;
        };

        /* Public user interface for using Camera in every platform*/
        class Camera : public aex::ALoopNode, public PreviewSource {
        public:

            struct delete_camera_impl {
                void operator()(void*); // CameraImpl*
            };

            typedef std::unique_ptr<void, delete_camera_impl> cam_impl;

            static const int BACK = 0;
            static const int FRONT = 1;

            Camera(int camId);
            virtual ~Camera();
            int getCamID() const;
            // Preview methods
        private:
            void step();
            void updateTexture();
            void updateTransformMatrix();
        void assertCameraOpened() const;
        public:
            void startPreview() throw (camera_exception);
            void stopPreview();
            const std::vector<Parameters>& getConfigurations() const;
            const Parameters& getParameters() const throw (camera_exception);
            std::unique_ptr<PreviewView> getPreviewView(FILTER filter = FILTER::NONE) throw (camera_exception);
            std::unique_ptr<PreviewView> getPixelBufferPV(const Size& fboSize = Size(0,0), FILTER filter = FILTER::NONE) throw (camera_exception);
        public:

            void setParameters(const Parameters& params) throw (camera_exception, camera_parameters_exception); // throw
            bool setDisplayOrientation(unsigned degrees) throw (camera_exception, camera_parameters_exception);
            unsigned getDisplayOrientation() const;

            static int getNumberOfCameras();
            static void initOnce();
            static bool isInitialized() {return s_bInitialized;}
            void open(bool withAudio = false) throw (camera_permissions_exception,camera_opening_exception, camera_no_device_exception); // throw
            void release();
            bool canRelease(); // cant release if there is some not invoked callbacks yet
        private:
            static void sendInit();
            void sendOpen() throw (camera_permissions_exception,camera_opening_exception, camera_no_device_exception); // throw
            void sendRelease();
        public:
            bool isOpened() const;
            bool isPreviewing() const;

            // parameters setting
            void setAntibanding(int param) throw (camera_exception, camera_parameters_exception);
            void setColorEffect(int param) throw (camera_exception, camera_parameters_exception);
            void setWhiteBalance(int param) throw (camera_exception, camera_parameters_exception);
            void setFlashMode(int param) throw (camera_exception, camera_parameters_exception);
            void setSceneMode(int param) throw (camera_exception, camera_parameters_exception);
            void setFocusMode(int param) throw (camera_exception, camera_parameters_exception);
            void setTorchMode(int param) throw (camera_exception, camera_parameters_exception);
            void setExposureMode(int param) throw (camera_exception, camera_parameters_exception);

            void setZoom(float zoom) throw (camera_exception, camera_parameters_exception);
            void setZoomSmoothly(float zoom, const std::function<void(Camera&)>& cb = [](Camera& ref) {
                /*nothing*/
            }) throw (camera_exception, camera_parameters_exception);
            void setSmoothZoomRatePerSecond(float rate) throw (camera_exception, camera_parameters_exception);
            void setExposureCompensation(float ev, const std::function<void(Camera&)>& cb = [](Camera& ref) {
                /*nothing*/
            }) throw (camera_exception, camera_parameters_exception);
            void setISO(float iso, const std::function<void(Camera&)>& cb = [](Camera& ref) {
            }) throw (camera_exception, camera_parameters_exception);
            void setExposureDuration(float ed, const std::function<void(Camera&)>& cb = [](Camera& ref) {
            }) throw (camera_exception, camera_parameters_exception);
            void setISOAndED(float iso, float ed, const std::function<void(Camera&)>& cb = [](Camera& ref) {
            }) throw (camera_exception, camera_parameters_exception);
            void setFocusAreas(const std::vector<Area>& areas) throw (camera_exception, camera_parameters_exception);
            void setPreviewFPSRange(Range range) throw (camera_exception, camera_parameters_exception);
            void setExposureAreas(const std::vector<Area>& areas) throw (camera_exception, camera_parameters_exception);
            void setLensPositionWithFocusModeFixed(float position, const std::function<void(Camera&)>& cb = [](Camera& ref) {
                /*nothing*/
            }) throw (camera_exception, camera_parameters_exception);
            void setRotation(int degrees) throw (camera_exception, camera_parameters_exception);
            void autoFocus(const std::function<void(bool, Camera&)>& cb) throw (camera_exception, camera_parameters_exception);
            void takePicture(const Picture& picture, const std::function<void(int, Camera&)>& cb) throw (camera_exception, camera_parameters_exception);
            void startVideo(const Video& video, const std::function<void(int, Camera&)>& cb);
            void stopVideo();
            bool isTakingVideo();
            bool isLowLightBoostSupported() const throw(camera_exception);
            void setLowLightBoost(bool enabled) throw(camera_exception);
        private:
            void afterPictureTaken();
            void afterSuccessOpenedCamera();
            void afterVideoStopped();
        public:

            void pushTask(const std::function<void()>& task) throw (camera_exception);

            // ALoopNode interface
            virtual bool init();
            virtual void pause();
            virtual void resume();
            virtual void update();
            virtual void finish();

            // PreviewSource interface
            virtual Texture* getTextureOES();
            virtual Texture* getTextureY();
            virtual Texture* getTextureUV();

            Size getPreviewSize() const;
            Size getPreviewSize(int deviceOrientationDegrees) const;

            friend class PreviewView;
            friend std::ostream& operator<<(std::ostream& out, const Camera& cam);
        private:
            int m_camID;
            unsigned degrees;
            bool degreesChanged;
            bool m_previewing;
            bool m_opened;
            bool m_withAudio;

            Parameters parameters;
            std::vector<Parameters> configurations;

            cam_impl m_camImpl;

            RotationMatrix m_rotationMatrix;
            bool m_newPreviewViewCreated;

            // thread safe (callback can add take picture task on eg. producers->consumer)
            ThreadSafeTasks hardwareTasks;

            std::atomic_flag takePicLock;
            std::atomic_flag autoFocusLock;
            std::atomic_flag videoLock;

            std::unique_ptr<Texture> textureOES;
            std::unique_ptr<Texture> textureY;
            std::unique_ptr<Texture> textureUV;
            //static std::unique_ptr<Texture> textureCamDevTexUnitLock; // TODO if camera is preview works good, remove it later
            static bool s_bInitialized;

            Camera(Camera const&) = delete;
            Camera& operator=(Camera const&) = delete;
            void loadConfigurations();

            void sendStartPreview() throw (camera_exception);
            void sendStopPreview();
            void sendCallbackExposureCompensation(const std::function<void()>& cb);
            void sendCallbackLensPosition(const std::function<void()>& cb);
            void sendCallbackSmoothZoom(const std::function<void()>& cb);
            void sendCallbackAutoFocus(const std::function<void(bool)>& cb);
            void sendCallbackTakePicture(const std::function<void(int) >& cb);
            void sendCallbackISO(const std::function<void()>& cb);
            void sendCallbackExposureDuration(const std::function<void()>& cb);
            void sendCallbackISOAndED(const std::function<void()>& cb);
            void sendCallbackStopVideo(const std::function<void(int)>& cb);

            void sendTakePicture(const Picture& picture) throw (camera_parameters_exception);
            void sendStartVideo(const Video& video) throw (camera_parameters_exception);
            void sendStopVideo();
            void sendCallbackOnNewFrame();

            std::string getFlatten() const;
        };
    } // end of namespace cam
} // end of namespace aex

#endif	/* CAMERA_HPP */
