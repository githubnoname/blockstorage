#include<stdio.h>
#include<string.h>

#include"cxblob.h"
#include"cxblobfiledriver.h"


#define BLOCK_SIZE 64
static char const *ARGV0;


int error(char const *aText){
    fprintf(stderr, "%s\n", aText);
    return 1;
}


int help(int code=0){
    printf("Usage: %s -f <file> [--init <rows>] {--put <id> <data> | --get <id>}\n", ARGV0);
    return code;
}


void print_block(uint64_t aBlockId, CXDataPtr const &aBlk){
    printf("%d -", int(aBlockId));
    if(aBlk == 0)
        printf(" Fail");
    else
        for(auto &x : *aBlk)
            printf(" 0x%02X" , int(x));
    printf("\n");
}


int main(int argc, char **argv){
    ARGV0 = argv[0];
    char const *fname = 0;
    char const *rows = 0;
    char const *put_id = 0;
    char const *put_data = 0;
    char const *get_id = 0;

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-f") == 0 && argc > i + 1)
            fname = argv[++i];
        else if(strcmp(argv[i], "--init") == 0 && argc > i + 1)
            rows = argv[++i];
        else if(strcmp(argv[i], "--put") == 0 && argc > i + 2){
            put_id = argv[++i];
            put_data = argv[++i];
        }
        else if(strcmp(argv[i], "--get") == 0 && argc > i + 1)
            get_id = argv[++i];
        else
            return help(1);
    }

    if(fname == 0)
        return error("You have to specify filename to open");
    AXDriverPtr driver(new CXBlobFileDriver(fname));
    std::unique_ptr<CXBlob> blob(new CXBlob(driver));

    bool ok = false;
    if(rows == 0)
        ok = blob->init();
    else
        ok = blob->init(BLOCK_SIZE, atoi(rows));

    if(!ok)
        return error("Init failed");

    if(put_id != 0 && put_data != 0){
        auto id = atoi(put_id);
        std::vector<uint8_t> data(BLOCK_SIZE);
        for(size_t i = 0; i < strlen(put_data) && i < BLOCK_SIZE; i++)
            data[i] = put_data[i];
        if(!blob->putBlock(id, data))
            return error("Can't insert data");
    }
    else if(get_id != 0){
        auto id = atoi(get_id);
        auto data = blob->getBlock(id);
        if(data == 0)
            return error("Can't get data");
        printf("%d => '%.*s'\n", id, BLOCK_SIZE, data->data());
    }

    return 0;
}
