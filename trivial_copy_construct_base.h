#pragma once

#include "trivial_copy_assign_base.h"

namespace optional_details {

template <typename T, bool trivial = std::is_trivially_copy_assignable_v<T>&&
                          std::is_trivially_copy_constructible_v<T>&&
                              std::is_trivially_destructible_v<T>>
struct trivial_copy_construct_base : trivial_copy_assign_base<T> {
  using trivial_copy_assign_base<T>::trivial_copy_assign_base;

  constexpr trivial_copy_construct_base() = default;

  constexpr trivial_copy_construct_base(
      const trivial_copy_construct_base& other) {
    if (other.is_present) {
      new (&this->data) T(other.data);
      this->is_present = true;
    }
  }

  trivial_copy_construct_base&
  operator=(const trivial_copy_construct_base& other) = default;

  constexpr trivial_copy_construct_base(trivial_copy_construct_base&&) =
      default;
  trivial_copy_construct_base&
  operator=(trivial_copy_construct_base&&) = default;
};

template <typename T>
struct trivial_copy_construct_base<T, true> : trivial_copy_assign_base<T> {
  using trivial_copy_assign_base<T>::trivial_copy_assign_base;
};

} // namespace optional_details
