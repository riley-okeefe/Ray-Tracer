/* 
* Backwards Ray Tracer
* Author: Riley O'Keefe
* CSCI-4471
*/

#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "Image.h"
#include "camera.h"
#include "sphere.h"
#include "imageplane.h"
#include "Triangle.h"
#include "Texture.h"
#include<cstdlib>
#include<list>

using namespace std;
//Data types
typedef Eigen::Matrix<float, 3, 1> Vec3;
typedef Eigen::Matrix<float, 2, 1> Vec2;


//Color functions
using Colour = cv::Vec3b; // BGR Value: Type aliasing (Colour is just an alias for cv:Vec3b type)
Colour white() { return Colour(255, 255, 255); }
Colour black() { return Colour(0, 0, 0); }
Colour red() { return Colour(255, 0, 0); }
Colour green() { return Colour(0, 255, 0); }
Colour blue() { return Colour(0, 0, 255); }


//bounding the channel wise pixel color between 0 to 255
unsigned char Clamp(int pixelCol)
{
	if (pixelCol < 0) return 0;
	if (pixelCol >= 255) return 255;
	return pixelCol;
}

/*
* Calculating the ray sphere intersection using the lightsource as origin and shadow ray direction for ray direction
*/
bool raySphereIntersect(Sphere s, const Vec3& origin, const Vec3& direction, const Vec3& center) {
	Vec3 oc = center - origin;
	return s.intersectRay(direction, oc);
}

/*
* Use of Moller-Trumbore Algorithm to check for shadow ray intersection with triangle surface
* Source: https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection.html
*/
bool rayTriangleIntersect(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Vec3 origin, const Vec3 direction) {
	float epsilon = 0.00001;
	Vec3 edge1 = v1 - v0; //(B-A)
	Vec3 edge2 = v2 - v0; //(C-A)
	Vec3 pvec = direction.cross(edge1);
	float det = edge2.dot(pvec);

	// if det is greater than -epsilon and epsilon then det is equal to zero and there is no intersection
	if (det > -epsilon && det < epsilon) {
		return false;
	}

	float invDet = 1.0 / det;

	Vec3 tvec = origin - v0;
	//if 0 < u > 1 reject and return false (no intersection)
	float u = tvec.dot(pvec) * invDet;
	if (u < 0.0 || u > 1.0) return false;

	Vec3 qvec = tvec.cross(edge1);

	//if 0 < v and u + v > 1 return false (no intersection)
	float v = direction.dot(qvec) * invDet;
	if (v < 0.0 || u + v > 1.0) return false;

	// if all above conditions are true then there's an intersection
	float t = edge2.dot(qvec) * invDet;

	if (t > epsilon) return true;

}

int main(int, char**) {

	//Create an image object with 500 x 500 resolution.
	Image image = Image(500, 500);
	ImagePlane imgPlane;
	imgPlane.setImage(image);
	imgPlane.setXCol(image.cols);
	imgPlane.setYRow(image.rows);

	//Coordinates of image rectangle
	Vec3 llc = Vec3(-1, -1, -1);//lower left corner
	Vec3 urc = Vec3(1, 1, -1);//upper right corner
	int width = urc(0) - llc(0); //get width of the image plane
	int height = urc(1) - llc(1); //get the height of the image plane
	Vec2 pixelUV = Vec2((float)width / image.cols, (float)height / image.rows); //get the pixel spacing

	//specify the camera position (cameraPoint)
	Camera camera;
	Vec3 cameraPoint = Vec3(0.0f, 0.0f, 0.0f);
	camera.setPos(cameraPoint);

	///Define the sphere: spherePos, sphereRadius
	Sphere sphere;
	Vec3 spherePos = Vec3(0.0f, -0.5f, -2.0f); // (0, 0, -5)
	sphere.setCenter(spherePos);
	float SphereRadius = 1.0f;
	sphere.setRadius(SphereRadius);

	//create the floor--use two triangles as follows. Note that the texture coordinates of triangle vertices are specified as well.
	list<Triangle>  tri_list{
		Triangle(Vec3(-7, -1, -1), Vec3(7, -1, -1), Vec3(-7, -1, -7), Vec2(0, 0), Vec2(1, 0), Vec2(0, 1)),
			Triangle(Vec3(7, -1, -1), Vec3(7, -1, -7), Vec3(-7, -1, -7), Vec2(1, 0), Vec2(1, 1), Vec2(0, 1)) };

	float epsilon = 0.00001;

	//specify the reflection coefficients of sphere, k_a, k_d, k_s
	Vec3 k_a = Vec3(0.2f, 0.2f, 0.2f);
	Vec3 k_d = Vec3(0.3f, 0.8f, 1.0f);
	Vec3 k_s = Vec3(1.0f, 1.0f, 1.0f);

	//specify the lighting source 
	Vec3 lightSource(2.0f, -4.0f, 3.0f);

	///specify the ambient, diffuse and specular light colors
	Vec3 lightAmbient = Vec3(0.5f, 0.5f, 0.5f); //gray light
	Vec3 lightDiffuse = Vec3(1.0f, 1.0f, 1.0f); // white
	Vec3 lightSpecular = Vec3(1.0f, 1.0f, 1.0f); //white

	Vec3 Origin = cameraPoint;

	// Textures
	Texture tex = Texture(); // floor texture
	Texture sphereTex = Texture(); // sphere texture
	Texture backTex = Texture();  // background texture
	//Load texture
	tex.loadTexture("Textures/water.png");
	sphereTex.loadTexture("Textures/earth1.png");
	backTex.loadTexture("Textures/galaxy.png");
	//end
	int h_samples = 8;
	int v_samples = 8;
	for (int i = 0; i < image.rows; ++i) {

		for (int j = 0; j < image.cols; ++j) {
			Vec3 colour(0, 0, 0);
			Colour pixelColor;
			for (int dx = 0; dx < h_samples; ++dx) {
				for (int dy = 0; dy < v_samples; ++dy) {

					pixelColor = backTex.get_pixel(j, i); // adding texture to background
					//pixelColor = Colour(0,165,255); // color the background

					Vec3 pixelPos = imgPlane.generatePixelPos(i, j, dx, dy);

					Vec3 sphereDirection = camera.generateRay(pixelPos);
					sphereDirection.normalize();

					if (sphere.intersectRay(sphereDirection) > 0) {

						Scalar t = sphere.intersectRay(sphereDirection);
						/*
						* Calculating ray intersection P(t) = O + td where:
						* A is the ray origin
						* d is ray direction
						* t is a point along the ray
						*/
						Vec3 Intersection = (Origin + sphereDirection) * t;
						Intersection.normalize();

						if (t > 0) {
							//Blinn-Phong shading
							Vec3 Normal = Intersection - sphere.getCenter();
							Normal.normalize();

							Vec3 LightVector = lightSource - Intersection;
							LightVector.normalize();

							//Compute difffuse term (diffuseterm)
							float diffuseTerm = LightVector.dot(Normal);
							if (diffuseTerm < 0) diffuseTerm = 0;

							//compute the half vector (HalfVec) between light vector and view vector (opposite of the ray direction)
							Vec3 halfVec = pixelPos + Origin;
							//Normalize the half vector
							halfVec.normalize();
							//Compute specular term (specularTerm), similar to diffuseTerm, but use halfVec and Normal
							float specTerm = halfVec.dot(Normal);

							Vec3 objectColor = Vec3(0.1f, 0.3f, 1.0f);
							Vec3 subc;
							Colour col;
							subc[0] = Clamp(((k_a[0] * lightAmbient[0] + k_d[0] * lightDiffuse[0] * diffuseTerm + k_s[0] * lightSpecular[0] * pow(specTerm, 1000)) * objectColor[0]) * 254);
							subc[1] = Clamp(((k_a[1] * lightAmbient[1] + k_d[1] * lightDiffuse[1] * diffuseTerm + k_s[1] * lightSpecular[1] * pow(specTerm, 1000)) * objectColor[1]) * 254);
							subc[2] = Clamp(((k_a[2] * lightAmbient[2] + k_d[2] * lightDiffuse[2] * diffuseTerm + k_s[2] * lightSpecular[2] * pow(specTerm, 1000)) * objectColor[2]) * 254);
							//pixelColor = sphereTex.get_pixel(j,i); // for adding texture to sphere
							//image(i, j) = pixelColor; // for adding texture to wphere
							colour += subc; // comment this out when using the pixelColor to texture sphere
						}
						else {
							//The camera is inside the sphere
							image(i, j) = black();
						}

					}
					else {
						//No intersection, discriminant < 0 (background)
						// value to flip triangle primitive position 
						int rotationValue = 2;
						// re-doing pixel position calculation to flip surface up-right
						Vec3 triPixelPos = imgPlane.generatePixelPos(i, j, dx, dy, rotationValue);

						Vec3 triDirection = triPixelPos - Origin;
						triDirection.normalize();

						//loop to go over the primitives in the scene
						for (auto it = tri_list.begin(); it != tri_list.end(); ++it) {

							//Ray-plane intersection
							Vec3 tNormal = it->normal_;
							tNormal.normalize();
							Scalar t = (float)(it->vertex1_ - Origin).dot(tNormal) / triDirection.dot(tNormal);

							//Ray-triangle intersection...compute the intersection point of the ray-plane intersection
							Vec3 triIntersection = Origin + t * triDirection;
							Vec3 baryCoords = it->BaryCentric(triIntersection);
							Vec3 sphereIntersection = Origin + t * sphereDirection; //intersection for shadow using the sphere ray direction

							Vec3 shadowDirection = lightSource - sphereIntersection; // Get direction of shadow ray
							shadowDirection.normalize();
							bool inShadow = false;

							//check if the intersection point lies in the triangle by using barycentric coordinates for testing
							if (fabs(baryCoords.sum() - 1) < epsilon && (baryCoords[0] <= 1 && baryCoords[0] >= 0) &&
								(baryCoords[1] <= 1 && baryCoords[1] >= 0) && (baryCoords[2] <= 1 && baryCoords[2] >= 0)) {
								// check if shadow ray intersects with sphere
								if (raySphereIntersect(sphere, lightSource, shadowDirection, spherePos)) {
									inShadow = true;
								}
								else {
									//check if shadow ray intersects with triangle surface
									for (auto it2 = tri_list.begin(); it2 != tri_list.end(); ++it2) {
										if (rayTriangleIntersect(it2->vertex1_, it2->vertex2_, it2->vertex3_, lightSource, shadowDirection)) {
											inShadow = true;
											break;
										}
									}
								}
								if (!inShadow) {
									// use of billinear interpolation to add texture to surface
									pixelColor = tex.texture_lookup_bilinear(it->getTextCoord(triIntersection));
								}
								else {
									// in the shadow, so add black color to shadow
									pixelColor = black();
								}
							}
						}
						image(i, j) = pixelColor;
					}
				}
			}
			/* Code for averaging subpixels for anti aliasing
			 * Comment out this code for texturing sphere 
			 */
			Vec3 spherePixelPos = imgPlane.generatePixelPos(i,j);

			Vec3 sphereDirection = spherePixelPos - Origin;
			sphereDirection.normalize();

			if (sphere.intersectRay(sphereDirection) >= 0) {
				// Averaging the subpixels for anti-aliasing
				colour /= (h_samples * v_samples);
				image(i, j) = Colour(colour[0], colour[1], colour[2]);
			}
			if (sphere.intersectRay(sphereDirection) < 0) {
				image(i,j) = pixelColor;
			}
		}
	}
	// save the image
	image.save("./result.png");
	image.display();

	return EXIT_SUCCESS;
}
