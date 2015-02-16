varying float height;


void main()
{ 
		height =  gl_Vertex[1];
     

		// forward the vertex color
        gl_FrontColor = gl_Color; //pass the colour along to the next phase (fragment shader)

		// transform the texture coordinates
		gl_TexCoord[0] = gl_MultiTexCoord0;
		
		// transform the vertex position
        gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}
