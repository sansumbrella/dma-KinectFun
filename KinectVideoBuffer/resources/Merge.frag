//uniform sampler2D color0;
//uniform sampler2D color1;
//uniform sampler2D color2;
//uniform sampler2D color3;
//uniform sampler2D color4;
//uniform sampler2D color5;
//uniform sampler2D color6;
//uniform sampler2D color7;
//
//uniform sampler2D depth0;
//uniform sampler2D depth1;
//uniform sampler2D depth2;
//uniform sampler2D depth3;
//uniform sampler2D depth4;
//uniform sampler2D depth5;
//uniform sampler2D depth6;
//uniform sampler2D depth7;

uniform sampler2D depths[8];
uniform sampler2D colors[8];

void main()
{
	vec2 tc = gl_TexCoord[0].st;
	vec4 d = texture2D( depths[0], tc );
	vec4 c = texture2D( colors[0], tc );
	
	vec4 nearestColor = texture2D( colors[0], tc );
	float nearestValue = texture2D( depths[0], tc ).r;
	
	for ( int i=1; i<8; i++ )
	{
		if( texture2D( depths[i], tc).r > nearestValue )
		{
			nearestColor = texture2D( colors[i], tc );
			nearestValue = texture2D( depths[i], tc ).r;
		}
	}
	
	gl_FragColor = nearestColor / max(0.01,nearestValue);	
}

vec2 colorCoordFromDepthCoord( vec2 d )
{
	vec2 c = d;	
	return c;
}