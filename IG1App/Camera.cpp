#include "Shader.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>


using namespace glm;

Camera::Camera(Viewport* vp)
  : mViewMat(1.0)
  , mProjMat(1.0)
  , xRight(vp->width() / 2.0)
  , xLeft(-xRight)
  , yTop(vp->height() / 2.0)
  , yBot(-yTop)
  , mViewPort(vp)
{
	setPM();
}

void
Camera::uploadVM() const
{
	/*
	Shader::setUniform4All("modelView", mViewMat);
	
	Shader* lightShader = Shader::get("simple_light");
	lightShader->use();
	lightShader->setUniform("lightDir", glm::vec4(glm::normalize(mViewMat * glm::vec4(-1.0, -1.0, -1.0, 0.0))));
	*/
}

void
Camera::setVM()
{
	mViewMat = lookAt(mEye, mLook, mUp); // glm::lookAt defines the view matrix
	setAxes();
}

void
Camera::set2D()
{
	mEye = {0, 0, 500};
	mLook = {0, 0, 0};
	mUp = {0, 1, 0};
	mAng = -90;
	mRadio = mEye[2];
	viewType = 1;
	setVM();
}

void
Camera::set3D()
{
	mEye = {500, 500, 500};
	mLook = {0, 0, 0};
	mUp = {0, 1, 0};
	mAng = -45;
	mRadio = mEye[2];
	viewType = 2;
	setVM();
}

void
Camera::pitch(GLdouble a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(1.0, 0, 0));
	// glm::rotate returns mViewMat * rotationMatrix
}

void
Camera::yaw(GLdouble a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(0, 1.0, 0));
	// glm::rotate returns mViewMat * rotationMatrix
}

void
Camera::roll(GLdouble a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(0, 0, 1.0));
	// glm::rotate returns mViewMat * rotationMatrix
}

void
Camera::setSize(GLdouble xw, GLdouble yh)
{
	xRight = xw / 2.0;
	xLeft = -xRight;
	yTop = yh / 2.0;
	yBot = -yTop;
	setPM();
}

void
Camera::setScale(GLdouble s)
{
	mScaleFact -= s;
	if (mScaleFact < 0)
		mScaleFact = 0.01;
	setPM();
}

void
Camera::setPM()
{
	if (bOrto) { //  if orthogonal projection
		mProjMat = ortho(xLeft * mScaleFact,
		                 xRight * mScaleFact,
		                 yBot * mScaleFact,
		                 yTop * mScaleFact,
		                 mNearVal,
		                 mFarVal);
		// glm::ortho defines the orthogonal projection matrix
	}
	else {
		mProjMat = frustum(xLeft * mScaleFact,
			xRight * mScaleFact,
			yBot * mScaleFact,
			yTop * mScaleFact,
			mNearVal * 200,
			mFarVal * 400);
	}
	
}

void
Camera::uploadPM() const
{
	Shader::setUniform4All("projection", mProjMat);
}

void
Camera::upload() const
{
	mViewPort->upload();
	uploadVM();
	uploadPM();
}

//Ap 38
void
Camera::setAxes() {
	mRight = row(mViewMat, 0);
	mUpward= row(mViewMat, 1);
	mFront = -row(mViewMat, 2);
}

void 
Camera::moveLR(GLfloat cs) { // Left / Right ...
	mEye += mRight * cs;
	mLook += mRight * cs;
	setVM();
}

void 
Camera::moveUD(GLfloat cs) { // Up / Down
	mEye += mUpward * cs;
	mLook += mUpward * cs;
	setVM();
}

void 
Camera::moveFB(GLfloat cs) { // Left / Right ...
	mEye += mFront * cs;
	mLook += mFront * cs;
	setVM();
}

//Ap41-43
void 
Camera::changePrj() {
		
	bOrto = !bOrto;
	
	setPM();
}

//Ap45
void
Camera::yawReal(GLfloat cs) {// Up/Down
	mLook += mRight * cs;
	setVM();
}

void
Camera::pitchReal(GLfloat cs) {// Up/Down
	mLook += mUpward * cs;
	mUp = mUpward;
	setVM();
}

void 
Camera::rollReal(GLfloat cs) {
	glm::mat4 roll_mat = glm::rotate(glm::mat4(1.0f), glm::radians(cs), mFront);
	mUp = glm::mat3(roll_mat) * mUp;
	setVM();
}

//Ap46-47, óbita en torno a la escena
void 
Camera::orbit(GLdouble incAng, GLdouble incY) {
	mAng += incAng;
	mEye.x = mLook.x + cos(radians(mAng)) * mRadio;
	mEye.z = mLook.z - sin(radians(mAng)) * mRadio;
	mEye.y += incY;
	setVM();
}

//Ap48, vista cenital
void
Camera::setCenital() {
	mEye = { 0, 500, 0 };
	mLook = { 0, 0, 0 };
	mUp = { 0, 0, -1 };

	mAng = -45;
	mRadio = mEye[1];

	viewType = 3;

	setVM();
}

