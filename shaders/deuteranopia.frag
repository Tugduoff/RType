uniform sampler2D texture;

void main() {
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    // Deuteranopia Conversion
    float r = color.r * 0.625 + color.g * 0.375;
    float g = color.r * 0.7 + color.g * 0.3;
    float b = color.b;

    gl_FragColor = vec4(r, g, b, color.a);
}
