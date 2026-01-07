#include <iostream>
#include <string>
#include <sstream>
#include "PhysicalMemory.h"
#include "Cache.h"

using namespace std;

int main(){
    PhysicalMemory ram(4096); 
    Cache cache;
    string line,command;
    cout<<"$ ";
    while(getline(cin, line)){
        if(line.empty()){
            cout << "$ "; 
            continue;
        }
        stringstream ss(line);
        ss>>command;

        if(command=="exit" || command=="quit") break;
        
        else if(command=="init"){
            size_t size;
            ss>>size;
            ram = PhysicalMemory(size);
        }
        else if(command=="set"){
            string subcmd,mode;
            ss>>subcmd>>mode;
            if(subcmd=="allocator"){
                if(mode=="first_fit") ram.setStrategy(FIRST_FIT);
                else if(mode=="best_fit") ram.setStrategy(BEST_FIT);
                else if(mode=="worst_fit") ram.setStrategy(WORST_FIT);
                else cout<<"Unknown mode. Use: first_fit, best_fit, worst_fit\n";
            }
        }
        else if(command=="malloc"){
            size_t size;
            if (ss>>size) ram.malloc(size);
        }
        else if(command=="free"){
            int id;
            if (ss>>id) ram.free(id);
        }
        else if(command=="dump") ram.dump();
        else if(command=="stats") ram.stats();
        else if(command=="read"){
            uint32_t addr; 
            if(ss>>hex>>addr) cache.access(addr,false);
            else cout<<"Usage: read <hex_addr>\n";
        }
        else if(command=="write"){
            uint32_t addr; 
            if (ss>>hex>>addr) cache.access(addr,true);
            else cout<<"Usage: write <hex_addr>\n";
        }
        else if(command=="cache_stats"){
            cache.stats();
        }
        else if(command=="cache_config"){
            int level;
            size_t size,blk,assoc;
            if(ss>>level>>size>>blk>>assoc){
                cache.reconfigure(level,size,blk,assoc);
            }else{
                cout<<"Usage: cache_config <1|2> <size> <block_size> <assoc>\n";
            }
        }
        else if(command=="help"){
            cout<<"Available Commands:\n"
                <<"  init <size>             Initialize physical memory with <size> bytes\n"
                <<"  set allocator <mode>    Set allocation strategy (first_fit, best_fit, worst_fit)\n"
                <<"  malloc <size>           Allocate <size> bytes of physical memory\n"
                <<"  free <id>               Free the memory block with ID <id>\n"
                <<"  dump                    Display the current physical memory map\n"
                <<"  stats                   Show physical memory fragmentation statistics\n"
                <<"  read <hex_addr>         Simulate a CPU read at <hex_addr>\n"
                <<"  write <hex_addr>        Simulate a CPU write at <hex_addr>\n"
                <<"  cache_stats             Show cache hit/miss and AMAT statistics\n"
                <<"  cache_config <l> <s> <b> <a>  Configure cache level <l> (1 or 2) with size <s>,\n"
                <<"                                block size <b>, and associativity <a>\n"
                <<"  exit / quit             Exit the simulator\n";
        }
        else cout<<"Unknown command.\n";

        cout<<"$ ";
    }
    return 0;
}