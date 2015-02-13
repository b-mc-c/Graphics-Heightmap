varying float height;

varying vec3 N;
varying vec3 v;

void main()
{ 
    v = vec3(gl_ModelViewMatrix * gl_Vertex);      
    N = normalize(gl_NormalMatrix * gl_Normal);

	height =  gl_Vertex[1];

	// transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    // forward the vertex color
    gl_FrontColor = gl_Color;//pass the colour along to the next pahse (fragment shader)

}
