// SPDX-FileCopyrightText: 2026 Maxim Dokukin (maxdokukin.com)
// SPDX-License-Identifier: GPL-3.0-only
// xewe-os-module-scheduler/xewe-os-module-scheduler.ino
//
// Validation firmware for the Scheduler module: the XeWeOS framework plus this module.
// It also declares the modules this one requires (Wifi, Time); scripts/validate.sh copies them in
// from their sibling repos.
// Build it with scripts/validate.sh rather than opening this sketch directly.

#include <XeWeOS.h>

#include "src/Wifi/Wifi.h"
#include "src/Time/Time.h"
#include "src/Scheduler/Scheduler.h"


xewe::os::ModuleController os({
    .project_name    = "xewe-os-module-scheduler",
    .version         = "0.1.0",
    .build_timestamp = __DATE__ " " __TIME__,
    .url             = "https://github.com/xewe-labs/xewe-os-module-scheduler",
});

Wifi      wifi        (os);
Time      time_module (os, wifi);
Scheduler scheduler   (os, time_module);


void setup() {
    os.begin();
}

void loop() {
    os.loop();
}
