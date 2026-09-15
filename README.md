# xewe-os-module-scheduler

Runs stored commands on a weekly schedule. A module for [XeWe OS](https://github.com/xewe-labs/xewe-os), built on the
[XeWeOS framework](https://github.com/xewe-labs/xewe-library-os).

## Commands

**Prefix:** `$schedule` · requires Time

Runs stored commands on a weekly schedule. Times are minutes from midnight (0-1439), days are
0 (Monday) to 6 (Sunday); several commands are separated by `|`.

| Command | Description | Sample Usage |
| :--- | :--- | :--- |
| **`add`** | Add a schedule: `<start> <end> <day> <RRGGBB> "<cmd1\|cmd2>"`. | `$schedule add 480 1020 1 FF0000 "$pins gpio_write 8 1"` |
| **`remove`** | Remove a schedule by id. | `$schedule remove 1` |

## Requirements

| | |
|---|---|
| Modules | [xewe-os-module-time](https://github.com/xewe-labs/xewe-os-module-time) (and, through it, xewe-os-module-wifi) |
| Libraries | XeWeOS (>=0.1.0) and its dependencies (XeWeUtils, XeWeSerial, XeWeNvs, XeWeCli, ArduinoJson) |
| Boards | ESP32-C3, ESP32-C6, ESP32-S3 (arduino-esp32 3.x) |

Metadata and dependencies are declared in [`module.properties`](module.properties).

## Layout

| Path | |
|---|---|
| `src/Scheduler/` | the module |
| `xewe-os-module-scheduler.ino` | validation firmware: framework + required modules + this module |
| `scripts/validate.sh` | assembles and compiles the validation firmware |

## Validate

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

## Use in firmware

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

## License

GPL-3.0. See [LICENSE.txt](LICENSE.txt).
