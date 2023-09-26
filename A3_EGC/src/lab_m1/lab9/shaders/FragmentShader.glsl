#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
// (student): Declare various other uniforms
uniform float mix_factor;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // (student): Calculate the out_color using the texture2D() function.
    out_color = vec4(1);
    // out_color = texture2D(texture_1, texcoord);
    out_color=mix(
        texture2D(texture_1, texcoord),
        texture2D(texture_2, texcoord),
        mix_factor
    );
    if(out_color[3]==0)
        discard;
    out_color*=(out_color[3]);
    // out_color[3]=1-out_color[3];

}
