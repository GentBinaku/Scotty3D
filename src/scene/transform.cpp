
#include "transform.h"

Mat4 Transform::local_to_parent() const {
  return Mat4::translate(translation) * rotation.to_mat() * Mat4::scale(scale);
}

Mat4 Transform::parent_to_local() const {
  return Mat4::scale(1.0f / scale) * rotation.inverse().to_mat() *
         Mat4::translate(-translation);
}

Mat4 Transform::local_to_world() const {
  // If there is a parent, compose with its local_to_world transform
  if (auto p = parent.lock()) {
    return p->local_to_world() * local_to_parent();
  }
  // If there is no parent, we are already in world space
  return local_to_parent();
}

Mat4 Transform::world_to_local() const {
  // If there is a parent, compose its world_to_local with our parent_to_local
  if (auto p = parent.lock()) {
    return parent_to_local() * p->world_to_local();
  }
  // If no parent, just return our parent_to_local matrix
  return parent_to_local();
}

bool operator!=(const Transform &a, const Transform &b) {
  return a.parent.lock() != b.parent.lock() || a.translation != b.translation ||
         a.rotation != b.rotation || a.scale != b.scale;
}
