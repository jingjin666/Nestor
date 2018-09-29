#ifndef LIB_DEF_H
#define LIB_DEF_H

#define  DEF_OCTET_NBR_BITS                                8u

/* ----------------- INTEGER DEFINES ------------------ */
#define  DEF_INT_32_NBR_BITS                              32u
#define  DEF_INT_32_MASK                          0xFFFFFFFFu

#define  DEF_INT_32U_MIN_VAL                               0u
#define  DEF_INT_32U_MAX_VAL                      4294967295u

#define  DEF_INT_32S_MIN_VAL_ONES_CPL           (-2147483647)
#define  DEF_INT_32S_MAX_VAL_ONES_CPL             2147483647

#define  DEF_INT_32S_MIN_VAL                            (DEF_INT_32S_MIN_VAL_ONES_CPL - 1)
#define  DEF_INT_32S_MAX_VAL                             DEF_INT_32S_MAX_VAL_ONES_CPL

#define  DEF_INT_32U_NBR_DIG_MIN                           1u
#define  DEF_INT_32U_NBR_DIG_MAX                          10u

#define  DEF_INT_32S_NBR_DIG_MIN                          10u
#define  DEF_INT_32S_NBR_DIG_MAX                          10u


/* --------------- CPU INTEGER DEFINES ---------------- */
#define  DEF_INT_CPU_NBR_BITS                           (CPU_CFG_DATA_SIZE     * DEF_OCTET_NBR_BITS)
#define  DEF_INT_CPU_NBR_BITS_MAX                       (CPU_CFG_DATA_SIZE_MAX * DEF_OCTET_NBR_BITS)


#if   (DEF_INT_CPU_NBR_BITS == DEF_INT_32_NBR_BITS)

#define  DEF_INT_CPU_MASK                                DEF_INT_32_MASK

#define  DEF_INT_CPU_U_MIN_VAL                           DEF_INT_32U_MIN_VAL
#define  DEF_INT_CPU_U_MAX_VAL                           DEF_INT_32U_MAX_VAL

#define  DEF_INT_CPU_S_MIN_VAL                           DEF_INT_32S_MIN_VAL
#define  DEF_INT_CPU_S_MAX_VAL                           DEF_INT_32S_MAX_VAL

#define  DEF_INT_CPU_S_MIN_VAL_ONES_CPL                  DEF_INT_32S_MIN_VAL_ONES_CPL
#define  DEF_INT_CPU_S_MAX_VAL_ONES_CPL                  DEF_INT_32S_MAX_VAL_ONES_CPL

#else

#error  "CPU_CFG_DATA_SIZE  illegally #defined in 'cpu.h'      "
#error  "                   [See 'cpu.h  CONFIGURATION ERRORS']"

#endif

/*
*********************************************************************************************************
*                                             BIT MACRO'S
*********************************************************************************************************
*/

#define  DEF_BIT(bit)                                                   (1u << (bit))

#define  DEF_BIT_MASK(bit_mask, bit_shift)                                     ((bit_mask) << (bit_shift))

#define  DEF_BIT_FIELD(bit_field, bit_shift)                                 ((((bit_field) >= DEF_INT_CPU_NBR_BITS) ? (DEF_INT_CPU_U_MAX_VAL)     \
                                                                                                                     : (DEF_BIT(bit_field) - 1uL)) \
                                                                                                                            << (bit_shift))

#endif /* LIB_DEF_H */
