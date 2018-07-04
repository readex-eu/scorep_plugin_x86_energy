# Score-P power and energy event plugin counter

This is the Score-P power and energy event plugin counter for Intel Sandy Bridge and 
AMD Bulldozer. The plugin supports reading `msr` registers directly or through the 
`x86_adapt` library.
For the original synchronous C version with Vampir Trace support please look
at Commit [a11bbb1](https://github.com/score-p/scorep_plugin_x86_energy/commit/a11bbb1).
For a C version with an interface like the HDEEM plugin and Vampir Trace support at
Commit [0238bba](https://github.com/score-p/scorep_plugin_x86_energy/commit/0238bba). For the documentation of this
plugin version you should consider this ReadMe instead.

## Compilation and Installation

### Prerequisites

To compile this plugin, you need:

* A C++ 14 compiler

* CMake

* Score-P Version 2+ (`SCOREP_METRIC_PLUGIN_VERSION` >= 1 (for C++ version)


### Build Options


* `SCOREP_CONFIG`

    Path to the `scorep-config` tool including the file name

    > Note: If you have `scorep-config` in your `PATH`, it should be found by CMake.

* `CMAKE_INSTALL_PREFIX`

    Directory where the resulting plugin will be installed (`lib/` suffix will be added)

* `ENABLE_MPI`

    Enables MPI communication for the sync plugin, and allows to run more than one MPI process
    per node. Default: ON

* `Build Options of x86_energy` (see [here](https://github.com/tud-zih-energy/x86_energy))
    All Build Options of x86_energy can be used, if x86_energy is automatically built, 
    because it can't be found. This Options will passed through to the
    `x86_energy ` CMake and ignored for this plugin.


### Building

1. Create a build directory

```
mkdir build
cd build
```

2. Invoking CMake

```
cmake .. 
```

3. Invoking make

```
make
```

4. Install the files (optional)

```
make install
```

> Note: Make sure to add the subfolder `lib` to your `LD_LIBRARY_PATH` or if you
> didn't use make install that the plugin is in a location listed in 
> `LD_LIBRARY_PATH`. For example add the build directory to `LD_LIBRARY_PATH`:

>       export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD


## Usage

### Score-P

To add a x86_energy metric to your trace, you have to add `x86_energy_plugin` to the environment variable
`SCOREP_METRIC_PLUGINS`. This is the preferred version for tracing. It preserves the best possible
time resolution of the measurements.

To add the synchronous metric, add `x86_energy_sync_plugin` to the environment variable
`SCOREP_METRIC_PLUGINS`. This can be useful for profiling. Measurements are only taken at
events, e.g. enter / leave of regions.

You have to add the list of the metric channel you are interested in to the environment
variable `SCOREP_METRIC_X86_ENERGY_PLUGIN` or `SCOREP_METRIC_X86_ENERGY_SYNC_PLUGIN`.

Note: Score-P does not support per-host post-mortem plugins with profiling. If you want to
use the post-mortem (`x86_energy_plugin`) plugin, you should enable tracing and disable profiling by using:

      export SCOREP_ENABLE_PROFILING="false"
      export SCOREP_ENABLE_TRACING="true"

The sync plugin (`x86_energy_sync_plugin`) works with profiling and tracing.

> Note: The plugin is implemented as a strictly synchronous plugin. Therefore, it measures per
> thread. As x86_energy can just be used nodewide, the plugin does some thread and if enabled MPI
> operations in order to obtain the responsible thread and process. The trace file might hold some
> traces that are reported as 0. The profile might report wrong profiling statistics. Moreover, the
> plugin reports mJ as interfer values. This operations are necessary to be compatible with PTF
> (http://periscope.in.tum.de/)
 
> The interface for this plugin is built like the correspondending hdeem plugin.

### Available Metrics

`SCOREP_METRIC_X86_ENERGY_PLUGIN` or 
`SCOREP_METRIC_X86_ENERGY_SYNC_PLUGIN` specifies the software events that shall
be recorded when tracing an application. You can add the following metrics:

* `*`
    whatever is given, there are always all energy metrics recorded.


### Environment variables

* `SCOREP_METRIC_X86_ENERGY_SYNC_PLUGIN`/`SCOREP_METRIC_X86_ENERGY_PLUGIN`

    Comma-separated list of sensors, and physical quantity. Can also contain wildcards, e.g.
    `BLADE/E,*/E`

* `SCOREP_METRIC_X86_ENERGY_SYNC_PLUGIN_VERBOSE`/`SCOREP_METRIC_X86_ENERGY_SYNC_PLUGIN_VERBOSE` (default `WARN`)

    Controls the output verbosity of the plugin. Possible values are: `FATAL`, `ERROR`, `WARN`,
    `INFO`, `DEBUG`. If set to any other value, `DEBUG` is used. Case in-sensitive.

* `SCOREP_METRIC_X86_ENERGY_SYNC_PLUGIN_OFFSET` (default `70.0`)

    Can be set to any double variable. Defines the offset in W that is added to 
    the blade, because with RAPL only the energy consumption of the
    CPU is measurable. Blade is the sum of all RAPL values excepted the core
    because there are included in package.
    The default value of 70W was measured on Taurus

* `SCOREP_X86_ENERGY_PLUGIN_INTERVALL_US` (default `50000`)

    The time in microsecs, between two consecutive reads of the power/energy values. A longer interval
    means less disturbance, a shorter interval is more exact.

    On Intel CPUs, the registers are updated roughly every msec. If you choose an interval of 1ms
    you might find highly variating power consumptions. To gain most exact values
    relating to temporal granularity, you should set the intervall to 10, but this
    gives less accurate values. If you can live with less granularity, you should
    set it to 100000. This gives also more accurate values.

## Authors

### Original C version
* Joseph Schuchart (joseph.schuchart at tu-dresden dot de)
* Michael Werner (michael.werner3 at tu-dresden dot de)

### C++ versions
* Andreas Gocht  (andreas.gocht  at tu-dresden dot de)
* Sven Schiffner (sven.schiffner at tu-dresden dot de)

