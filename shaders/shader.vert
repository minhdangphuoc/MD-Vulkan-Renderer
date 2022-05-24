#version 450

////////////////// VERSION 0.1.0 ////////////////
/*
// layout(location = 0) out vec3 fragColor;

// vec2 positions[3] = vec2[](
//     vec2(0.0, -0.5),
//     vec2(0.5, 0.5),
//     vec2(-0.5, 0.5)
// );

// vec3 colors[3] = vec3[](
//     vec3(1.0, 0.0, 0.0),
//     vec3(0.0, 1.0, 0.0),
//     vec3(0.0, 0.0, 1.0)
// );



void main() {
    // gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    // fragColor = colors[gl_VertexIndex];
}
*/
////////////////// VERSION 0.1.0 ////////////////

layout( location = 0 ) in vec2 inPosition;
layout( location = 1 ) in vec3 inColor;

layout (push_constant) uniform Push 
{
    vec2 offset;
    vec3 color;
} push;

void main()
{
    gl_Position = vec4( inPosition + push.offset, 0.0, 1.0 );
    // fragColor = inColor;
    // fragColor = push.color;
}