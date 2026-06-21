#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h"
#include "common.h"

typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Output File Info */
    char output_fname[50];
    FILE *fptr_output;

    long secret_file_size;
    char extn_secret_file[10];

} DecodeInfo;

/* Function Prototypes */
Status do_decoding(DecodeInfo *decInfo);

Status open_files_decode(DecodeInfo *decInfo);

Status decode_magic_string(DecodeInfo *decInfo);
char decode_byte_from_lsb(char *buffer);

Status decode_secret_file_extn_size(int *extn_size, DecodeInfo *decInfo);
Status decode_secret_file_extn(DecodeInfo *decInfo, int extn_size);

Status decode_secret_file_size(DecodeInfo *decInfo);
Status decode_secret_file_data(DecodeInfo *decInfo);

#endif
