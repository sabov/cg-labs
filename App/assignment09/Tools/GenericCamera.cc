/***********************************************************************
 * Copyright 2011-2013 Computer Graphics Group RWTH Aachen University. *
 * All rights reserved.                                                *
 * Distributed under the terms of the MIT License (see LICENSE.TXT).   *
 **********************************************************************/

#include <cassert>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "GenericCamera.hh"

using namespace std;

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif


float  calcDegToRad(float  d) {return (d * (M_PI / 180.0));}
double calcDegToRad(double d) {return (d * (M_PI / 180.0));}
float  calcRadToDeg(float  r) {return (r * (180.0  / M_PI));}
double calcRadToDeg(double r) {return (r * (180.0  / M_PI));}

GenericCamera::GenericCamera() :
    mPosition(),
    mProjectionMode(PERSPECTIVE_PROJECTION),
    mStereoMode(MONO),
    mCurrentEye(EYE_LEFT),
    mHorizontalFieldOfView(75.0),
    mAspectRatio( 4.0/3.0 ),
    mInterpupillaryDistance( 0.064 ), // 0.064 m = 6.4 cm - mean human eye distance: 6.47cm (male), 6.23cm (female)
    mNearClippingPlane(0.1),     // 10 cm
    mFarClippingPlane(5000.0),   // 5000 meter
    mLookAtDistance(500.0)       // half a kilometer away of the screen
{
    setRotationMatrix( glm::mat3(1.0f) );
}

void GenericCamera::FPSstyleLookAround( float _deltaX, float _deltaY )
{
    float yaw   = 0.0f;
    float pitch = 0.0f;
    glm::mat3 R = getRotationMatrix3();

    // get roll / pitch / yaw from the current rotation matrix:
    float yaw1 = asin(-R[2][0]);
    float yaw2 = M_PI - asin(-R[2][0]);

    float pitch1  = (cos(yaw1) > 0)? atan2(R[2][1], R[2][2]): atan2(-R[2][1], -R[2][2]);
    float pitch2  = (cos(yaw2) > 0)? atan2(R[2][1], R[2][2]): atan2(-R[2][1], -R[2][2]);

    float roll1   = (cos(yaw1) > 0)? atan2(R[1][0], R[0][0]): atan2(-R[1][0], -R[0][0]);
    float roll2   = (cos(yaw2) > 0)? atan2(R[1][0], R[0][0]): atan2(-R[1][0], -R[0][0]);

    // we assume no roll at all, in that case one of the roll variants will be 0.0
    // if not, use the smaller one -> minimize the camera "jump" as this will destroy
    // information
    if ( std::abs(roll1) <= std::abs(roll2) ) {
        yaw   = -yaw1;
        pitch = -pitch1;
    } else {
        yaw   = -yaw2;
        pitch = -pitch2;
    }

    // add rotation diffs given:
    yaw = yaw + _deltaX;
    pitch = glm::clamp( pitch + _deltaY, -0.5f * (float)M_PI, 0.5f*(float)M_PI );

    // create rotation matices, seperated so we have full control over the order:
    glm::mat4 newRotY = glm::yawPitchRoll( yaw, 0.0f, 0.0f );
    glm::mat4 newRotX = glm::yawPitchRoll( 0.0f, pitch, 0.0f );

    // multiplication order is important to prevent roll:
    setRotationMatrix( newRotX * newRotY );
}

void GenericCamera::setRotationMatrix(glm::mat3 _matrix)
{
    mRotationMatrix = _matrix;
}

void GenericCamera::setRotationMatrix(glm::mat4 _matrix)
{
    mRotationMatrix = glm::mat3(_matrix);
}

void GenericCamera::setLookAtMatrix(const glm::vec3 &_position, const glm::vec3 &_target, const glm::vec3 &_up)
{
    setPosition(_position);
    setTarget(_target, _up);
}

glm::mat4 GenericCamera::getTranslationMatrix4() const
{
    glm::mat4 trans;
    trans[3][0] = -mPosition.x;
    trans[3][1] = -mPosition.y;
    trans[3][2] = -mPosition.z;
    return trans;
}

glm::vec3 GenericCamera::getUpDirection     () const
{
    glm::vec3 up(mRotationMatrix[0][1], mRotationMatrix[1][1], mRotationMatrix[2][1]);
    assert(glm::distance(getInverseRotationMatrix3() * glm::vec3(0.0f, 1.0f, 0.0f), up) < .01);
    return up;
}
glm::vec3 GenericCamera::getRightDirection  () const
{
    glm::vec3 right(mRotationMatrix[0][0], mRotationMatrix[1][0], mRotationMatrix[2][0]);
    assert(glm::distance(getInverseRotationMatrix3() * glm::vec3(1.0f, 0.0f, 0.0f), right) < .01);
    return right;
}
glm::vec3 GenericCamera::getForwardDirection() const
{
    glm::vec3 forward(-mRotationMatrix[0][2], -mRotationMatrix[1][2], -mRotationMatrix[2][2]);
    assert(glm::distance(getInverseRotationMatrix3() * glm::vec3(0.0f, 0.0f, -1.0f), forward) < .01);
    return forward;
}

void GenericCamera::setTarget(const glm::vec3 &_target, const glm::vec3 &_up)
{
    glm::vec3 forwardVector = _target - mPosition;
    mLookAtDistance = glm::length( forwardVector );

    forwardVector         = forwardVector / (float)mLookAtDistance; // normalize
    glm::vec3 rightVector = glm::normalize(glm::cross( forwardVector, _up ));
    glm::vec3 upVector    = glm::cross( rightVector, forwardVector );

    glm::mat3 rotMatrix;
    rotMatrix[0][0] =  rightVector.x;
    rotMatrix[0][1] =  upVector.x;
    rotMatrix[0][2] = -forwardVector.x;
    rotMatrix[1][0] =  rightVector.y;
    rotMatrix[1][1] =  upVector.y;
    rotMatrix[1][2] = -forwardVector.y;
    rotMatrix[2][0] =  rightVector.z;
    rotMatrix[2][1] =  upVector.z;
    rotMatrix[2][2] = -forwardVector.z;

    setRotationMatrix( rotMatrix );
}

void GenericCamera::setHorizontalFieldOfView(float _fovh)
{
    assert( _fovh < 180.0f );
    assert( _fovh > 0.0f );
    mHorizontalFieldOfView = _fovh;
}

void GenericCamera::setVerticalFieldOfView(float _fovv)
{
    assert( _fovv < 180.0f );
    assert( _fovv > 0.0f );
	
    // we only save the aspectRatio and the horizontal FoV
	// so if we change the vertical FoV, we change the aspectRatio
	
    //mAspectRatio = tan( calcDegToRad(0.5f * mHorizontalFieldOfView) ) / tan( calcDegToRad(0.5f * _fovv) );

    float x = tan( calcDegToRad(0.5f * _fovv) ) * mAspectRatio;
    mHorizontalFieldOfView = calcRadToDeg( 2.0f * atan( x ) );
}

float GenericCamera::getVerticalFieldOfView() const
{
    return calcRadToDeg( atan( tan( calcDegToRad(0.5f*mHorizontalFieldOfView) ) / mAspectRatio ) *2.0f );
}
	
void GenericCamera::setNearClippingPlane(float _plane)
{
    assert (_plane > 0.0f);
    mNearClippingPlane = _plane;
}

void GenericCamera::setFarClippingPlane(float _plane)
{
    assert (_plane > 0.0f);
    mFarClippingPlane = _plane;
}

void GenericCamera::setLookAtDistance(float _distance)
{
    assert (_distance > 0.0f);
    mLookAtDistance = _distance;
}

glm::mat4 GenericCamera::getViewMatrix() const
{
    if (mStereoMode == MONO) {
        return getMonoViewMatrix();
    } else {
        // all kinds of stereo
        bool eyeIsLeftEye = (getEye() == EYE_LEFT);
        return getStereoViewMatrix( eyeIsLeftEye, mStereoMode );
    }
}

glm::mat4 GenericCamera::getStereoViewMatrix( bool _leftEye, StereoMode _stereoMode ) const
{
    // The view matrix is independent of the projection mode (isometric or perspective)
    // so only the stereo mode has to be checked.
    assert( _stereoMode != MONO && "mono is not a stereo mode!" );

    float cameraPositionShiftValue = (mInterpupillaryDistance*0.5f); // shift to the right
    if (_leftEye) cameraPositionShiftValue *= -1.0f;      // if left eye shift to the left

    if ( (_stereoMode == PARALLEL_SHIFT) || (_stereoMode == OFF_AXIS) ) {
        //
        // parallel shift and off-axis have the same view matrices:
        // just shift the camera position to the left/right by half the eye-distance
        //

        //ACGL::Utils::debug() << "WARNING: getStereoViewMatrix() is not tested yet" << std::endl; // remove after testing

        glm::mat3 inverseRotation = getInverseRotationMatrix3();
        glm::vec3 eyePosition = mPosition + (inverseRotation * glm::vec3(cameraPositionShiftValue,0.0f,0.0f) );

        glm::mat4 m(mRotationMatrix);
        m[3][0] = -(m[0][0] * eyePosition.x + m[1][0] * eyePosition.y + m[2][0] * eyePosition.z);
        m[3][1] = -(m[0][1] * eyePosition.x + m[1][1] * eyePosition.y + m[2][1] * eyePosition.z);
        m[3][2] = -(m[0][2] * eyePosition.x + m[1][2] * eyePosition.y + m[2][2] * eyePosition.z);
        return m;
    }

    // else it has to be toe-in:
    assert( _stereoMode == TOE_IN && "unsupported stereo mode!" );
    //
    // Toe-in: shift the camera position to the left/right by half the eye-distance and
    //         rotate a bit inwards so that the two cameras focus the same point
    //         at the look-at distance (focal point)

    assert(0 && "getStereoViewMatrix() for TOE_IN is not implemented yet!");
    return glm::mat4();
}

glm::mat4 GenericCamera::getInverseViewMatrix() const
{
    if (mStereoMode == MONO) {
        return getMonoInverseViewMatrix();
    }

    glm::mat4 viewMatrix = getViewMatrix();
    return glm::inverse( viewMatrix );
}

glm::mat4 GenericCamera::getProjectionMatrix() const
{
    if (mStereoMode == MONO) {
        return getMonoProjectionMatrix();
    } else {
        // all kinds of stereo
        bool eyeIsLeftEye = (getEye() == EYE_LEFT);
        return getStereoProjectionMatrix( eyeIsLeftEye, mStereoMode );
    }
}

glm::mat4 GenericCamera::getMonoProjectionMatrix() const
{
    glm::mat4 projectionMatrix; // identity matrix

    if ( getProjectionMode() == ISOMETRIC_PROJECTION )
    {
        // we don't set the left/right/top/bottom values explicitly, so we want that
        // all object at our focal distance appear the same in perspective and isometric view
        float right  = tan( calcDegToRad(getHorizontalFieldOfView() * 0.5f) ) * mLookAtDistance;
        float left   = -right;
        float top    = tan( calcDegToRad(getVerticalFieldOfView() * 0.5f) ) * mLookAtDistance;
        float bottom = -top;

        // we do the same here as a glOrtho call would do.
        projectionMatrix[ 0][0] =  2.0f / (right - left);
        projectionMatrix[ 1][1] =  2.0f / (top   - bottom);
        projectionMatrix[ 2][2] = -2.0f / (mFarClippingPlane - mNearClippingPlane);
        projectionMatrix[ 0][3] = -(right+left) / (right-left);
        projectionMatrix[ 1][3] = -(top+bottom) / (top-bottom);
        projectionMatrix[ 2][3] = -(mFarClippingPlane+mNearClippingPlane)/(mFarClippingPlane-mNearClippingPlane);
        projectionMatrix[ 3][3] =  1.0;
    }
    else if ( mProjectionMode == PERSPECTIVE_PROJECTION )
    {
        projectionMatrix = glm::perspective( (float)getHorizontalFieldOfView(), (float)getAspectRatio(), (float)mNearClippingPlane, (float)mFarClippingPlane );
    }
    else assert(0 && "unsupported projection mode");

    return projectionMatrix;
}

void GenericCamera::move( const glm::vec3 &_vector )
{
    glm::mat3 inverseRotation = getInverseRotationMatrix3();
    mPosition += (inverseRotation * _vector);
}

glm::mat4 GenericCamera::getMonoViewMatrix() const
{
    glm::mat4 m(mRotationMatrix);
    m[3][0] = -(m[0][0] * mPosition.x + m[1][0] * mPosition.y + m[2][0] * mPosition.z);
    m[3][1] = -(m[0][1] * mPosition.x + m[1][1] * mPosition.y + m[2][1] * mPosition.z);
    m[3][2] = -(m[0][2] * mPosition.x + m[1][2] * mPosition.y + m[2][2] * mPosition.z);
    return m;
}

glm::mat4 GenericCamera::getMonoInverseViewMatrix() const
{
    glm::mat4 m(glm::transpose(mRotationMatrix));
    m[3][0] = mPosition.x;
    m[3][1] = mPosition.y;
    m[3][2] = mPosition.z;
    return m;
}

glm::mat4 GenericCamera::getStereoProjectionMatrix( bool _leftEye, StereoMode _stereoMode ) const
{
    assert( _stereoMode != MONO && "mono is not a stereo mode!" );

    if ( getProjectionMode() == ISOMETRIC_PROJECTION )
    {
        // very unusual, prepare for headaches!
        return getMonoProjectionMatrix();
    }

    if ((_stereoMode == PARALLEL_SHIFT) || (_stereoMode == TOE_IN))
    {
        // the view matrix changes but the projection matrix stays the same
        return getMonoProjectionMatrix();
    }

    // so off-axis it is!
    assert( _stereoMode == OFF_AXIS && "unknown projection mode!" );
    //
    // In this mode the camera positions (view matrix) is shifted to the left/right still looking
    // straight ahead. The projection is also looking ahead but the projection center is
    // off (hence off-axis).
    // There is one plane in front of the cameras where the view-frusta match.
    // This should be the distance to the physical screen from the users position.


    assert(0 && "getStereoViewMatrix() is not implemented for OFF_AXIS yet!");
    return glm::mat4();

}

float GenericCamera::getFocalLenghtInPixel() const
{
    return ( (float) mViewportSize.x ) / ( 2.0f * tan( calcDegToRad(0.5f * mHorizontalFieldOfView) ) );
}

void GenericCamera::setFocalLengthInPixel( float _focalLengthInPixel )
{
    float hFoVrad = 2.0f * atan( ( 0.5f * mViewportSize.x ) * (1.0f / _focalLengthInPixel ) );
    setHorizontalFieldOfView( calcRadToDeg( hFoVrad ) );
}

