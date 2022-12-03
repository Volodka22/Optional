#pragma once

#include <utility>

namespace optional_details {

template <typename T, bool enabled = std::is_copy_assignable_v<T>>
struct copy_assign_base {
  constexpr copy_assign_base() = default;
  constexpr copy_assign_base(const copy_assign_base&) = default;
  copy_assign_base& operator=(const copy_assign_base&) = delete;
  constexpr copy_assign_base(copy_assign_base&&) = default;
  copy_assign_base& operator=(copy_assign_base&&) = default;
};

template <typename T>
struct copy_assign_base<T, true> {};

template <typename T, bool enabled = std::is_move_constructible_v<T>>
struct copy_construct_base {
  constexpr copy_construct_base() = default;
  constexpr copy_construct_base(const copy_construct_base&) = delete;
  copy_construct_base& operator=(const copy_construct_base&) = delete;
  constexpr copy_construct_base(copy_construct_base&&) = default;
  copy_construct_base& operator=(copy_construct_base&&) = default;
};

template <typename T>
struct copy_construct_base<T, true> {};

template <typename T, bool enabled = std::is_move_assignable_v<T>>
struct move_assign_base {
  move_assign_base() = default;
  move_assign_base(const move_assign_base&) = default;
  move_assign_base& operator=(const move_assign_base&) = default;
  move_assign_base(move_assign_base&&) = default;
  move_assign_base& operator=(move_assign_base&&) = delete;
};

template <typename T>
struct move_assign_base<T, true> {};

template <typename T, bool enabled = std::is_move_constructible_v<T>>
struct move_construct_base {
  move_construct_base() = default;
  move_construct_base(const move_construct_base&) = default;
  move_construct_base& operator=(const move_construct_base&) = default;
  move_construct_base(move_construct_base&&) = delete;
  move_construct_base& operator=(move_construct_base&&) = delete;
};

template <typename T>
struct move_construct_base<T, true> {};

} // namespace optional_details
