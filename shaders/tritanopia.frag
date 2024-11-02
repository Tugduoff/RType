uniform sampler2D texture;

void main() {
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    // Tritanopia Conversion
    float r = color.r;
    float g = color.g * 0.7 + color.b * 0.3;
    float b = color.r * 0.0 + color.g * 0.3 + color.b * 0.7;

    gl_FragColor = vec4(r, g, b, color.a);
}
