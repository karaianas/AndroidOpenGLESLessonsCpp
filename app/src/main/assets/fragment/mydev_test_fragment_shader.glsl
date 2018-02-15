precision mediump float;       	// Set the default precision to medium. We don't need as high of a
								// precision in the fragment shader.
uniform vec3 u_LightPos;       	// The position of the light in eye space.
//uniform sampler2D u_Texture;    // The input texture.

varying vec3 v_Position;
varying vec3 v_Normal;
varying vec3 v_Coeff;
//varying vec2 v_TexCoordinate;


void main()
{
	// Will be used for attenuation.
    float distance = length(u_LightPos - v_Position);

	// Get a lighting direction vector from the light to the vertex.
    vec3 lightVector = normalize(u_LightPos - v_Position);

	// Calculate the dot product of the light vector and vertex normal. If the normal and light vector are
	// pointing in the same direction then it will get max illumination.
    float diffuse = max(dot(v_Normal, lightVector), 0.0);

	// Add attenuation.
    diffuse = diffuse * (1.0 / distance);

    // Add ambient lighting
    diffuse = diffuse + 0.2;

	// Multiply the color by the diffuse illumination level and texture value to get final output color.
    //gl_FragColor = (diffuse * texture2D(u_Texture, v_TexCoordinate));
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    gl_FragColor = vec4(v_Coeff, 1.0);
  }

