#include"mymathlib.h"


namespace mymathlib
{
	Mat4 operator*(Mat4 lhs, Mat4 rhs){
		Mat4 ret;
		for (int i = 0; i < 16; i++)
		{
			ret.mat[i] = lhs.mat[(i / 4) * 4] * rhs.mat[i % 4] + lhs.mat[(i / 4) * 4 + 1] * rhs.mat[(i % 4) + 4] + lhs.mat[(i / 4) * 4 + 2] * rhs.mat[i % 4 + 8] + lhs.mat[(i / 4) * 4 + 3] * rhs.mat[i % 4 + 12];
		}
		return ret;
	}

	Mat4 translate(Mat4 m, glm::vec3 v){
		m.mat[12] += v.x;
		m.mat[13] += v.y;
		m.mat[14] += v.z;

		return m;
	}

	Mat4 scale(Mat4 m, glm::vec3 v){
		m.mat[0] *= v.x;
		m.mat[5] *= v.y;
		m.mat[10] *= v.z;

		return m;
	}

	Mat4 r(Mat4 m, glm::vec3 v, float angles){
		Mat4 rmat;
		rmat.mat[0] = v.x*v.x*(1 - cos(angles)) + cos(angles);
		rmat.mat[1] = v.x*v.y*(1 - cos(angles)) - v.z*sin(angles);
		rmat.mat[2] = v.x*v.z*(1 - cos(angles)) + v.y*sin(angles);
		rmat.mat[3] = 0.0f;
		rmat.mat[4] = v.x*v.y*(1 - cos(angles)) + v.z*sin(angles);
		rmat.mat[5] = v.y*v.y*(1 - cos(angles)) + cos(angles);
		rmat.mat[6] = v.y*v.z*(1 - cos(angles)) - v.x*sin(angles);
		rmat.mat[7] = 0.0f;
		rmat.mat[8] = v.x*v.z*(1 - cos(angles)) - v.y*sin(angles);
		rmat.mat[9] = v.y*v.z*(1 - cos(angles)) + v.x*sin(angles);
		rmat.mat[10] = v.z*v.z*(1 - cos(angles)) + cos(angles);
		rmat.mat[11] = 0.0f;
		rmat.mat[12] = 0.0f;
		rmat.mat[13] = 0.0f;
		rmat.mat[14] = 0.0f;
		rmat.mat[15] = 1.0f;
		return  rmat*m;
	}

	Mat4 rotate(Mat4 m, Quat a){
		return r(m, a.axis(), a.w);
	}

    glm::mat4 aiMat_col_cast(const aiMatrix4x4 aimat){
        glm::mat4 mat = glm::mat4();
        mat[0][0] = aimat.a1;
        mat[0][1] = aimat.a2;
        mat[0][2] = aimat.a3;
        mat[0][3] = aimat.a4;
        mat[1][0] = aimat.b1;
        mat[1][1] = aimat.b2;
        mat[1][2] = aimat.b3;
        mat[1][3] = aimat.b4;
        mat[2][0] = aimat.c1;
        mat[2][1] = aimat.c2;
        mat[2][2] = aimat.c3;
        mat[2][3] = aimat.c4;
        mat[3][0] = aimat.d1;
        mat[3][1] = aimat.d2;
        mat[3][2] = aimat.d3;
        mat[3][3] = aimat.d4;

        return mat;
    }

    glm::mat4 aiMat_row_cast(const aiMatrix4x4 aimat){
        glm::mat4 mat = glm::mat4();
        mat[0][0] = aimat.a1;
        mat[1][0] = aimat.a2;
        mat[2][0] = aimat.a3;
        mat[3][0] = aimat.a4;
        mat[0][1] = aimat.b1;
        mat[1][1] = aimat.b2;
        mat[2][1] = aimat.b3;
        mat[3][1] = aimat.b4;
        mat[0][2] = aimat.c1;
        mat[1][2] = aimat.c2;
        mat[2][2] = aimat.c3;
        mat[3][2] = aimat.c4;
        mat[0][3] = aimat.d1;
        mat[1][3] = aimat.d2;
        mat[2][3] = aimat.d3;
        mat[3][3] = aimat.d4;

        return mat;
    }
}