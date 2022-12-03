#pragma once

#include "toggles.h"
#include "trivial_move_construct_base.h"

struct nullopt_t {
  explicit constexpr nullopt_t() noexcept = default;
};

inline constexpr nullopt_t const nullopt;

template <typename T>
struct optional : private optional_details::trivial_move_construct_base<T>,
                  private optional_details::copy_assign_base<T>,
                  private optional_details::move_assign_base<T>,
                  private optional_details::copy_construct_base<T>,
                  private optional_details::move_construct_base<T> {
  using optional_details::trivial_move_construct_base<
      T>::trivial_move_construct_base;
  using optional_details::copy_assign_base<T>::copy_assign_base;
  using optional_details::move_assign_base<T>::move_assign_base;
  using optional_details::copy_construct_base<T>::copy_construct_base;
  using optional_details::move_construct_base<T>::move_construct_base;

  constexpr optional(const optional&) = default;
  constexpr optional(optional&&) = default;
  constexpr optional() = default;

  optional& operator=(const optional&) = default;
  optional& operator=(optional&&) = default;

  optional& operator=(nullopt_t) noexcept {
    this->reset();
    return *this;
  }

  constexpr optional(nullopt_t) noexcept {}

  constexpr T& operator*() noexcept {
    return this->data;
  }

  constexpr T const& operator*() const noexcept {
    return this->data;
  }

  constexpr T* operator->() noexcept {
    return &this->data;
  }

  constexpr T const* operator->() const noexcept {
    return &this->data;
  }

  constexpr explicit operator bool() const noexcept {
    return has_value();
  }

  [[nodiscard]] constexpr bool has_value() const noexcept {
    return this->is_present;
  }

  template <typename... Args>
  void emplace(Args&&... args) {
    this->reset();
    new (&this->data) T(std::forward<Args>(args)...);
    this->is_present = true;
  }

  void reset() {
    static_cast<optional_details::destroy_base<T>*>(this)->reset();
  }

  void swap(optional& other) {
    if (has_value() && other.has_value()) {
      using std::swap;
      swap(this->data, other.data);
    } else if (has_value() && !other.has_value()) {
      other.data = std::move(this->data);
      this->is_present = false;
      this->reset();
    } else if (!has_value() && other.has_value()) {
      other.swap(*this);
    }
  }
};

template <typename T>
constexpr bool operator==(optional<T> const& a, optional<T> const& b) {
  if (static_cast<bool>(a) != static_cast<bool>(b)) {
    return false;
  }
  if (!static_cast<bool>(a)) {
    return true;
  }
  return *a == *b;
}

template <typename T>
constexpr bool operator!=(optional<T> const& a, optional<T> const& b) {
  if (static_cast<bool>(a) != static_cast<bool>(b)) {
    return true;
  }
  if (!static_cast<bool>(a)) {
    return false;
  }
  return *a != *b;
}

template <typename T>
constexpr bool operator<(optional<T> const& a, optional<T> const& b) {
  if (!static_cast<bool>(b)) {
    return false;
  }
  if (!static_cast<bool>(a)) {
    return true;
  }
  return *a < *b;
}

template <typename T>
constexpr bool operator<=(optional<T> const& a, optional<T> const& b) {
  if (!static_cast<bool>(b)) {
    return !static_cast<bool>(a);
  }
  if (!static_cast<bool>(a)) {
    return true;
  }
  return *a <= *b;
}

template <typename T>
constexpr bool operator>(optional<T> const& a, optional<T> const& b) {
  if (!static_cast<bool>(b)) {
    return static_cast<bool>(a);
  }
  if (!static_cast<bool>(a)) {
    return false;
  }
  return *a > *b;
}

template <typename T>
constexpr bool operator>=(optional<T> const& a, optional<T> const& b) {
  if (!static_cast<bool>(b)) {
    return true;
  }
  if (!static_cast<bool>(a)) {
    return false;
  }
  return *a >= *b;
}
