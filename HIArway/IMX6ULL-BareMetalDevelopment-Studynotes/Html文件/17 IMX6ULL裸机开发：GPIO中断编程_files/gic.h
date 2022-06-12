#ifndef   __GIC_H__
#define   __GIC_H__

/*form core_ca7.h*/
#define     __I     volatile const           /*!< defines 'read only' permissions */
#define     __O     volatile                 /*!< defines 'write only' permissions*/
#define     __IO    volatile                 /*!< defines 'read / write' permissions*/

/* form core_ca7.h, following defines should be used for structure members */
#define     __IM     volatile const      /*! Defines 'read only' structure member permissions */
#define     __OM     volatile            /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile            /*! Defines 'read / write' structure member permissions */

typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef signed long long s64;
typedef unsigned long long u64;


typedef		u8		uint8_t;
typedef		s8		int8_t;
typedef		u16		uint16_t;
typedef		s16		int16_t;
typedef		u32		uint32_t;
typedef		s32		int32_t;
typedef		u64		uint64_t;
typedef		s64		int64_t;


typedef struct
{
        uint32_t RESERVED0[1024];
  __IOM uint32_t D_CTLR;                 /*!< Offset: 0x1000 (R/W) Distributor Control Register */
  __IM  uint32_t D_TYPER;                /*!< Offset: 0x1004 (R/ )  Interrupt Controller Type Register */
  __IM  uint32_t D_IIDR;                 /*!< Offset: 0x1008 (R/ )  Distributor Implementer Identification Register */
        uint32_t RESERVED1[29];
  __IOM uint32_t D_IGROUPR[16];          /*!< Offset: 0x1080 - 0x0BC (R/W) Interrupt Group Registers */
        uint32_t RESERVED2[16];
  __IOM uint32_t D_ISENABLER[16];        /*!< Offset: 0x1100 - 0x13C (R/W) Interrupt Set-Enable Registers */
        uint32_t RESERVED3[16];
  __IOM uint32_t D_ICENABLER[16];        /*!< Offset: 0x1180 - 0x1BC (R/W) Interrupt Clear-Enable Registers */
        uint32_t RESERVED4[16];
  __IOM uint32_t D_ISPENDR[16];          /*!< Offset: 0x1200 - 0x23C (R/W) Interrupt Set-Pending Registers */
        uint32_t RESERVED5[16];
  __IOM uint32_t D_ICPENDR[16];          /*!< Offset: 0x1280 - 0x2BC (R/W) Interrupt Clear-Pending Registers */
        uint32_t RESERVED6[16];
  __IOM uint32_t D_ISACTIVER[16];        /*!< Offset: 0x1300 - 0x33C (R/W) Interrupt Set-Active Registers */
        uint32_t RESERVED7[16];
  __IOM uint32_t D_ICACTIVER[16];        /*!< Offset: 0x1380 - 0x3BC (R/W) Interrupt Clear-Active Registers */
        uint32_t RESERVED8[16];
  __IOM uint8_t  D_IPRIORITYR[512];      /*!< Offset: 0x1400 - 0x5FC (R/W) Interrupt Priority Registers */
        uint32_t RESERVED9[128];
  __IOM uint8_t  D_ITARGETSR[512];       /*!< Offset: 0x1800 - 0x9FC (R/W) Interrupt Targets Registers */
        uint32_t RESERVED10[128];
  __IOM uint32_t D_ICFGR[32];            /*!< Offset: 0x1C00 - 0xC7C (R/W) Interrupt configuration registers */
        uint32_t RESERVED11[32];
  __IM  uint32_t D_PPISR;                /*!< Offset: 0x1D00 (R/ ) Private Peripheral Interrupt Status Register */
  __IM  uint32_t D_SPISR[15];            /*!< Offset: 0x1D04 - 0xD3C (R/ ) Shared Peripheral Interrupt Status Registers */
        uint32_t RESERVED12[112];
  __OM  uint32_t D_SGIR;                 /*!< Offset: 0x1F00 ( /W) Software Generated Interrupt Register */
        uint32_t RESERVED13[3];
  __IOM uint8_t  D_CPENDSGIR[16];        /*!< Offset: 0x1F10 - 0xF1C (R/W) SGI Clear-Pending Registers */
  __IOM uint8_t  D_SPENDSGIR[16];        /*!< Offset: 0x1F20 - 0xF2C (R/W) SGI Set-Pending Registers */
        uint32_t RESERVED14[40];
  __IM  uint32_t D_PIDR4;                /*!< Offset: 0x1FD0 (R/ ) Peripheral ID4 Register */
  __IM  uint32_t D_PIDR5;                /*!< Offset: 0x1FD4 (R/ ) Peripheral ID5 Register */
  __IM  uint32_t D_PIDR6;                /*!< Offset: 0x1FD8 (R/ ) Peripheral ID6 Register */
  __IM  uint32_t D_PIDR7;                /*!< Offset: 0x1FDC (R/ ) Peripheral ID7 Register */
  __IM  uint32_t D_PIDR0;                /*!< Offset: 0x1FE0 (R/ ) Peripheral ID0 Register */
  __IM  uint32_t D_PIDR1;                /*!< Offset: 0x1FE4 (R/ ) Peripheral ID1 Register */
  __IM  uint32_t D_PIDR2;                /*!< Offset: 0x1FE8 (R/ ) Peripheral ID2 Register */
  __IM  uint32_t D_PIDR3;                /*!< Offset: 0x1FEC (R/ ) Peripheral ID3 Register */
  __IM  uint32_t D_CIDR0;                /*!< Offset: 0x1FF0 (R/ ) Component ID0 Register */
  __IM  uint32_t D_CIDR1;                /*!< Offset: 0x1FF4 (R/ ) Component ID1 Register */
  __IM  uint32_t D_CIDR2;                /*!< Offset: 0x1FF8 (R/ ) Component ID2 Register */
  __IM  uint32_t D_CIDR3;                /*!< Offset: 0x1FFC (R/ ) Component ID3 Register */

  __IOM uint32_t C_CTLR;                 /*!< Offset: 0x2000 (R/W) CPU Interface Control Register */
  __IOM uint32_t C_PMR;                  /*!< Offset: 0x2004 (R/W) Interrupt Priority Mask Register */
  __IOM uint32_t C_BPR;                  /*!< Offset: 0x2008 (R/W) Binary Point Register */
  __IM  uint32_t C_IAR;                  /*!< Offset: 0x200C (R/ ) Interrupt Acknowledge Register */
  __OM  uint32_t C_EOIR;                 /*!< Offset: 0x2010 ( /W) End Of Interrupt Register */
  __IM  uint32_t C_RPR;                  /*!< Offset: 0x2014 (R/ ) Running Priority Register */
  __IM  uint32_t C_HPPIR;                /*!< Offset: 0x2018 (R/ ) Highest Priority Pending Interrupt Register */
  __IOM uint32_t C_ABPR;                 /*!< Offset: 0x201C (R/W) Aliased Binary Point Register */
  __IM  uint32_t C_AIAR;                 /*!< Offset: 0x2020 (R/ ) Aliased Interrupt Acknowledge Register */
  __OM  uint32_t C_AEOIR;                /*!< Offset: 0x2024 ( /W) Aliased End Of Interrupt Register */
  __IM  uint32_t C_AHPPIR;               /*!< Offset: 0x2028 (R/ ) Aliased Highest Priority Pending Interrupt Register */
        uint32_t RESERVED15[41];
  __IOM uint32_t C_APR0;                 /*!< Offset: 0x20D0 (R/W) Active Priority Register */
        uint32_t RESERVED16[3];
  __IOM uint32_t C_NSAPR0;               /*!< Offset: 0x20E0 (R/W) Non-secure Active Priority Register */
        uint32_t RESERVED17[6];
  __IM  uint32_t C_IIDR;                 /*!< Offset: 0x20FC (R/ ) CPU Interface Identification Register */
        uint32_t RESERVED18[960];
  __OM  uint32_t C_DIR;                  /*!< Offset: 0x3000 ( /W) Deactivate Interrupt Register */
} GIC_Type;


void gic_init(void);
void gic_enable_irq(unsigned int nr);
void gic_disable_irq(unsigned int nr);
int get_gic_irq(void);
int clear_gic_irq(int nr);

#endif

