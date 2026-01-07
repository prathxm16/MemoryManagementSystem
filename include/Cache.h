#ifndef CACHE_H
#define CACHE_H

#include <string>
#include <vector>
#include <cstdint>

struct CacheLine {
    uint32_t tag;
    bool valid;
};

class CacheLevel {
private:
    std::string name;
    size_t capacity_bytes;
    size_t block_size;
    size_t associativity;
    size_t num_sets;
    int latency_cycles;

    int offset_bits;
    int index_bits;
    int tag_bits;

    std::vector<std::vector<CacheLine>> sets;
    std::vector<int> fifo_next_victim; 

    long long hits;
    long long misses;

    int log2_int(size_t n);

public:
    CacheLevel(std::string n, size_t size, size_t blk_size, size_t assoc, int latency);
    int getLatency() const;
    bool access(uint32_t address);
    void install(uint32_t address);
    void printStats();
};

class Cache {
private:
    CacheLevel* l1;
    CacheLevel* l2;
    int ram_latency;
    
    long long total_access_cycles;
    long long total_requests;

public:
    Cache();
    ~Cache();

    void reconfigure(int level, size_t size, size_t block, size_t assoc);
    void access(uint32_t address, bool is_write);
    void stats();
};

#endif