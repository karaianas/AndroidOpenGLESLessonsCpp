uniform mat4 u_MVPMatrix;
uniform mat4 u_MVMatrix;
uniform vec3 u_Light[4];

attribute vec4 a_Position;
attribute vec3 a_Normal;
attribute vec4 a_Coeff;

varying vec3 v_Position;
varying vec3 v_Normal;
varying vec3 v_Coeff;

void main()
{
	// Transform the vertex into eye space.
	v_Position = vec3(u_MVMatrix * a_Position);

	// Pass through the texture coordinate.
	//v_TexCoordinate = a_TexCoordinate;

	// Transform the normal's orientation into eye space.
    v_Normal = vec3(u_MVMatrix * vec4(a_Normal, 0.0));

    // Calculate dot product
    //v_Coeff = vec3(a_Coeff[1], a_Coeff[2], a_Coeff[3]);
    float r = a_Coeff[0] * u_Light[0][0] + a_Coeff[1] * u_Light[1][0] + a_Coeff[2] * u_Light[2][0] + a_Coeff[3] * u_Light[3][0];
    float g = a_Coeff[0] * u_Light[0][1] + a_Coeff[1] * u_Light[1][1] + a_Coeff[2] * u_Light[2][1] + a_Coeff[3] * u_Light[3][1];
    float b = a_Coeff[0] * u_Light[0][2] + a_Coeff[1] * u_Light[1][2] + a_Coeff[2] * u_Light[2][2] + a_Coeff[3] * u_Light[3][2];
    v_Coeff = vec3(r, g, b);

	// gl_Position is a special variable used to store the final position.
	// Multiply the vertex by the matrix to get the final point in normalized screen coordinates.
	gl_Position = u_MVPMatrix * a_Position;
}