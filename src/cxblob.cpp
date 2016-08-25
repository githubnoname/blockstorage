#include"cxblob.h"


template<typename TSeq>
static uint64_t datahash(TSeq const &aData){
    uint64_t seed = aData.size();
    for(auto &x : aData)
        seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
}


bool CXBlob::init(){
    if(!mDriver->init())
        return false;
    mInfo = mDriver->blobInfo();
    for(auto &x : mDriver->blocksInfos())
        mBlocks.addBlock(x);
    return true;
}


bool CXBlob::init(uint64_t aBlockSize, uint64_t aBlocksLimit){
    if(!mDriver->init(aBlockSize, aBlocksLimit))
        return false;
    mInfo = mDriver->blobInfo();
    return true;
}


bool CXBlob::putBlock(uint64_t aBlockId, CXData const &aData){
    if(mBlocks.size() >= mInfo.mBlocksLimit)
        return false;
    if(mBlocks.findId(aBlockId))
        return false;
    auto hash = datahash(aData);
    auto blk = mBlocks.findHash(hash);
    if(blk != 0 && mDriver->compare(blk, aData))
        blk = mDriver->addReference(aBlockId, blk);
    else
        blk = mDriver->addBlock(aBlockId, hash, aData);
    if(blk == 0)
        return false;
    mBlocks.addBlock(blk);
    return true;
}


CXDataPtr CXBlob::getBlock(uint64_t aBlockId){
    if(auto blk = mBlocks.findId(aBlockId))
        return mDriver->getBlock(blk);
    return 0;
}
