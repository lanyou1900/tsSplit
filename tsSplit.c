/*
 * =====================================================================================
 *
 *    Filename:  tsSplit.c
 *
 *    Description:  Split ts.
 *
 *       Version:  1.0
 *       Created:  2013-6-25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *       Author:  Li WenYan <suma.lwy@gmail.com>
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

//#include "tsSplit.h"
#include "version.h"

#define INPUT_PARA_NUM           (4)
#define TAG_BUFF_SIZE            (10*1024*1024) /* 10M */ 
#define TS_PKT_LEN               (188)


unsigned char g_tga_buff[TAG_BUFF_SIZE];

char *option;
char *input_fname;
char output_fname[256] = "";
int split_num = 0;

static void show_help()
{
    puts("Usage: tsSplit [Option] input filename split num");
    puts("Option:");
    puts("  -ts: Split ts file by packet, hex fomat");
    puts("  -pts: Split ts file by packet, ascii fomat");
    puts("  -bts: Split ts file by bytes, ascii fomat");
    puts("  -h, --help: show help");
    puts("  -v, --version: show version");
}

static void show_version()
{
    printf("tsSplit version %s.%s.%s \n", VER_MAJOR, VER_MINOR, VER_RELEA);
}

static int fsize(FILE *fp)
{
    int file_size;
    if(NULL == fp)
    {
        puts("NULL fp ERROR!");
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return file_size;
}

static int phase_parameter(int argc, char *argv[]);

static uint16_t tsGetPid(unsigned char *buf)
{
    uint16_t pid = ((buf[1] & 0x1f) << 8) | buf[2];
    return pid;
}


int main(int argc, char *argv[])
{
    unsigned char *rgb_buff;
    int data_size;
    FILE *in_fp = NULL;
    FILE *out_fp = NULL;
    int ts_idx;
    int i;
    unsigned char buff[188];

    memset(buff, 0, 188);

    if(0 != phase_parameter(argc, argv))
    {
        return -1;
    }

    rgb_buff = g_tga_buff;

    printf("input_fname:%s !\n", input_fname);
    if((in_fp = fopen(input_fname, "rb")) == NULL)
    {
        printf("\n open %s failed!\n", input_fname);
        return -1;
    }

    if((out_fp = fopen(output_fname, "wb")) == NULL)
    {
        printf("\n open %s failed!\n", output_fname);
        return -1;
    }

    ts_idx = 0;
    if( (!strcmp(option, "-ts")) ||
        (!strcmp(option, "-pts")) ||
        (!strcmp(option, "-bts"))
     )
    {
        while( (TS_PKT_LEN == fread(rgb_buff, 1, TS_PKT_LEN, in_fp)) &&
               (ts_idx < split_num)
             )
        {
            ts_idx++;
            if(!strcmp(option, "-ts"))
            {
                fwrite(rgb_buff, 1, TS_PKT_LEN, out_fp);
            }
            for(i = 0; i<TS_PKT_LEN; i++)
            {
                if(strcmp(option, "-ts"))
                {
                    fprintf(out_fp, "%02x", rgb_buff[i]);
                }
                
                if(!strcmp(option, "-bts"))
                {
                    fprintf(out_fp, "\n");
                }
            }
            if(!strcmp(option, "-pts"))
            {
                fprintf(out_fp, "\n");
            }
        }
    }

    fclose(out_fp);

    return 0;
}

static int phase_parameter(int argc, char *argv[])
{
    int i;

    if(argc != INPUT_PARA_NUM)
    {
        if( (argc == 2) && (!strcmp(argv[1], "-v")) )
        {
            show_version();
            return -1;
        }
        show_help();
        return -1;
    }

    option = argv[1];
    input_fname = argv[2];
    split_num = atoi(argv[3]);
    if( (strcmp(option, "-ts")) &&
        (strcmp(option, "-pts")) &&
        (strcmp(option, "-bts"))// &&
       // (strcmp(option, "-bmp"))
    )
    {
        printf("Unknown option err:%s !\n", option);
        return -1;
    }

    if(split_num <= 0)
    {
        printf("Unnormal split number:%d !\n", split_num);
        return -1;
    }

    if( (!strcmp(option, "-ts")) ||
        (!strcmp(option, "-pts")) ||
        (!strcmp(option, "-bts"))
     )
    {
        sprintf(output_fname, "%s%s\0", input_fname, ".ts");
    }

    return 0;
}




