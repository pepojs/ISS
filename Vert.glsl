#version 330

layout(location=0) in vec3 in_Position;
layout(location=1) in vec4 in_Colour;
layout(location=2) in vec2 in_TexturCoord;

out Vertex
{
    vec4 Colour;
    vec2 TexturCoord;

}Out;

uniform mat4 transformacja;
uniform mat4 rzutowanie;
uniform mat4 widok;

void main(void)
{
    if(rzutowanie == mat4(0.0f))
    {
        if(transformacja == mat4(0.0f))
        {
            if(widok == mat4(0.0f))
                gl_Position = vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
            else
                gl_Position = widok * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
        }
        else
        {
            if(widok == mat4(0.0f))
                gl_Position = transformacja * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
            else
                gl_Position = widok * transformacja * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
        }

    }
    else
    {
        if(transformacja == mat4(0.0f))
        {
            if(widok == mat4(0.0f))
                gl_Position = rzutowanie * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
            else
                gl_Position = rzutowanie * widok * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
        }
        else
        {
            if(widok == mat4(0.0f))
                gl_Position = rzutowanie * transformacja * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
            else
                gl_Position = rzutowanie * widok * transformacja * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
        }

    }


    Out.Colour = in_Colour;
    Out.TexturCoord = vec2(in_TexturCoord.x, 1 - in_TexturCoord.y);
}

