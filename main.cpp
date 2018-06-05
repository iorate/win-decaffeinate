
// win-decaffeinate
//
// Copyright iorate 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>
#include <iostream>
#include <utility>
#include <windows.h>
#include <PowrProf.h>
#include <nonsugar.hpp>

template <class F>
struct scope_exit
{
    scope_exit(F const &f) : m_f(f) {}
    scope_exit(F &&f) : m_f(std::move(f)) {}

    scope_exit(scope_exit const &) = delete;
    scope_exit &operator=(scope_exit const &) = delete;

    ~scope_exit() { m_f(); }

    F m_f;
};

template <class F>
scope_exit(F) -> scope_exit<F>;

BOOL bRet;
DWORD dwRet;

int main(int argc, char **argv)
try {
    auto const cmd = nonsugar::command<char>("decaffeinate", "put the system to sleep")
        .flag<'h'>({'h'}, {"help"}, "", "display this help and exit")
        .flag<'v'>({'v'}, {"version"}, "", "display the version info and exit")
        .flag<'d'>({'d'}, {"display"}, "", "put only the display to sleep")
        .flag<'t', int>({'t'}, {"timeout"}, "PERIOD", "specify the timeout value in seconds")
        ;
    auto const opts = nonsugar::parse(argc, argv, cmd);
    if (opts.has<'h'>()) {
        std::cout << nonsugar::usage(cmd);
        return 0;
    }
    if (opts.has<'v'>()) {
        std::cout << "decaffeinate 1.0\n";
        return 0;
    }

    HANDLE timer = nullptr;
    scope_exit const close_timer = [&] {
        if (timer) {
            CloseHandle(timer);
        }
    };
    auto const display_only = opts.has<'d'>();
    if (opts.has<'t'>()) {
        timer = CreateWaitableTimer(0, 1, 0);
        if (!timer) { throw 0; }
        LARGE_INTEGER timeout;
        timeout.QuadPart = -(std::max)(opts.get<'t'>(), 0) * 10000000LL;
        bRet = SetWaitableTimer(timer, &timeout, 0, nullptr, nullptr, display_only ? 0 : 1);
        if (!bRet) { throw 0; }
    }

    if (display_only) {
        bRet = PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
        if (!bRet) { throw 0; }
    } else {
        bRet = SetSuspendState(0, 0, 0);
        if (!bRet) { throw 0; }
    }

    if (timer) {
        dwRet = WaitForSingleObject(timer, INFINITE);
        if (dwRet != WAIT_OBJECT_0) { throw 0; }
        if (display_only) {
            INPUT input{INPUT_MOUSE, {0, 0, 0, MOUSEEVENTF_MOVE, 0, 0}};
            bRet = SendInput(1, &input, sizeof(input));
            if (!bRet) { throw 0; }
        }
    }
} catch (nonsugar::error const &e) {
    std::cerr << e.message() << '\n';
    return 1;
} catch (...) {
    std::cerr << "decaffeinate: unexpected error occurred\n";
    return 1;
}
