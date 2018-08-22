READEX specifics
=============

Compilation
-------------
The plugin *must* be compiled with `ENABLE_MPI`

Usage during Design Time Analysis with Periscope
-------------
For READEX purposes, we use the x86_energy_sync_plugin, which enables us to attribute energy to specific regions. To use the plugin, it must be enabled during runs with Periscope. To do so, use the following environment variables:
```
export SCOREP_METRIC_PLUGINS=x86_energy_sync_plugin
export SCOREP_METRIC_PLUGINS_SEP=";"
export SCOREP_METRIC_X86_ENERGY_SYNC_PLUGIN="BLADE/E"
export SCOREP_METRIC_X86_ENERGY_PLUGIN_INTERVALL_US=0
export SCOREP_METRIC_X86_ENERGY_SYNC_PLUGIN_OFFSET=70 # This is the offset that is set to the sum of package and DRAM consumptions to compute BLADE power and highly system dependent
```
Furthermore, the metric source definition of your `readex_config.xml` should look like this:
```
<metricPlugin>
  <name>x86_energy_sync_plugin</name>
</metricPlugin>
<metrics>
  <node_energy>x86_energy/BLADE/E</node_energy>
</metrics>
```
