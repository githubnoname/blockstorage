#ifndef CXBLOCKMAP
#define CXBLOCKMAP

#include<map>

#include"axdriver.h"


class CXBlockMap{
public:
    SXBlockInfoPtr findHash(uint64_t aHash) const;
    SXBlockInfoPtr findId(uint64_t aId) const;
    void addBlock(SXBlockInfoPtr aBlock);
    std::vector<uint64_t> ids() const;
    size_t size() const { return mIdToBlock.size(); }

private:
    std::map<uint64_t, SXBlockInfoPtr> mIdToBlock;
};


#endif
