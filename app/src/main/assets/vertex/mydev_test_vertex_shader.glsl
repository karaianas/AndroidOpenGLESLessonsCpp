uniform mat4 u_MVPMatrix;
uniform mat4 u_MVMatrix;
uniform vec3 u_Light[16];

attribute vec4 a_Position;
attribute vec3 a_Normal;
attribute vec4 a_Coeff;
attribute vec4 a_Coeff2;
attribute vec4 a_Coeff3;
attribute vec4 a_Coeff4;

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

    r += a_Coeff2[0] * u_Light[4][0] + a_Coeff2[1] * u_Light[5][0] + a_Coeff2[2] * u_Light[6][0] + a_Coeff2[3] * u_Light[7][0];
    g += a_Coeff2[0] * u_Light[4][1] + a_Coeff2[1] * u_Light[5][1] + a_Coeff2[2] * u_Light[6][1] + a_Coeff2[3] * u_Light[7][1];
    b += a_Coeff2[0] * u_Light[4][2] + a_Coeff2[1] * u_Light[5][2] + a_Coeff2[2] * u_Light[6][2] + a_Coeff2[3] * u_Light[7][2];

    r += a_Coeff3[0] * u_Light[8][0] + a_Coeff3[1] * u_Light[9][0] + a_Coeff3[2] * u_Light[10][0] + a_Coeff3[3] * u_Light[11][0];
    g += a_Coeff3[0] * u_Light[8][1] + a_Coeff3[1] * u_Light[9][1] + a_Coeff3[2] * u_Light[10][1] + a_Coeff3[3] * u_Light[11][1];
    b += a_Coeff3[0] * u_Light[8][2] + a_Coeff3[1] * u_Light[9][2] + a_Coeff3[2] * u_Light[10][2] + a_Coeff3[3] * u_Light[11][2];


    r += a_Coeff4[0] * u_Light[12][0] + a_Coeff4[1] * u_Light[13][0] + a_Coeff4[2] * u_Light[14][0] + a_Coeff4[3] * u_Light[15][0];
    g += a_Coeff4[0] * u_Light[12][1] + a_Coeff4[1] * u_Light[13][1] + a_Coeff4[2] * u_Light[14][1] + a_Coeff4[3] * u_Light[15][1];
    b += a_Coeff4[0] * u_Light[12][2] + a_Coeff4[1] * u_Light[13][2] + a_Coeff4[2] * u_Light[14][2] + a_Coeff4[3] * u_Light[15][2];

    v_Coeff = vec3(r, g, b);

	// gl_Position is a special variable used to store the final position.
	// Multiply the vertex by the matrix to get the final point in normalized screen coordinates.
	gl_Position = u_MVPMatrix * a_Position;
}