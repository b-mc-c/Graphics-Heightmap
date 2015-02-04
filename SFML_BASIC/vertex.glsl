void main()
{

	//gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;


	// transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    // forward the vertex color
    gl_FrontColor = gl_Color;//pass the colour along to the next pahse (fragment shader)
}
