/**
 * @file InterfaceImpl.hpp
 * @author paul
 * @date 02.01.19
 * @brief Defines the used interface as an abstraction for all components building on the interface.
 */

#ifndef SERIALTOOLBOX_INTERFACEIMPL_HPP
#define SERIALTOOLBOX_INTERFACEIMPL_HPP

#ifdef POSIX
    #include "InterfacePosix.hpp"
#else
    #error "Cannot build on Windows"
#endif

namespace util::serial {
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
    using InterfaceImplementation = util::serial::InterfacePosix;
#else
    using InterfaceImplementation = util::serial::ImACompilerError
#endif
} // namespace util::serial

#endif
