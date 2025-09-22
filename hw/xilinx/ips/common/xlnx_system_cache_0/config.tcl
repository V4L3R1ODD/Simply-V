# Author: Valerio Di Domenico <valerio.didomenico@unina.it>
# Description: AXI System Cache needed to manage exclusive acceses

create_ip -name system_cache -vendor xilinx.com -library ip -version 5.0 -module_name $::env(IP_NAME)

set_property -dict [list \
  CONFIG.C_CACHE_LINE_LENGTH {32} \
  CONFIG.C_FREQ {250} \
  CONFIG.C_CACHE_SIZE {32768} \
  CONFIG.C_M0_AXI_DATA_WIDTH {512} \
  CONFIG.C_S0_AXI_GEN_DATA_WIDTH {64} \
  CONFIG.C_NUM_OPTIMIZED_PORTS {0} \
  CONFIG.C_NUM_GENERIC_PORTS {1} \
  CONFIG.C_S0_AXI_GEN_ID_WIDTH $::env(MBUS_ID_WIDTH) \
  CONFIG.C_S0_AXI_GEN_ADDR_WIDTH $::env(MBUS_ADDR_WIDTH) \
  CONFIG.C_BASEADDR {0x30000} \
  CONFIG.C_HIGHADDR {0x7ffff} \
  CONFIG.C_ENABLE_EXCLUSIVE {1} \
  CONFIG.C_CCIX0_CACHE_LINE_SIZE {128} \
] [get_ips $::env(IP_NAME)]

set_property CONFIG.C_M0_AXI_THREAD_ID_WIDTH  $::env(MBUS_ID_WIDTH)       [get_ips $::env(IP_NAME)]
set_property CONFIG.C_M0_AXI_ADDR_WIDTH $::env(MBUS_ADDR_WIDTH)  [get_ips $::env(IP_NAME)]









