uniform sampler2D snowTexture;

uniform sampler2D waterTexture;

uniform sampler2D grassTexture;

void main()
{
    // lookup the pixel in the texture
    vec4 pixel1 = texture2D(waterTexture, gl_TexCoord[0].xy);
	vec4 pixel2 = texture2D(grassTexture, gl_TexCoord[0].xy) ;
	vec4 pixel3 = texture2D(snowTexture, gl_TexCoord[0].xy);
    // multiply it by the color
    gl_FragColor =  pixel3 * pixel2 * pixel3 ;//* gl_Color ;
}