/**

Microsoft UF2

The MIT License (MIT)

Copyright (c) Microsoft Corporation

All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#ifndef UF2FORMAT_H
#define UF2FORMAT_H 1

#include <stdint.h>
#include <stdbool.h>

//--------------------------------------------------------------------+
// UF2 Configuration
//--------------------------------------------------------------------+

#define UF2_VERSION         "0.0.1"
#define UF2_INDEX_URL       ""
#define UF2_VOLUME_LABEL    "HucKeys"
#define UF2_PRODUCT_NAME    MACRO2STR(PRODUCT)
#define UF2_BOARD_ID        MACRO2STR(DEVICE_VER)
#define BOARD_UF2_FAMILY_ID 0x1aa143c7

#ifndef CFG_UF2_FLASH_SIZE
#define CFG_UF2_FLASH_SIZE (400 * 1024)
#endif
// Number of 512-byte blocks in the exposed filesystem, default is just under 32MB
// The filesystem needs space for the current file, text files, uploaded file, and FAT
#ifndef CFG_UF2_NUM_BLOCKS
#define CFG_UF2_NUM_BLOCKS (0x10109)
#endif
// Sectors per FAT cluster, must be increased proportionally for larger filesystems
#ifndef CFG_UF2_SECTORS_PER_CLUSTER
#define CFG_UF2_SECTORS_PER_CLUSTER (1)
#endif
#define MAX_BLOCKS (CFG_UF2_FLASH_SIZE / 256 + 100)

// All entries are little endian.
#define UF2_MAGIC_START0 0x0A324655UL // "UF2\n"
#define UF2_MAGIC_START1 0x9E5D5157UL // Randomly selected
#define UF2_MAGIC_END    0x0AB16F30UL // Ditto
// If set, the block is "comment" and should not be flashed to the device
#define UF2_FLAG_NOFLASH  0x00000001
#define UF2_FLAG_FAMILYID 0x00002000

#define STATIC_ASSERT(_exp) _Static_assert(_exp, "static assert failed")
#define UF2_ARRAY_SIZE(_arr) (sizeof(_arr) / sizeof(_arr[0]))
#define UF2_DIV_CEIL(_v, _d) (((_v) / (_d)) + ((_v) % (_d) ? 1 : 0))

typedef struct {
    uint8_t JumpInstruction[3];
    uint8_t OEMInfo[8];
    uint16_t SectorSize;
    uint8_t SectorsPerCluster;
    uint16_t ReservedSectors;
    uint8_t FATCopies;
    uint16_t RootDirectoryEntries;
    uint16_t TotalSectors16;
    uint8_t MediaDescriptor;
    uint16_t SectorsPerFAT;
    uint16_t SectorsPerTrack;
    uint16_t Heads;
    uint32_t HiddenSectors;
    uint32_t TotalSectors32;
    uint8_t PhysicalDriveNum;
    uint8_t Reserved;
    uint8_t ExtendedBootSig;
    uint32_t VolumeSerialNumber;
    uint8_t VolumeLabel[11];
    uint8_t FilesystemIdentifier[8];
} __attribute__((packed)) FAT_BootBlock;

typedef struct {
    char name[8];
    char ext[3];
    uint8_t attrs;
    uint8_t reserved;
    uint8_t createTimeFine;
    uint16_t createTime;
    uint16_t createDate;
    uint16_t lastAccessDate;
    uint16_t highStartCluster;
    uint16_t updateTime;
    uint16_t updateDate;
    uint16_t startCluster;
    uint32_t size;
} __attribute__((packed)) DirEntry;
STATIC_ASSERT(sizeof(DirEntry) == 32);

typedef struct FileContent {
    char const name[11];
    void const *content;
    uint32_t size; // OK to use uint32_T b/c FAT32 limits filesize to (4GiB - 2)

    // computing fields based on index and size
    uint16_t cluster_start;
    uint16_t cluster_end;
} FileContent_t;

typedef struct {
    uint32_t numBlocks;
    uint32_t numWritten;
    bool aborted; // aborting update and reset
    uint8_t writtenMask[MAX_BLOCKS / 8 + 1];
} WriteState;

typedef struct {
    // 32 byte header
    uint32_t magicStart0;
    uint32_t magicStart1;
    uint32_t flags;
    uint32_t targetAddr;
    uint32_t payloadSize;
    uint32_t blockNo;
    uint32_t numBlocks;
    uint32_t familyID;
    // raw data;
    uint8_t data[476];
    // store magic also at the end to limit damage from partial block reads
    uint32_t magicEnd;
} UF2_Block;

void uf2_init();
void uf2_read_block(uint32_t block_no, uint8_t *data);
int uf2_write_block(uint32_t block_no, uint8_t *data, WriteState *state);

#endif
