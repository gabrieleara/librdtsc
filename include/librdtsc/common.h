#ifndef LIBCYCLES_COMMON_H
#define LIBCYCLES_COMMON_H

#include <stdint.h>

#define SET_USED(x) ((void)(x))

#ifndef asm
#define asm __asm__
#endif

#define __experimental

/** C extension macro for environments lacking C11 features. */
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 201112L
#define RDTSC_STD_C11 __extension__
#else
#define RDTSC_STD_C11
#endif

/*********** Macros for pointer arithmetic ********/

/**
 * add a byte-value offset to a pointer
 */
#define RDTSC_PTR_ADD(ptr, x) ((void *)((uintptr_t)(ptr) + (x)))

/**
 * subtract a byte-value offset from a pointer
 */
#define RDTSC_PTR_SUB(ptr, x) ((void *)((uintptr_t)ptr - (x)))

/**
 * get the difference between two pointer values, i.e. how far apart
 * in bytes are the locations they point two. It is assumed that
 * ptr1 is greater than ptr2.
 */
#define RDTSC_PTR_DIFF(ptr1, ptr2) ((uintptr_t)(ptr1) - (uintptr_t)(ptr2))

/**
 * Workaround to cast a const field of a structure to non-const type.
 */
#define RDTSC_CAST_FIELD(var, field, type)                                       \
    (*(type *)((uintptr_t)(var) + offsetof(typeof(*(var)), field)))

/*********** Macros/static functions for doing alignment ********/

/**
 * Macro to align a pointer to a given power-of-two. The resultant
 * pointer will be a pointer of the same type as the first parameter, and
 * point to an address no higher than the first parameter. Second parameter
 * must be a power-of-two value.
 */
#define RDTSC_PTR_ALIGN_FLOOR(ptr, align)                                        \
    ((typeof(ptr))RDTSC_ALIGN_FLOOR((uintptr_t)ptr, align))

/**
 * Macro to align a value to a given power-of-two. The resultant value
 * will be of the same type as the first parameter, and will be no
 * bigger than the first parameter. Second parameter must be a
 * power-of-two value.
 */
#define RDTSC_ALIGN_FLOOR(val, align)                                            \
    (typeof(val))((val) & (~((typeof(val))((align)-1))))

/**
 * Macro to align a pointer to a given power-of-two. The resultant
 * pointer will be a pointer of the same type as the first parameter, and
 * point to an address no lower than the first parameter. Second parameter
 * must be a power-of-two value.
 */
#define RDTSC_PTR_ALIGN_CEIL(ptr, align)                                         \
    RDTSC_PTR_ALIGN_FLOOR((typeof(ptr))RDTSC_PTR_ADD(ptr, (align)-1), align)

/**
 * Macro to align a value to a given power-of-two. The resultant value
 * will be of the same type as the first parameter, and will be no lower
 * than the first parameter. Second parameter must be a power-of-two
 * value.
 */
#define RDTSC_ALIGN_CEIL(val, align)                                             \
    RDTSC_ALIGN_FLOOR(((val) + ((typeof(val))(align)-1)), align)

/**
 * Macro to align a pointer to a given power-of-two. The resultant
 * pointer will be a pointer of the same type as the first parameter, and
 * point to an address no lower than the first parameter. Second parameter
 * must be a power-of-two value.
 * This function is the same as RDTSC_PTR_ALIGN_CEIL
 */
#define RDTSC_PTR_ALIGN(ptr, align) RDTSC_PTR_ALIGN_CEIL(ptr, align)

/**
 * Macro to align a value to a given power-of-two. The resultant
 * value will be of the same type as the first parameter, and
 * will be no lower than the first parameter. Second parameter
 * must be a power-of-two value.
 * This function is the same as RDTSC_ALIGN_CEIL
 */
#define RDTSC_ALIGN(val, align) RDTSC_ALIGN_CEIL(val, align)

/**
 * Macro to align a value to the multiple of given value. The resultant
 * value will be of the same type as the first parameter and will be no lower
 * than the first parameter.
 */
#define RDTSC_ALIGN_MUL_CEIL(v, mul)                                             \
    ((((v) + (typeof(v))(mul)-1) / ((typeof(v))(mul))) * (typeof(v))(mul))

/**
 * Macro to align a value to the multiple of given value. The resultant
 * value will be of the same type as the first parameter and will be no higher
 * than the first parameter.
 */
#define RDTSC_ALIGN_MUL_FLOOR(v, mul)                                            \
    (((v) / ((typeof(v))(mul))) * (typeof(v))(mul))

/**
 * Macro to align value to the nearest multiple of the given value.
 * The resultant value might be greater than or less than the first parameter
 * whichever difference is the lowest.
 */
#define RDTSC_ALIGN_MUL_NEAR(v, mul)                                             \
    ({                                                                         \
        typeof(v) ceil = RDTSC_ALIGN_MUL_CEIL(v, mul);                           \
        typeof(v) floor = RDTSC_ALIGN_MUL_FLOOR(v, mul);                         \
        (ceil - (v)) > ((v)-floor) ? floor : ceil;                             \
    })

/**
 * Checks if a pointer is aligned to a given power-of-two value
 *
 * @param ptr
 *   The pointer whose alignment is to be checked
 * @param align
 *   The power-of-two value to which the ptr should be aligned
 *
 * @return
 *   True(1) where the pointer is correctly aligned, false(0) otherwise
 */
static inline int rdtsc_is_aligned(void *ptr, unsigned align) {
    return RDTSC_PTR_ALIGN(ptr, align) == ptr;
}

#endif // LIBCYCLES_COMMON_H
