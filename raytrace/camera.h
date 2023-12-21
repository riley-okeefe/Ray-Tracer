#ifndef CAMERA_H
#define CAMERA_H

#include <Eigen/Dense>
#include <Eigen/Geometry>

//Data types
typedef Eigen::Matrix<float, 3, 1> Vec3;
typedef Eigen::Matrix<float, 2, 1> Vec2;

class Camera
{
public:
	/*
	* function to generate ray direction
	*/
	Vec3 generateRay(Vec3 const& pt);
	void setPos(Vec3 camPos) {
		mPosition_ = camPos;
	}
	Vec3 getPos() {
		return mPosition_;
	}
private:
	Vec3 mPosition_;
};

// inline functions
inline Vec3 Camera::generateRay(Vec3 const& pt) {
	Vec3 origin = mPosition_;
	Vec3 direction = pt - mPosition_;
	return direction;
}

#endif