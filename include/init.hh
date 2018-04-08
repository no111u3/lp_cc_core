/* Copyright 2018 Boris Vinogradov <no111u3@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * System C++ initialization
 * @file init.hh
 * @author Boris Vinogradov
 */

#include <types.hh>

#ifndef INIT_HH
#define INIT_HH

namespace lp {
    namespace {
        // constructor type
        using ctor = void();

        // data and bss section starts and ends
        extern "C" u32_t __text_end;
        extern "C" u32_t __data_start;
        extern "C" u32_t __data_end;
        extern "C" u32_t __bss_start;
        extern "C" u32_t __bss_end;

        // constructors for static variables and static arrays
        extern "C" ctor *__ctors_begin[];
        extern "C" ctor *__ctors_end[];
        extern "C" ctor *__preinit_array_start[];
        extern "C" ctor *__preinit_array_end[];
        extern "C" ctor *__init_array_start[];
        extern "C" ctor *__init_array_end[];
    }

    struct initer {
        // C++ system initialization function
        static inline void __attribute__((always_inline)) setup_cc_env() noexcept {
            u32_t *src;
            u32_t *dest;

            // Setup initial values for inited data
            src = &__text_end;
            dest = &__data_start;
            if (src != dest) {
                while (dest < &__data_end) {
                    *(dest++) = *(src++);
                }
            }

            // Set zeros for uninited data
            dest = &__bss_start;
            while (dest < &__bss_end) {
                *(dest++) = 0;
            }

            // Setup static objects
            call_ctors(__preinit_array_start, __preinit_array_end);
            call_ctors(__ctors_begin, __ctors_end);
            call_ctors(__init_array_start, __init_array_end);
        }

    private:
        // calling construct function
        static void call_ctors(const ctor **start, const ctor **end) noexcept {
            for (; start < end; ++start) {
                if (*start)
                    (*start)();
            }
        }
    };
}

#endif // INIT_HH
