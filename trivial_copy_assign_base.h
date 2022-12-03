#pragma once

#include "destroy_base.h"

namespace optional_details {

template <typename T, bool trivial = std::is_trivially_copy_assignable_v<T>&&
                          std::is_trivially_copy_constructible_v<T>&&
                              std::is_trivially_destructible_v<T>>
struct trivial_copy_assign_base : destroy_base<T> {
  using destroy_base<T>::destroy_base;

  constexpr trivial_copy_assign_base() = default;

  constexpr trivial_copy_assign_base(const trivial_copy_assign_base& other) =
      default;

  trivial_copy_assign_base& operator=(const trivial_copy_assign_base& other) {
    if (this->is_present) {
      if (other.is_present) {
        this->data = other.data;
      } else {
        this->reset();
      }
    } else {
      if (other.is_present) {
        new (&this->data) T(other.data);
        this->is_present = true;
      }
    }

    return *this;
  }

  constexpr trivial_copy_assign_base(trivial_copy_assign_base&&) = default;
  trivial_copy_assign_base& operator=(trivial_copy_assign_base&&) = default;
};

template <typename T>
struct trivial_copy_assign_base<T, true> : destroy_base<T> {
  using destroy_base<T>::destroy_base;
};
} // namespace optional_details
