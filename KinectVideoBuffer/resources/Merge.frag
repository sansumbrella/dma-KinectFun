uniform sampler2D color0;
uniform sampler2D color1;
uniform sampler2D color2;
uniform sampler2D color3;
uniform sampler2D color4;
uniform sampler2D color5;
uniform sampler2D color6;
uniform sampler2D color7;

uniform sampler2D depth0;
uniform sampler2D depth1;
uniform sampler2D depth2;
uniform sampler2D depth3;
uniform sampler2D depth4;
uniform sampler2D depth5;
uniform sampler2D depth6;
uniform sampler2D depth7;

void main()
{
	vec4 d = texture2D( depth1, gl_TexCoord[0].st );
	vec4 c = texture2D( color1, gl_TexCoord[0].st );

	vec4 nearestColor = texture2D( color0, gl_TexCoord[0].st);
	float nearestValue = texture2D( depth0, gl_TexCoord[0].st).r;
	
	if( texture2D( depth1, gl_TexCoord[0].st).r > nearestValue )
	{
		nearestColor = texture2D( color1, gl_TexCoord[0].st);
		nearestValue = texture2D( depth1, gl_TexCoord[0].st).r;
	}

	if( texture2D( depth2, gl_TexCoord[0].st).r > nearestValue )
	{
		nearestColor = texture2D( color2, gl_TexCoord[0].st);
		nearestValue = texture2D( depth2, gl_TexCoord[0].st).r;
	}

	if( texture2D( depth3, gl_TexCoord[0].st).r > nearestValue )
	{
		nearestColor = texture2D( color3, gl_TexCoord[0].st);
		nearestValue = texture2D( depth3, gl_TexCoord[0].st).r;
	}

	if( texture2D( depth4, gl_TexCoord[0].st).r > nearestValue )
	{
		nearestColor = texture2D( color4, gl_TexCoord[0].st);
		nearestValue = texture2D( depth4, gl_TexCoord[0].st).r;
	}

	if( texture2D( depth5, gl_TexCoord[0].st).r > nearestValue )
	{
		nearestColor = texture2D( color5, gl_TexCoord[0].st);
		nearestValue = texture2D( depth5, gl_TexCoord[0].st).r;
	}

	if( texture2D( depth6, gl_TexCoord[0].st).r > nearestValue )
	{
		nearestColor = texture2D( color6, gl_TexCoord[0].st);
		nearestValue = texture2D( depth6, gl_TexCoord[0].st).r;
	}

	if( texture2D( depth7, gl_TexCoord[0].st).r > nearestValue )
	{
		nearestColor = texture2D( color7, gl_TexCoord[0].st);
		nearestValue = texture2D( depth7, gl_TexCoord[0].st).r;
	}
	
	gl_FragColor = nearestColor / max(0.01,nearestValue);	
}

vec2 colorCoordFromDepthCoord( vec2 d )
{
	vec2 c = d;
	
	return c;
}