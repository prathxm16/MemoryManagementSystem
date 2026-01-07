#include "Cache.h"
#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;


int CacheLevel::log2_int(size_t n) {
    int targetlevel = 0;
    while (n >>= 1) ++targetlevel;
    return targetlevel;
}

CacheLevel::CacheLevel(string n, size_t size, size_t blk_size, size_t assoc, int latency) 
    : name(n), capacity_bytes(size), block_size(blk_size), associativity(assoc), latency_cycles(latency), hits(0), misses(0) {
    
    num_sets = capacity_bytes / (block_size * associativity);
    sets.resize(num_sets, vector<CacheLine>(associativity, {0, false}));
    fifo_next_victim.resize(num_sets, 0);

    offset_bits = log2_int(block_size);
    index_bits = log2_int(num_sets);
    tag_bits = 32 - index_bits - offset_bits;

    cout << "[" << name << "] Init: " << size << "B, Block: " << blk_size 
         << "B, Assoc: " << assoc << " (Latency: " << latency << " cycles)\n";
}

int CacheLevel::getLatency() const { return latency_cycles; }

bool CacheLevel::access(uint32_t address) {
    uint32_t index = (address >> offset_bits) & ((1 << index_bits) - 1);
    uint32_t tag = address >> (offset_bits + index_bits);

    for (const auto& line : sets[index]) {
        if (line.valid && line.tag == tag) {
            hits++;
            return true; 
        }
    }
    misses++;
    return false; 
}

void CacheLevel::install(uint32_t address) {
    uint32_t index = (address >> offset_bits) & ((1 << index_bits) - 1);
    uint32_t tag = address >> (offset_bits + index_bits);

    int way = fifo_next_victim[index];
    sets[index][way].valid = true;
    sets[index][way].tag = tag;
    fifo_next_victim[index] = (way + 1) % associativity;
}

void CacheLevel::printStats() {
    long long total = hits + misses;
    double hit_rate = (total > 0) ? (double)hits / total * 100.0 : 0.0;
    cout << name << " Stats: Hits=" << hits << " Misses=" << misses 
         << " Rate=" << fixed << setprecision(2) << hit_rate << "%\n";
}


Cache::Cache() : total_access_cycles(0), total_requests(0) {
    l1 = new CacheLevel("L1", 1024, 32, 1, 1);
    l2 = new CacheLevel("L2", 4096, 32, 2, 10);
    ram_latency = 100;
}

Cache::~Cache() { delete l1; delete l2; }

void Cache::reconfigure(int level, size_t size, size_t block, size_t assoc) {
    if (level == 1) {
        delete l1;
        l1 = new CacheLevel("L1", size, block, assoc, 1);
    } else if (level == 2) {
        delete l2;
        l2 = new CacheLevel("L2", size, block, assoc, 10);
    }
}

void Cache::access(uint32_t address, bool is_write) {
    (void)is_write; 
    total_requests++;
    int current_request_cycles = 0;

    current_request_cycles += l1->getLatency();
    if (l1->access(address)) {
        total_access_cycles += current_request_cycles;
        return; 
    }

    current_request_cycles += l2->getLatency();
    if (l2->access(address)) {
        l1->install(address);
        total_access_cycles += current_request_cycles;
        return;
    }

    current_request_cycles += ram_latency;
    l2->install(address);
    l1->install(address);
    total_access_cycles += current_request_cycles;
}

void Cache::stats() {
    cout << "\n--- Cache Statistics ---\n";
    l1->printStats();
    l2->printStats();
    
    double amat = (total_requests > 0) ? (double)total_access_cycles / total_requests : 0.0;
    
    cout << "--- Performance Metrics ---\n";
    cout << "Total Requests: " << total_requests << "\n";
    cout << "Total Cycles Spent: " << total_access_cycles << "\n";
    cout << "Avg Memory Access Time (AMAT): " << amat << " cycles\n";
    cout << "------------------------\n";
}