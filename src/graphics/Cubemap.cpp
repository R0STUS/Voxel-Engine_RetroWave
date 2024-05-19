#include "Cubemap.h"
#include "gl_util.h"

#include <GL/glew.h>

Cubemap::Cubemap(uint width, uint height, ImageFormat imageFormat) 
  : Texture(0, width, height) 
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    uint format = gl::to_gl_format(imageFormat);
    for (uint face = 0; face < 6; face++) {
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 
            0, 
            format, 
            width, 
            height, 
            0, 
            format, 
            GL_UNSIGNED_BYTE, 
            NULL
        );
    }
}

void Cubemap::bind(){
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

void Cubemap::unbind() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
