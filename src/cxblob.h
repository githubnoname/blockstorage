#ifndef CXBLOB
#define CXBLOB

#include"cxblockmap.h"
#include"axdriver.h"


// CXBlob is corresponded for top level logic of data storage, e.g.
// - creating references for equal data,
// - handling blocks by ids,
// - prevent holding blocks with same ids,
// - limiting blocks amount.


class CXBlob{
public:
    CXBlob(AXDriverPtr aDriver) : mDriver(aDriver){ }
    bool init();
    bool init(uint64_t aBlockSize, uint64_t aBlocksLimit);
    bool putBlock(uint64_t aBlockId, CXData const &aData);
    CXDataPtr getBlock(uint64_t aBlockId);
    std::vector<uint64_t> blocks() const { return mBlocks.ids(); }

private:
    AXDriverPtr mDriver;
    CXBlockMap mBlocks;
    SXBlobInfo mInfo;
};

#endif
