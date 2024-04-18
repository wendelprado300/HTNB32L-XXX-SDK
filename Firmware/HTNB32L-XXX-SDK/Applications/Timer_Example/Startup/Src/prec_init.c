#include <string.h>
#include "qcx212.h"
#include "cache_qcx212.h"

extern UINT32 Load$$LOAD_IRAM$$Base	;
extern UINT32 Image$$LOAD_IRAM$$Base	;
extern UINT32 Image$$LOAD_IRAM$$Limit	;
extern UINT32 Load$$LOAD_DRAM_SHARED$$Base  ;
extern UINT32 Image$$LOAD_DRAM_SHARED$$Base ;
extern UINT32 Image$$LOAD_DRAM_SHARED$$Limit	;
extern UINT32 Image$$LOAD_DRAM_SHARED$$ZI$$Base;
extern UINT32 Image$$LOAD_DRAM_SHARED$$ZI$$Limit;

void SetZIDataToZero(void)
{
    UINT32 *start_addr;
    UINT32 *end_addr  ;
    UINT32 length;
    start_addr = &(Image$$LOAD_DRAM_SHARED$$ZI$$Base) ;
    end_addr   = &(Image$$LOAD_DRAM_SHARED$$ZI$$Limit);
    length = (UINT32)end_addr - (UINT32)start_addr;
    memset((UINT32 *)start_addr, 0, length);
}


void CopyDataRamtoImage(void)
{
    UINT32 *src;
    UINT32 *dst;
    UINT32 length;
    dst    = &(Image$$LOAD_IRAM$$Base);
    src    = &(Load$$LOAD_IRAM$$Base);
    length = (unsigned int)&(Image$$LOAD_IRAM$$Limit) - (unsigned int)&(Image$$LOAD_IRAM$$Base);
    length /= sizeof(unsigned int);

    if(dst != src)
    {
        while(length >0)
        {
            dst[length-1]=src[length-1];
            length--; 
        }
    }
}
void CopyRWDataFromBin(void)
{
    UINT32 *src;
    UINT32 *dst;
    UINT32 length;
    dst    = &(Image$$LOAD_DRAM_SHARED$$Base);
    src    = &(Load$$LOAD_DRAM_SHARED$$Base);
    length = (unsigned int)&(Image$$LOAD_DRAM_SHARED$$Limit) - (unsigned int)&(Image$$LOAD_DRAM_SHARED$$Base);
    length /= sizeof(unsigned int);

    if(dst != src)
    {
        while(length >0)
        {
          dst[length-1]=src[length-1];
          length--; 
        }
    }
}

