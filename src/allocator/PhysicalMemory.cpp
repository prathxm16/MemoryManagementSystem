#include "PhysicalMemory.h"
#include <iostream>
#include <climits>
#include <iomanip>
#include <cmath>

using namespace std;

PhysicalMemory::PhysicalMemory(size_t size_in_bytes, size_t alignment) 
    : total_size(size_in_bytes), word_alignment(alignment), 
      total_requests(0), successful_allocations(0), 
      current_strategy(FIRST_FIT), next_id(1) {
    
    ram.resize(total_size, 0);
    memory_map.clear();
    memory_map.push_back({0, total_size, 0, true, -1});
    cout << "[RAM] Initialized: " << total_size << " bytes.\n";
}

void PhysicalMemory::setStrategy(AllocationStrategy strategy) {
    current_strategy = strategy;
    string s_name = (strategy == FIRST_FIT) ? "First Fit" : (strategy == BEST_FIT) ? "Best Fit" : "Worst Fit";
    cout << "Allocator set to: " << s_name << "\n";
}

int64_t PhysicalMemory::malloc(size_t request_size) {
    total_requests++;
    int pid = next_id++; 
    size_t actual_size = request_size;
    size_t padding = 0;
    
    if (request_size % word_alignment != 0) {
        padding = word_alignment - (request_size % word_alignment);
        actual_size = request_size + padding;
    }

    auto best_block_it = memory_map.end();
    
    if(current_strategy==FIRST_FIT){
        for (auto it = memory_map.begin(); it != memory_map.end(); ++it) {
            if (it->is_free && it->size >= actual_size) {
                best_block_it = it;
                break; 
            }
        }
    }else if(current_strategy==BEST_FIT){
        size_t min_diff = SIZE_MAX;
        for (auto it = memory_map.begin(); it != memory_map.end(); ++it) {
            if (it->is_free && it->size >= actual_size) {
                size_t diff = it->size - actual_size;
                if (diff < min_diff) {
                    min_diff = diff;
                    best_block_it = it;
                }
            }
        }
    }else if(current_strategy==WORST_FIT){
        size_t max_size = 0;
        for (auto it = memory_map.begin(); it != memory_map.end(); ++it) {
            if (it->is_free && it->size >= actual_size) {
                if (it->size > max_size) {
                    max_size = it->size;
                    best_block_it = it;
                }
            }
        }
    }

    if(best_block_it!=memory_map.end()){
        if(best_block_it->size > actual_size){
            MemoryBlock free_chunk;
            free_chunk.start_address = best_block_it->start_address + actual_size;
            free_chunk.size = best_block_it->size - actual_size;
            free_chunk.padding = 0;
            free_chunk.is_free = true;
            free_chunk.id = -1;

            best_block_it->size = actual_size;
            best_block_it->padding = padding;
            best_block_it->is_free = false;
            best_block_it->id = pid;

            memory_map.insert(next(best_block_it), free_chunk);
        }else{
            best_block_it->is_free = false;
            best_block_it->padding = padding;
            best_block_it->id = pid;
        }
        successful_allocations++;
        cout << "Allocated block id=" << pid << " at address=0x" 
             << hex << uppercase << best_block_it->start_address << dec << "\n";
        return best_block_it->start_address;
    }else{
        cout << "Error: Allocation failed (Not enough contiguous memory)\n";
        return -1;
    }
}

void PhysicalMemory::free(int id) {
    bool found = false;
    for (auto it = memory_map.begin(); it != memory_map.end(); ++it) {
        if (!it->is_free && it->id == id) {
            it->is_free = true;
            it->id = -1;
            it->padding = 0;
            found = true;
            cout << "Block " << id << " freed";
            
            auto next_it = next(it);
            if (next_it != memory_map.end() && next_it->is_free) {
                it->size += next_it->size;
                memory_map.erase(next_it);
                cout << " (merged next)";
            }
            if (it != memory_map.begin()) {
                auto prev_it = prev(it);
                if (prev_it->is_free) {
                    prev_it->size += it->size;
                    memory_map.erase(it);
                    cout << " (merged prev)";
                }
            }
            cout << "\n";
            break;
        }
    }
    if (!found) cout << "Error: Block ID " << id << " not found.\n";
}

void PhysicalMemory::dump() {
    cout << "\n--- Memory Dump ---\n";
    for (const auto& block : memory_map) {
        cout << "[0x" << hex << uppercase << setfill('0') << setw(4) << block.start_address 
             << " - 0x" << setw(4) << (block.start_address + block.size - 1) << dec << "] ";
        if (block.is_free) cout << "FREE (" << block.size << " bytes)";
        else cout << "USED (id=" << block.id << ", size=" << block.size << ", pad=" << block.padding << ")";
        cout << "\n";
    }
    cout << "-------------------\n";
}

void PhysicalMemory::stats() {
    size_t total_allocated = 0;
    size_t total_free = 0;
    size_t total_internal_frag = 0;
    size_t largest_free_block = 0;

    for (const auto& block : memory_map) {
        if (block.is_free) {
            total_free += block.size;
            if (block.size > largest_free_block) largest_free_block = block.size;
        } else {
            total_allocated += block.size;
            total_internal_frag += block.padding;
        }
    }
    double ext_frag_percent = (total_free > 0) ? (1.0 - ((double)largest_free_block / total_free)) * 100.0 : 0.0;
    cout << "Total memory:           " << total_size << "\n";
    cout << "Used memory:            " << total_allocated << "\n";
    cout << "Free memory:            " << total_free << "\n";
    cout << "Internal fragmentation: " << total_internal_frag << " bytes\n";
    cout << "External fragmentation: " << fixed << setprecision(1) << ext_frag_percent << "%\n";
}