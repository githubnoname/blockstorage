#include"cxblockmap.h"


SXBlockInfoPtr CXBlockMap::findHash(uint64_t aHash) const {
    for(auto it = mIdToBlock.begin(); it != mIdToBlock.end(); it++)
        if(it->second->mHash == aHash)
            return it->second;
    return 0;
}

SXBlockInfoPtr CXBlockMap::findId(uint64_t aId) const {
    auto it = mIdToBlock.find(aId);
    if(it != mIdToBlock.end())
        return it->second;
    return 0;
}

void CXBlockMap::addBlock(SXBlockInfoPtr aBlock){
    mIdToBlock.insert({ aBlock->mId, aBlock });
}

std::vector<uint64_t> CXBlockMap::ids() const {
    std::vector<uint64_t> res;
    for(auto &x : mIdToBlock)
        res.push_back(x.first);
    return res;
}
