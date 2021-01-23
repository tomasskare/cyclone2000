#include <android/log.h>
#include <string.h>

#include <GLES2/gl2.h>

#include "debug.h"
#include "object.h"
#include "native_main.h"
#include "shaders.h"

/* Normal shaders for flat screen. */

static const GLchar *shaders_v_code[] = {
  /* Gouraud vertex shader without lighting. */
  "precision highp float;\n"
  "attribute vec4 a_Position;\n"
  "attribute vec4 a_Colour;\n"
  "uniform mat4 u_ModelMatrix;\n"
  "uniform mat4 u_ViewMatrix;\n"
  "uniform mat4 u_ProjMatrix;\n"
  "varying vec4 v_Colour;\n"
  "void main() {\n"
  "  mat4 mvp = (u_ProjMatrix * u_ViewMatrix) * u_ModelMatrix;\n"
  "  gl_Position = mvp * a_Position;\n"
  "  v_Colour.rgb = a_Colour.rgb;\n"
  "}\n",

  /* Single-colour line vertex shader without lighting. */
  "precision highp float;\n"
  "attribute vec4 a_Position;\n"
  "uniform mat4 u_ModelMatrix;\n"
  "uniform mat4 u_ViewMatrix;\n"
  "uniform mat4 u_ProjMatrix;\n"
  "void main() {\n"
  "  mat4 mvp = (u_ProjMatrix * u_ViewMatrix) * u_ModelMatrix;\n"
  "  gl_Position = mvp * a_Position;\n"
  "}\n",

  /* Single-colour point vertex shader without lighting. */
  "precision highp float;\n"
  "attribute vec4 a_Position;\n"
  "uniform float u_PointSize;\n"
  "uniform mat4 u_ModelMatrix;\n"
  "uniform mat4 u_ViewMatrix;\n"
  "uniform mat4 u_ProjMatrix;\n"
  "void main() {\n"
  "  mat4 mvp = (u_ProjMatrix * u_ViewMatrix) * u_ModelMatrix;\n"
  "  gl_Position = mvp * a_Position;\n"
  "  gl_PointSize = u_PointSize;\n"
  "}\n",

  /* Texture map vertex shader without lighting. */
  "precision highp float;\n"
  "attribute vec4 a_Position;\n"
  "attribute vec2 a_TexCoord;\n"
  "uniform mat4 u_ModelMatrix;\n"
  "uniform mat4 u_ViewMatrix;\n"
  "uniform mat4 u_ProjMatrix;\n"
  "varying vec2 v_TexCoord;\n"
  "void main() {\n"
  "  mat4 mvp = (u_ProjMatrix * u_ViewMatrix) * u_ModelMatrix;\n"
  "  gl_Position = mvp * a_Position;\n"
  "  v_TexCoord = a_TexCoord;\n"
  "}\n",

  /* Texture map vertex shader with barrel distortion. */
  "precision highp float;\n"
  "attribute vec4 a_Position;\n"
  "attribute vec2 a_TexCoord;\n"
  "uniform mat4 u_ModelMatrix;\n"
  "uniform mat4 u_ViewMatrix;\n"
  "uniform mat4 u_ProjMatrix;\n"
  "varying vec2 v_TexCoord;\n"
  "void main() {\n"
  "  mat4 mvp = (u_ProjMatrix * u_ViewMatrix) * u_ModelMatrix;\n"
  "  gl_Position = mvp * a_Position;\n"
  "  v_TexCoord = a_TexCoord;\n"
  "}\n",

  /* Vertex shader for fragment lighting, one light source. */
  "precision highp float;\n"
  "attribute vec4 a_Position;\n"
  "attribute vec4 a_Colour;\n"
  "attribute vec3 a_Normal;\n"
  "uniform mat4 u_ModelMatrix;\n"
  "uniform mat4 u_ViewMatrix;\n"
  "uniform mat4 u_ProjMatrix;\n"
  "uniform mat3 u_NormalMatrix;\n"
  "varying vec4 v_Colour;\n"
  "varying vec3 v_Normal;\n"
  "void main() {\n"
  "  mat4 mvp = (u_ProjMatrix * u_ViewMatrix) * u_ModelMatrix;\n"
  "  gl_Position = mvp * a_Position;\n"
  "  v_Colour.rgb = a_Colour.rgb;\n"
  "  v_Normal = u_NormalMatrix * a_Normal;\n"
  "}\n",

  /* Vertex shader for fragment lighting, max 10 point lights. */
  "precision mediump float;\n"
  "attribute vec4 a_Position;\n"
  "attribute vec4 a_Colour;\n"
  "attribute vec3 a_Normal;\n"
  "uniform mat4 u_ModelMatrix;\n"
  "uniform mat4 u_ViewMatrix;\n"
  "uniform mat4 u_ProjMatrix;\n"
  "uniform mat3 u_NormalMatrix;\n"
  "uniform mat4 u_LightMtx[10];\n"
  "varying vec4 v_Colour;\n"
  "varying vec3 v_Normal;\n"
  "varying vec3 v_LightDir[10];\n"
  "varying vec3 v_Eye;\n"
  "void main() {\n"
  "  mat4 mv = u_ViewMatrix * u_ModelMatrix;\n"
  "  mat4 mvp = (u_ProjMatrix * u_ViewMatrix) * u_ModelMatrix;\n"
  "  gl_Position = mvp * a_Position;\n"
  "  v_Colour.rgb = a_Colour.rgb;\n"
  "  v_Normal = normalize(u_NormalMatrix * a_Normal);\n"
  "  vec4 pos = mv * a_Position;\n"
  "  v_Eye = normalize(vec3(-pos));\n"
  "  for(int i = 0 ; i < 10 ; i++) {\n"
  "    mat4 lmv = u_ViewMatrix * u_LightMtx[i];\n"
  "    vec4 lpos = lmv * vec4(0.0, 0.0, 0.0, 1.0);\n"
  "    v_LightDir[i] = vec3(lpos - pos);\n"
  "  }\n"
  "}\n",

  /* Single-colour fancy point vertex shader without lighting. */
  "precision highp float;\n"
  "attribute vec4 a_Position;\n"
  "uniform float u_PointSize;\n"
  "uniform mat4 u_ModelMatrix;\n"
  "uniform mat4 u_ViewMatrix;\n"
  "uniform mat4 u_ProjMatrix;\n"
  "void main() {\n"
  "  mat4 mv = u_ViewMatrix * u_ModelMatrix;\n"
  "  mat4 mvp = (u_ProjMatrix * u_ViewMatrix) * u_ModelMatrix;\n"
  "  gl_Position = mvp * a_Position;\n"
  "  vec4 pos = mv * a_Position;\n"
  "  float dist = clamp(pos.z * 0.05, -1.0, 1.0) + 1.0;\n"
  "  gl_PointSize = u_PointSize * dist;\n"
  "}\n",
};

static const GLchar *shaders_f_code[] = {
  /* Gouraud fragment shader without lighting. */
  "precision mediump float;\n"
  "uniform float u_Alpha;\n"
  "varying vec4 v_Colour;\n"
  "void main() {\n"
  "  gl_FragColor = vec4(v_Colour.rgb, u_Alpha);\n"
  "}\n",

  /* Single-colour line fragment shader without lighting. */
  "precision mediump float;\n"
  "uniform float u_Alpha;\n"
  "uniform vec4 u_Colour;\n"
  "void main() {\n"
  "  gl_FragColor = vec4(u_Colour.rgb, u_Alpha);\n"
  "}\n",

  /* Single-colour point fragment shader without lighting. */
  "precision mediump float;\n"
  "uniform float u_Alpha;\n"
  "uniform vec4 u_Colour;\n"
  "void main() {\n"
  "  gl_FragColor = vec4(u_Colour.rgb, u_Alpha);\n"
  "}\n",

  /* Texture map fragment shader without lighting. */
  "precision mediump float;\n"
  "uniform sampler2D u_Texture;\n"
  "uniform float u_Alpha;\n"
  "varying vec2 v_TexCoord;\n"
  "void main() {\n"
  "  vec4 texColor = texture2D(u_Texture, v_TexCoord);\n"
  "  gl_FragColor = vec4(texColor.rgb, texColor.a * u_Alpha);\n"
  "}\n",


  /* Texture map fragment shader with barrel distortion. */
  "precision mediump float;\n"
  "uniform sampler2D u_Texture;\n"
  "uniform float u_Alpha;\n"
  "varying vec2 v_TexCoord;\n"
  "void main() {\n"
  "  vec2 texc = 2.0 * (v_TexCoord - 0.5);\n"
  "  float theta = atan(texc.y, texc.x);\n"
  "  float radius = length(texc);\n"
  "  float b = 0.05 * radius * radius;\n"
  "  float c = 0.10 * radius;\n"
  "  float d = 0.85;\n"
  "  radius = radius * (b + c + d);\n"
  "  float alpha = 1.0;\n"
  "  texc.x = radius * cos(theta);\n"
  "  texc.y = radius * sin(theta);\n"
  "  if(abs(texc.x) > 1.0 || abs(texc.y) > 1.0)\n"
  "    discard;\n"
  "  if(abs(texc.x) > 0.9)\n"
  "    alpha *= 1.0 - 10.0 * (abs(texc.x) - 0.9);\n"
  "  if(abs(texc.y) > 0.9)\n"
  "    alpha *= 1.0 - 10.0 * (abs(texc.y) - 0.9);\n"
  "  alpha = max(alpha, 0.0);\n"
  "  texc = (texc + 1.0) / 2.0;\n"
  "  vec4 texColor = texture2D(u_Texture, texc);\n"
  "  gl_FragColor = vec4(texColor.rgb, alpha);\n"
  "}\n",

  /* Fragment shader for fragment lighting, one light source. */
  "precision mediump float;\n"
  "struct DirectionalLight {\n"
  "  vec3 direction;\n"
  "  vec3 halfplane;\n"
  "  vec4 ambient;\n"
  "  vec4 diffuse;\n"
  "  vec4 specular;\n"
  "};\n"
  "uniform DirectionalLight u_Light1;\n"
  "uniform float u_Alpha;\n"
  "uniform vec4 u_SpecularColour;\n"
  "uniform float u_SpecularShininess;\n"
  "varying vec4 v_Colour;\n"
  "varying vec3 v_Normal;\n"
  "void main() {\n"
  "  float normDotLightDir = max(0.0, dot(v_Normal, normalize(u_Light1.direction)));\n"
  "  float normDotLightHP = max(0.0, dot(v_Normal, normalize(u_Light1.halfplane)));\n"
  "  vec4 ambientLight = u_Light1.ambient * v_Colour;\n"
  "  vec4 diffuseLight = normDotLightDir * u_Light1.diffuse * v_Colour;\n"
  "  vec4 specularLight = vec4(0.0);\n"
  "  specularLight = pow(normDotLightHP, u_SpecularShininess) * u_Light1.specular * u_SpecularColour;\n"
  "  vec4 colour = ambientLight + diffuseLight + specularLight;\n"
  "  gl_FragColor = vec4(colour.rgb, u_Alpha);\n"
  "}\n",

  /* Fragment shader for fragment lighting, max 10 point light source. */
  "precision mediump float;\n"
  "struct DirectionalLight {\n"
  "  vec3 direction;\n"
  "  vec3 halfplane;\n"
  "  vec4 ambient;\n"
  "  vec4 diffuse;\n"
  "  vec4 specular;\n"
  "};\n"
  "uniform DirectionalLight u_Light1;\n"
  "uniform float u_Alpha;\n"
  "uniform vec4 u_SpecularColour;\n"
  "uniform float u_SpecularShininess;\n"
  "uniform float u_LightIntensity[10];\n"
  "uniform vec4 u_LightDiffuse[10];\n"
  "uniform vec4 u_LightSpecular[10];\n"
  "varying vec4 v_Colour;\n"
  "varying vec3 v_Normal;\n"
  "varying vec3 v_LightDir[10];\n"
  "varying vec3 v_Eye;\n"
  "void main() {\n"
  "  float normDotLightDir = max(0.0, dot(v_Normal, u_Light1.direction));\n"
  "  float normDotLightHP = max(0.0, dot(v_Normal, u_Light1.halfplane));\n"
  "  vec4 ambientLight = u_Light1.ambient * v_Colour;\n"
  "  vec4 diffuseLight = normDotLightDir * u_Light1.diffuse * v_Colour;\n"
  "  vec4 specularLight = vec4(0.0);\n"
  "  specularLight = pow(normDotLightHP, u_SpecularShininess) * u_Light1.specular * u_SpecularColour;\n"
  "  vec4 colour = ambientLight + diffuseLight + specularLight;\n"
  "  for(int i = 0 ; i < 10 ; i++) {\n"
  "    float dist = length(v_LightDir[i]);\n"
  "    vec3 dir = v_LightDir[i] / dist;\n"
  "    if(dist < 0.7) {\n"
  "      float intensity = (-1.429 * dist + 1.0) * u_LightIntensity[i] * max(dot(v_Normal, dir), 0.0);\n"
  "      vec4 spec = vec4(0.0);\n"
  "      vec3 h = normalize(dir + v_Eye);\n"
  "      float intSpec = max(dot(h, v_Normal), 0.0);\n"
  "      spec = max(0.0, (-2.5 * dist + 1.0)) * u_LightSpecular[i] * u_SpecularColour * pow(intSpec, u_SpecularShininess);\n"
  "      colour += intensity * u_LightDiffuse[i] + spec;\n"
  "    }\n"
  "  }\n"
  "  gl_FragColor = vec4(colour.rgb, u_Alpha);\n"
  "}\n",

  /* Single-colour fancy point fragment shader without lighting. */
  "precision mediump float;\n"
  "uniform float u_Alpha;\n"
  "uniform vec4 u_Colour;\n"
  "void main() {\n"
  "  float dist = length(2.0 * gl_PointCoord - 1.0);\n"
  "  float myAlpha = 1.0 - pow(dist, 4.0);\n"
  "  float spec = pow(1.0 - dist, 4.0);\n"
  "  vec4 colour = vec4(1.0) * spec + u_Colour;\n"
  "  gl_FragColor = vec4(colour.rgb, myAlpha * u_Alpha);\n"
  "}\n",

};

static GLuint shaders_programs[SHADERS_NUM];

GLuint shaders_get_program(int type)
{
  return shaders_programs[type];
}

void shaders_init(void)
{
  int i, vshader, fshader;

  for(i = 0 ; i < SHADERS_NUM ; i++) {
    const GLchar **vcode = &shaders_v_code[i];
    const GLchar **fcode = &shaders_f_code[i];
    const GLint vlen = strlen(*vcode);
    const GLint flen = strlen(*fcode);
    const GLint *vlenp = &vlen;
    const GLint *flenp = &flen;

    vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, vcode, vlenp);
    glCompileShader(vshader);

    fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, fcode, flenp);
    glCompileShader(fshader);

    shaders_programs[i] = glCreateProgram();
    glAttachShader(shaders_programs[i], vshader);
    glAttachShader(shaders_programs[i], fshader);
    glLinkProgram(shaders_programs[i]);
  }
}
