#ifndef AXDRIVER
#define AXDRIVER

#include<stdint.h>
#include<vector>
#include<memory>


// AXDriver is an interface to data storage.
// Derived class should be resposible for phisical representation of blocks and blobs.
//
// Basically block is represented by header (SXBlockInfo, a combo of block's ID and hash) and data.
// Reference is special sort of block, it is a header that points to already stored block.
//
// Props:
// Each block may be handled by header.
// Driver should be able to store blocks as well as references.
// Driver should not check an uniqueness of block's data or ID, see CXBlob.


using CXData = std::vector<uint8_t>;
using CXDataPtr = std::shared_ptr<CXData>;


struct SXBlockInfo{
    SXBlockInfo(uint64_t aId, uint64_t aHash) : mId(aId), mHash(aHash){ }
    uint64_t mId;
    uint64_t mHash;
};
using SXBlockInfoPtr = std::shared_ptr<SXBlockInfo>;


struct SXBlobInfo{
    uint64_t mBlockSize;
    uint32_t mBlocksLimit;
};


class AXDriver{
public:
    virtual ~AXDriver(){ }

    // open already exists storage
    virtual bool init() = 0;
    // init new storage
    virtual bool init(uint64_t aBlockSize, uint32_t aBlocksLimit) = 0;
    virtual SXBlockInfoPtr addReference(uint64_t aBlockId, SXBlockInfoPtr aBlk) = 0;
    virtual SXBlockInfoPtr addBlock(uint64_t aBlockId, uint64_t aHash, CXData const &aData) = 0;
    virtual CXDataPtr getBlock(SXBlockInfoPtr aBlock) const = 0;

    virtual SXBlobInfo blobInfo() const = 0;
    virtual std::vector<SXBlockInfoPtr> blocksInfos() = 0;

    // Default comparison reads all block content into the memory.
    // Derived class could optimize this.
    virtual bool compare(SXBlockInfoPtr aBlock, CXData const &aData) const {
        if(auto data = getBlock(aBlock))
            return *data == aData;
        return false;
    }
};
using AXDriverPtr = std::shared_ptr<AXDriver>;


#endif
