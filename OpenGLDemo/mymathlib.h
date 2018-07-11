#ifndef MYMATHLIB_H
#define MYMATHLIB_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

struct Quat{
	Quat(){
		x = 0.0f;
		y = 1.0f;
		z = 0.0f;
		w = 0.0f;
	}
	Quat(float xx, float yy, float zz, float ww){
		x = xx;
		y = yy;
		z = zz;
		w = ww;
	}
	glm::vec3 axis(){
		return glm::vec3(x, y, z);
	}
	float x;
	float y;
	float z;
	float w;
};

namespace mymathlib
{
	struct Mat4
	{
		Mat4(){
			mat[0] = 1.0f;
			mat[1] = 0.0f;
			mat[2] = 0.0f;
			mat[3] = 0.0f;
			mat[4] = 0.0f;
			mat[5] = 1.0f;
			mat[6] = 0.0f;
			mat[7] = 0.0f;
			mat[8] = 0.0f;
			mat[9] = 0.0f;
			mat[10] = 1.0f;
			mat[11] = 0.0f;
			mat[12] = 0.0f;
			mat[13] = 0.0f;
			mat[14] = 0.0f;
			mat[15] = 1.0f;
		}

		glm::mat4 tomat4(){
			glm::mat4 ret;
			for (int i = 0; i < 16; i++)
			{
				ret[i/4][i%4] = mat[i];
			}
			return ret;
		}

		float mat[16];

	};
	
	Mat4 operator*(Mat4 lhs, Mat4 rhs);
	Mat4 translate(Mat4 m, glm::vec3 v);
	Mat4 scale(Mat4 m, glm::vec3 v);
	Mat4 r(Mat4 m, glm::vec3 v, float angles);
	Mat4 rotate(Mat4 m, Quat a);
}


#endif