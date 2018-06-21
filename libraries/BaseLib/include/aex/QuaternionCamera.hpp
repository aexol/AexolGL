//-----------------------------------------------------------------------------
//Copyright (c) 2007-2008 dhpoware. All Rights Reserved.
//
//Permission is hereby granted, free of charge, to any person obtaining a
//copy of this software and associated documentation files (the "Software"),
//to deal in the Software without restriction, including without limitation
//the rights to use, copy, modify, merge, publish, distribute, sublicense,
//and/or sell copies of the Software, and to permit persons to whom the
//Software is furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in
//all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#ifndef __aexolGL__QuaternionCamera__
#define __aexolGL__QuaternionCamera__

#include <aex/Vector3.hpp>
#include <aex/Vector2.hpp>
#include <aex/Quaternion.hpp>
#include <aex/Matrix.hpp>
#include <aex/Matrix3.hpp>
#include <aex/Frustum.hpp>

//Bug in boost. Compiler is not properly detected and some boost/type_traits headers
//are not included.
#if defined(_MSC_BUILD) && defined(__clang__)
#include <boost/type_traits/is_default_constructible.hpp>
#include <boost/type_traits/is_assignable.hpp>
#endif
#include <boost/signals2.hpp>
#include <mutex>
namespace aex {

    class AWindow;
    class Aex;
    class AexRenderer;

    enum class CAMERA_MODES {
        NONE = 0, ORBITING_MODE = 1, FP_MODE = 2, FLIGHT_MODE = 3, SPECTATOR_MODE = 4
    };

    typedef aex::math::Vector3 Vector3;
    
    class AEX_PUBLIC_API QuaternionCamera {
        //typedef aex::math::Matrix Matrix;
        typedef aex::math::Matrix3 Matrix3;
        //typedef aex::math::Vector3 Vector3;
        
        typedef aex::math::Vector2 Vector2;
        typedef aex::math::Quaternion Quaternion;
        typedef aex::Frustum Frustum;
    public:
        
        void setDirtyFlag();
        QuaternionCamera();
        virtual ~QuaternionCamera();

        /**
         *	@brief	Camera look at target
         *
         *	@param 	Vector 	target
         */
        
        void reset();
        
        void lookAt(Vector3 target);
        void lookFrom(Vector3 eye);

        void lookAt(Vector3 eye, Vector3 target);

        /**
         *	@brief	Camera look at eye,target,up vec
         *
         *	@param 	Vector 	Eye position
         *	@param 	Vector 	target position
         *	@param 	Vector 	up Vector
         */
        void lookAt(Vector3 eye, Vector3 target, Vector3 up);

        /**
         *	@brief	  Moves the camera by the specified amount of world units in the specified direction in world space. Vector*Vector
         *
         *	@param 	Vector 	direction Vector3
         *	@param 	Vector 	amount Vector3
         */
        void move(Vector3, Vector3);

        /**
         *	@brief	Moves the camera by the specified amount of world units in the specified direction in world space. Vector*scale
         *
         *	@param 	Vector 	direction Vector3
         *	@param 	float 	scale
         */
        void move(Vector3, float);

        /**
         *	@brief	Moves the camera by the specified amount of world units in the specified direction in world space. Only Vector
         *
         *	@param 	Vector
         */
        void move(Vector3);


        /**
         *	@brief	 Moves the camera by dx world units to the left or right; dy world units upwards or downwards; and dz world units forwards or backwards.
         *
         *	@param 	float 	dx
         *	@param 	float 	dy
         *	@param 	float 	dz
         */
        void move(float, float, float);

        /**
         * @brief Move camera along it's target vector.
         *
         * Move camera in a direction pointed by target vector of the camera. Z axis is always an unit vector. Movement vector magnitude will be equal scale.
         */
        void moveForward(float scale = 1.0f);

        /**
         *	@brief	Construct a projection matrix based on the horizontal field of view 'fovx' rather than the more traditional vertical field of view 'fovy'.
         *	@param 	float 	fovy
         *	@param 	float 	widh
         *    	@param 	float 	height
         *	@param 	float 	zNear
         *	@param 	float 	zFar
         */
        void perspective(float fov, float width, float height, float zNear, float zFar);

        /**
         *	@brief	Construct a projection matrix based on the horizontal field of view 'fovx' rather than the more traditional vertical field of view 'fovy'.
         *	@param 	float 	fovy
         *	@param 	float 	aspect ratio
         *	@param 	float 	zNear
         *	@param 	float 	zFar
         */
        void perspective(float, float, float, float);

        void resizeWindow(float windowH, float windowW);

        void trackWindowSize(AWindow& window);
        void setFarPlane(float farplane);
        void setNearPlane(float nearplane);
        void ortho(float l, float r, float b, float t, float n, float f);
        void perspectiveOrtho(float width, float height, float n, float f);
        /**
         *	@brief	 Rotates the camera based on its current behavior. Note that not all behaviors support rolling.
         *
         *	@param 	float   heading degrees
         *	@param 	float 	pitch degrees
         *	@param 	float 	roll degrees
         */
        void rotate(float, float, float);
        void rotate(Vector3 rotation);
        /**
         *	@brief	 This method applies a scaling factor to the rotation angles prior to using these rotation angles to rotate the camera. This method is usually called when the camera is being rotated using an input device (such as a mouse or a joystick).
         *
         *	@param 	float 	heading degrees
         *	@param 	float 	pitch degrees
         *	@param 	float 	roll degrees
         */
        void rotateSmoothly(float, float, float);

        /**
         *	@brief	 Undo any camera rolling by leveling the camera. When the camera is orbiting this method will cause the camera to become level with the orbit target.
         */
        void undoRoll();

        /**
         *	@brief Moves the camera using Newton's second law of motion. Unit mass is assumed here to somewhat simplify the calculations. The direction vector is in the range [-1,1].
         *
         *	@param 	Vector 	Direction vector
         *	@param 	float 	elapsed time
         */
        void updatePosition(Vector3, float);

        /**
         *	@brief	 Moves the camera closer to or further away from the orbit target. The zoom amounts are in world units.
         *	@param 	float 	zoom
         *	@param 	float 	min zoom
         *	@param 	float 	max zoom
         */
        void zoom(float, float, float);

        /**
         *	@brief	Update and return ViewProjection Matrix
         *
         *	@return	Matrix
         */
        virtual Matrix transforms();


        // Access

        /**
         *	@brief	Get projection matrix
         *
         *	@return	Matrix
         */
        Matrix get_projection_matrix() {
            return m_projMatrix;
        }

        /**
         *	@brief	Get projection matrix reference
         *
         *	@return	Matrix reference
         */
        const Matrix& get_projection_matrix() const {
            return m_projMatrix;
        }

        /**
         *	@brief	Get ortho matrix
         *
         *	@return	Matrix
         */
        Matrix get_ortho_matrix() {
            return m_orthoMatrix;
        }

        /**
         *	@brief	Get ortho matrix reference
         *
         *	@return	Matrix reference
         */
        const Matrix& get_ortho_matrix() const {
            return m_orthoMatrix;
        }

        /**
         *	@brief	Get view matrix
         *
         *	@return	Matrix
         */
        virtual Matrix get_view_matrix();

        /**
         *	@brief	Get view matrix reference
         *
         *	@return	Matrix reference
         */
        //virtual const Matrix& get_view_matrix() const;

        /**
         *	@brief	Get ViewProjection matrix
         *
         *	@return	Matrix
         */
        virtual Matrix get_view_projection_matrix();

        /**
         *	@brief	Get ViewProjection matrix reference
         *
         *	@return	Matrix reference
         */
        //virtual const Matrix& get_view_projection_matrix() const;

        /**
         *	@brief	Get frustrum
         *
         *	@return	Frustrum
         */
        const Frustum& getFrustum();

        /**
         *	@brief	Get frustrum
         *
         *	@return	Frustrum
         */
        Frustum getFrustum() const;

        /**
         *	@brief	Get orientation quaternion
         *
         *	@return	Quaternion
         */
        virtual Quaternion get_orientation();

        /**
         *	@brief	Get orientation quaternion reference
         *
         *	@return	Quaternion reference
         */
        virtual const Quaternion& get_orientation() const;



        // Setter methods.

        /**
         *	@brief	Set accereration vector
         *
         *	@param 	acceleration 	Vector3
         */
        void set_acceleration(const Vector3& acceleration) {
            m_acceleration = acceleration;
        }

        /**
         *	@brief	Set camera behavior mode
         *
         *	@param 	newBehavior  ( NONE, FP_MODE, FLIGHT_MODE, SPECTATOR_MODE, ORBITING_MODE)
         */
        void set_behavior(CAMERA_MODES newBehavior);

        void set_first_person_mode() {
            set_behavior(CAMERA_MODES::FP_MODE);
        }

        void set_flight_mode() {
            set_behavior(CAMERA_MODES::FLIGHT_MODE);
        }

        void set_spectator_mode() {
            set_behavior(CAMERA_MODES::SPECTATOR_MODE);
        }

        void set_orbiting_mode() {
            set_behavior(CAMERA_MODES::ORBITING_MODE);
        }

        /**
         *	@brief	Set camera velocity from Vector
         *
         *	@param 	currentVelocity Vector3
         */
        void set_current_velocity(const Vector3& currentVelocity) {
            m_currentVelocity = currentVelocity;
        }

        /**
         *	@brief	Set camera velocity from float xyz
         *
         *	@param 	x 	x velocity
         *	@param 	y 	y velocity
         *	@param 	z 	z velocity
         */
        void set_current_velocity(float x, float y, float z) {
            m_currentVelocity = Vector3(x, y, z);
        }

        /**
         *	@brief	Set max value of orbiting camera zoom
         *
         *	@param 	orbitMaxZoom 	float
         */
        void setOrbitMaxZoom(float orbitMaxZoom) {
            m_orbitMaxZoom = orbitMaxZoom;
        }

        /**
         *	@brief	Set min value of orbiting camera zoom
         *
         *	@param 	orbitMinZoom 	float
         */
        void setOrbitMinZoom(float orbitMinZoom) {
            m_orbitMinZoom = orbitMinZoom;
        }

        /**
         *	@brief	Set distance of orbiting camera
         *
         *	@param 	orbitOffsetDistance 	float distance
         */
        void setOrbitOffsetDistance(float orbitOffsetDistance);

        void setRotation(float headingDegrees, float pitchDegrees, float rollDegrees);
        void setRotation(Vector3 vector);

        /**
         *	@brief	Set orientation of camera from quaternion
         *
         *	@param 	newOrientation 	Quaternion
         */
        void setOrientation(Quaternion newOrientation);

        /**
         *	@brief	Set position from Vector3
         *
         *	@param 	newEye 	Vector3
         */
        void set_position(Vector3 newEye) {
            if (m_behavior != CAMERA_MODES::ORBITING_MODE) {
                m_eye = newEye;
            } else {
                m_target = newEye;
            }
            updateViewMatrix();
        }

        void set_position(float x, float y, float z) {
            set_position(Vector3(x, y, z));
        }

        virtual Vector3 GetPosition();

        /**
         *	@brief Determines the behavior of Y axis rotations when the camera is orbiting a target. When preferTargetYAxisOrbiting is true all Y axis rotations are about the orbit target's local Y axis. When preferTargetYAxisOrbiting is false then the camera's local Y axis is used instead.
         *
         *	@param 	preferTargetYAxisOrbiting 	<#preferTargetYAxisOrbiting description#>
         */
        inline void set_prefer_target_Y_axis_orbiting(bool preferTargetYAxisOrbiting);

        /**
         *	@brief	Set rotation speed
         *
         *	@param 	rotationSpeed 	float
         */
        void set_rotation_speed(float rotationSpeed) {
            m_rotationSpeed = rotationSpeed;
        }

        /**
         *	@brief	Set camera velocity from Vector3
         *
         *	@param 	velocity 	Vector3
         */
        void set_velocity(Vector3 velocity) {
            m_velocity = velocity;
        }

        /**
         *	@brief	Set camera velocity from float xyz
         *
         *	@param 	x 	x velocity
         *	@param 	y 	y velocity
         *	@param 	z 	z velocity
         */
        void set_velocity(float x, float y, float z) {
            m_velocity = Vector3(x, y, z);
        }

        //Transforms

        /**
         *	@brief Implements the rotation logic for the first person style and spectator style camera behaviors. Roll is ignored.
         *
         *	@param 	headingDegrees 	heading degrees
         *	@param 	pitchDegrees    pitch degrees
         */
        void rotateFirstPerson(float headingDegrees, float pitchDegrees);

        /**
         *	@brief  Implements the rotation logic for the flight style camera behavior.
         *
         *	@param 	headingDegrees 	heading degrees
         *	@param 	pitchDegrees 	pitch degrees
         *	@param 	rollDegrees 	roll degrees
         */
        void rotateFlight(float headingDegrees, float pitchDegrees, float rollDegrees);

        /**
         *	@brief	Implements the rotation logic for the orbit style camera behavior. Roll is ignored for target Y axis orbiting. Briefly here's how this orbit camera implementation works. Switching to the orbit camera behavior via the setBehavior() method will set the camera's orientation to match the orbit target's orientation. Calls to rotateOrbit() will rotate this orientation. To turn this into a third person style view the updateViewMatrix() method will gett the camera position back 'm_orbitOffsetDistance' world units along the camera's local z axis from the orbit target's world position.
         *
         *	@param 	headingDegrees 	headingd degrees
         *	@param 	pitchDegrees 	pitch degrees
         *	@param 	rollDegrees 	roll degrees
         */
        void rotateOrbit(float headingDegrees, float pitchDegrees, float rollDegrees);

        /**
         *	@brief	 Updates the camera's velocity based on the supplied movement direction and the elapsed time (since this method was last called). The movement direction is in the range [-1,1].
         *
         *	@param 	direction 	Direction Vector3
         *	@param 	elapsedTimeSec 	elapsed time
         */
        void updateVelocity(const Vector3& direction, float elapsedTimeSec);

        /**
         *	@brief	Reconstruct the view matrix
         */
        void updateViewMatrix();
        void setOrthoMatrix(Matrix orthoMatrix);
        void setWindowH(int windowH);
        int getWindowH() const;
        void setWindowW(int windowW);
        int getWindowW() const;

        bool isCameraMoved() const {
            return m_cameraMoved;
        }
        Vector3 getTarget() const;
        Vector3 getTargetYAxis() const;
        void setCurrentVelocity(Vector3 currentVelocity);
        Vector3 getCurrentVelocity() const;
        void setXAxis(Vector3 xAxis);
        Vector3 getXAxis() const;
        void setZAxis(Vector3 zAxis);
        Vector3 getZAxis() const;
        void setYAxis(Vector3 yAxis);
        Vector3 getYAxis() const;
        float getOrbitOffsetDistance() const;
        Vector3 getEye() const;
        void setCameraRotation(Vector3 cameraRotation);
        Vector3 getCameraRotation() const;
        float getZnear() const;
        void setProjMatrix(Matrix projMatrix);
        Matrix getProjMatrix() const;

        const aex::string& getName() const;
        void setName(const aex::string& name);
        
        float getFovy() const;
        
        Vector3 unProject(float x, float y, float z);
        Vector3 unProject(float x, float y);
        Vector3 unProject(Vector2 point);
        Vector3 unProject(Vector3 point);
        void setAex(Aex* aex);
        Aex* getAex() const;
        void unsetAex();
        float getAspectRatio() const;
        float getZfar() const;
        
        
    private:
        static CONSTEXPR float DEFAULT_ROTATION_SPEED = 0.3f;
        static CONSTEXPR float DEFAULT_FOVX = 90.0f;
        static CONSTEXPR float DEFAULT_ZNEAR = 0.1f;
        static CONSTEXPR float DEFAULT_ZFAR = 1000.0f;
        static CONSTEXPR float DEFAULT_QUATER = 0.25f;
        static CONSTEXPR float DEFAULT_HALF = 0.5f;
        static CONSTEXPR float DEFAULT_UNIT = 1.0f;
        static CONSTEXPR float DEFAULT_ORBIT_MIN_ZOOM = DEFAULT_ZNEAR + DEFAULT_UNIT;
        static CONSTEXPR float DEFAULT_ORBIT_MAX_ZOOM = DEFAULT_ZFAR * DEFAULT_HALF;
        static CONSTEXPR float DEFAULT_ORBIT_OFFSET_DISTANCE = DEFAULT_ORBIT_MIN_ZOOM + (DEFAULT_ORBIT_MAX_ZOOM - DEFAULT_ORBIT_MIN_ZOOM) * DEFAULT_QUATER;
        ;
        static const Vector3 WORLD_XAXIS;
        static const Vector3 WORLD_YAXIS;
        static const Vector3 WORLD_ZAXIS;

        CAMERA_MODES m_behavior;
        bool m_updateFrustum;
        bool m_preferTargetYAxisOrbiting;
        float m_accumPitchDegrees;
        float m_savedAccumPitchDegrees;
        float m_rotationSpeed;
        float m_fovy;
        float m_aspectRatio;
        float m_znear;
        float m_zfar;
        float m_orbitMinZoom;
        float m_orbitMaxZoom;
        float m_orbitOffsetDistance;
        float m_firstPersonYOffset;
        Vector3 m_eye;
        Vector3 m_returnEye;
        Vector3 m_savedEye;
        Vector3 m_target;
        Vector3 m_targetYAxis;
        Vector3 m_xAxis;
        Vector3 m_yAxis;
        Vector3 m_zAxis;
        Vector3 m_viewDir;
        Vector3 m_acceleration;
        Vector3 m_currentVelocity;
        Vector3 m_velocity;
        Vector3 m_cameraRotation;
        Quaternion m_orientation;
        Quaternion m_savedOrientation;
        Matrix m_viewMatrix;
        Matrix m_projMatrix;
        Matrix m_orthoMatrix;
        Matrix m_viewProjMatrix;
        Frustum m_frustum;
        bool m_cameraMoved;
        int m_windowW;
        int m_windowH;
        boost::signals2::scoped_connection m_resizeCallback;
        void windowResizedCallback(std::size_t w, std::size_t h);
        aex::string m_name;
        
        Aex* m_aex = nullptr;
        
    protected:
        void setCleanFlag();
    };

    void
    QuaternionCamera::set_prefer_target_Y_axis_orbiting(bool preferTargetYAxisOrbiting) {
        // Determines the behavior of Y axis rotations when the camera is
        // orbiting a target. When preferTargetYAxisOrbiting is true all
        // Y axis rotations are about the orbit target's local Y axis.
        // When preferTargetYAxisOrbiting is false then the camera's
        // local Y axis is used instead.

        m_preferTargetYAxisOrbiting = preferTargetYAxisOrbiting;

        if (m_preferTargetYAxisOrbiting) {
            undoRoll();
        }
    }

    typedef shared_ptr<QuaternionCamera> QuaternionCamera_ptr;
}

#endif /* defined(__aexolGL__QuaternionCamera__) */
