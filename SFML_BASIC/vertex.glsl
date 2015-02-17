varying float height;
varying vec3 N;
varying vec3 v;  

void main()
{ 
		height =  gl_Vertex.y;
		v = vec3(gl_ModelViewMatrix * gl_Vertex);       
		N = normalize(gl_NormalMatrix * gl_Normal);


		// forward the vertex color
        gl_FrontColor = gl_Color; //pass the colour along to the next phase (fragment shader)

		// transform the texture coordinates
		gl_TexCoord[0] = gl_MultiTexCoord0;
		
		// transform the vertex position
        gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}
