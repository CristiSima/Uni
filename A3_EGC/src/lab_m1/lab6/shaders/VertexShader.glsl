#version 330

// Input
// (student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 texture_coordinate;
layout(location = 3) in vec3 vertex_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float TimePassed;

// Output
// (student): Output values to fragment shader
out vec3 frag_color;
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture_coordinate;

void main()
{
    // (student): Send output to fragment shader
    frag_color = vertex_color;
    frag_color.x+=0.2f*sin(TimePassed);
    // frag_color = vec3(vertex_color[0]*sin(TimePassed), vertex_color[1]*sin(TimePassed), vertex_color[2]*sin(TimePassed));
    // (student): Compute gl_Position
    gl_Position =  Projection* View * Model*mat4(
       1, 0, 0, sin(TimePassed),
       0, 1, 0, 0,
       0, 0, 1, 0,
       0, 0, 0, 1
   ) * vec4(v_position, 1.0);

   // v_position[0]+=5*sin(TimePassed);
    frag_position=v_position;
    frag_normal=v_normal;
    frag_texture_coordinate=texture_coordinate;
}
