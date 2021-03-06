precision mediump float;       	// Set the default precision to medium. We don't need as high of a
								// precision in the fragment shader.

varying vec3 v_Position;
varying vec3 v_Normal;
varying vec3 v_Coeff;

void main()
{
	// Multiply the color by the diffuse illumination level and texture value to get final output color.
    //gl_FragColor = (diffuse * texture2D(u_Texture, v_TexCoordinate));
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    gl_FragColor = vec4(v_Coeff, 1.0);//v_Coeff
  }

