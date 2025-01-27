#pragma once
/**
 * this module has the purpose of initializing a multi-spi interface and
 * executing device specific commands, with the final objective of interacting
 * with memory modules.
 *
 * -V
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ext_error_codes.h"
#include "ext_typedefs.h"
#include "hwlist_agent.h"
#include "stm32l4r9xx.h"

// memory device include
#include "stm32l4r9_module_mspi_mt29.h"

/**
 * the module shall not include board/device specific informations,
 * therefore board cmsis headers are not included
 *
 * the methods used for board/device implementation shall be overloaded,
 * therefore defined as a standard interface
 * INCLUDE HERE:
 * - Board specific mspi header file
 * - Device specific mspi header file
 */ /** the following interfaces have been defined:
 * BOARD
 * void mspi_gpio_cfg(void)
 * void mspi_sys_cfg(void)
 * DEVICE
 * void mspi_dev_cfg(void)
 *
 * -V
 */

struct mspi_cmd {
  /* this structure is the command specification
   */

  uint8_t is_double_mem;

  u32 fun_mode;
  u32 instr_mode;
  u32 addr_mode;
  u32 data_mode;

  u32 addr_size;
  u32 data_size;
  u32 dummy_size;

  unsigned int instr_cmd;
  unsigned int addr_cmd;
  unsigned int data_cmd;
  u8 data_cmd_embedded;

  u32 autopoll_mask;
  u32 autopoll_match;
};

struct mspi_interface {
  /* device characteristics */
  u8 interface_select;
  u8 flash_select;
  u8 is_double_mem;

  u32 speed_hz;
  u32 timeout;

  /* transfer context */
  u8 use_dma;
  u32 *data_ptr;
  u32 size_tr; // size in bytes of the transfer

  // u32* dma_peripheral_addr; -- not needed set up at configuration of dma
  // controller u32* dma_memory_addr; -- not needed set up at configuration of
  // dma controller

  // function handling function pointers
  void (*registers_cleanup)(void);
  u16 (*wait_busy)(void);
};

/**
 * --------------------------------------------------------------------------- *
 *       INTERFACE SPECIFIC METHODS
 * --------------------------------------------------------------------------- *
 */
u32 mspi_init(void);
u32 mspi1_gpio_init(void);
u32 mspi2_gpio_init(void);
u32 mspi1_peripheral_init(void);
u32 mspi2_peripheral_init(void);
u32 mspi_mux_init(void);
u32 mspi1_dev_cfg(void);
u32 mspi2_dev_cfg(void);
u32 mspi1_dmamux_init(void);
u32 mspi2_dmamux_init(void);
u32 mspi1_dmachannel_push_init(void);
void mspi1_dmachannel_pull_init(void);
void mspi2_dmachannel_push_init(void);
void mspi2_dmachannel_pull_init(void);

// methods declarations
void mspi_interface_cleanup(struct mspi_interface);
u16 mspi_interface_wait_busy(struct mspi_interface);
u16 mspi_transfer(struct mspi_interface,
                  struct mspi_cmd (*device_fun_handler)(void *), void *);
