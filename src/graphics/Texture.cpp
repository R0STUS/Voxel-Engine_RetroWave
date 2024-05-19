#include "Texture.h"
#include <GL/glew.h>
#include <stdexcept>
#include <memory>

#include "ImageData.h"
#include "gl_util.h"

Texture::Texture(uint id, uint width, uint height) 
    : id(id), width(width), height(height) {
}

Texture::Texture(ubyte* data, uint width, uint height, ImageFormat imageFormat) 
    : width(width), height(height) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLenum format = gl::to_gl_format(imageFormat);
    glTexImage2D(
        GL_TEXTURE_2D, 0, format, width, height, 0,
        format, GL_UNSIGNED_BYTE, (GLvoid *) data
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

void Texture::bind(){
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::reload(ubyte* data){
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *) data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

ImageData* Texture::readData() {
    std::unique_ptr<ubyte[]> data (new ubyte[width * height * 4]);
    glBindTexture(GL_TEXTURE_2D, id);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.get());
    glBindTexture(GL_TEXTURE_2D, 0);
    return new ImageData(ImageFormat::rgba8888, width, height, data.release());
}

void Texture::setNearestFilter() {
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture* Texture::from(const ImageData* image) {
    uint width = image->getWidth();
    uint height = image->getHeight();
    const void* data = image->getData();
    return new Texture((ubyte*)data, width, height, image->getFormat());
}

uint Texture::getWidth() const {
    return width;
}

uint Texture::getHeight() const {
    return height;
}

uint Texture::getId() const {
    return id;
}
