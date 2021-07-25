

precision mediump float;

varying vec2 fragmentPosition;
varying vec4 fragmentColor;
varying vec2 fragmentUV;

uniform sampler2D mySampler;

void main() {    
    gl_FragColor = fragmentColor* texture2D(mySampler, fragmentUV);
}