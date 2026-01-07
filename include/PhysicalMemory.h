#ifndef PHYSICAL_MEMORY_H
#define PHYSICAL_MEMORY_H

#include <vector>
#include <list>
#include <cstddef>
#include <cstdint>

using namespace std;

enum AllocationStrategy{
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT
};

struct MemoryBlock{
    size_t start_address;
    size_t size;
    size_t padding;
    bool is_free;
    int id; 
};

class PhysicalMemory{
private:
    vector<uint8_t> ram;
    list<MemoryBlock> memory_map;
    size_t total_size;
    size_t word_alignment;
    
    int total_requests;
    int successful_allocations;
    AllocationStrategy current_strategy;
    int next_id; 

public:
    PhysicalMemory(size_t size_in_bytes=1024, size_t alignment=4);
    
    void setStrategy(AllocationStrategy strategy);
    int64_t malloc(size_t request_size);
    void free(int id);
    void dump();
    void stats();
};

#endif