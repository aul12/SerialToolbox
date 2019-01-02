/**
 * @file InterfaceImpl.hpp
 * @author paul
 * @date 02.01.19
 * @brief InterfaceImpl @TODO
 */

#ifndef SERIALTOOLBOX_INTERFACEIMPL_HPP
#define SERIALTOOLBOX_INTERFACEIMPL_HPP

#ifdef POSIX
#include "InterfacePosix.hpp"
#else
#error "Cannot build on Windows"
#endif

namespace util::serial {
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
    using InterfaceImplementation = util::serial::InterfacePosix;
#else
    using InterfaceImplementation = util::serial::ImACompilerError
#endif
}

#endif //SERIALTOOLBOX_INTERFACEIMPL_HPP
