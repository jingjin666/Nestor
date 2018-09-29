#ifndef CPU_H
#define CPU_H
#include  <os_cpu_def.h>

typedef            void        CPU_VOID;
typedef            char        CPU_CHAR;                        /*  8-bit character                                     */
typedef  unsigned  char        CPU_BOOLEAN;                     /*  8-bit boolean or logical                            */
typedef  unsigned  char        CPU_INT08U;                      /*  8-bit unsigned integer                              */
typedef    signed  char        CPU_INT08S;                      /*  8-bit   signed integer                              */
typedef  unsigned  short       CPU_INT16U;                      /* 16-bit unsigned integer                              */
typedef    signed  short       CPU_INT16S;                      /* 16-bit   signed integer                              */
typedef  unsigned  int         CPU_INT32U;                      /* 32-bit unsigned integer                              */
typedef    signed  int         CPU_INT32S;                      /* 32-bit   signed integer                              */
typedef  unsigned  long  long  CPU_INT64U;                      /* 64-bit unsigned integer                              */
typedef    signed  long  long  CPU_INT64S;                      /* 64-bit   signed integer                              */

typedef            float       CPU_FP32;                        /* 32-bit floating point                                */
typedef            double      CPU_FP64;                        /* 64-bit floating point                                */

typedef  volatile  CPU_INT08U  CPU_REG08;                       /*  8-bit register                                      */
typedef  volatile  CPU_INT16U  CPU_REG16;                       /* 16-bit register                                      */
typedef  volatile  CPU_INT32U  CPU_REG32;                       /* 32-bit register                                      */
typedef  volatile  CPU_INT64U  CPU_REG64;                       /* 64-bit register                                      */


typedef            void      (*CPU_FNCT_VOID)(void);            /* See Note #2a.                                        */
typedef            void      (*CPU_FNCT_PTR )(void *p_obj);     /* See Note #2b.                                        */

/*
*********************************************************************************************************
*                                       CPU WORD CONFIGURATION
*
* Note(s) : (1) Configure CPU_CFG_ADDR_SIZE, CPU_CFG_DATA_SIZE, & CPU_CFG_DATA_SIZE_MAX with CPU's &/or 
*               compiler's word sizes :
*
*                   CPU_WORD_SIZE_08             8-bit word size
*                   CPU_WORD_SIZE_16            16-bit word size
*                   CPU_WORD_SIZE_32            32-bit word size
*                   CPU_WORD_SIZE_64            64-bit word size
*
*           (2) Configure CPU_CFG_ENDIAN_TYPE with CPU's data-word-memory order :
*
*               (a) CPU_ENDIAN_TYPE_BIG         Big-   endian word order (CPU words' most  significant
*                                                                         octet @ lowest memory address)
*               (b) CPU_ENDIAN_TYPE_LITTLE      Little-endian word order (CPU words' least significant
*                                                                         octet @ lowest memory address)
*********************************************************************************************************
*/

                                                                /* Define  CPU         word sizes (see Note #1) :       */
#define  CPU_CFG_ADDR_SIZE              CPU_WORD_SIZE_32        /* Defines CPU address word size  (in octets).          */
#define  CPU_CFG_DATA_SIZE              CPU_WORD_SIZE_32        /* Defines CPU data    word size  (in octets).          */
#define  CPU_CFG_DATA_SIZE_MAX          CPU_WORD_SIZE_64        /* Defines CPU maximum word size  (in octets).          */

#define  CPU_CFG_ENDIAN_TYPE            CPU_ENDIAN_TYPE_LITTLE  /* Defines CPU data    word-memory order (see Note #2). */

typedef CPU_INT32U CPU_ADDR;

 /* 堆栈数据类型重定义 */
typedef CPU_INT32U CPU_STK;
typedef CPU_ADDR   CPU_STK_SIZE;

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void CPU_IntDis(void);/* 在 os_cpu_a.s 中实现 */


/*
*********************************************************************************************************
*                                          INTERRUPT SOURCES
*********************************************************************************************************
*/

#define  CPU_INT_STK_PTR                                   0u
#define  CPU_INT_RESET                                     1u
#define  CPU_INT_NMI                                       2u
#define  CPU_INT_HFAULT                                    3u
#define  CPU_INT_MEM                                       4u
#define  CPU_INT_BUSFAULT                                  5u
#define  CPU_INT_USAGEFAULT                                6u
#define  CPU_INT_RSVD_07                                   7u
#define  CPU_INT_RSVD_08                                   8u
#define  CPU_INT_RSVD_09                                   9u
#define  CPU_INT_RSVD_10                                  10u
#define  CPU_INT_SVCALL                                   11u
#define  CPU_INT_DBGMON                                   12u
#define  CPU_INT_RSVD_13                                  13u
#define  CPU_INT_PENDSV                                   14u
#define  CPU_INT_SYSTICK                                  15u
#define  CPU_INT_EXT0                                     16u

/*
*********************************************************************************************************
*											 CPU REGISTERS
*********************************************************************************************************
*/

#define  CPU_REG_MPU_REG_BASE        (*((CPU_REG32 *)(0xE000ED9C)))             /* MPU Region Base Addr Reg.            */
#define  CPU_REG_MPU_REG_ATTR        (*((CPU_REG32 *)(0xE000EDA0)))             /* MPU Region Attrib & Size Reg.        */

#define  CPU_REG_DBG_CTRL            (*((CPU_REG32 *)(0xE000EDF0)))             /* Debug Halting Ctrl & Status Reg.     */
#define  CPU_REG_DBG_SELECT          (*((CPU_REG32 *)(0xE000EDF4)))             /* Debug Core Reg Selector Reg.         */
#define  CPU_REG_DBG_DATA            (*((CPU_REG32 *)(0xE000EDF8)))             /* Debug Core Reg Data     Reg.         */
#define  CPU_REG_DBG_INT             (*((CPU_REG32 *)(0xE000EDFC)))             /* Debug Except & Monitor Ctrl Reg.     */


/*
*********************************************************************************************************
*                                          CPU REGISTER BITS
*********************************************************************************************************
*/

#define  CPU_REG_NVIC_NVIC           (*((CPU_REG32 *)(0xE000E004)))             /* Int Ctrl'er Type Reg.                */
#define  CPU_REG_NVIC_ST_CTRL        (*((CPU_REG32 *)(0xE000E010)))             /* SysTick Ctrl & Status Reg.           */
#define  CPU_REG_NVIC_ST_RELOAD      (*((CPU_REG32 *)(0xE000E014)))             /* SysTick Reload      Value Reg.       */
#define  CPU_REG_NVIC_ST_CURRENT     (*((CPU_REG32 *)(0xE000E018)))             /* SysTick Current     Value Reg.       */
#define  CPU_REG_NVIC_ST_CAL         (*((CPU_REG32 *)(0xE000E01C)))             /* SysTick Calibration Value Reg.       */
	
#define  CPU_REG_NVIC_SETEN(n)       (*((CPU_REG32 *)(0xE000E100 + (n) * 4u)))  /* IRQ Set En Reg.                      */

#define  CPU_REG_NVIC_CLREN(n)       (*((CPU_REG32 *)(0xE000E180 + (n) * 4u)))  /* IRQ Clr En Reg.                      */
#define  CPU_REG_NVIC_SETPEND(n)     (*((CPU_REG32 *)(0xE000E200 + (n) * 4u)))  /* IRQ Set Pending Reg.                 */
#define  CPU_REG_NVIC_CLRPEND(n)     (*((CPU_REG32 *)(0xE000E280 + (n) * 4u)))  /* IRQ Clr Pending Reg.                 */
#define  CPU_REG_NVIC_ACTIVE(n)      (*((CPU_REG32 *)(0xE000E300 + (n) * 4u)))  /* IRQ Active Reg.                      */
#define  CPU_REG_NVIC_PRIO(n)        (*((CPU_REG32 *)(0xE000E400 + (n) * 4u)))  /* IRQ Prio Reg.                        */
	
#define  CPU_REG_NVIC_CPUID          (*((CPU_REG32 *)(0xE000ED00)))             /* CPUID Base Reg.                      */
#define  CPU_REG_NVIC_ICSR           (*((CPU_REG32 *)(0xE000ED04)))             /* Int Ctrl State  Reg.                 */
#define  CPU_REG_NVIC_VTOR           (*((CPU_REG32 *)(0xE000ED08)))             /* Vect Tbl Offset Reg.                 */
#define  CPU_REG_NVIC_AIRCR          (*((CPU_REG32 *)(0xE000ED0C)))             /* App Int/Reset Ctrl Reg.              */
#define  CPU_REG_NVIC_SCR            (*((CPU_REG32 *)(0xE000ED10)))             /* System Ctrl Reg.                     */
#define  CPU_REG_NVIC_CCR            (*((CPU_REG32 *)(0xE000ED14)))             /* Cfg    Ctrl Reg.                     */
#define  CPU_REG_NVIC_SHPRI1         (*((CPU_REG32 *)(0xE000ED18)))             /* System Handlers  4 to  7 Prio.       */
#define  CPU_REG_NVIC_SHPRI2         (*((CPU_REG32 *)(0xE000ED1C)))             /* System Handlers  8 to 11 Prio.       */
#define  CPU_REG_NVIC_SHPRI3         (*((CPU_REG32 *)(0xE000ED20)))             /* System Handlers 12 to 15 Prio.       */
#define  CPU_REG_NVIC_SHCSR          (*((CPU_REG32 *)(0xE000ED24)))             /* System Handler Ctrl & State Reg.     */
#define  CPU_REG_NVIC_CFSR           (*((CPU_REG32 *)(0xE000ED28)))             /* Configurable Fault Status Reg.       */
#define  CPU_REG_NVIC_HFSR           (*((CPU_REG32 *)(0xE000ED2C)))             /* Hard  Fault Status Reg.              */
#define  CPU_REG_NVIC_DFSR           (*((CPU_REG32 *)(0xE000ED30)))             /* Debug Fault Status Reg.              */
#define  CPU_REG_NVIC_MMFAR          (*((CPU_REG32 *)(0xE000ED34)))             /* Mem Manage Addr Reg.                 */
#define  CPU_REG_NVIC_BFAR           (*((CPU_REG32 *)(0xE000ED38)))             /* Bus Fault  Addr Reg.                 */
#define  CPU_REG_NVIC_AFSR           (*((CPU_REG32 *)(0xE000ED3C)))             /* Aux Fault Status Reg.                */
	
#define  CPU_REG_NVIC_PFR0           (*((CPU_REG32 *)(0xE000ED40)))             /* Processor Feature Reg 0.             */
#define  CPU_REG_NVIC_PFR1           (*((CPU_REG32 *)(0xE000ED44)))             /* Processor Feature Reg 1.             */
#define  CPU_REG_NVIC_DFR0           (*((CPU_REG32 *)(0xE000ED48)))             /* Debug     Feature Reg 0.             */
#define  CPU_REG_NVIC_AFR0           (*((CPU_REG32 *)(0xE000ED4C)))             /* Aux       Feature Reg 0.             */
#define  CPU_REG_NVIC_MMFR0          (*((CPU_REG32 *)(0xE000ED50)))             /* Memory Model Feature Reg 0.          */
#define  CPU_REG_NVIC_MMFR1          (*((CPU_REG32 *)(0xE000ED54)))             /* Memory Model Feature Reg 1.          */
#define  CPU_REG_NVIC_MMFR2          (*((CPU_REG32 *)(0xE000ED58)))             /* Memory Model Feature Reg 2.          */
#define  CPU_REG_NVIC_MMFR3          (*((CPU_REG32 *)(0xE000ED5C)))             /* Memory Model Feature Reg 3.          */
#define  CPU_REG_NVIC_ISAFR0         (*((CPU_REG32 *)(0xE000ED60)))             /* ISA Feature Reg 0.                   */
#define  CPU_REG_NVIC_ISAFR1         (*((CPU_REG32 *)(0xE000ED64)))             /* ISA Feature Reg 1.                   */
#define  CPU_REG_NVIC_ISAFR2         (*((CPU_REG32 *)(0xE000ED68)))             /* ISA Feature Reg 2.                   */
#define  CPU_REG_NVIC_ISAFR3         (*((CPU_REG32 *)(0xE000ED6C)))             /* ISA Feature Reg 3.                   */
#define  CPU_REG_NVIC_ISAFR4         (*((CPU_REG32 *)(0xE000ED70)))             /* ISA Feature Reg 4.                   */
#define  CPU_REG_NVIC_SW_TRIG        (*((CPU_REG32 *)(0xE000EF00)))             /* Software Trigger Int Reg.            */
	
#define  CPU_REG_MPU_TYPE            (*((CPU_REG32 *)(0xE000ED90)))             /* MPU Type Reg.                        */
#define  CPU_REG_MPU_CTRL            (*((CPU_REG32 *)(0xE000ED94)))             /* MPU Ctrl Reg.                        */
#define  CPU_REG_MPU_REG_NBR         (*((CPU_REG32 *)(0xE000ED98)))             /* MPU Region Nbr Reg.                  */
#define  CPU_REG_MPU_REG_BASE        (*((CPU_REG32 *)(0xE000ED9C)))             /* MPU Region Base Addr Reg.            */
#define  CPU_REG_MPU_REG_ATTR        (*((CPU_REG32 *)(0xE000EDA0)))             /* MPU Region Attrib & Size Reg.        */
																
#define  CPU_REG_DBG_CTRL            (*((CPU_REG32 *)(0xE000EDF0)))             /* Debug Halting Ctrl & Status Reg.     */
#define  CPU_REG_DBG_SELECT          (*((CPU_REG32 *)(0xE000EDF4)))             /* Debug Core Reg Selector Reg.         */
#define  CPU_REG_DBG_DATA            (*((CPU_REG32 *)(0xE000EDF8)))             /* Debug Core Reg Data     Reg.         */
#define  CPU_REG_DBG_INT             (*((CPU_REG32 *)(0xE000EDFC)))             /* Debug Except & Monitor Ctrl Reg.     */
																
																
/*
*********************************************************************************************************
*										   CPU REGISTER BITS
*********************************************************************************************************
*/

/* ---------- SYSTICK CTRL & STATUS REG BITS ---------- */
#define  CPU_REG_NVIC_ST_CTRL_COUNTFLAG           0x00010000
#define  CPU_REG_NVIC_ST_CTRL_CLKSOURCE           0x00000004
#define  CPU_REG_NVIC_ST_CTRL_TICKINT             0x00000002
#define  CPU_REG_NVIC_ST_CTRL_ENABLE              0x00000001

/* -------- SYSTICK CALIBRATION VALUE REG BITS -------- */
#define  CPU_REG_NVIC_ST_CAL_NOREF                0x80000000
#define  CPU_REG_NVIC_ST_CAL_SKEW                 0x40000000

/* -------------- INT CTRL STATE REG BITS ------------- */
#define  CPU_REG_NVIC_ICSR_NMIPENDSET             0x80000000
#define  CPU_REG_NVIC_ICSR_PENDSVSET              0x10000000
#define  CPU_REG_NVIC_ICSR_PENDSVCLR              0x08000000
#define  CPU_REG_NVIC_ICSR_PENDSTSET              0x04000000
#define  CPU_REG_NVIC_ICSR_PENDSTCLR              0x02000000
#define  CPU_REG_NVIC_ICSR_ISRPREEMPT             0x00800000
#define  CPU_REG_NVIC_ICSR_ISRPENDING             0x00400000
#define  CPU_REG_NVIC_ICSR_RETTOBASE              0x00000800

/* ------------- VECT TBL OFFSET REG BITS ------------- */
#define  CPU_REG_NVIC_VTOR_TBLBASE                0x20000000

/* ------------ APP INT/RESET CTRL REG BITS ----------- */
#define  CPU_REG_NVIC_AIRCR_ENDIANNESS            0x00008000
#define  CPU_REG_NVIC_AIRCR_SYSRESETREQ           0x00000004
#define  CPU_REG_NVIC_AIRCR_VECTCLRACTIVE         0x00000002
#define  CPU_REG_NVIC_AIRCR_VECTRESET             0x00000001

/* --------------- SYSTEM CTRL REG BITS --------------- */
#define  CPU_REG_NVIC_SCR_SEVONPEND               0x00000010
#define  CPU_REG_NVIC_SCR_SLEEPDEEP               0x00000004
#define  CPU_REG_NVIC_SCR_SLEEPONEXIT             0x00000002

/* ----------------- CFG CTRL REG BITS ---------------- */
#define  CPU_REG_NVIC_CCR_STKALIGN                0x00000200
#define  CPU_REG_NVIC_CCR_BFHFNMIGN               0x00000100
#define  CPU_REG_NVIC_CCR_DIV_0_TRP               0x00000010
#define  CPU_REG_NVIC_CCR_UNALIGN_TRP             0x00000008
#define  CPU_REG_NVIC_CCR_USERSETMPEND            0x00000002
#define  CPU_REG_NVIC_CCR_NONBASETHRDENA          0x00000001

/* ------- SYSTEM HANDLER CTRL & STATE REG BITS ------- */
#define  CPU_REG_NVIC_SHCSR_USGFAULTENA           0x00040000
#define  CPU_REG_NVIC_SHCSR_BUSFAULTENA           0x00020000
#define  CPU_REG_NVIC_SHCSR_MEMFAULTENA           0x00010000
#define  CPU_REG_NVIC_SHCSR_SVCALLPENDED          0x00008000
#define  CPU_REG_NVIC_SHCSR_BUSFAULTPENDED        0x00004000
#define  CPU_REG_NVIC_SHCSR_MEMFAULTPENDED        0x00002000
#define  CPU_REG_NVIC_SHCSR_USGFAULTPENDED        0x00001000
#define  CPU_REG_NVIC_SHCSR_SYSTICKACT            0x00000800
#define  CPU_REG_NVIC_SHCSR_PENDSVACT             0x00000400
#define  CPU_REG_NVIC_SHCSR_MONITORACT            0x00000100
#define  CPU_REG_NVIC_SHCSR_SVCALLACT             0x00000080
#define  CPU_REG_NVIC_SHCSR_USGFAULTACT           0x00000008
#define  CPU_REG_NVIC_SHCSR_BUSFAULTACT           0x00000002
#define  CPU_REG_NVIC_SHCSR_MEMFAULTACT           0x00000001

/* -------- CONFIGURABLE FAULT STATUS REG BITS -------- */
#define  CPU_REG_NVIC_CFSR_DIVBYZERO              0x02000000
#define  CPU_REG_NVIC_CFSR_UNALIGNED              0x01000000
#define  CPU_REG_NVIC_CFSR_NOCP                   0x00080000
#define  CPU_REG_NVIC_CFSR_INVPC                  0x00040000
#define  CPU_REG_NVIC_CFSR_INVSTATE               0x00020000
#define  CPU_REG_NVIC_CFSR_UNDEFINSTR             0x00010000
#define  CPU_REG_NVIC_CFSR_BFARVALID              0x00008000
#define  CPU_REG_NVIC_CFSR_STKERR                 0x00001000
#define  CPU_REG_NVIC_CFSR_UNSTKERR               0x00000800
#define  CPU_REG_NVIC_CFSR_IMPRECISERR            0x00000400
#define  CPU_REG_NVIC_CFSR_PRECISERR              0x00000200
#define  CPU_REG_NVIC_CFSR_IBUSERR                0x00000100
#define  CPU_REG_NVIC_CFSR_MMARVALID              0x00000080
#define  CPU_REG_NVIC_CFSR_MSTKERR                0x00000010
#define  CPU_REG_NVIC_CFSR_MUNSTKERR              0x00000008
#define  CPU_REG_NVIC_CFSR_DACCVIOL               0x00000002
#define  CPU_REG_NVIC_CFSR_IACCVIOL               0x00000001

/* ------------ HARD FAULT STATUS REG BITS ------------ */
#define  CPU_REG_NVIC_HFSR_DEBUGEVT               0x80000000
#define  CPU_REG_NVIC_HFSR_FORCED                 0x40000000
#define  CPU_REG_NVIC_HFSR_VECTTBL                0x00000002

/* ------------ DEBUG FAULT STATUS REG BITS ----------- */
#define  CPU_REG_NVIC_DFSR_EXTERNAL               0x00000010
#define  CPU_REG_NVIC_DFSR_VCATCH                 0x00000008
#define  CPU_REG_NVIC_DFSR_DWTTRAP                0x00000004
#define  CPU_REG_NVIC_DFSR_BKPT                   0x00000002
#define  CPU_REG_NVIC_DFSR_HALTED                 0x00000001


/*
*********************************************************************************************************
*                                          CPU REGISTER MASK
*********************************************************************************************************
*/

#define  CPU_MSK_NVIC_ICSR_VECT_ACTIVE            0x000001FF

#endif /* CPU_H */
