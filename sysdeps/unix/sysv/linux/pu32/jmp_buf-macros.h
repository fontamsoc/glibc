// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

/* Produced by this snippet:
printf ("#define JMP_BUF_SIZE %d\n", sizeof (jmp_buf));
printf ("#define JMP_BUF_ALIGN %d\n", __alignof__ (jmp_buf));
printf ("#define SIGJMP_BUF_SIZE %d\n", sizeof (sigjmp_buf));
printf ("#define SIGJMP_BUF_ALIGN %d\n", __alignof__ (sigjmp_buf));
printf ("#define MASK_WAS_SAVED_OFFSET %d\n", offsetof (struct __jmp_buf_tag, __mask_was_saved));
printf ("#define SAVED_MASK_OFFSET %d\n", offsetof (struct __jmp_buf_tag, __saved_mask)); */

#define JMP_BUF_SIZE 196
#define JMP_BUF_ALIGN 4
#define SIGJMP_BUF_SIZE 196
#define SIGJMP_BUF_ALIGN 4
#define MASK_WAS_SAVED_OFFSET 64
#define SAVED_MASK_OFFSET 68
