
uniform sampler2D texture;
uniform sampler2D Ztexture;
uniform vec3 clouds_col;
uniform vec3 skycol;
uniform float scale;
uniform vec2 persp;
uniform vec2 rozm;

varying vec2 Texture_Coordinate;
varying float col;
varying float len;

float LinearizeDepth(float z)
{
  return (2.0 * persp.x) / (persp.y + persp.x - z * (persp.y - persp.x));
}

void main(void)
{ 	
  vec4 Texture_Color = texture2D(texture, Texture_Coordinate);
  if (Texture_Color.a < 0.01) discard;

  float pom = dot(Texture_Color.rgb, vec3(0.5));

  float zd = texture2D(Ztexture, vec2(gl_FragCoord.xy * rozm)).x;
  float Z_ColorPix = LinearizeDepth(zd);
  float Z_ColorTex = LinearizeDepth(gl_FragCoord.z);

  if (Z_ColorTex > Z_ColorPix) Texture_Color.a *= clamp((Z_ColorTex - Z_ColorPix) * scale, 0.0, 1.0);
    else Texture_Color.a *= clamp((Z_ColorPix - Z_ColorTex) * scale, 0.0, 1.0);

  gl_FragColor = vec4(Texture_Color.rgb * col * skycol + (clouds_col * pom), Texture_Color.a * len);
}
