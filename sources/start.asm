%define __NR_exit    60
%define __NR_getgid 104
%define __NR_getuid 102
%define __NR_getpid  39
%define __NR_write    1

BITS 64

EXTERN main

GLOBAL __cxa_atexit
GLOBAL __gxx_personality_v0
GLOBAL __dso_handle
GLOBAL _start
GLOBAL _ZN10__internal6getgidEv
GLOBAL _ZN10__internal6getpidEv
GLOBAL _ZN10__internal6getuidEv
GLOBAL _ZN10__internal5writeEiPKcm

SECTION .data

__dso_handle: DQ 0

SECTION .text

__cxa_atexit:
    ret

__gxx_personality_v0:
    ret

; __internal::getgid()
_ZN10__internal6getgidEv:
    enter 0, 0

    mov rax, __NR_getgid
    syscall

    leave
    ret

; __internal::getuid()
_ZN10__internal6getuidEv:
    enter 0, 0

    mov rax, __NR_getuid
    syscall

    leave
    ret

; __internal::getpid()
_ZN10__internal6getpidEv:
    enter 0, 0

    mov rax, __NR_getpid
    syscall

    leave
    ret

; __internal::write(const char*, unsigned long)
_ZN10__internal5writeEiPKcm:
    enter 0, 0

    mov rax, __NR_write
    syscall

    leave
    ret

_start:
    ; argc
    pop rdi

    ; argv
    mov rsi, rsp

    lea r8, [rsp+rdi*8]

    ; r = main(argc, argv)
    call main

    ; exit(r)
    mov rdi, rax
    mov rax, __NR_exit
    syscall
