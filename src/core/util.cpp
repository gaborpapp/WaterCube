#pragma once

#include <glm/gtx/string_cast.hpp>

#include "util.h"

using namespace core;

void util::log(char* format, ...) {
    va_list vl;

    //  format is the last argument specified; all
    //  others must be accessed using the variable-argument macros.
    va_start(vl, format);
    static char message[1024];
    for (int i = 0; i < 1024; i++) {
        message[i] = '\0';
    }

    vsprintf(message, format, vl);
    va_end(vl);

    int len = (int)strlen(message);
    message[len] = '\n';

    OutputDebugStringA(message);
}

/**
 * Generic run shader on particles - one for each
 */
void util::runProg(ivec3 work_groups) {
    gl::dispatchCompute(work_groups.x, work_groups.y, work_groups.z);
}

void util::runProg(int work_groups) { runProg(ivec3(work_groups, 1, 1)); }

gl::GlslProgRef util::compileComputeShader(char* filename) {
    return gl::GlslProg::create(gl::GlslProg::Format().compute(loadAsset(filename)));
}

std::vector<Particle> util::getParticles(gl::SsboRef particle_buffer, int num_items) {
    gl::ScopedBuffer scoped_particles(particle_buffer);
    std::vector<Particle> particles(num_items);
    glGetBufferSubData(particle_buffer->getTarget(), 0, num_items * sizeof(Particle),
                       particles.data());
    glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);
    return particles;
}

std::vector<Particle> util::getParticles(GLuint buffer, int num_items) {
    std::vector<Particle> particles(num_items);
    gl::ScopedBuffer scoped_buffer(GL_SHADER_STORAGE_BUFFER, buffer);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, num_items * sizeof(Particle), particles.data());
    glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);
    return particles;
}

void util::setParticles(gl::SsboRef particle_buffer, std::vector<Particle> particles) {
    glBufferData(particle_buffer->getTarget(), particles.size() * sizeof(Particle),
                 particles.data(), GL_DYNAMIC_DRAW);
    glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);
}

std::vector<uint32_t> util::getUints(GLuint buffer, int num_items) {
    std::vector<uint32_t> data(num_items, 0);
    gl::ScopedBuffer scoped_buffer(GL_SHADER_STORAGE_BUFFER, buffer);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, num_items * sizeof(uint32_t), data.data());
    glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);
    return data;
}

std::vector<uint32_t> util::getUints(gl::Texture1dRef tex, int num_items) {
    gl::ScopedTextureBind scoped_tex(tex->getTarget(), tex->getId());
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    std::vector<uint32_t> data(num_items);
    glGetTexImage(tex->getTarget(), 0, GL_RED_INTEGER, GL_UNSIGNED_INT, data.data());
    return data;
}

std::vector<uint32_t> util::getUints(gl::Texture3dRef tex, int num_items) {
    gl::ScopedTextureBind scoped_tex(tex->getTarget(), tex->getId());
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    std::vector<uint32_t> data(num_items);
    glGetTexImage(tex->getTarget(), 0, GL_RED_INTEGER, GL_UNSIGNED_INT, data.data());
    return data;
}

std::vector<float> util::getFloats(GLuint buffer, int num_items) {
    std::vector<float> data(num_items, 0);
    gl::ScopedBuffer scoped_buffer(GL_SHADER_STORAGE_BUFFER, buffer);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, num_items * sizeof(float), data.data());
    glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);
    return data;
}

std::vector<float> util::getFloats(gl::Texture3dRef tex, int num_items) {
    gl::ScopedTextureBind scoped_tex(tex->getTarget(), tex->getId());
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    std::vector<float> data(num_items);
    glGetTexImage(tex->getTarget(), 0, GL_RED, GL_FLOAT, data.data());
    return data;
}

std::vector<vec3> util::getVecs(gl::Texture3dRef tex, int num_items) {
    gl::ScopedTextureBind scoped_tex(tex->getTarget(), tex->getId());
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    std::vector<vec3> data(num_items);
    glGetTexImage(tex->getTarget(), 0, GL_RGBA32F, GL_UNSIGNED_INT, data.data());
    return data;
}

void util::printParticles(GLuint particle_buffer, GLuint debug_buffer, int n, float bin_size) {
    auto particles = util::getParticles(particle_buffer, n);
    auto debug = util::getUints(debug_buffer, n);
    util::log("-----Particles-----");
    for (int i = 0; i < n; i++) {
        Particle p = particles[i];
        std::string s = "p=<" + glm::to_string(p.position) + ">";
        s += " v=<" + glm::to_string(p.velocity) + ">";
        s += " d=" + std::to_string(p.density) + ", pr=" + std::to_string(p.pressure);
        s += " binCoord=" + glm::to_string(ivec3(p.position / bin_size));
        s += " debug=" + std::to_string(debug[i]);
        util::log("%s", s.c_str());
    }
}
