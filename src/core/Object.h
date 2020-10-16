#pragma once

#include <memory>
#include <string>

#include "cinder/gl/gl.h"

#include "./BaseObject.h"

using namespace ci;

namespace core {

    template <class D> struct Object : public BaseObject {
    public:
        Object(const std::string& name):
            BaseObject(name),
            mass_(1.0),
            position_(0),
            velocity_(0),
            acceleration_(0)
        {}

        float mass() { return mass_; }
        D& mass(float mass) {
            mass_ = mass;
            return dthis();
        }
        
        vec3 position() { return position_; }
        D& position(vec3 position) {
            position_ = position;
            return dthis();
        }

        vec3 velocity() { return velocity_; }
        D& velocity(vec3 velocity) {
            velocity_ = velocity;
            return dthis();
        }

        vec3 acceleration() { return acceleration_; }
        D& acceleration(vec3 acceleration) {
            acceleration_ = acceleration;
            return dthis();
        }

        void applyForce(vec3 force) {
            acceleration_ += force / mass_;
        }

        void update(double time) {
            velocity_ += acceleration_ * (float)time;
            position_ += velocity_ * (float)time;
            acceleration_ *= 0.0;
        }

    private:
        friend D;
        Object() = default;

        D& dthis() { return static_cast<D&>(*this); }
        D& dthis() const { return static_cast<D const &>(*this); }

        float mass_;
        vec3 position_, velocity_, acceleration_;

    };

};