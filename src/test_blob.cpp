#include"cxblob.h"
#include"cxblobfiledriver.h"
#include"testutils.h"


#define BLOCK_SIZE   8
#define BLOCKS_LIMIT 5


void print_block(std::vector<uint8_t> const &aData, FILE *out = stdout){
    for(auto &x : aData)
        fprintf(out, "0x%02X " , int(x));
    fprintf(out, "\n");
}


bool compare_data(CXDataPtr aBlk, std::vector<uint8_t> const &aExpected){
    if(aBlk == 0)
        ERROR("Cannot retrieve data");
    if(*aBlk != aExpected){
        fprintf(stderr, "Retrieved data: "); print_block(*aBlk, stderr);
        fprintf(stderr, "Expected:       "); print_block(aExpected, stderr);
        return false;
    }
    return true;
}


bool check_insert(CXBlob *aBlob, uint64_t aBlockId, std::vector<uint8_t> const &aData){
    if(!aBlob->putBlock(aBlockId, aData))
        ERROR("Could not insert valid data");
    auto blk = aBlob->getBlock(aBlockId);
    return compare_data(blk, aData);
}


static std::vector<uint8_t> v1 = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
static std::vector<uint8_t> v2 = { 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
static std::vector<uint8_t> v3 = { 0x0D, 0x0E, 0x0A, 0x0D, 0x0B, 0x0E, 0x0E, 0x0F };
static std::vector<uint8_t> v4 = { 0x07, 0x06, 0x05, 0x05, 0xBB, 0xEE, 0x0E, 0x0F };


bool test_blob_new(){
    // prepare blob
    AXDriverPtr driver(new CXBlobFileDriver("test_blob.xxx"));
    CXBlob blob(driver);

    // init blob
    if(!blob.init(BLOCK_SIZE, BLOCKS_LIMIT))
        ERROR("Cannot init blob");

    // get block by wrong id
    if(blob.getBlock(42) != 0)
        ERROR("Data has been retrieved by invalid id");

    // put invalid block (wrong vector size)
    if(blob.putBlock(1, { 0x00 }))
        ERROR("Invalid data has been inserted");

    // INSERT 1
    // put valid block
    if(!check_insert(&blob, 1, v1))
        return false;

    // put invalid block (the same id as previous one)
    if(blob.putBlock(1, v2) != 0)
        ERROR("Block has been inserted with id collision");

    return true;
}


bool test_blob_reuse(){
    // prepare blob
    AXDriverPtr driver(new CXBlobFileDriver("test_blob.xxx"));
    CXBlob blob(driver);

    // init blob
    if(!blob.init())
        ERROR("Cannot init blob");

    // INSERT 2
    if(!check_insert(&blob, 2, v2))
        return false;

    // INSERT 3
    // put reference to the first block, inserted from previous test
    if(!check_insert(&blob, 3, v1))
        return false;

    // INSERT 4
    if(!check_insert(&blob, 4, v3))
        return false;

    // INSERT 5
    // put reference to the previous block
    if(!check_insert(&blob, 5, v3))
        return false;

    // put extra block
    if(blob.putBlock(6, v4) != 0)
        ERROR("Extra block has been inserted");

    // .. or a reference
    if(blob.putBlock(7, v1) != 0)
        ERROR("Extra block has been inserted");

    return true;
}

int main(){
    return !(TEST(test_blob_new) &&
             TEST(test_blob_reuse));
}
