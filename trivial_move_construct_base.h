#pragma once

#include "trivial_move_assign_base.h"

namespace optional_details {

template <typename T, bool trivial = std::is_trivially_move_constructible_v<T>>
struct trivial_move_construct_base : trivial_move_assign_base<T> {
  using trivial_move_assign_base<T>::trivial_move_assign_base;

  constexpr trivial_move_construct_base() = default;
  constexpr trivial_move_construct_base(const trivial_move_construct_base&) =
      default;
  trivial_move_construct_base&
  operator=(const trivial_move_construct_base& other) = default;

  constexpr trivial_move_construct_base(trivial_move_construct_base&& other) {
    if (other.is_present) {
      new (&this->data) T(std::move(other.data));
      this->is_present = true;
    }
  }

  trivial_move_construct_base&
  operator=(trivial_move_construct_base&& other) = default;
};

template <typename T>
struct trivial_move_construct_base<T, true> : trivial_move_assign_base<T> {
  using trivial_move_assign_base<T>::trivial_move_assign_base;
};

} // namespace optional_details
