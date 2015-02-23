uniform sampler2D snowTexture;

uniform sampler2D waterTexture;

uniform sampler2D grassTexture;

varying float height;
varying vec3 N;
varying vec3 v;    

void main()
{
	float waterStrenght = 0.0;// percentage of water texture to be drawn
	float grassStrenght = 0.0;// percentage of grass texture to be drawn
	float snowStrenght = 0.0;// percentage of snow texture to be drawn
	float scaleFactor = 36.4285;//scale relating to the height of the map ie 255 / 7
	float strenght = 0.0;
	vec4 finalColor = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 Ispec = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 Idiff;
	vec4 diffuse;
	vec3 position;
	vec3 L = normalize(gl_LightSource[0].position.xyz - v);   
    vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)  
    vec3 R = normalize(-reflect(L,N));

	// lookup the pixel in the texture
    vec4 pixel1 =  texture2D(waterTexture, gl_TexCoord[0].xy);
	vec4 pixel2 =  texture2D(grassTexture, gl_TexCoord[0].xy);
	vec4 pixel3 =  texture2D(snowTexture, gl_TexCoord[0].xy);
	
	if ((height / scaleFactor ) > 0.75)
	{
		snowStrenght = 1.0;
	}
	else if((height /scaleFactor)  < 0.75 && (height /scaleFactor)  > 0.6 )
	{
		strenght = ((height / scaleFactor) - 0.6) / 0.15;
		snowStrenght = strenght;
		grassStrenght = 1.0 - strenght;
	}
	else if((height /scaleFactor)  > 0.1 )
	{
		grassStrenght = 1.0;
	}
	else if((height /scaleFactor)  < 0.1 && (height /scaleFactor)  > 0.05 )
	{
		strenght = ((height / scaleFactor) - 0.05) / 0.04;
		grassStrenght = strenght;
		waterStrenght = 1.0 - strenght;
	}
	else if((height / scaleFactor) >= 0.0 )
	{
		//specular light
	    Ispec = gl_FrontLightProduct[0].specular * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
		Ispec = clamp(Ispec, 0.0, 1.0);
		waterStrenght = 1.0;
	}
	//old code drove me mad :/
   //calculate Diffuse Term:
   //Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);
   //Idiff = clamp(Idiff, 0.0, 1.0);

   //this diffuse code works
    diffuse = vec4(0.5, 0.5, 0.5, 255);
	position = vec3(50, 30, 50);

	vec3 d = normalize(position.xyz - v); 
	vec4 finalDiffuse = diffuse * max(dot(N,L), 0.0);
    finalDiffuse = clamp(finalDiffuse, 0.0, 1.0);

    //write Total Color
    finalColor = finalDiffuse + Ispec;

    // multiply it by the color
   // gl_FragColor = ((waterStrenght * pixel1) + (grassStrenght *pixel2) + (snowStrenght *pixel3));// * gl_Color ;// finalColor ;//;
    gl_FragColor = ((waterStrenght * pixel1) + (grassStrenght *pixel2) + (snowStrenght *pixel3)) * finalColor;
}

