/* Copyright (C) 2016 Murray Cumming
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/
 */

#ifndef _MURRAYC_TUPLE_UTILS_TUPLE_TRANSFORM_EACH_CONST_H_
#define _MURRAYC_TUPLE_UTILS_TUPLE_TRANSFORM_EACH_CONST_H_

#include <tuple-utils/tuple_cat.h>
#include <tuple-utils/tuple_cdr.h>
#include <tuple-utils/tuple_start.h>
#include <tuple-utils/tuple_end.h>
#include <type_traits>

namespace tupleutils
{

namespace {

template<template<typename> class T_transformer, std::size_t index>
struct tuple_transform_each_const_impl
{
  template<typename T>
  static
  decltype(auto)
  tuple_transform_each_const(const T& t) {
    using element_type = typename std::tuple_element<index, T>::type;
    const auto element = T_transformer<element_type>::transform(std::get<index>(t));
    const auto t_element = std::make_tuple(element);
    
    const auto t_start = tuple_start<index>(t);

    constexpr auto size = std::tuple_size<T>::value; 
    const auto t_end = tuple_end<size - index - 1>(t);

    const auto t_with_transformed_element =
      std::tuple_cat(t_start, t_element, t_end);
    return tuple_transform_each_const_impl<T_transformer, index - 1>::tuple_transform_each_const(
      t_with_transformed_element);
  }
};

template<template<typename> class T_transformer>
struct tuple_transform_each_const_impl<T_transformer, 0>
{
  template<typename T>
  static
  decltype(auto)
  tuple_transform_each_const(const T& t) {
    constexpr std::size_t index = 0;

    using element_type = typename std::tuple_element<index, T>::type;
    const auto element = T_transformer<element_type>::transform(std::get<index>(t));
    const auto tuple_element = std::make_tuple(element);
    const auto tuple_rest = tuple_cdr(t);
    return std::tuple_cat(tuple_element, tuple_rest);
  }
};

} //anonymous namespace

/**
 * Get a tuple with each element having the transformed value of the element
 * in the original tuple.
 */
template<template<typename> class T_transformer, typename T>
decltype(auto)
tuple_transform_each_const(const T& t) {
  constexpr auto size = std::tuple_size<T>::value; 
  return tuple_transform_each_const_impl<T_transformer, size - 1>::tuple_transform_each_const(t);
}

} //namespace tupleutils

#endif //_MURRAYC_TUPLE_UTILS_TUPLE_TRANSFORM_EACH_CONST_H_
