// SPDX-License-Identifier: BSD-2-Clause

#include <array>
#include <csignal>

#include "JVMSignals.hpp"

namespace {

// List of signals which the JVM set, which we want to save and after initializing
// CEF, restore.
//
// https://github.com/chromiumembedded/java-cef/blob/master/native/signal_restore_posix.cpp
constexpr std::array<int, 13> signalsToRestore = {
    SIGHUP,
    SIGINT,
    SIGQUIT,
    SIGILL,
    SIGABRT,
    SIGFPE,
    SIGSEGV,
    SIGALRM,
    SIGTERM,
    SIGCHLD,
    SIGBUS,
    SIGTRAP,
    SIGPIPE,
};

std::array<struct sigaction, signalsToRestore.size()> savedActions;

}

namespace JVMSignals {

void Backup()
{
    struct sigaction action { };

    for (size_t i = 0; i < signalsToRestore.size(); ++i) {
        memset(&action, 0, sizeof(action));

        sigaction(signalsToRestore[i], nullptr, &action);

        savedActions[i] = action;
    }
}

void Restore()
{
    for (size_t i = 0; i < signalsToRestore.size(); ++i) {
        sigaction(signalsToRestore[i], &savedActions[i], nullptr);
    }
}

}
