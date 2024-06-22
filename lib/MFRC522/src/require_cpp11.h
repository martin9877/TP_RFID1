/**
 * @file require_cpp11.h
 * @author Recomentado por Martin Girardo
 * @brief 
 * @version 0.1
 * @date 2024-06-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */
 */

/**
 * Copyright (c) 2016 by Ludwig Grill (www.rotzbua.de)
 * Throws error if c++11 is not supported
 */
#ifndef REQUIRE_CPP11_H
#define REQUIRE_CPP11_H

#if __cplusplus < 201103L
#error "This library needs at least a C++11 compliant compiler, maybe compiler argument for C++11 support is missing or if you use Arduino IDE upgrade to version >=1.6.6"
#endif

#endif // REQUIRE_CPP11_H
