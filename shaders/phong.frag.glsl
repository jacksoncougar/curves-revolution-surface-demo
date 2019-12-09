#version 330 core

in vec3 eye;
in vec3 fnormal;
in vec3 fposition;
in vec2 fuv;
in vec3 fcolor;

in mat3x3 view3x3;

out vec3 color;

uniform bool textured = false;
uniform sampler2D tnormal;
uniform sampler2D tao;
uniform sampler2D troughness;
uniform sampler2D talbedo;

struct material_constants {
    float ks;// specular reflection constant
    float kd;// diffuse reflection constant
    float ka;// ambient reflection constant
    float alpha;// shininess constant
};

struct light {
    mat4x4 transform;
    vec3 position;
    vec3 i_s;// specular intensity
    vec3 i_d;// diffuse intensity
    bool enabled;
};

uniform material_constants material = { 0.5, 0.2, 0.3, 256 };
uniform light light0 = { mat4x4(1), vec3(5, 10, 0.5), vec3(.7, .7, .7), vec3(.8, .8, .8), true };


uniform int p[512] = { 151,160,137,91,90,15,
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
151,160,137,91,90,15,
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };

float lerp(float t, float a, float b)
{
	return a + t * (b - a);
}

float fade(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10); 
}

float gradient(int hash, vec3 value)
{
	float x = value.x;
	float y = value.y;
	float z = value.z;

	switch (hash & 0xF)
	{
	case 0x0: return  x + y;
	case 0x1: return -x + y;
	case 0x2: return  x - y;
	case 0x3: return -x - y;
	case 0x4: return  x + z;
	case 0x5: return -x + z;
	case 0x6: return  x - z;
	case 0x7: return -x - z;
	case 0x8: return  y + z;
	case 0x9: return -y + z;
	case 0xA: return  y - z;
	case 0xB: return -y - z;
	case 0xC: return  y + x;
	case 0xD: return -y + z;
	case 0xE: return  y - x;
	case 0xF: return -y - z;
	default: return 0;
	}
}

float perlin(in vec3 value)
{
	float x = value.x;
	float y = value.y;
	float z = value.z;

	int X = int(floor(x)) & 255;
	int Y = int(floor(y)) & 255;
	int Z = int(floor(z)) & 255;

	x = x - floor(x);
	y = y - floor(y);
	z = z - floor(z);

	float u = fade(x);
	float v = fade(y);
	float w = fade(z);

	int A	= p[X	 ] + Y,
		AA	= p[A	 ] + Z,
		AB	= p[A + 1] + Z,
		B	= p[X + 1] + Y,
		BA	= p[B	 ] + Z,
		BB	= p[B + 1] + Z;
		
		return lerp(w,
		lerp(v, lerp(u, gradient(p[AA],		vec3( x,		y,	z )),
						gradient(p[BA],		vec3( x - 1,	y,	z ))),
				lerp(u, gradient(p[AB],		vec3( x,	 y - 1,	z )),
						gradient(p[BB],		vec3( x - 1, y - 1,	z )))),
		lerp(v, lerp(u, gradient(p[AA + 1], vec3( x,	 y,		z - 1 )),
						gradient(p[BA + 1], vec3( x - 1, y,		z - 1 ))),
				lerp(u, gradient(p[AB + 1], vec3( x,	 y - 1,	z - 1 )),
						gradient(p[BB + 1], vec3( x - 1, y - 1,	z - 1 )))));
						
}

void main() {
	
    vec3 world_normal = normalize( texture(tnormal, fuv).rgb + fnormal);
    if(!textured){
	world_normal = normalize(vec3(perlin(view3x3 * fposition * 100),perlin(view3x3 * fposition * 101),perlin(view3x3 * fposition * 102)) * 0.8 + fnormal);
	}
	vec3 world_position = fposition;
    vec3 camera_direction  = normalize(eye - world_position);

    vec3 diffuse = vec3(0);
    vec3 specular = vec3(0);
    vec3 i_a = vec3(0);// ambient;
    {
        vec3 light_position = light0.position;
        vec3 light_direction = normalize(light_position - world_position);
        vec3 reflection_direction = reflect(light_direction, world_normal);

        diffuse += material.kd
        * max(0, dot(light_direction, world_normal))
        * light0.i_d;

        if (length(diffuse) > 0)
        {
            specular += material.ks
            * pow(max(0, dot(reflection_direction, camera_direction)), texture(troughness, fuv).r)
            * light0.i_s;
        }

        i_a += light0.i_d * material.ka;
    }

	if(!textured)
	{
		color = vec3(perlin(fposition * 100),perlin(fposition * 101),perlin(fposition * 102)) * (i_a + diffuse + specular);
	}
    else
	{ 
		color = texture(talbedo, fuv).rgb * texture(tao, fuv).rgb * (i_a + diffuse + specular);
	}
}