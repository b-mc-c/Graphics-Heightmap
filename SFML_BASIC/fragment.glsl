uniform sampler2D snowTexture;

uniform sampler2D waterTexture;

uniform sampler2D grassTexture;

varying float height;

void main()
{
	
	float waterStrenght = 0.0f;
	float grassStrenght = 0.0f;
	float snowStrenght = 0.0f;
	float scaleFactor = 36.4285f;
	float strenght = 0.0f;
	if ((height / scaleFactor ) > 0.75f)
	{
		snowStrenght = 1.0f;
	}
	else if((height /scaleFactor)  < 0.75f && (height /scaleFactor)  > 0.6f )
	{
		strenght = ((height / scaleFactor) - 0.6) / 0.15;
		snowStrenght = strenght;
		grassStrenght = 1 - strenght;
	}
	else if((height /scaleFactor)  > 0.1f )
	{
		grassStrenght = 1.0f;
	}
	else if((height /scaleFactor)  < 0.1f && (height /scaleFactor)  > 0.05f )
	{
		strenght = ((height / scaleFactor) - 0.05) / 0.04f;
		grassStrenght = strenght;
		waterStrenght = 1 - strenght;
	}

	else if((height / scaleFactor) >= 0.0f )
	{
		waterStrenght = 1.0f;
	}

    // lookup the pixel in the texture
    vec4 pixel1 =  texture2D(waterTexture, gl_TexCoord[0].xy);
	vec4 pixel2 =  texture2D(grassTexture, gl_TexCoord[0].xy);
	vec4 pixel3 =  texture2D(snowTexture, gl_TexCoord[0].xy);
    // multiply it by the color
    gl_FragColor =  (waterStrenght * pixel1) + (grassStrenght *pixel2) + (snowStrenght *pixel3) ;//* gl_Color ;
}

