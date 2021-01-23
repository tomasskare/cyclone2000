#include <math.h>

#include "matrix.h"

/* Matrices are 4x4 column major. */


static float vector_length(float x, float y, float z)
{
  return sqrtf(x * x + y * y + z * z);
}

void matrix_reset(float *resmtx)
{
  int i;

  for(i = 0 ; i < 16 ; i++)
    resmtx[i] = 0.0f;
}

void matrix_identity(float *resmtx)
{
  int i;

  for(i = 0 ; i < 16 ; i++) {
      resmtx[i] = 0.0f;
  }

  for(i = 0 ; i < 16 ; i += 5) {
      resmtx[i] = 1.0f;
  }
}

void matrix_copy(float *resmtx, float *origmtx)
{
  int i;

  for(i = 0 ; i < 16 ; i++)
    resmtx[i] = origmtx[i];
}

void matrix_multiply(float *resmtx, float *mtx1, float *mtx2)
{
  int r, c;

  for(c = 0 ; c < 4 ; c++) {
    for(r = 0 ; r < 4 ; r++) {
      resmtx[c * 4 + r] =
	(mtx1[0 * 4 + r] * mtx2[c * 4 + 0] +
	 mtx1[1 * 4 + r] * mtx2[c * 4 + 1] +
	 mtx1[2 * 4 + r] * mtx2[c * 4 + 2] +
	 mtx1[3 * 4 + r] * mtx2[c * 4 + 3]);
    }
  }
}

void matrix_set_rotate(float *resmtx, int axis, float angle)
{
  float sina = sinf(angle * M_PI / 180.0f);
  float cosa = cosf(angle * M_PI / 180.0f);

  matrix_identity(resmtx);

  if(axis == MTX_AXIS_X) {
    resmtx[1 * 4 + 1] = cosa;
    resmtx[2 * 4 + 1] = -sina;
    resmtx[1 * 4 + 2] = sina;
    resmtx[2 * 4 + 2] = cosa;
  } else if(axis == MTX_AXIS_Y) {
    resmtx[0 * 4 + 0] = cosa;
    resmtx[0 * 4 + 2] = -sina;
    resmtx[2 * 4 + 0] = sina;
    resmtx[2 * 4 + 2] = cosa;
  } else {
    resmtx[0 * 4 + 0] = cosa;
    resmtx[1 * 4 + 0] = -sina;
    resmtx[0 * 4 + 1] = sina;
    resmtx[1 * 4 + 1] = cosa;
  }
}

void matrix_set_translate(float *resmtx, float tx, float ty, float tz)
{
  matrix_identity(resmtx);
  resmtx[3 * 4 + 0] = tx;
  resmtx[3 * 4 + 1] = ty;
  resmtx[3 * 4 + 2] = tz;
}

void matrix_set_scale(float *resmtx, float sx, float sy, float sz)
{
  matrix_reset(resmtx);
  resmtx[0 * 4 + 0] = sx;
  resmtx[1 * 4 + 1] = sy;
  resmtx[2 * 4 + 2] = sz;
  resmtx[3 * 4 + 3] = 1.0f;
}

void matrix_rotate(float *resmtx, int axis, float angle)
{
  float opmtx[16];
  float tmpmtx[16];

  matrix_set_rotate(opmtx, axis, angle);
  matrix_multiply(tmpmtx, resmtx, opmtx);
  matrix_copy(resmtx, tmpmtx);
}

void matrix_translate(float *resmtx, float tx, float ty, float tz)
{
  float opmtx[16];
  float tmpmtx[16];

  matrix_set_translate(opmtx, tx, ty, tz);
  matrix_multiply(tmpmtx, resmtx, opmtx);
  matrix_copy(resmtx, tmpmtx);
}

void matrix_scale(float *resmtx, float sx, float sy, float sz)
{
  float opmtx[16];
  float tmpmtx[16];

  matrix_set_scale(opmtx, sx, sy, sz);
  matrix_multiply(tmpmtx, resmtx, opmtx);
  matrix_copy(resmtx, tmpmtx);
}

void matrix_normal_to(float *resmtx, float *modelmtx, float *viewmtx)
{
  int c, r;
  float det, tmp;
  float mvmtx[16], tmpmtx[9];

  matrix_multiply(mvmtx, viewmtx, modelmtx);

  for(c = 0 ; c < 3 ; c++) {
    tmpmtx[c * 3 + 0] = mvmtx[c * 4 + 0];
    tmpmtx[c * 3 + 1] = mvmtx[c * 4 + 1];
    tmpmtx[c * 3 + 2] = mvmtx[c * 4 + 2];
  }

  // Get determinant
  det =
    tmpmtx[0 * 3 + 0] * (tmpmtx[1 * 3 + 1] * tmpmtx[2 * 3 + 2] -
			 tmpmtx[2 * 3 + 1] * tmpmtx[1 * 3 + 2]) -
    tmpmtx[1 * 3 + 0] * (tmpmtx[0 * 3 + 1] * tmpmtx[2 * 3 + 2] -
			 tmpmtx[2 * 3 + 1] * tmpmtx[0 * 3 + 2]) +
    tmpmtx[2 * 3 + 0] * (tmpmtx[0 * 3 + 1] * tmpmtx[1 * 3 + 2] -
			 tmpmtx[1 * 3 + 1] * tmpmtx[0 * 3 + 2]);

  if(det == 0.0f) {
    int i;

    for(i = 0 ; i < 9 ; i++)
      resmtx[i] = tmpmtx[i];
    return;
  }

  // Transpose in place
  tmp = tmpmtx[0*3+1]; tmpmtx[0*3+1] = tmpmtx[1*3+0]; tmpmtx[1*3+0] = tmp;
  tmp = tmpmtx[0*3+2]; tmpmtx[0*3+2] = tmpmtx[2*3+0]; tmpmtx[2*3+0] = tmp;
  tmp = tmpmtx[1*3+2]; tmpmtx[1*3+2] = tmpmtx[2*3+1]; tmpmtx[2*3+1] = tmp;

  // Calculate minor determinants, sign corrected, devide by matrix det.
  for(c = 0 ; c < 3 ; c++) {
    for(r = 0 ; r < 3 ; r++) {
      int c1 = (c + 1) % 3;
      int c2 = (c + 2) % 3;
      int r1 = (r + 1) % 3;
      int r2 = (r + 2) % 3;

      resmtx[c * 3 + r] = (tmpmtx[c1 * 3 + r1] * tmpmtx[c2 * 3 + r2] -
			   tmpmtx[c1 * 3 + r2] * tmpmtx[c2 * 3 + r1]) / det;
    }
  }

  // Transpose the result.
  tmp = resmtx[0*3+1]; resmtx[0*3+1] = resmtx[1*3+0]; resmtx[1*3+0] = tmp;
  tmp = resmtx[0*3+2]; resmtx[0*3+2] = resmtx[2*3+0]; resmtx[2*3+0] = tmp;
  tmp = resmtx[1*3+2]; resmtx[1*3+2] = resmtx[2*3+1]; resmtx[2*3+1] = tmp;
}

void matrix_rotate_to(float *resmtx, float *origmtx,
		      int axis, float angle)
{
  float opmtx[16];

  matrix_set_rotate(opmtx, axis, angle);
  matrix_multiply(resmtx, origmtx, opmtx);
}

void matrix_translate_to(float *resmtx, float *origmtx,
			 float tx, float ty, float tz)
{
  float opmtx[16];

  matrix_set_translate(opmtx, tx, ty, tz);
  matrix_multiply(resmtx, origmtx, opmtx);
}

void matrix_scale_to(float *resmtx, float *origmtx,
		     float sx, float sy, float sz)
{
  float opmtx[16];

  matrix_set_scale(opmtx, sx, sy, sz);
  matrix_multiply(resmtx, origmtx, opmtx);
}

void matrix_set_perspective(float *resmtx, float fovy, float aspect,
			    float near, float far)
{
  float f = 1.0f / tanf(fovy * (M_PI / 360.0f));
  float range = 1.0f / (near - far);

  matrix_reset(resmtx);
  resmtx[0] = f / aspect;
  resmtx[5] = f;
  resmtx[10] = (far + near) * range;
  resmtx[11] = -1.0f;
  resmtx[14] = 2.0f * far * near * range;
}

void matrix_set_ortho(float *resmtx, float left, float right,
		      float bottom, float top, float near, float far)
{
  float r_width  = 1.0f / (right - left);
  float r_height = 1.0f / (top - bottom);
  float r_depth  = 1.0f / (far - near);
  float x =  2.0f * (r_width);
  float y =  2.0f * (r_height);
  float z = -2.0f * (r_depth);
  float tx = -(right + left) * r_width;
  float ty = -(top + bottom) * r_height;
  float tz = -(far + near) * r_depth;

  matrix_reset(resmtx);
  resmtx[0] = x;
  resmtx[5] = y;
  resmtx[10] = z;
  resmtx[12] = tx;
  resmtx[13] = ty;
  resmtx[14] = tz;
  resmtx[15] = 1.0f;
}

void matrix_set_lookat(float *resmtx,
		       float ex, float ey, float ez,
		       float cx, float cy, float cz,
		       float ux, float uy, float uz)
{
  float fx = cx - ex;
  float fy = cy - ey;
  float fz = cz - ez;

  // Normalize f
  float rlf = 1.0f / vector_length(fx, fy, fz);
  fx *= rlf;
  fy *= rlf;
  fz *= rlf;

  // compute s = f x up (x means "cross product")
  float sx = fy * uz - fz * uy;
  float sy = fz * ux - fx * uz;
  float sz = fx * uy - fy * ux;

  // and normalize s
  float rls = 1.0f / vector_length(sx, sy, sz);
  sx *= rls;
  sy *= rls;
  sz *= rls;

  // compute u = s x f
  ux = sy * fz - sz * fy;
  uy = sz * fx - sx * fz;
  uz = sx * fy - sy * fx;

  resmtx[0] = sx;
  resmtx[1] = ux;
  resmtx[2] = -fx;
  resmtx[3] = 0.0f;
  resmtx[4] = sy;
  resmtx[5] = uy;
  resmtx[6] = -fy;
  resmtx[7] = 0.0f;
  resmtx[8] = sz;
  resmtx[9] = uz;
  resmtx[10] = -fz;
  resmtx[11] = 0.0f;
  resmtx[12] = 0.0f;
  resmtx[13] = 0.0f;
  resmtx[14] = 0.0f;
  resmtx[15] = 1.0f;

  matrix_translate(resmtx, -ex, -ey, -ez);
}
