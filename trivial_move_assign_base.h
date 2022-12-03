#pragma once

#include "trivial_copy_construct_base.h"

namespace optional_details {

template <typename T, bool trivial = std::is_trivially_move_assignable_v<T>&&
                          std::is_trivially_move_constructible_v<T>&&
                              std::is_trivially_destructible_v<T>>
struct trivial_move_assign_base : trivial_copy_construct_base<T> {
  using trivial_copy_construct_base<T>::trivial_copy_construct_base;

  constexpr trivial_move_assign_base() = default;
  constexpr trivial_move_assign_base(const trivial_move_assign_base&) = default;
  trivial_move_assign_base&
  operator=(const trivial_move_assign_base&) = default;

  constexpr trivial_move_assign_base(trivial_move_assign_base&& other) =
      default;

  trivial_move_assign_base& operator=(trivial_move_assign_base&& other) {
    if (this->is_present) {
      if (other.is_present) {
        this->data = std::move(other.data);
      } else {
        this->reset();
      }
    } else {
      if (other.is_present) {
        new (&this->data) T(std::move(other.data));
        this->is_present = true;
      }
    }

    return *this;
  }
};

template <typename T>
struct trivial_move_assign_base<T, true> : trivial_copy_construct_base<T> {
  using trivial_copy_construct_base<T>::trivial_copy_construct_base;
};

} // namespace optional_details
