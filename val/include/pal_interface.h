/** @file
 * Copyright (c) 2016-2019, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#ifndef __PAL_INTERFACE_H__
#define __PAL_INTERFACE_H__

#ifdef TARGET_LINUX
#include <linux/slab.h>
#endif

#ifdef TARGET_LINUX
  typedef char          char8_t;
  typedef long long int addr_t;
#else
  typedef CHAR8  char8_t;
  typedef CHAR16 char16_t;
  typedef UINT8  uint8_t;
  typedef UINT16 uint16_t;
  typedef UINT32 uint32_t;
  typedef UINT64 uint64_t;
  typedef UINT64 addr_t;

#endif

#define TIMEOUT_LARGE    0x1000000
#define TIMEOUT_MEDIUM   0x100000
#define TIMEOUT_SMALL    0x1000

#define PCIE_READ_ERR  -1

/**  PE Test related Definitions **/

/**
  @brief  number of PEs discovered
**/
typedef struct {
  uint32_t num_of_pe;
}PE_INFO_HDR;

/**
  @brief  structure instance for PE entry
**/
typedef struct {
  uint32_t   pe_num;    ///< PE Index
  uint32_t   attr;      ///< PE attributes
  uint64_t   mpidr;     ///< PE MPIDR
  uint32_t   pmu_gsiv;  ///< PMU Interrupt ID
}PE_INFO_ENTRY;

typedef struct {
  PE_INFO_HDR    header;
  PE_INFO_ENTRY  pe_info[];
}PE_INFO_TABLE;

void pal_pe_create_info_table(PE_INFO_TABLE *pe_info_table);

/**
  @brief  Structure to Pass SMC arguments. Return data is also filled into
          the same structure.
**/
typedef struct {
  uint64_t  Arg0;
  uint64_t  Arg1;
  uint64_t  Arg2;
  uint64_t  Arg3;
  uint64_t  Arg4;
  uint64_t  Arg5;
  uint64_t  Arg6;
  uint64_t  Arg7;
} ARM_SMC_ARGS;

void pal_pe_call_smc(ARM_SMC_ARGS *args);
void pal_pe_execute_payload(ARM_SMC_ARGS *args);
uint32_t pal_pe_install_esr(uint32_t exception_type, void (*esr)(uint64_t, void *));
/* ********** PE INFO END **********/


/** GIC Tests Related definitions **/

/**
  @brief  GIC Info header - Summary of GIC subsytem
**/
typedef struct {
  uint32_t gic_version;
  uint32_t num_gicd;
  uint32_t num_gicrd;
  uint32_t num_its;
}GIC_INFO_HDR;

typedef enum {
  ENTRY_TYPE_CPUIF = 0x1000,
  ENTRY_TYPE_GICD,
  ENTRY_TYPE_GICRD,
  ENTRY_TYPE_GICITS
}GIC_INFO_TYPE_e;

/* Interrupt Trigger Type */
typedef enum {
  INTR_TRIGGER_INFO_LEVEL_LOW,
  INTR_TRIGGER_INFO_LEVEL_HIGH,
  INTR_TRIGGER_INFO_EDGE_FALLING,
  INTR_TRIGGER_INFO_EDGE_RISING
}INTR_TRIGGER_INFO_TYPE_e;

/**
  @brief  structure instance for GIC entry
**/
typedef struct {
  uint32_t type;
  uint64_t base;
}GIC_INFO_ENTRY;

/**
  @brief  GIC Information Table
**/
typedef struct {
  GIC_INFO_HDR   header;
  GIC_INFO_ENTRY gic_info[];  ///< Array of Information blocks - instantiated for each GIC type
}GIC_INFO_TABLE;

void     pal_gic_create_info_table(GIC_INFO_TABLE *gic_info_table);
uint32_t pal_gic_install_isr(uint32_t int_id, void (*isr)(void));
void pal_gic_end_of_interrupt(uint32_t int_id);
uint32_t pal_gic_request_irq(unsigned int irq_num, unsigned int mapped_irq_num, void *isr);
void pal_gic_free_interrupt(unsigned int irq_num, unsigned int mapped_irq_num);
uint32_t pal_gic_set_intr_trigger(uint32_t int_id, INTR_TRIGGER_INFO_TYPE_e trigger_type);


/** Timer tests related definitions **/

/**
  @brief  GIC Info header - Summary of Timer subsytem
**/
typedef struct {
  uint32_t s_el1_timer_flag;
  uint32_t ns_el1_timer_flag;
  uint32_t el2_timer_flag;
  uint32_t el2_virt_timer_flag;
  uint32_t s_el1_timer_gsiv;
  uint32_t ns_el1_timer_gsiv;
  uint32_t el2_timer_gsiv;
  uint32_t virtual_timer_flag;
  uint32_t virtual_timer_gsiv;
  uint32_t el2_virt_timer_gsiv;
  uint32_t num_platform_timer;
  uint32_t num_watchdog;
  uint32_t sys_timer_status;
}TIMER_INFO_HDR;

#define TIMER_TYPE_SYS_TIMER 0x2001

/**
  @brief  structure instance for TIMER entry
**/
typedef struct {
  uint32_t type;
  uint32_t timer_count;
  uint64_t block_cntl_base;
  uint8_t  frame_num[8];
  uint64_t GtCntBase[8];
  uint64_t GtCntEl0Base[8];
  uint32_t gsiv[8];
  uint32_t virt_gsiv[8];
  uint32_t flags[8];
}TIMER_INFO_GTBLOCK;

typedef struct {
  TIMER_INFO_HDR     header;
  TIMER_INFO_GTBLOCK gt_info[];
}TIMER_INFO_TABLE;

void pal_timer_create_info_table(TIMER_INFO_TABLE *timer_info_table);

/** Watchdog tests related definitions **/

/**
  @brief  Watchdog Info header - Summary of Watchdog subsytem
**/
typedef struct {
  uint32_t num_wd;  ///< number of Watchdogs present in the system
}WD_INFO_HDR;

/**
  @brief  structure instance for Watchdog entry
**/
typedef struct {
  uint64_t wd_ctrl_base;     ///< Watchdog Control Register Frame
  uint64_t wd_refresh_base;  ///< Watchdog Refresh Register Frame
  uint32_t wd_gsiv;          ///< Watchdog Interrupt ID
  uint32_t wd_flags;
}WD_INFO_BLOCK;

/**
  @brief Watchdog Info Table
**/
typedef struct {
  WD_INFO_HDR    header;
  WD_INFO_BLOCK  wd_info[];  ///< Array of Information blocks - instantiated for each WD Controller
}WD_INFO_TABLE;

void pal_wd_create_info_table(WD_INFO_TABLE  *wd_table);


/* PCIe Tests related definitions */

/**
  @brief PCI Express Info Table
**/
typedef struct {
  addr_t   ecam_base;     ///< ECAM Base address
  uint32_t segment_num;   ///< Segment number of this ECAM
  uint32_t start_bus_num; ///< Start Bus number for this ecam space
  uint32_t end_bus_num;   ///< Last Bus number
}PCIE_INFO_BLOCK;

typedef struct {
  uint32_t num_entries;
  PCIE_INFO_BLOCK  block[];
}PCIE_INFO_TABLE;

uint64_t pal_pcie_get_mcfg_ecam(void);
void     pal_pcie_create_info_table(PCIE_INFO_TABLE *PcieTable);
uint32_t pal_pcie_read_cfg(uint32_t bdf, uint32_t offset, uint32_t *data);
uint32_t pal_pcie_get_bdf_wrapper(uint32_t class_code, uint32_t start_bdf);
void *pal_pci_bdf_to_dev(uint32_t bdf);
void pal_pci_read_config_byte(uint32_t bdf, uint8_t offset, uint8_t *val);
void pal_pci_write_config_byte(uint32_t bdf, uint8_t offset, uint8_t val);
void pal_pcie_read_ext_cap_word(uint32_t seg, uint32_t bus, uint32_t dev, uint32_t fn,
                                uint32_t ext_cap_id, uint8_t offset, uint16_t *val);
uint32_t pal_pcie_get_pcie_type(uint32_t seg, uint32_t bus, uint32_t dev, uint32_t fn);
uint32_t pal_pcie_p2p_support(uint32_t seg, uint32_t bus, uint32_t dev, uint32_t fn);
uint32_t pal_pcie_multifunction_support(uint32_t seg, uint32_t bus, uint32_t dev, uint32_t fn);

/**
  @brief  Instance of SMMU INFO block
**/
typedef struct {
  uint32_t arch_major_rev;  ///< Version 1 or 2 or 3
  addr_t base;              ///< SMMU Controller base address
}SMMU_INFO_BLOCK;

typedef struct {
  uint32_t segment;
  uint32_t ats_attr;
  uint32_t cca;          //Cache Coherency Attribute
}IOVIRT_RC_INFO_BLOCK;

typedef struct {
  uint64_t base;
  uint32_t overflow_gsiv;
  uint32_t node_ref;
} IOVIRT_PMCG_INFO_BLOCK;

typedef enum {
  IOVIRT_NODE_ITS_GROUP = 0x00,
  IOVIRT_NODE_NAMED_COMPONENT = 0x01,
  IOVIRT_NODE_PCI_ROOT_COMPLEX = 0x02,
  IOVIRT_NODE_SMMU = 0x03,
  IOVIRT_NODE_SMMU_V3 = 0x04,
  IOVIRT_NODE_PMCG = 0x05
}IOVIRT_NODE_TYPE;

typedef enum {
  IOVIRT_FLAG_DEVID_OVERLAP_SHIFT,
  IOVIRT_FLAG_STRID_OVERLAP_SHIFT,
  IOVIRT_FLAG_SMMU_CTX_INT_SHIFT,
}IOVIRT_FLAG_SHIFT;

typedef struct {
  uint32_t input_base;
  uint32_t id_count;
  uint32_t output_base;
  uint32_t output_ref;
}ID_MAP;

typedef union {
  uint32_t id[4];
  ID_MAP map;
}NODE_DATA_MAP;

#define MAX_NAMED_COMP_LENGTH 256

typedef union {
  char name[MAX_NAMED_COMP_LENGTH];
  IOVIRT_RC_INFO_BLOCK rc;
  IOVIRT_PMCG_INFO_BLOCK pmcg;
  uint32_t its_count;
  SMMU_INFO_BLOCK smmu;
}NODE_DATA;

typedef struct {
  uint32_t type;
  uint32_t num_data_map;
  NODE_DATA data;
  uint32_t flags;
  NODE_DATA_MAP data_map[];
}IOVIRT_BLOCK;

#define IOVIRT_NEXT_BLOCK(b) (IOVIRT_BLOCK *)((uint8_t*)(&b->data_map[0]) + b->num_data_map * sizeof(NODE_DATA_MAP))
#define IOVIRT_CCA_MASK ~((uint32_t)0)

typedef struct {
  uint32_t num_blocks;
  uint32_t num_smmus;
  uint32_t num_pci_rcs;
  uint32_t num_named_components;
  uint32_t num_its_groups;
  uint32_t num_pmcgs;
  IOVIRT_BLOCK blocks[];
}IOVIRT_INFO_TABLE;

void pal_iovirt_create_info_table(IOVIRT_INFO_TABLE *iovirt);
uint32_t pal_iovirt_check_unique_ctx_intid(uint64_t smmu_block);
uint32_t pal_iovirt_unique_rid_strid_map(uint64_t rc_block);

/**
  @brief SMMU Info Table
**/
typedef struct {
  uint32_t smmu_num_ctrl;       ///< Number of SMMU Controllers in the system
  SMMU_INFO_BLOCK smmu_block[]; ///< Array of Information blocks - instantiated for each SMMU Controller
}SMMU_INFO_TABLE;

void     pal_smmu_create_info_table(SMMU_INFO_TABLE *smmu_info_table);
uint32_t pal_smmu_check_device_iova(void *port, uint64_t dma_addr);
void     pal_smmu_device_start_monitor_iova(void *port);
void     pal_smmu_device_stop_monitor_iova(void *port);
uint32_t pal_smmu_max_pasids(uint64_t smmu_base);


/** Peripheral Tests related definitions **/

/**
  @brief  Summary of Peripherals in the system
**/
typedef struct {
  uint32_t    num_usb;   ///< Number of USB  Controllers
  uint32_t    num_sata;  ///< Number of SATA Controllers
  uint32_t    num_uart;  ///< Number of UART Controllers
  uint32_t    num_all;   ///< Number of all PCI Controllers
}PERIPHERAL_INFO_HDR;

typedef enum {
  PERIPHERAL_TYPE_USB = 0x2000,
  PERIPHERAL_TYPE_SATA,
  PERIPHERAL_TYPE_UART,
  PERIPHERAL_TYPE_OTHER,
  PERIPHERAL_TYPE_NONE
}PER_INFO_TYPE_e;

/**
  @brief  Instance of peripheral info
**/
typedef struct {
  PER_INFO_TYPE_e  type;  ///< PER_INFO_TYPE
  uint32_t         bdf;   ///< Bus Device Function
  uint64_t         base0; ///< Base Address of the controller
  uint64_t         base1; ///< Base Address of the controller
  uint32_t         irq;   ///< IRQ to install an ISR
  uint32_t         flags;
  uint32_t         msi;   ///< MSI Enabled
  uint32_t         msix;  ///< MSIX Enabled
  uint32_t         max_pasids;
}PERIPHERAL_INFO_BLOCK;

/**
  @brief Peripheral Info Structure
**/
typedef struct {
  PERIPHERAL_INFO_HDR     header;
  PERIPHERAL_INFO_BLOCK   info[]; ///< Array of Information blocks - instantiated for each peripheral
}PERIPHERAL_INFO_TABLE;

void  pal_peripheral_create_info_table(PERIPHERAL_INFO_TABLE *per_info_table);
uint32_t pal_peripheral_is_pcie(uint32_t seg, uint32_t bus, uint32_t dev, uint32_t fn);

/**
  @brief MSI(X) controllers info structure
**/

typedef struct {
  uint32_t         vector_upper_addr; ///< Bus Device Function
  uint32_t         vector_lower_addr; ///< Base Address of the controller
  uint32_t         vector_data;       ///< Base Address of the controller
  uint32_t         vector_control;    ///< IRQ to install an ISR
  uint32_t         vector_irq_base;   ///< Base IRQ for the vectors in the block
  uint32_t         vector_n_irqs;     ///< Number of irq vectors in the block
  uint32_t         vector_mapped_irq_base; ///< Mapped IRQ number base for this MSI
}PERIPHERAL_VECTOR_BLOCK;

typedef struct PERIPHERAL_VECTOR_LIST_STRUCT
{
  PERIPHERAL_VECTOR_BLOCK vector;
  struct PERIPHERAL_VECTOR_LIST_STRUCT *next;
}PERIPHERAL_VECTOR_LIST;

uint32_t pal_get_msi_vectors (uint32_t seg, uint32_t bus, uint32_t dev, uint32_t fn, PERIPHERAL_VECTOR_LIST **mvector);

#define LEGACY_PCI_IRQ_CNT 4  // Legacy PCI IRQ A, B, C. and D
#define MAX_IRQ_CNT 0xFFFF    // This value is arbitrary and may have to be adjusted

typedef struct {
  uint32_t  irq_list[MAX_IRQ_CNT];
  uint32_t  irq_count;
} PERIFERAL_IRQ_LIST;

typedef struct {
  PERIFERAL_IRQ_LIST  legacy_irq_map[LEGACY_PCI_IRQ_CNT];
} PERIPHERAL_IRQ_MAP;

#define DEVCTL_SNOOP_BIT 11        // Device control register no snoop bit

uint32_t pal_pcie_get_legacy_irq_map(uint32_t seg, uint32_t bus, uint32_t dev, uint32_t fn, PERIPHERAL_IRQ_MAP *irq_map);
uint32_t pal_pcie_is_device_behind_smmu(uint32_t seg, uint32_t bus, uint32_t dev, uint32_t fn);
uint32_t pal_pcie_get_root_port_bdf(uint32_t *seg, uint32_t *bus, uint32_t *dev, uint32_t *func);
uint32_t pal_pcie_get_device_type(uint32_t seg, uint32_t bus, uint32_t dev, uint32_t fn);
uint32_t pal_pcie_get_snoop_bit(uint32_t seg, uint32_t bus, uint32_t dev, uint32_t fn);
uint32_t pal_pcie_get_dma_support(uint32_t seg, uint32_t bus, uint32_t dev, uint32_t fn);
uint32_t pal_pcie_get_dma_coherent(uint32_t seg, uint32_t bus, uint32_t dev, uint32_t fn);
uint32_t pal_pcie_is_devicedma_64bit(uint32_t seg, uint32_t bus, uint32_t dev, uint32_t fn);
uint32_t pal_pcie_scan_bridge_devices_and_check_memtype(uint32_t seg, uint32_t bus,
                                                            uint32_t dev, uint32_t fn);
/**
  @brief DMA controllers info structure
**/
typedef enum {
  DMA_TYPE_USB  =  0x2000,
  DMA_TYPE_SATA,
  DMA_TYPE_OTHER,
}DMA_INFO_TYPE_e;

typedef struct {
  DMA_INFO_TYPE_e type;
  void            *target;   ///< The actual info stored in these pointers is implementation specific.
  void            *port;
  void            *host;     // It will be used only by PAL. hence void.
  uint32_t        flags;
}DMA_INFO_BLOCK;

typedef struct {
  uint32_t         num_dma_ctrls;
  DMA_INFO_BLOCK   info[];    ///< Array of information blocks - per DMA controller
}DMA_INFO_TABLE;

void pal_dma_create_info_table(DMA_INFO_TABLE *dma_info_table);
uint32_t pal_dma_start_from_device(void *dma_target_buf, uint32_t length,
                          void *host, void *dev);
uint64_t
pal_dma_mem_alloc(void **buffer, uint32_t length, void *dev, uint32_t flags);

void
pal_dma_mem_free(void *buffer, addr_t mem_dma, unsigned int length, void *port, unsigned int flags);

uint32_t pal_dma_start_to_device(void *dma_source_buf, uint32_t length,
                         void *host, void *target, uint32_t timeout);

void pal_dma_scsi_get_dma_addr(void *port, void *dma_addr, uint32_t *dma_len);
int pal_dma_mem_get_attrs(void *buf, uint32_t *attr, uint32_t *sh);


/* Memory INFO table */
typedef enum {
  MEMORY_TYPE_DEVICE = 0x1000,
  MEMORY_TYPE_NORMAL,
  MEMORY_TYPE_RESERVED,
  MEMORY_TYPE_NOT_POPULATED,
  MEMORY_TYPE_LAST_ENTRY
}MEM_INFO_TYPE_e;

typedef struct {
  MEM_INFO_TYPE_e type;
  uint64_t        phy_addr;
  uint64_t        virt_addr;
  uint64_t        size;
  uint64_t        flags;  //To Indicate Cacheablility etc..
}MEM_INFO_BLOCK;

typedef struct {
  uint64_t  dram_base;
  uint64_t  dram_size;
  MEM_INFO_BLOCK  info[];
} MEMORY_INFO_TABLE;

void  pal_memory_create_info_table(MEMORY_INFO_TABLE *memoryInfoTable);
uint64_t pal_memory_ioremap(void *addr, uint32_t size, uint32_t attr);
void pal_memory_unmap(void *addr);


/* Common Definitions */
void     pal_print(char8_t *string, uint64_t data);
void     pal_print_raw(uint64_t addr, char8_t *string, uint64_t data);


void    *pal_mem_alloc(uint32_t size);
void    *pal_mem_alloc_coherent(void *dev, unsigned int size, void *pa);
void     pal_mem_free(void *buffer);
void     pal_mem_free_coherent(void *dev, unsigned int size, void *va, void *pa);
void    *pal_mem_virt_to_phys(void *va);

void     pal_mem_allocate_shared(uint32_t num_pe, uint32_t sizeofentry);
void     pal_mem_free_shared(void);
uint64_t pal_mem_get_shared_addr(void);

uint32_t pal_mmio_read(uint64_t addr);
void     pal_mmio_write(uint64_t addr, uint32_t data);

void     pal_pe_update_elr(void *context, uint64_t offset);
uint64_t pal_pe_get_esr(void *context);
uint64_t pal_pe_get_far(void *context);
void     pal_pe_data_cache_ops_by_va(uint64_t addr, uint32_t type);

#define CLEAN_AND_INVALIDATE  0x1
#define CLEAN                 0x2
#define INVALIDATE            0x3

/* Exerciser definitions */
#define EXERCISER_CLASSCODE 0x010203
#define MAX_ARRAY_SIZE 32
#define TEST_REG_COUNT 10
#define TEST_DDR_REGION_CNT 5

typedef struct {
    uint64_t buf[MAX_ARRAY_SIZE];
} EXERCISER_INFO_BLOCK;

typedef struct {
    uint32_t                num_exerciser_cards;
    EXERCISER_INFO_BLOCK    info[];  //Array of information blocks - per stimulus generation controller
} EXERCISER_INFO_TABLE;

typedef enum {
    EXERCISER_NUM_CARDS = 0x1
} EXERCISER_INFO_TYPE;

typedef enum {
    EDMA_NO_SUPPORT   = 0x0,
    EDMA_COHERENT     = 0x1,
    EDMA_NOT_COHERENT = 0x2,
    EDMA_FROM_DEVICE  = 0x3,
    EDMA_TO_DEVICE    = 0x4
} EXERCISER_DMA_ATTR;

typedef enum {
    SNOOP_ATTRIBUTES = 0x1,
    LEGACY_IRQ       = 0x2,
    MSIX_ATTRIBUTES  = 0x3,
    DMA_ATTRIBUTES   = 0x4,
    P2P_ATTRIBUTES   = 0x5,
    PASID_ATTRIBUTES = 0x6
} EXERCISER_PARAM_TYPE;

typedef enum {
    EXERCISER_RESET = 0x1,
    EXERCISER_ON    = 0x2,
    EXERCISER_OFF   = 0x3,
    EXERCISER_ERROR = 0x4
} EXERCISER_STATE;

typedef enum {
    START_DMA     = 0x1,
    GENERATE_MSI  = 0x2,
    GENERATE_L_INTR = 0x3,  //Legacy interrupt
    MEM_READ      = 0x4,
    MEM_WRITE     = 0x5,
    CLEAR_INTR    = 0x6,
    PASID_TLP_START = 0x7,
    PASID_TLP_STOP  = 0x8,
    NO_SNOOP_TLP_START = 0x9,
    NO_SNOOP_TLP_STOP  = 0xa
} EXERCISER_OPS;

typedef enum {
    ACCESS_TYPE_RD = 0x0,
    ACCESS_TYPE_RW = 0x1
} ECAM_REG_ATTRIBUTE;

struct ecam_reg_data {
    uint32_t offset;    //Offset into 4096 bytes ecam config reg space
    uint32_t attribute;
    uint32_t value;
};

struct exerciser_data_cfg_space {
    struct ecam_reg_data reg[TEST_REG_COUNT];
};

typedef enum {
    DEVICE_nGnRnE = 0x0,
    DEVICE_nGnRE  = 0x1,
    DEVICE_nGRE   = 0x2,
    DEVICE_GRE    = 0x3
} ARM_DEVICE_MEM;

typedef enum {
    NORMAL_NC = 0x4,
    NORMAL_WT = 0x5
} ARM_NORMAL_MEM;

typedef enum {
    MMIO_PREFETCHABLE = 0x0,
    MMIO_NON_PREFETCHABLE = 0x1
} BAR_MEM_TYPE;

struct exerciser_data_bar_space {
    void *base_addr;
    BAR_MEM_TYPE type;
};

typedef union exerciser_data {
    struct exerciser_data_cfg_space cfg_space;
    struct exerciser_data_bar_space bar_space;
} exerciser_data_t;

typedef enum {
    EXERCISER_DATA_CFG_SPACE = 0x1,
    EXERCISER_DATA_BAR0_SPACE = 0x2,
} EXERCISER_DATA_TYPE;


void pal_exerciser_create_info_table(EXERCISER_INFO_TABLE *exerciser_info_table);
uint32_t pal_exerciser_get_info(EXERCISER_INFO_TYPE type, uint32_t instance);
uint32_t pal_exerciser_set_param(EXERCISER_PARAM_TYPE type, uint64_t value1, uint64_t value2, uint32_t instance);
uint32_t pal_exerciser_get_param(EXERCISER_PARAM_TYPE type, uint64_t *value1, uint64_t *value2, uint32_t instance);
uint32_t pal_exerciser_set_state(EXERCISER_STATE state, uint64_t *value, uint32_t instance);
uint32_t pal_exerciser_get_state(EXERCISER_STATE state, uint64_t *value, uint32_t instance);
uint32_t pal_exerciser_ops(EXERCISER_OPS ops, uint64_t param, uint32_t instance);
uint32_t pal_exerciser_get_data(EXERCISER_DATA_TYPE type, exerciser_data_t *data, uint32_t instance);

#endif

