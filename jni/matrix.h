#ifndef _MATRIX_H_
#define _MATRIX_H_

void matrix_reset(float *resmtx);
void matrix_identity(float *resmtx);
void matrix_copy(float *resmtx, float *origmtx);
void matrix_multiply(float *resmtx, float *mtx1, float *mtx2);

#define MTX_AXIS_X 0
#define MTX_AXIS_Y 1
#define MTX_AXIS_Z 2
void matrix_set_rotate(float *resmtx, int axis, float angle);
void matrix_set_translate(float *resmtx, float tx, float ty, float tz);
void matrix_set_scale(float *resmtx, float sx, float sy, float sz);

void matrix_rotate(float *resmtx, int axis, float angle);
void matrix_translate(float *resmtx, float tx, float ty, float tz);
void matrix_scale(float *resmtx, float sx, float sy, float sz);

void matrix_rotate_to(float *resmtx, float *origmtx,
		      int axis, float angle);
void matrix_translate_to(float *resmtx, float *origmtx,
			 float tx, float ty, float tz);
void matrix_scale_to(float *resmtx, float *origmtx,
		     float sx, float sy, float sz);

// resmtx is 3x3
void matrix_normal_to(float *resmtx, float *modelmtx, float *viewmtx);

void matrix_set_perspective(float *resmtx, float fovy, float aspect,
			    float near, float far);
void matrix_set_ortho(float *resmtx, float left, float right,
		      float bottom, float top, float near, float far);
void matrix_set_lookat(float *resmtx,
		       float ex, float ey, float ez,
		       float cx, float cy, float cz,
		       float ux, float uy, float uz);

#endif /* _MATRIX_H_ */
