#include "framework/graphics/include/ApiInternal.hpp"
#include <aex/QuaternionCamera.hpp>
#include <aex/Math.hpp>
#include <aex/Quaternion.hpp>
#include <aex/Logger.hpp>
#include <aex/AWindow.hpp>
#include <aex/Aex.hpp>
namespace aex {
    const Vector QuaternionCamera::WORLD_XAXIS = Vector3(1.0f, 0.0f, 0.0f);
    const Vector QuaternionCamera::WORLD_YAXIS = Vector3(0.0f, 1.0f, 0.0f);
    const Vector QuaternionCamera::WORLD_ZAXIS = Vector3(0.0f, 0.0f, 1.0f);

    QuaternionCamera::QuaternionCamera() :
    m_behavior(CAMERA_MODES::FP_MODE),
    m_updateFrustum(true),
    m_preferTargetYAxisOrbiting(true),
    m_accumPitchDegrees(0.0f),
    m_savedAccumPitchDegrees(0.0f),
    m_rotationSpeed(QuaternionCamera::DEFAULT_ROTATION_SPEED),
    m_fovy(45.0f),
    m_aspectRatio(0.0f),
    m_znear(QuaternionCamera::DEFAULT_ZNEAR),
    m_zfar(QuaternionCamera::DEFAULT_ZFAR),
    m_orbitMinZoom(QuaternionCamera::DEFAULT_ORBIT_MIN_ZOOM),
    m_orbitMaxZoom(QuaternionCamera::DEFAULT_ORBIT_MAX_ZOOM),
    m_orbitOffsetDistance(QuaternionCamera::DEFAULT_ORBIT_OFFSET_DISTANCE),
    m_eye(0.0f, 0.0f, 0.0f),
    m_returnEye(0.0f, 0.0f, 0.0f),
    m_savedEye(0.0f, 0.0f, 0.0f),
    m_target(0.0f, 0.0f, 0.0f),
    m_targetYAxis(0.0f, 1.0f, 0.0f),
    m_xAxis(1.0f, 0.0f, 0.0f),
    m_yAxis(0.0f, 1.0f, 0.0f),
    m_zAxis(0.0f, 0.0f, 1.0f),
    m_viewDir(0.0f, 0.0f, -1.0f),
    m_acceleration(0.0f, 0.0f, 0.0f),
    m_currentVelocity(0.0f, 0.0f, 0.0f),
    m_velocity(0.0f, 0.0f, 0.0f),
    m_cameraRotation(0.0f, 0.0f, 0.0f),
    m_orientation(aex::math::getIdentityQuaternion()),
    m_savedOrientation(aex::math::getIdentityQuaternion()),
    m_viewMatrix(),
    m_projMatrix(),
    m_orthoMatrix(),
    m_viewProjMatrix(),
    m_frustum(),
    m_cameraMoved(true),
    m_resizeCallback(),
    m_name()
    {
        ortho(1.0, 0.0, 0.0, 1.0, 0.0, 50.0);
    }
    
    QuaternionCamera::~QuaternionCamera()
    {
        
    }

    void
    QuaternionCamera::windowResizedCallback(std::size_t w, std::size_t h)
    {
        resizeWindow((float) w, (float) h);
    }

    Vector QuaternionCamera::getTarget() const
    {
        return m_target;
    }

    Vector QuaternionCamera::getTargetYAxis() const
    {
        return m_targetYAxis;
    }

    void QuaternionCamera::setCurrentVelocity(Vector3 currentVelocity)
    {
        m_currentVelocity = currentVelocity;
    }

    Vector QuaternionCamera::getCurrentVelocity() const
    {
        return m_currentVelocity;
    }

    void QuaternionCamera::setXAxis(Vector3 xAxis)
    {
        m_xAxis = xAxis;
    }

    Vector QuaternionCamera::getXAxis() const
    {
        return m_xAxis;
    }

    void QuaternionCamera::setZAxis(Vector3 zAxis)
    {
        m_zAxis = zAxis;
        m_zAxis.normalizeThis();
    }

    Vector QuaternionCamera::getZAxis() const
    {
        return m_zAxis;
    }

    void QuaternionCamera::setYAxis(Vector3 yAxis)
    {
        m_yAxis = yAxis;
    }

    Vector QuaternionCamera::getYAxis() const
    {
        return m_yAxis;
    }

    float QuaternionCamera::getOrbitOffsetDistance() const
    {
        return m_orbitOffsetDistance;
    }

    Vector QuaternionCamera::getEye() const
    {
        return m_eye;
    }

    void QuaternionCamera::setCameraRotation(Vector3 cameraRotation)
    {
        m_cameraRotation = cameraRotation;
        Quaternion rot = aex::math::getQuaternionFromEulerDeg(m_cameraRotation.x, m_cameraRotation.y, m_cameraRotation.z);
        setOrientation(rot);
    }

    Vector QuaternionCamera::getCameraRotation() const
    {
        return m_cameraRotation;
    }

    float QuaternionCamera::getZnear() const
    {
        return m_znear;
    }

    void QuaternionCamera::setProjMatrix(Matrix projMatrix)
    {
        m_projMatrix = projMatrix;
    }

    Matrix QuaternionCamera::getProjMatrix() const
    {
        return m_projMatrix;
    }
    
    void
    QuaternionCamera::reset()
    {
        

        
        m_eye = Vector3();
        m_returnEye = Vector3();
        m_savedEye = Vector3();
        m_target = Vector3(1.0,0.0,0.0);
        m_targetYAxis = Vector3(0.0,1.0,0.0);
        m_xAxis = Vector3(1.0f, 0.0f, 0.0f);
        m_yAxis = Vector3(0.0f, 1.0f, 0.0f);
        m_zAxis = Vector3(0.0f, 0.0f, 1.0f);
        m_viewDir = Vector3(0.0f, 0.0f, -1.0f);
        m_acceleration = Vector3();
        m_currentVelocity = Vector3();
        m_velocity = Vector3();
        m_cameraRotation = Vector3();
        m_orientation = Quaternion();
        m_savedOrientation = Quaternion();
        
        updateViewMatrix();
    }

    void
    QuaternionCamera::lookAt(Vector3 target)
    {
        lookAt(m_eye, target, m_targetYAxis);
    }

    void QuaternionCamera::lookFrom(Vector3 eye)
    {
        lookAt(eye, m_target, m_targetYAxis);
    }

    void
    QuaternionCamera::lookAt(Vector3 eyePos, Vector3 target)
    {
        lookAt(eyePos, target, m_targetYAxis);
    }

    void
    QuaternionCamera::lookAt(Vector3 eyePos, Vector3 target, Vector3 up)
    {
        // Cache values.
        m_targetYAxis = up;
        m_eye = eyePos;
        m_target = target;

        Matrix temp = aex::math::lookAt(m_eye, m_target, m_targetYAxis);
        m_accumPitchDegrees = aex::math::Math::RAD2DEG(asin(temp[9]));
        m_orientation = aex::math::getQuaternionFromMatrix(temp);

        updateViewMatrix();
    }

    void
    QuaternionCamera::moveForward(float x)
    {
        Vector3 movementDir = m_viewDir * x;
        move(movementDir);
    }

    void
    QuaternionCamera::move(float dx, float dy, float dz)
    {
        // Moves the camera by dx world units to the left or right; dy
        // world units upwards or downwards; and dz world units forwards
        // or backwards.
        Vector3 forwards;

        if (m_behavior == CAMERA_MODES::FP_MODE)
        {
            // Calculate the forwards direction. Can't just use the camera's local
            // z axis as doing so will cause the camera to move more slowly as the
            // camera's view approaches 90 degrees straight up and down.

            forwards = WORLD_YAXIS * m_xAxis;
            forwards = forwards.unit();
        } else
        {
            forwards = m_viewDir;
        }


        if (m_behavior == CAMERA_MODES::ORBITING_MODE)
        {
            Vector3 target = m_target;
            // Orbiting camera is always positioned relative to the
            // target position. See updateViewMatrix().
            target = target + (m_xAxis * dx);
            target = target + (WORLD_YAXIS * dy);
            target = target + (forwards * dz);
            m_target = target;
        } else
        {
            Vector3 eye = m_eye;
            eye = eye + (m_xAxis * dx);
            eye = eye + (WORLD_YAXIS * dy);
            eye = eye + (forwards * dz);
            m_eye = eye;
        }

        updateViewMatrix();
    }

    void
    QuaternionCamera::move(Vector3 direction, Vector3 amount)
    {
        move(direction * amount);
    }

    void
    QuaternionCamera::move(Vector3 direction, float scale)
    {
        move(direction, Vector3(scale, scale, scale));
    }

    void
    QuaternionCamera::move(Vector3 direction)
    {
        // Moves the camera by the specified amount of world units in the specified
        // direction in world space. Only Vector

        if (m_behavior != CAMERA_MODES::ORBITING_MODE)
        {
            m_eye += direction;
        } else
        {
            m_target += direction;
        }

        updateViewMatrix();
    }

    void
    QuaternionCamera::perspective(float fov, float width, float height, float zNear, float zFar)
    {
        // Construct a projection matrix based on the horizontal field of view
        // 'fovx' rather than the more traditional vertical field of view 'fovy'.
        m_windowW = width;
        m_windowH = height;
        perspective(fov, width / height, zNear, zFar);
    }

    void QuaternionCamera::setDirtyFlag()
    {
        m_cameraMoved = true;
    }
    
    void QuaternionCamera::setCleanFlag()
    {
        m_cameraMoved = false;
    }

    void
    QuaternionCamera::perspective(float fovy, float aspect, float znear, float zfar)
    {
        m_fovy = fovy;
        m_aspectRatio = aspect;
        m_znear = znear;
        m_zfar = zfar;
        m_projMatrix = aex::math::perspective(fovy, aspect, znear, zfar);
        m_viewProjMatrix = m_projMatrix * m_viewMatrix;
        m_cameraMoved = true;
        m_updateFrustum = true;
    }

    void QuaternionCamera::setFarPlane(float farplane)
    {
        perspective(m_fovy, m_aspectRatio, m_znear, farplane);
    }

    void QuaternionCamera::setNearPlane(float nearplane)
    {
        perspective(m_fovy, m_aspectRatio, nearplane, m_zfar);
    }

    void QuaternionCamera::resizeWindow(float windowW, float windowH)
    {
        perspective(m_fovy, windowW, windowH, m_znear, m_zfar);
    }

    void
    QuaternionCamera::perspectiveOrtho(float width, float height, float n, float f)
    {
        m_orthoMatrix = aex::math::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, n, f);
        m_fovy = 0.0f;
        m_aspectRatio = width / height;
        m_znear = n;
        m_zfar = f;
        m_projMatrix = m_orthoMatrix;
        m_viewProjMatrix = m_projMatrix * m_viewMatrix;
        m_cameraMoved = true;
        m_updateFrustum = true;
    }

    void
    QuaternionCamera::ortho(float l, float r, float b, float t, float n, float f)
    {
        m_orthoMatrix = aex::math::ortho(l, r, b, t, n, f);
    }

    void
    QuaternionCamera::rotate(float headingDegrees, float pitchDegrees, float rollDegrees)
    {
        // Rotates the camera based on its current behavior.
        // Note that not all behaviors support rolling.

        pitchDegrees = -pitchDegrees;
        headingDegrees = -headingDegrees;
        rollDegrees = -rollDegrees;

        m_cameraRotation.x += pitchDegrees;
        m_cameraRotation.y += headingDegrees;
        m_cameraRotation.z += rollDegrees;

        switch (m_behavior) {
            default:
                break;

            case CAMERA_MODES::FP_MODE:
            case CAMERA_MODES::SPECTATOR_MODE:
            {
                rotateFirstPerson(headingDegrees, pitchDegrees);
            }
                break;

            case CAMERA_MODES::FLIGHT_MODE:
            {
                rotateFlight(headingDegrees, pitchDegrees, rollDegrees);
            }
                break;

            case CAMERA_MODES::ORBITING_MODE:
            {
                rotateOrbit(headingDegrees, pitchDegrees, rollDegrees);
            }
                break;
        }

        updateViewMatrix();
    }

    void QuaternionCamera::rotate(Vector3 rotation)
    {
        rotate(rotation.x, rotation.y, rotation.z);
    }

    void
    QuaternionCamera::rotateSmoothly(float headingDegrees, float pitchDegrees, float rollDegrees)
    {
        // This method applies a scaling factor to the rotation angles prior to
        // using these rotation angles to rotate the camera. This method is usually
        // called when the camera is being rotated using an input device (such as a
        // mouse or a joystick).

        headingDegrees *= m_rotationSpeed;
        pitchDegrees *= m_rotationSpeed;
        rollDegrees *= m_rotationSpeed;

        rotate(headingDegrees, pitchDegrees, rollDegrees);
    }

    void
    QuaternionCamera::undoRoll()
    {
        // Undo any camera rolling by leveling the camera. When the camera is
        // orbiting this method will cause the camera to become level with the
        // orbit target.

        if (m_behavior == CAMERA_MODES::ORBITING_MODE)
        {
            lookAt(m_eye, m_target, m_targetYAxis);
        } else
        {
            lookAt(m_eye, m_eye + m_viewDir, WORLD_YAXIS);
        }
    }

    void
    QuaternionCamera::updatePosition(Vector3 direction, float elapsedTimeSec)
    {
        // Moves the camera using Newton's second law of motion. Unit mass is
        // assumed here to somewhat simplify the calculations. The direction vector
        // is in the range [-1,1].

        if (m_currentVelocity.dot(m_currentVelocity) != 0.0f)
        {
            // Only move the camera if the velocity vector is not of zero length.
            // Doing this guards against the camera slowly creeping around due to
            // floating point rounding errors.

            Vector3 displacement = (m_currentVelocity * elapsedTimeSec) + (m_acceleration * 0.5f * elapsedTimeSec * elapsedTimeSec);

            // Floating point rounding errors will slowly accumulate and cause the
            // camera to move along each axis. To prevent any unintended movement
            // the displacement vector is clamped to zero for each direction that
            // the camera isn't moving in. Note that the updateVelocity() method
            // will slowly decelerate the camera's velocity back to a stationary
            // state when the camera is no longer moving along that direction. To
            // account for this the camera's current velocity is also checked.

            if (direction.x == 0.0f && aex::math::Math::closeEnough(m_currentVelocity.x, 0.0f))
            {
                displacement.x = 0.0f;
            }

            if (direction.y == 0.0f && aex::math::Math::closeEnough(m_currentVelocity.y, 0.0f))
            {
                displacement.y = 0.0f;
            }

            if (direction.z == 0.0f && aex::math::Math::closeEnough(m_currentVelocity.z, 0.0f))
            {
                displacement.z = 0.0f;
            }

            move(displacement.x, displacement.y, displacement.z);
        }
        //if(gl->isVPupdated())

        // Continuously update the camera's velocity vector even if the camera
        // hasn't moved during this call. When the camera is no longer being moved
        // the camera is decelerating back to its stationary state.

        updateVelocity(direction, elapsedTimeSec);
        m_updateFrustum = true;
    }

    void
    QuaternionCamera::zoom(float zoom, float minZoom, float maxZoom)
    {
        if (m_behavior == CAMERA_MODES::ORBITING_MODE)
        {
            // Moves the camera closer to or further away from the orbit
            // target. The zoom amounts are in world units.

            m_orbitMaxZoom = maxZoom;
            m_orbitMinZoom = minZoom;

            Vector3 offset = m_eye * m_target;

            m_orbitOffsetDistance = offset.length();
            offset = offset.unit();
            m_orbitOffsetDistance += zoom;
            m_orbitOffsetDistance = aex::math::Math::min(aex::math::Math::max(m_orbitOffsetDistance, minZoom), maxZoom);

            offset = offset * m_orbitOffsetDistance;
            m_eye = offset + m_target;

            updateViewMatrix();
        } else
        {
            // For the other behaviors zoom is interpreted as changing the
            // horizontal field of view. The zoom amounts refer to the horizontal
            // field of view in degrees.

            zoom = aex::math::Math::min(aex::math::Math::max(zoom, minZoom), maxZoom);
            perspective(zoom, m_aspectRatio, m_znear, m_zfar);
        }
    }

    Matrix
    QuaternionCamera::transforms()
    {
        if(!m_aex){
           m_cameraMoved = false;
        }
        
        return m_viewProjMatrix;
    }

    void
    QuaternionCamera::set_behavior(CAMERA_MODES newBehavior)
    {
        CAMERA_MODES prevBehavior = m_behavior;

        if (prevBehavior == newBehavior)
        {
            return;
        }

        m_behavior = newBehavior;

        switch (newBehavior) {

            default:
                break;

            case CAMERA_MODES::FP_MODE:
            {
                switch (prevBehavior) {
                    default:
                        break;

                    case CAMERA_MODES::NONE:
                        break;

                    case CAMERA_MODES::FLIGHT_MODE:
                    {
                        m_eye.y = m_firstPersonYOffset;
                        updateViewMatrix();
                    }
                        break;

                    case CAMERA_MODES::SPECTATOR_MODE:
                    {
                        m_eye.y = m_firstPersonYOffset;
                        updateViewMatrix();
                    }
                        break;

                    case CAMERA_MODES::ORBITING_MODE:
                    {
                        m_eye.x = m_savedEye.x;
                        m_eye.z = m_savedEye.z;
                        m_eye.y = m_firstPersonYOffset;
                        m_orientation = m_savedOrientation;
                        m_accumPitchDegrees = m_savedAccumPitchDegrees;
                        updateViewMatrix();
                    }
                        break;
                }

                undoRoll();
            }
                break;

            case CAMERA_MODES::SPECTATOR_MODE:
            {
                switch (prevBehavior) {
                    default:
                        break;

                    case CAMERA_MODES::FLIGHT_MODE:
                    {
                        updateViewMatrix();
                    }
                        break;

                    case CAMERA_MODES::ORBITING_MODE:
                    {
                        m_eye = m_savedEye;
                        m_orientation = m_savedOrientation;
                        m_accumPitchDegrees = m_savedAccumPitchDegrees;
                        updateViewMatrix();
                    }
                        break;
                }

                undoRoll();
            }
                break;

            case CAMERA_MODES::FLIGHT_MODE:
            {
                if (prevBehavior == CAMERA_MODES::ORBITING_MODE)
                {
                    m_eye = m_savedEye;
                    m_orientation = m_savedOrientation;
                    m_accumPitchDegrees = m_savedAccumPitchDegrees;
                    updateViewMatrix();
                } else
                {
                    m_savedEye = m_eye;
                    updateViewMatrix();
                }
            }
                break;

            case CAMERA_MODES::ORBITING_MODE:
            {
                if (prevBehavior == CAMERA_MODES::FP_MODE)
                {
                    m_firstPersonYOffset = m_eye.y;
                }

                m_savedEye = m_eye;
                m_savedOrientation = m_orientation;
                m_savedAccumPitchDegrees = m_accumPitchDegrees;

                m_targetYAxis = m_yAxis;

                Vector3 newEye = m_eye + (m_zAxis * m_orbitOffsetDistance);
                Vector3 newTarget = m_eye;

                lookAt(newEye, newTarget, m_targetYAxis);
            }
                break;
        }
    }

    void
    QuaternionCamera::setOrbitOffsetDistance(float orbitOffsetDistance)
    {
        m_orbitOffsetDistance = orbitOffsetDistance;
        updateViewMatrix();
    }

    void
    QuaternionCamera::setRotation(float headingDegrees, float pitchDegrees, float rollDegrees)
    {
        Quaternion newOrientation = aex::math::getQuaternionFromEulerDeg(pitchDegrees, headingDegrees, rollDegrees);
        std::array<float, 16>& m = newOrientation.getMatrix().getM();

        m_accumPitchDegrees = aex::math::Math::RAD2DEG(asin(m[9]));
        m_orientation = newOrientation;


        updateViewMatrix();
    }

    void QuaternionCamera::setRotation(Vector3 vector)
    {
        setRotation(vector.x, vector.y, vector.z);
    }

    void
    QuaternionCamera::setOrientation(Quaternion newOrientation)
    {
        std::array<float, 16>& m = newOrientation.getMatrix().getM();
        // Store the pitch for this new orientation.
        // First person and spectator behaviors limit pitching to
        // 90 degrees straight up and down.

        m_accumPitchDegrees = aex::math::Math::RAD2DEG(asin(m[9]));

        // First person and spectator behaviors don't allow rolling.
        // Negate any rolling that might be encoded in the new orientation.

        m_orientation = newOrientation;

        /*if(m_behavior == CAMERA_MODES::FP_MODE || m_behavior == CAMERA_MODES::SPECTATOR_MODE)
        {
            lookAt(m_eye, m_eye + m_viewDir, WORLD_YAXIS);
        }*/

        updateViewMatrix();
    }

    void
    QuaternionCamera::rotateFirstPerson(float headingDegrees, float pitchDegrees)
    {
        // Implements the rotation logic for the first person style and
        // spectator style camera behaviors. Roll is ignored.

        m_accumPitchDegrees += pitchDegrees;

        if (m_accumPitchDegrees > 90.0f)
        {
            pitchDegrees = 90.0f - (m_accumPitchDegrees - pitchDegrees);
            m_accumPitchDegrees = 90.0f;
        }

        if (m_accumPitchDegrees < -90.0f)
        {
            pitchDegrees = -90.0f - (m_accumPitchDegrees - pitchDegrees);
            m_accumPitchDegrees = -90.0f;
        }

        Quaternion rot;

        // Rotate camera about the world y axis.
        // Note the order the quaternions are multiplied. That is important!
        if (headingDegrees != 0.0f)
        {
            rot = aex::math::getQuaternionFromAxisAngleDeg(WORLD_YAXIS, headingDegrees);
            m_orientation = m_orientation * rot;
        }

        // Rotate camera about the local x axis.
        // Note the order the quaternions are multiplied. That is important!
        if (pitchDegrees != 0.0f)
        {
            rot = aex::math::getQuaternionFromAxisAngleDeg(WORLD_XAXIS, pitchDegrees);
            m_orientation = rot * m_orientation;
        }
    }

    void
    QuaternionCamera::rotateFlight(float headingDegrees, float pitchDegrees, float rollDegrees)
    {
        // Implements the rotation logic for the flight style camera behavior.

        m_accumPitchDegrees += pitchDegrees;

        if (m_accumPitchDegrees > 360.0f)
        {
            m_accumPitchDegrees -= 360.0f;
        }

        if (m_accumPitchDegrees < -360.0f)
        {
            m_accumPitchDegrees += 360.0f;
        }

        Quaternion rot = aex::math::getQuaternionFromEulerDeg(pitchDegrees, headingDegrees, rollDegrees);

        m_orientation = rot * m_orientation;
    }

    void
    QuaternionCamera::rotateOrbit(float headingDegrees, float pitchDegrees, float rollDegrees)
    {
        // Implements the rotation logic for the orbit style camera behavior.
        // Roll is ignored for target Y axis orbiting.
        //
        // Briefly here's how this orbit camera implementation works. Switching to
        // the orbit camera behavior via the setBehavior() method will set the
        // camera's orientation to match the orbit target's orientation. Calls to
        // rotateOrbit() will rotate this orientation. To turn this into a third
        // person style view the updateViewMatrix() method will move the camera
        // position back 'm_orbitOffsetDistance' world units along the camera's
        // local z axis from the orbit target's world position.

        Quaternion rot;

        if (m_preferTargetYAxisOrbiting)
        {
            if (headingDegrees != 0.0f)
            {
                rot = aex::math::getQuaternionFromAxisAngleDeg(WORLD_YAXIS, headingDegrees);
                m_orientation = m_orientation * rot;
            }

            if (pitchDegrees != 0.0f)
            {
                rot = aex::math::getQuaternionFromAxisAngleDeg(WORLD_XAXIS, pitchDegrees);
                m_orientation = rot * m_orientation;
            }
        } else
        {
            rot = aex::math::getQuaternionFromEulerDeg(pitchDegrees, headingDegrees, rollDegrees);
            m_orientation = rot * m_orientation;
        }
    }

    void
    QuaternionCamera::updateVelocity(const Vector3& direction, float elapsedTimeSec)
    {
        // Updates the camera's velocity based on the supplied movement direction
        // and the elapsed time (since this method was last called). The movement
        // direction is in the range [-1,1].

        if (direction.x != 0.0f)
        {
            // Camera is moving along the x axis.
            // Linearly accelerate up to the camera's max speed.

            m_currentVelocity.x += direction.x * m_acceleration.x * elapsedTimeSec;

            if (m_currentVelocity.x > m_velocity.x)
            {
                m_currentVelocity.x = m_velocity.x;
            } else if (m_currentVelocity.x < -m_velocity.x)
            {
                m_currentVelocity.x = -m_velocity.x;
            }
        } else
        {
            // Camera is no longer moving along the x axis.
            // Linearly decelerate back to stationary state.

            if (m_currentVelocity.x > 0.0f)
            {
                if ((m_currentVelocity.x -= m_acceleration.x * elapsedTimeSec) < 0.0f)
                {
                    m_currentVelocity.x = 0.0f;
                }
            } else
            {
                if ((m_currentVelocity.x += m_acceleration.x * elapsedTimeSec) > 0.0f)
                {
                    m_currentVelocity.x = 0.0f;
                }
            }
        }

        if (direction.y != 0.0f)
        {
            // Camera is moving along the y axis.
            // Linearly accelerate up to the camera's max speed.

            m_currentVelocity.y += direction.y * m_acceleration.y * elapsedTimeSec;

            if (m_currentVelocity.y > m_velocity.y)
            {
                m_currentVelocity.y = m_velocity.y;
            } else if (m_currentVelocity.y < -m_velocity.y)
            {
                m_currentVelocity.y = -m_velocity.y;
            }
        } else
        {
            // Camera is no longer moving along the y axis.
            // Linearly decelerate back to stationary state.

            if (m_currentVelocity.y > 0.0f)
            {
                if ((m_currentVelocity.y -= m_acceleration.y * elapsedTimeSec) < 0.0f)
                {
                    m_currentVelocity.y = 0.0f;
                }
            } else
            {
                if ((m_currentVelocity.y += m_acceleration.y * elapsedTimeSec) > 0.0f)
                {
                    m_currentVelocity.y = 0.0f;
                }
            }
        }

        if (direction.z != 0.0f)
        {
            // Camera is moving along the z axis.
            // Linearly accelerate up to the camera's max speed.

            m_currentVelocity.z += direction.z * m_acceleration.z * elapsedTimeSec;

            if (m_currentVelocity.z > m_velocity.z)
            {
                m_currentVelocity.z = m_velocity.z;
            } else if (m_currentVelocity.z < -m_velocity.z)
            {
                m_currentVelocity.z = -m_velocity.z;
            }
        } else
        {
            // Camera no longer moving along the z axis.
            // Linearly decelerate back to stationary state.

            if (m_currentVelocity.z > 0.0f)
            {
                if ((m_currentVelocity.z -= m_acceleration.z * elapsedTimeSec) < 0.0f)
                {
                    m_currentVelocity.z = 0.0f;
                }
            } else
            {
                if ((m_currentVelocity.z += m_acceleration.z * elapsedTimeSec) > 0.0f)
                {
                    m_currentVelocity.z = 0.0f;
                }
            }
        }
    }

    void
    QuaternionCamera::updateViewMatrix()
    {
        // Reconstruct the view matrix.
        Vector3 pos = m_eye;
        if(m_aex)
        {
            Matrix mat =  m_aex->getFinalModelMatrix();
            pos = mat.transformPoint( pos / aex::math::extract_scale_from_matrix(mat), 1.0) ;
            m_viewMatrix = ( m_orientation *  aex::math::extract_rotation_from_matrix(mat.inverse())).getMatrix(); 
        }
        else
        {
            m_viewMatrix = m_orientation.getMatrix();
        }
        
        m_xAxis = Vector3(m_viewMatrix[0], m_viewMatrix[1], m_viewMatrix[2]);
        m_yAxis = Vector3(m_viewMatrix[4], m_viewMatrix[5], m_viewMatrix[6]);
        m_zAxis = Vector3(m_viewMatrix[8], m_viewMatrix[9], m_viewMatrix[10]);
        
        if (m_behavior == CAMERA_MODES::ORBITING_MODE)
        {
            // Calculate the new camera position based on the current
            // orientation. The camera must always maintain the same
            // distance from the target. Use the current offset vector
            // to determine the correct distance from the target.

            pos = m_target + (m_zAxis * m_orbitOffsetDistance);
        }
        
        m_returnEye=pos;
        //m_eye=pos;
        
        m_viewDir = -m_zAxis;
        Vector3 neg_eye = -pos;

        m_viewMatrix[3] = m_xAxis % neg_eye;
        m_viewMatrix[7] = m_yAxis % neg_eye;
        m_viewMatrix[11] = m_zAxis % neg_eye;

        m_viewProjMatrix = m_projMatrix * m_viewMatrix;
       
        m_updateFrustum = true;
        m_cameraMoved = true;
    }

    void QuaternionCamera::setOrthoMatrix(Matrix orthoMatrix)
    {
        m_orthoMatrix = orthoMatrix;
    }

    void QuaternionCamera::setWindowH(int windowH)
    {
        m_windowH = windowH;
    }

    int QuaternionCamera::getWindowH() const
    {
        return m_windowH;
    }

    void QuaternionCamera::setWindowW(int windowW)
    {
        m_windowW = windowW;
    }

    int QuaternionCamera::getWindowW() const
    {
        return m_windowW;
    }

    void QuaternionCamera::trackWindowSize(AWindow& window)
    {
        m_resizeCallback = window.addWindowSizeChange(boost::bind(&QuaternionCamera::windowResizedCallback, this, _1, _2));
    }

    Frustum
    QuaternionCamera::getFrustum() const
    {

        if (m_updateFrustum)
        {
            Frustum frustum;
            frustum.setFrustum(m_viewProjMatrix);
            return frustum;
        }
        return m_frustum;
    }

    const Frustum&
    QuaternionCamera::getFrustum()
    {
        if (m_updateFrustum)
        {
            m_frustum.setFrustum(m_viewProjMatrix);
            m_updateFrustum = false;
        }
        return m_frustum;
    }

    const aex::string&
    QuaternionCamera::getName() const
    {
        return m_name;
    }

    void
    QuaternionCamera::setName(const aex::string& name)
    {
        m_name = name;
    }

    aex::math::Vector3 QuaternionCamera::unProject(float x, float y, float z)
    {
        Vector3 point;
        point.x = (x - 0.5f) * 2.0f;
        point.y = (y - 0.5f) * 2.0f;
        point.z = (z - 0.5f) * 2.0f;
        aex::math::Matrix vnt = m_viewMatrix;
        vnt[3] = vnt[7] = vnt[11] = 0.0f;
        return (m_projMatrix * vnt).inverse().transformPoint(point, 1.0f);
    }

    aex::math::Vector3 QuaternionCamera::unProject(float x, float y)
    {
        return unProject(x, y, 0.0f);
    }

    aex::math::Vector3 QuaternionCamera::unProject(Vector2 point)
    {
        return unProject(point.x, point.x, 0.0f);
    }

    aex::math::Vector3 QuaternionCamera::unProject(Vector3 point)
    {
        return unProject(point.x, point.y, point.z);
    }

    void QuaternionCamera::setAex(Aex* aex)
    {
        m_aex = aex;
    }
    
    void QuaternionCamera::unsetAex()
    {
        m_aex = nullptr;
    }

    float QuaternionCamera::getAspectRatio() const
    {
        return m_aspectRatio;
    }

    float QuaternionCamera::getZfar() const
    {
        return m_zfar;
    }

    Aex* QuaternionCamera::getAex() const
    {
        return m_aex;
    }

    float QuaternionCamera::getFovy() const
    {
        return m_fovy;
    }

    Vector QuaternionCamera::GetPosition()
    {
        return m_returnEye;
        
    }

    Matrix QuaternionCamera::get_view_matrix()
    {
        return m_viewMatrix;
    }

    /*const Matrix& QuaternionCamera::get_view_matrix() const {
                return m_viewMatrix;
            }*/
    Matrix QuaternionCamera::get_view_projection_matrix()
    {
        return m_viewProjMatrix;
    }

    /*const Matrix& QuaternionCamera::get_view_projection_matrix() const {
                return m_viewProjMatrix;
            }*/

    aex::math::Quaternion QuaternionCamera::get_orientation()
    {
        return m_orientation;
    }

    const aex::math::Quaternion& QuaternionCamera::get_orientation() const
    {
        return m_orientation;
    }
    
}
