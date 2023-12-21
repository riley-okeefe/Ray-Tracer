#ifndef SPHERE_H
#define SPHERE_H

#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "camera.h"
//Data types
typedef Eigen::Matrix<float, 3, 1> Vec3;
typedef Eigen::Matrix<float, 2, 1> Vec2;

class Sphere 
{
public:
	/*
	* function for calculating ray sphere intersection 
	*/
	inline float Sphere::intersectRay(Vec3 const& ray) {
		Vec3 oc = Vec3(0.0f, 0.0f, 0.0f) - mCenter_;

		float A = ray.dot(ray);
		float B = 2 * (ray.dot(oc));
		float C = ((oc).dot(oc)) - mRadius_ * mRadius_;

		float discriminant = (B * B) - (4 * (A) * (C));

		if (discriminant < 0) {
			return -1.0;
		}
		else {
			float t1 = (-B + sqrt(B * B - (4 * (A) * (C))) / (2) * (A));
			float t2 = (-B - sqrt(B * B - (4 * (A) * (C))) / (2) * (A));

			float tVal = std::min(t1, t2);

			return tVal;
		}
	}

	inline bool Sphere::intersectRay(Vec3 const& ray, Vec3 oc) {
		float A = ray.dot(ray);
		float B = 2 * (ray.dot(oc));
		float C = ((oc).dot(oc)) - mRadius_ * mRadius_;

		float discriminant = (B * B) - (4 * (A) * (C));

		if (discriminant < 0) {
			return false;
		}
		else {
			float t1 = (-B + sqrt(B * B - (4 * (A) * (C))) / (2) * (A));
			float t2 = (-B - sqrt(B * B - (4 * (A) * (C))) / (2) * (A));

			float tVal = std::min(t1, t2);

			return tVal >= 0;
		}
	}
	void setCenter(Vec3 const& center) {
		mCenter_ = center;
	}
	Vec3 getCenter() {
		return mCenter_;
	}
	void setRadius(float radius) {
		mRadius_ = radius;
	}
	float getRadius() {
		return mRadius_;
	}
private:
	Vec3 mCenter_;
	float mRadius_;

};

#endif