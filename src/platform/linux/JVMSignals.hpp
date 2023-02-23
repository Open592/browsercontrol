// SPDX-License-Identifier: BSD-2-Clause

#pragma once

namespace JVMSignals {

// CefInitialize will overwrite JVM signal handlers. This causes issues with JIT'd code
// and results in random crashes. Backup the present signal handlers, and after
// returning from CefInitialize restore them to their previous actions.
//
// See: https://bitbucket.org/chromiumembedded/java-cef/issues/41/mac-jcef-frequently-crashing-in-thread
void Backup();

// Restore signals back to their state when we invoked `Backup`
void Restore();

}
