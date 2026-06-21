#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include <string.h>

int main(int argc, char *argv[])
{
    EncodeInfo E1;
    //Validate Argc
    if(argc<=3){
        printf("Usage:\n");
        printf("Encoding: ./a.out -e <src.bmp> <secret.txt> [stego.bmp]\n");
        printf("Decoding: ./a.out -d <stego.bmp> [output.txt]\n");
        return 0;
    }
    int res = check_operation_type(argv);
    //ENCODING
    if (res == e_encode)
    {
        printf("INFO: Selected Encoding\n");
        if(read_and_validate_encode_args(argv,&E1) == e_success){
            printf("INFO: Read and validate encode_args is success\n");
            if (do_encoding(&E1) == e_success)
            {
                printf("INFO: Encoding is success\n");
            }
            else{
                return e_failure;
            }
            
        }
        else{
            printf("ERROR: Failed to read and validate encode_args\n");
        }
    }
    //DECODING
    else if (res == e_decode)
    {
        printf("INFO: Dencoding Started\n");
        DecodeInfo decInfo;
        memset(&decInfo, 0, sizeof(DecodeInfo));

        printf("INFO: Selected Decoding\n");

        /* stego image */
        decInfo.stego_image_fname = argv[2];

        /* optional output file */
        if (argv[3] != NULL)
            strcpy(decInfo.output_fname, argv[3]);
        else
            strcpy(decInfo.output_fname, "Decodeit");

        if (do_decoding(&decInfo) == e_success)
        {
            printf("INFO: Decoding completed successfully\n");
        }
        else
        {
            printf("ERROR: Decoding failed\n");
        }
    }
    else{
        printf("ERROR: !!! Not Supported Operation\n");
        printf("ERROR: Usage:\n For Encoding: ./a.out -e file.bmp secret.txt\n For Decoding: ./a.out -d file.bmp\n");
    }
    
    return 0;
}

OperationType check_operation_type(char *argv[]){

    if(strcmp(argv[1],"-e") == 0){
        return e_encode;
    }
    else if(strcmp(argv[1], "-d")==0)
    {
        return e_decode;
    }
    else{
        return e_unsupported;
    }

}