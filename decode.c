#include <stdio.h>
#include <string.h>
#include "decode.h"

/* Skip BMP header */
static void skip_bmp_header(FILE *fptr)
{
    fseek(fptr, 54, SEEK_SET);
}

/* Open files */
Status open_files_decode(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        return e_failure;
    }
    return e_success;
}

/* Decode single byte from LSB */
char decode_byte_from_lsb(char *buffer)
{
    char data = 0;
    for (int i = 0; i < 8; i++)
    {
        data = (data << 1) | (buffer[i] & 1);
    }
    return data;
}

/* Decode magic string */
Status decode_magic_string(DecodeInfo *decInfo)
{
    char buffer[8];
    char magic[3];   // length of MAGIC_STRING = 2 + '\0'

    for (int i = 0; i < 2; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego_image);
        magic[i] = decode_byte_from_lsb(buffer);
    }
    magic[2] = '\0';

    if (strcmp(magic, MAGIC_STRING) == 0)
        return e_success;
    else
        return e_failure;
}

/* Decode extension size */
Status decode_secret_file_extn_size(int *extn_size, DecodeInfo *decInfo)
{
    char buffer[32];
    *extn_size = 0;

    fread(buffer, 32, 1, decInfo->fptr_stego_image);

    for (int i = 0; i < 32; i++)
    {
        *extn_size = (*extn_size << 1) | (buffer[i] & 1);
    }
    return e_success;
}

/* Decode extension */
Status decode_secret_file_extn(DecodeInfo *decInfo, int extn_size)
{
    char buffer[8];
    //decode extension
    for (int i = 0; i < extn_size; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego_image);
        decInfo->extn_secret_file[i] = decode_byte_from_lsb(buffer);
    }
    decInfo->extn_secret_file[extn_size] = '\0';

    // sprintf(decInfo->output_fname, "decoded%s", decInfo->extn_secret_file);
    
    /* Decide output filename */
    if (decInfo->output_fname[0] == '\0')
    {
        /* No name provided by user */
        sprintf(decInfo->output_fname, "decoded%s",
                decInfo->extn_secret_file);
    }
    else
    {
        /* Name provided but no extension */
        if (strchr(decInfo->output_fname, '.') == NULL)
        {
            strcat(decInfo->output_fname,
                   decInfo->extn_secret_file);
        }
    }
    
    decInfo->fptr_output = fopen(decInfo->output_fname, "w");
    if (decInfo->fptr_output == NULL)
        return e_failure;

    return e_success;
}

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32];
    decInfo->secret_file_size = 0;

    fread(buffer, 32, 1, decInfo->fptr_stego_image);

    for (int i = 0; i < 32; i++)
    {
        decInfo->secret_file_size =
            (decInfo->secret_file_size << 1) | (buffer[i] & 1);
    }
    return e_success;
}

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char buffer[8];
    char ch;

    for (int i = 0; i < decInfo->secret_file_size; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego_image);
        ch = decode_byte_from_lsb(buffer);
        fwrite(&ch, 1, 1, decInfo->fptr_output);
    }
    return e_success;
}

/* Main decoding flow */
Status do_decoding(DecodeInfo *decInfo)
{
    open_files_decode(decInfo);
    skip_bmp_header(decInfo->fptr_stego_image);

    if (decode_magic_string(decInfo) == e_failure)
    {
        printf("ERROR: Magic string mismatch\n");
        return e_failure;
    }

    int extn_size;
    decode_secret_file_extn_size(&extn_size, decInfo);
    decode_secret_file_extn(decInfo, extn_size);
    decode_secret_file_size(decInfo);
    decode_secret_file_data(decInfo);

    printf("INFO: Decoding completed successfully\n");
    return e_success;
}
