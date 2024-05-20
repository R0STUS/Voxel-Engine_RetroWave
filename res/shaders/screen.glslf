in vec2 v_coord;
out vec4 f_color;

uniform sampler2D u_texture0;
uniform float u_timer;
uniform float radius;
uniform vec2 dir;
uniform ivec2 u_screenSize;
uniform float u_stripe;

float random(vec2 st){
    return fract(sin(dot(st.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec4 applyVignette(vec4 color)
{
    vec2 position = (gl_FragCoord.xy / u_screenSize) - vec2(0.5);
    float dist = length(position);

    float radius = 0.88;
    float softness = 0.375;
    float vignette = smoothstep(radius, radius - softness, dist);

    color.rgb = color.rgb - (1.0 - vignette);

    return color;
}

void main() {
    float totalWeight = 0.0;

    vec2 uv = v_coord;

    vec3 color;

    float offset = sin(u_timer * 0.000005) * 0.1;

    vec4 textureColor = texture(u_texture0, v_coord);

    vec2 glitchedTexCoord = uv + vec2(offset, 0.0);

    float stripeMask = smoothstep(uv.y - 0.1 / 2.0, uv.y + 0.1 / 2.0, fract(uv.y * 500));

    vec2 centerCoord = vec2(0.5, 0.5);

    vec4 centerColor = texture(u_texture0, centerCoord);

    float centerBrightness = dot(centerColor.rgb, vec3(0.2126, 0.7152, 0.0722));

    float glitch = random(vec2(1.0, u_timer)) * 0.5 + 0.5;
    vec2 uvRedGlitch = mix(uv + 0.0005, glitchedTexCoord, stripeMask);
    vec2 uvGreenGlitch = mix(uv, glitchedTexCoord, stripeMask);
    vec2 uvBlueGlitch = mix(uv - 0.0005, glitchedTexCoord, stripeMask);

    vec2 redUV = uvRedGlitch + vec2(0.0005, 0.0);
    vec4 redColor = texture(u_texture0, redUV);

    vec2 greenUV = uvGreenGlitch - vec2(0.0, 0.002);
    vec4 greenColor = texture(u_texture0, greenUV);

    vec2 blueUV = uvBlueGlitch - vec2(-0.0005, 0.0);
    vec4 blueColor = texture(u_texture0, blueUV);

    f_color = vec4(redColor.r, greenColor.g, blueColor.b, 1.0);
    f_color = applyVignette(f_color);

    float grayscale = (f_color.r + f_color.g + f_color.b) / 3.0;

    f_color = vec4(grayscale + (f_color.r * 0.9), grayscale + (f_color.g * 0.9), grayscale + (f_color.b * 0.9), f_color.a);
    f_color = vec4(f_color.rgb * 0.65, f_color.a);
}