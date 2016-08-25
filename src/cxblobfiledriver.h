#ifndef CXBLOBFILEDRIVER
#define CXBLOBFILEDRIVER

#include<stdio.h>
#include<string>

#include"axdriver.h"


// This drivers holds all blob data in the single file.
//
// File structure:
// -- Blob header --
// 8 bytes   BS = block size
// 4 bytes        blocks limit
//
// -- Block header --
// 8 bytes        block ID
// 8 bytes        hash of block data
// 8 bytes        if reference, points to another block data. 0, therwise.
// -- if reference, block is finished here --
// -- Otherwise --
// BS bytes       block data
//
// -- Block header --
// ...

class CXBlobFileDriver : public AXDriver{
public:
    CXBlobFileDriver(std::string const &aFileName) : mFileName(aFileName), mFile(0){ }
    ~CXBlobFileDriver();

    bool init() override;
    bool init(uint64_t aBlockSize, uint32_t aBlocksLimit) override;
    SXBlockInfoPtr addBlock(uint64_t aBlockId, uint64_t aHash, CXData const &aData) override;
    SXBlockInfoPtr addReference(uint64_t aBlockId, SXBlockInfoPtr aBlk) override;
    CXDataPtr getBlock(SXBlockInfoPtr aBlk) const override;
    std::vector<SXBlockInfoPtr> blocksInfos() override;
    SXBlobInfo blobInfo() const override { return mInfo; }

private:
    bool open(char const *aMethod);
    bool writeHeader();
    bool readHeader();
    SXBlockInfoPtr getNextBlockInfo();

    std::string mFileName;
    FILE *mFile;
    SXBlobInfo mInfo;
};


#endif
