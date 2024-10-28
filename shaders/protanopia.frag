uniform sampler2D texture;

void main() {
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    // Protanopia Conversion
    float r = color.r * 0.56667 + color.g * 0.43333;
    float g = color.r * 0.55833 + color.g * 0.44167;
    float b = color.b * 0.24167;

    gl_FragColor = vec4(r, g, b, color.a);
}
