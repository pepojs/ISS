#version 330

in Vertex
{
    vec4 Colour;
    vec2 TexturCoord;
}In;

out vec4 out_Colour;

uniform sampler2D Tekstura0;
uniform sampler2D Tekstura1;

void main(void)
{
    if(In.Colour == vec4(0.0f, 0.0f, 0.0f, 1.0f))
    {
        out_Colour = mix(texture(Tekstura0, In.TexturCoord),texture(Tekstura1, In.TexturCoord),0.2);
    }
    else
    {
        out_Colour = texture(Tekstura1, In.TexturCoord) * In.Colour;

        if(out_Colour == vec4(0.0f, 0.0f, 0.0f, 1.0f))
            out_Colour = In.Colour;
    }


}

