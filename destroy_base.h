#pragma once

#include <new>
#include <utility>

struct in_place_t {};

inline constexpr in_place_t const in_place;

namespace optional_details {

template <typename T, bool trivial = std::is_trivially_destructible_v<T>>
struct destroy_base {
  constexpr destroy_base() : is_present{false}, dummy{0} {}

  constexpr destroy_base(T const& x) : is_present{true}, data{x} {}

  constexpr destroy_base(T&& x) : is_present{true}, data{std::move(x)} {}

  template <typename... Args>
  constexpr explicit destroy_base(in_place_t, Args&&... args)
      : is_present{true}, data(std::forward<Args>(args)...) {}

  void reset() {
    if (is_present) {
      data.~T();
      is_present = false;
    }
  }

  ~destroy_base() {
    reset();
  }

  bool is_present{false};
  union {
    char dummy;
    T data;
  };
};

template <typename T>
struct destroy_base<T, true> {
  constexpr destroy_base() : is_present{false}, dummy{0} {}
  constexpr destroy_base(T const& x) : is_present{true}, data{x} {}
  constexpr destroy_base(T&& x) : is_present{true}, data{std::move(x)} {}

  template <typename... Args>
  constexpr explicit destroy_base(in_place_t, Args&&... args)
      : is_present{true}, data(std::forward<Args>(args)...) {}

  void reset() {
    if (is_present) {
      is_present = false;
    }
  }

  bool is_present{false};
  union {
    char dummy;
    T data;
  };
};

} // namespace optional_details
