#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "common.h"
/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("INFO: WIDTH = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("INFO: HEIGHT = %u\n", height);

    // Return image capacity
    return width * height * 3;
}
/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo){
    // strtok(argv[2],".");//beutiful.bmp --> bmp
    if(strstr(argv[2],".bmp") != NULL){
        encInfo->src_image_fname = argv[2];
        printf("INFO: %s taken as image file\n",argv[2]);
    }
    else {
        printf("ERROR: not taken as image file appropriately\n");
        return e_failure;
    }
        
    

    if(strstr(argv[3],".txt") != NULL){
        encInfo->secret_fname = argv[3];
        printf("INFO: %s taken as secret file\n",argv[3]);
    }
    else{
        printf("ERROR: not taken secret file appropriately\n");
        return e_failure;
    }

    if (argv[4] == NULL)
    {
        encInfo->stego_image_fname = "decodeit.bmp";
        printf("INFO: taken a default output file name as decodeit.bmp\n");
    }
    else{
        encInfo->stego_image_fname = argv[4];
        printf("INFO: %s taken as output file\n",argv[4]);
    }
        
    
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo){
    
    if(open_files(encInfo) == e_success){
        printf("INFO: Files are opened successfully\n");
        if(check_capacity(encInfo) == e_success){
            printf("INFO: Secret data can fit in image file\n");
        }
        else{
            printf("ERROR: Image size is not sufficient enough to fit secret data\n");
            return e_failure;
        }
    }
    else{
        printf("ERROR: Failed to open file\n");
        return e_failure;
    }
    if (copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
    {
        printf("INFO: Copy bmp header is a succuss.\n");
    }else{
        printf("ERROR: Copy bmp header is getting failure.\n");
    }
    
    if (encode_magic_string(MAGIC_STRING,encInfo)==e_success)
    {
        printf("INFO: Magic string encoded success\n");
    }
    else{
        printf("ERROR: Magic string is not getting encoded.\n");
    }
    encode_secret_file_extn_size(strlen(".txt"), encInfo);
    encode_secret_file_extn(".txt", encInfo);

    encode_secret_file_size(encInfo->size_secret_file, encInfo);
    encode_secret_file_data(encInfo);

    copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image);

    return e_success;

}

Status check_capacity(EncodeInfo *encInfo){
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    printf("INFO: size of secret.txt is %ld bytes\n",encInfo->size_secret_file);

    if (encInfo->image_capacity > 16+32+32+32+(encInfo->size_secret_file*8))
    {
        /*
            16             +    32                      +   32                              +   32                      +   (encInfo->size_secret_file*8)
            magic_string        size of file extesion       secret file extension itself        size of secret data         Secret data

        */
        return e_success;
    }
    else{
        return e_failure;
    }
    

}

uint get_file_size(FILE *fptr){
    /* 
    Find size of secret.txt
    */
   fseek(fptr,0,SEEK_END);
   uint size = ftell(fptr);
   rewind(fptr);
   return size;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image){
    char arr[54];
    fseek(fptr_src_image,0,SEEK_SET);

    if (fread(arr, 54, 1, fptr_src_image) != 1)
        return e_failure;

    if (fwrite(arr, 54, 1, fptr_dest_image) != 1)
        return e_failure;

    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo){
    encode_data_to_image(magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success; 
}

Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image){
    char arr[8];
    for (int i = 0; i < size; i++)
    {
        fread(arr,8,1,fptr_src_image);
        encode_byte_to_lsb(data[i], arr);
        fwrite(arr,8,1,fptr_stego_image);
    }
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer){
    for (int i = 0; i < 8; i++)
    {
        image_buffer[i] = (image_buffer[i]& 0xFE) | ((data>>(7-i))&1);
    }
    
    return e_success;
}


Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    encode_data_to_image(file_extn, strlen(file_extn),
                          encInfo->fptr_src_image,
                          encInfo->fptr_stego_image);
    return e_success;
}


Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char buffer[32];
    fread(buffer, 32, 1, encInfo->fptr_src_image);

    for (int i = 0; i < 32; i++)
    {
        buffer[i] = (buffer[i] & 0xFE) | ((file_size >> (31 - i)) & 1);
    }

    fwrite(buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char ch;
    while (fread(&ch, 1, 1, encInfo->fptr_secret))
    {
        char buffer[8];
        fread(buffer, 8, 1, encInfo->fptr_src_image);
        encode_byte_to_lsb(ch, buffer);
        fwrite(buffer, 8, 1, encInfo->fptr_stego_image);
    }
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while (fread(&ch, 1, 1, fptr_src))
    {
        fwrite(&ch, 1, 1, fptr_dest);
    }
    return e_success;
}

Status encode_secret_file_extn_size(int extn_size, EncodeInfo *encInfo)
{
    char buffer[32];

    /* Read 32 bytes from source image */
    fread(buffer, 32, 1, encInfo->fptr_src_image);

    /* Encode extension size into LSB */
    for (int i = 0; i < 32; i++)
    {
        buffer[i] = (buffer[i] & 0xFE) | ((extn_size >> (31 - i)) & 1);
    }

    /* Write modified bytes to stego image */
    fwrite(buffer, 32, 1, encInfo->fptr_stego_image);

    return e_success;
}
