precision mediump float;       	// Set the default precision to medium. We don't need as high of a
								// precision in the fragment shader.

uniform samplerCube skybox;

varying vec3 v_Position;
varying vec3 v_TexCoordinate;

void main()
{
	// Multiply the color by the diffuse illumination level and texture value to get final output color.
    gl_FragColor = textureCube(skybox, v_TexCoordinate);
  }

