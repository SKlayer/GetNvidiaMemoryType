#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

LPCWSTR  nvidiadllname = L"nvapi64.dll";

typedef enum _NV_GPU_RAM_TYPE
{
    NV_GPU_RAM_TYPE_UNKNOWN = 0,
    NV_GPU_RAM_TYPE_SDRAM = 1,
    NV_GPU_RAM_TYPE_DDR1 = 2,
    NV_GPU_RAM_TYPE_DDR2 = 3,
    NV_GPU_RAM_TYPE_GDDR2 = 4,
    NV_GPU_RAM_TYPE_GDDR3 = 5,
    NV_GPU_RAM_TYPE_GDDR4 = 6,
    NV_GPU_RAM_TYPE_DDR3 = 7,
    NV_GPU_RAM_TYPE_GDDR5 = 8,
    NV_GPU_RAM_TYPE_LPDDR2 = 9,
    NV_GPU_RAM_TYPE_GDDR5X = 10,
    NV_GPU_RAM_TYPE_HBM1 = 11,
    NV_GPU_RAM_TYPE_HBM2 = 12,
    NV_GPU_RAM_TYPE_SDDR4 = 13,
    NV_GPU_RAM_TYPE_GDDR6 = 14,
    NV_GPU_RAM_TYPE_GDDR6X = 15,
}NV_GPU_RAM_TYPE;

typedef enum _NV_GPU_RAM_VENDOR_ID
{
    NV_GPU_RAM_VENDOR_ID_UNKNOWN = 0,
    NV_GPU_RAM_VENDOR_ID_SAMSUNG = 1,
    NV_GPU_RAM_VENDOR_ID_QIMONDA = 2,
    NV_GPU_RAM_VENDOR_ID_ELPIDA = 3,
    NV_GPU_RAM_VENDOR_ID_ETRON = 4,
    NV_GPU_RAM_VENDOR_ID_NANYA = 5,
    NV_GPU_RAM_VENDOR_ID_HYNIX = 6,
    NV_GPU_RAM_VENDOR_ID_MOSEL = 7,
    NV_GPU_RAM_VENDOR_ID_WINBOND = 8,
    NV_GPU_RAM_VENDOR_ID_ESMT = 9,
    NV_GPU_RAM_VENDOR_ID_MICRON = 10
}NV_GPU_RAM_VENDOR_ID;

typedef unsigned long NvU32;

typedef struct {
    int value;
    struct {
        int   mindelta;
        int   maxdelta;
    } valueRange;
} NV_GPU_PERF_PSTATES20_PARAM_DELTA;

typedef struct {
    NvU32   domainId;
    NvU32   typeId;
    NvU32   bIsEditable : 1;
    NvU32   reserved : 31;
    NV_GPU_PERF_PSTATES20_PARAM_DELTA   freqDelta_kHz;
    union {
        struct {
            NvU32   freq_kHz;
        } single;
        struct {
            NvU32   minFreq_kHz;
            NvU32   maxFreq_kHz;
            NvU32   domainId;
            NvU32   minVoltage_uV;
            NvU32   maxVoltage_uV;
        } range;
    } data;
} NV_GPU_PSTATE20_CLOCK_ENTRY_V1;

typedef struct {
    NvU32   domainId;
    NvU32   bIsEditable : 1;
    NvU32   reserved : 31;
    NvU32   volt_uV;
    int     voltDelta_uV;
} NV_GPU_PSTATE20_BASE_VOLTAGE_ENTRY_V1;

typedef struct {
    NvU32   version;
    NvU32   bIsEditable : 1;
    NvU32   reserved : 31;
    NvU32   numPstates;
    NvU32   numClocks;
    NvU32   numBaseVoltages;
    struct {
        NvU32                                   pstateId;
        NvU32                                   bIsEditable : 1;
        NvU32                                   reserved : 31;
        NV_GPU_PSTATE20_CLOCK_ENTRY_V1          clocks[8];
        NV_GPU_PSTATE20_BASE_VOLTAGE_ENTRY_V1   baseVoltages[4];
    } pstates[16];
} NV_GPU_PERF_PSTATES20_INFO_V1;

typedef void* (*NvAPI_QueryInterface_t)(unsigned int offset);
typedef int(*NvAPI_Initialize_t)();
typedef int(*NvAPI_Unload_t)();
typedef int(*NvAPI_EnumPhysicalGPUs_t)(int** handles, int* count);
typedef int(*NvAPI_GPU_GetRamType_t)(int* handle, NV_GPU_RAM_TYPE* memtype);
typedef int(*NvAPI_GPU_GetRamMaker_t)(int* handle, NV_GPU_RAM_VENDOR_ID* pRamMaker);

NvAPI_QueryInterface_t NvQueryInterface = NULL;
NvAPI_Initialize_t NvInit = NULL;
NvAPI_Unload_t NvUnload = NULL;
NvAPI_EnumPhysicalGPUs_t NvEnumGPUs = NULL;
NvAPI_GPU_GetRamType_t NvGetMemType = NULL;
NvAPI_GPU_GetRamMaker_t NvGetMemMarker = NULL;

int main(int argc, char** argv)
{
    int nGPU = 0;
    NV_GPU_RAM_TYPE memtype = NV_GPU_RAM_TYPE_UNKNOWN;
    NV_GPU_RAM_VENDOR_ID memmarker = NV_GPU_RAM_VENDOR_ID_UNKNOWN;
    int* hdlGPU[64] = { 0 };
    NV_GPU_PERF_PSTATES20_INFO_V1 pstates_info;
    pstates_info.version = 0x11c94;

    NvQueryInterface = (NvAPI_QueryInterface_t)GetProcAddress(LoadLibrary(nvidiadllname), "nvapi_QueryInterface");
    NvInit = (NvAPI_Initialize_t)NvQueryInterface(0x0150E828);
    NvUnload = (NvAPI_Unload_t)NvQueryInterface(0xD22BDD7E);
    NvEnumGPUs = (NvAPI_EnumPhysicalGPUs_t)NvQueryInterface(0xE5AC921F);
    NvGetMemType = (NvAPI_GPU_GetRamType_t)NvQueryInterface(0x57F7CAAC);
    NvGetMemMarker = (NvAPI_GPU_GetRamMaker_t)NvQueryInterface(0x42AEA16AUL);

    NvInit();
    NvEnumGPUs(hdlGPU, &nGPU);
    NvGetMemType(hdlGPU[0], &memtype);
    NvGetMemMarker(hdlGPU[0], &memmarker);

    switch (memtype) {
    case NV_GPU_RAM_TYPE_SDRAM:
        printf("Memory Type: SDRAM");
        break;
    case NV_GPU_RAM_TYPE_DDR1:
        printf("Memory Type: DDR1");
        break;
    case NV_GPU_RAM_TYPE_DDR2:
        printf("Memory Type: DDR2");
        break;
    case NV_GPU_RAM_TYPE_GDDR2:
        printf("Memory Type: GDDR2");
        break;
    case NV_GPU_RAM_TYPE_GDDR3:
        printf("Memory Type: GDDR3");
        break;
    case NV_GPU_RAM_TYPE_GDDR4:
        printf("Memory Type: GDDR4");
        break;
    case NV_GPU_RAM_TYPE_DDR3:
        printf("Memory Type: DDR3");
        break;
    case NV_GPU_RAM_TYPE_GDDR5:
        printf("Memory Type: GDDR5");
        break;
    case NV_GPU_RAM_TYPE_LPDDR2:
        printf("Memory Type: LPDDR2");
        break;
    case NV_GPU_RAM_TYPE_GDDR5X:
        printf("Memory Type: GDDR5X");
        break;
    case NV_GPU_RAM_TYPE_HBM1:
        printf("Memory Type: HBM1");
        break;
    case NV_GPU_RAM_TYPE_HBM2:
        printf("Memory Type: HBM2");
        break;
    case NV_GPU_RAM_TYPE_SDDR4:
        printf("Memory Type: SDDR4");
        break;
    case NV_GPU_RAM_TYPE_GDDR6:
        printf("Memory Type: GDDR6");
        break;
    case NV_GPU_RAM_TYPE_GDDR6X:
        printf("Memory Type: GDDR6X");
        break;
    case NV_GPU_RAM_TYPE_UNKNOWN:
    default:
        printf("Memory Type: Unknown");
        break;
    }
    switch (memmarker) {
    case NV_GPU_RAM_VENDOR_ID_SAMSUNG:
        printf("(Samsung)\n");
        break;
    case NV_GPU_RAM_VENDOR_ID_QIMONDA:
        printf("(Qimonda)\n");
        break;
    case NV_GPU_RAM_VENDOR_ID_ELPIDA:
        printf("(Elpida)\n");
        break;
    case NV_GPU_RAM_VENDOR_ID_ETRON:
        printf("(Etron)\n");
        break;
    case NV_GPU_RAM_VENDOR_ID_NANYA:
        printf("(Nanya)\n");
        break;
    case NV_GPU_RAM_VENDOR_ID_HYNIX:
        printf("(Hynix)\n");
        break;
    case NV_GPU_RAM_VENDOR_ID_MOSEL:
        printf("(Mosel)\n");
        break;
    case NV_GPU_RAM_VENDOR_ID_WINBOND:
        printf("(Winbond)\n");
        break;
    case NV_GPU_RAM_VENDOR_ID_ESMT:
        printf("(ESMT)\n");
        break;
    case NV_GPU_RAM_VENDOR_ID_MICRON:
        printf("(Micron)\n");
        break;
    default:
        printf("(Unknown)\n");
        break;
    }
    NvUnload();
    return 0;
}
