#include"cxblobfiledriver.h"

namespace{
    struct SXBlockInfoFS : public SXBlockInfo{
        SXBlockInfoFS(uint64_t aId, uint64_t aHash, uint64_t aDataOffset = 0) : SXBlockInfo(aId, aHash), mDataOffset(aDataOffset){ }
        uint64_t mDataOffset;
    };
}


CXBlobFileDriver::~CXBlobFileDriver(){
    if(mFile != 0)
        fclose(mFile);
}


bool CXBlobFileDriver::init(){
    if(!open("r+"))
        return false;
    return readHeader();
}


bool CXBlobFileDriver::init(uint64_t aBlockSize, uint32_t aBlocksLimit){
    if(!open("w+"))
        return false;
    mInfo.mBlockSize = aBlockSize;
    mInfo.mBlocksLimit = aBlocksLimit;
    return writeHeader();
}


SXBlockInfoPtr CXBlobFileDriver::addBlock(uint64_t aBlockId, uint64_t aHash, CXData const &aData){
    if(aData.size() != mInfo.mBlockSize || fseek(mFile, 0, SEEK_END) == -1)
        return 0;
    std::unique_ptr<SXBlockInfoFS> blk(new SXBlockInfoFS(aBlockId, aHash));
    auto s = fwrite(blk.get(), sizeof(*blk), 1, mFile);
    if(s != 1)
        return 0;
    blk->mDataOffset = ftell(mFile);
    s = fwrite(aData.data(), 1, mInfo.mBlockSize, mFile);
    if(s != mInfo.mBlockSize)
        return 0;
    return SXBlockInfoPtr(blk.release());
}


SXBlockInfoPtr CXBlobFileDriver::addReference(uint64_t aBlockId, SXBlockInfoPtr aBlk){
    if(fseek(mFile, 0, SEEK_END) == -1)
        return 0;
    auto orig_blk = std::static_pointer_cast<SXBlockInfoFS>(aBlk);
    std::unique_ptr<SXBlockInfoFS> blk(new SXBlockInfoFS(aBlockId, aBlk->mHash, orig_blk->mDataOffset));
    auto s = fwrite(blk.get(), sizeof(*blk), 1, mFile);
    if(s != 1)
        return 0;
    return SXBlockInfoPtr(blk.release());
}


SXBlockInfoPtr CXBlobFileDriver::getNextBlockInfo(){
    std::unique_ptr<SXBlockInfoFS> blk(new SXBlockInfoFS(0, 0, 0));
    auto s = fread(blk.get(), sizeof(*blk), 1, mFile);
    if(s != 1)
        return 0;
    if(blk->mDataOffset == 0){
        blk->mDataOffset = ftell(mFile);
        if(fseek(mFile, mInfo.mBlockSize, SEEK_CUR) == -1)
            return 0;
    }
    return SXBlockInfoPtr(blk.release());
}


std::vector<SXBlockInfoPtr> CXBlobFileDriver::blocksInfos(){
    std::vector<SXBlockInfoPtr> out;
    if(fseek(mFile, sizeof(mInfo), SEEK_SET) == -1)
        return out;
    while(auto blk = getNextBlockInfo())
        out.push_back(blk);
    return out;
}


CXDataPtr CXBlobFileDriver::getBlock(SXBlockInfoPtr aBlk) const {
    auto blk = std::static_pointer_cast<SXBlockInfoFS>(aBlk);
    std::unique_ptr<CXData> data(new CXData(mInfo.mBlockSize));
    fseek(mFile, blk->mDataOffset, SEEK_SET);
    auto s = fread(data->data(), 1, mInfo.mBlockSize, mFile);
    if(s != mInfo.mBlockSize)
        return 0;
    return CXDataPtr(data.release());
}


bool CXBlobFileDriver::writeHeader(){
    return fwrite(&mInfo, sizeof(mInfo), 1, mFile) == 1;
}


bool CXBlobFileDriver::readHeader(){
    return fread(&mInfo, sizeof(mInfo), 1, mFile) == 1;
}


bool CXBlobFileDriver::open(char const *aMode){
    if(mFile != 0)
        return false;
    mFile = fopen(mFileName.c_str(), aMode);
    return mFile != 0;
}
