#pragma once

#define DISALLOW_COPY_AND_ASSIGN(T) \
  T(T const &) = delete;            \
  void operator=(T const &) = delete
