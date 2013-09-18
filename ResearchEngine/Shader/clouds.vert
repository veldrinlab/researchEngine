
#version 120

uniform vec3 sunpos;
uniform float scale;
uniform float dist_sun_cloud;
uniform float y_cloud_shadow;
uniform float top_cloud_light;
uniform float alpdist;

varying vec2 Texture_Coordinate;
varying float col;
varying float len;

void main(void)
{
  vec4 v = gl_Vertex;

  vec4 ModelView_Vert = gl_ModelViewMatrix * v;
  gl_ClipVertex = ModelView_Vert;

  Texture_Coordinate = gl_MultiTexCoord0.xy;

  vec3 v2 = normalize(v.xyz);
  col = max(dot(normalize(sunpos), v2), dist_sun_cloud);
  col *= Texture_Coordinate.y * y_cloud_shadow + top_cloud_light;
  col = clamp(col, 0.0, 1.0) * 2.0;

  Texture_Coordinate.y = step(0.0, Texture_Coordinate.y);

  gl_Position = gl_ModelViewProjectionMatrix * v;

  len = clamp(length(gl_Position) * alpdist * 2.0 - 2.0, 0.0, 1.0);
}
