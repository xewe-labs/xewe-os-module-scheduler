# xewe-os-module-scheduler — run stored commands on a weekly schedule

XeWe OS module · created 2026-09-15 (split out of xewe-os, where it was developed from 2026-07) · Solo: Max Dokukin · Status: Active (0.1.0)

## Overview

Runs stored commands on a weekly schedule. A module for [XeWe OS](https://github.com/xewe-labs/xewe-os),
built on the [XeWeOS framework](https://github.com/xewe-labs/xewe-library-os). A schedule block
has a start and end time (minutes from midnight), a weekday, a display colour and one or more CLI
commands; at the start minute of a matching day the commands run through the command line, so a
schedule can drive any installed module. Blocks are stored in NVS as one structured record and
reloaded on boot.

## Highlights

- Schedule blocks are `FlexData` structs (`id`, `start_time`, `end_time`, `day`, `displayed_color`, `commands`) persisted with `write_flex`/`read_flex` under the NVS key `schedules`
- Evaluated once per minute from `loop()`; nothing runs until the clock is synced (year ≥ 1970)
- Weekday mapping converts `tm_wday` (0 = Sunday) to the module's 0 = Monday … 6 = Sunday
- Input validation with range checks: start/end 0–1439, day 0–6, colour exactly 6 characters; several commands separated by `|`

## How it works

```
$schedule add 480 1020 1 FF0000 "$pins gpio_write 8 1" → validate → ScheduleBlock{id = max+1} → NVS
loop (once per minute): Time.get_current_time() → day + minute match start_time → xewe_cli.execute(each command)
```

- **`Scheduler` class** (`src/Scheduler/`) — a `xewe::os::Module` with id `schedule`; requires Time (and through it Wifi); cannot be disabled; the schedule becomes active after the reboot that follows first setup.
- **API** — `add(...)`, `remove(id)`, `get_all_json()`, `load_from_nvs()`, `save_to_nvs()`; `status` prints all blocks as JSON.
- `end_time` and `displayed_color` are stored for interfaces that draw the schedule; only the start minute triggers commands.

### Commands

**Prefix:** `$schedule` · requires Time

Times are minutes from midnight (0-1439), days are 0 (Monday) to 6 (Sunday); several commands are separated by `|`.

| Command | Description | Sample Usage |
| :--- | :--- | :--- |
| **`add`** | Add a schedule: `<start> <end> <day> <RRGGBB> "<cmd1\|cmd2>"`. | `$schedule add 480 1020 1 FF0000 "$pins gpio_write 8 1"` |
| **`remove`** | Remove a schedule by id. | `$schedule remove 1` |

### Requirements

| | |
|---|---|
| Modules | [xewe-os-module-time](https://github.com/xewe-labs/xewe-os-module-time) (and, through it, xewe-os-module-wifi) |
| Libraries | XeWeOS (>=0.1.0) and its dependencies (XeWeUtils, XeWeSerial, XeWeNvs, XeWeCli, ArduinoJson) |
| Boards | ESP32-C3, ESP32-C6, ESP32-S3 (arduino-esp32 3.x) |

Metadata and dependencies are declared in [`module.properties`](module.properties).

### Layout

| Path | |
|---|---|
| `src/Scheduler/` | the module (`Scheduler.h`, `Scheduler.cpp`) |
| `xewe-os-module-scheduler.ino` | validation firmware: framework + required modules + this module |
| `scripts/validate.sh` | assembles and compiles the validation firmware |
| `module.properties` | metadata read by xewe-os `setup.sh` and `validate.sh` |

## Results

| Metric | Value | Baseline / note |
|---|---|---|
| Source | 287 lines (`Scheduler.h` 87, `Scheduler.cpp` 200) | `wc -l` |
| Commands | 2 | `add`, `remove` |
| Resolution | 1 minute, weekly repeat | start minute only |

A module has no measured results; the table lists what it provides.

## Getting started

### Use in XeWe OS

Choose `scheduler` in xewe-os `setup.sh`; Time and Wifi are added automatically.

### Validate

Clone the modules this one requires next to this repo, then:

```bash
scripts/validate.sh                         # compile for c3, c6 and s3
scripts/validate.sh -b c3                   # one board
scripts/validate.sh -b c3 -p /dev/ttyACM0   # compile, upload and run on a board
```

The script copies this module and its required modules into `build/xewe-os-module-scheduler/` and compiles it with
`arduino-cli`. Environment variables:

* `XEWE_MODULES_DIR` - where required module repos are cloned (default: the folder containing this repo)
* `XEWE_LIBRARIES_DIR` - folder with `xewe-library-*` clones to build against instead of installed libraries

### Use in firmware

Copy `src/Scheduler/` (and the required modules' folders) into the sketch's `src/`, then:

```cpp
#include "src/Wifi/Wifi.h"
#include "src/Time/Time.h"
#include "src/Scheduler/Scheduler.h"

Wifi wifi(os);
Time time_module(os, wifi);
Scheduler scheduler(os, time_module);
```

Declare required modules before this one.

## Documents

- [module.properties](module.properties)
- Firmware: [xewe-os](https://github.com/xewe-labs/xewe-os) · registry: [xewe-os-modules](https://github.com/xewe-labs/xewe-os-modules) · framework: [xewe-library-os](https://github.com/xewe-labs/xewe-library-os)
- License: GPL-3.0. See [LICENSE.txt](LICENSE.txt).
