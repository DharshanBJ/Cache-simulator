/*
 * memsim.cc
 *
 *  Created on: Nov 19, 2017
 *      Author: dharshan
 */
#include<sstream>
#include<vector>
#include<iostream>
#include<string>
#include<fstream>

#include <iomanip>

using namespace std;

int address_bits = 32,cache_size = 4096,block_size = 16,block_offset = 4,associativity = 2,index_bits = 17;
int tag = 32-17-4;
int cst=131072;
int total = 0,reads = 0,writes = 0,cache_hits = 0,cache_misses = 0;

char* binary_character_to_hexadecimal(string chr)
{
    if(chr == "0") { return "0"; }
    else if(chr == "1") { return "1"; }
    else if(chr == "000") { return "0"; }
    else if(chr == "001") { return "1"; }
    else if(chr == "010") { return "2"; }
    else if(chr == "011") { return "3"; }
    else if(chr == "100") { return "4"; }
    else if(chr == "101") { return "5"; }
    else if(chr == "110") { return "6"; }
    else if(chr == "111") { return "7"; }
    
    return NULL;
}

char* hex_to_bin(char chr)
{
    switch(toupper(chr))
    {
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
        case '0': return "0000";
        case '1': return "0001";
        case '7': return "0111";
        case '8': return "1000";
        case '2': return "0010";
        case '4': return "0100";
        case '5': return "0101";
        case '3': return "0011";
        case '6': return "0110";
        case '9': return "1001";
        
    }
    return NULL;
}

int main(int argc,  char **argv){
    string filename(argv[1]);
    vector<string> bank1;
    vector<string> bank2;
    vector<int> valid1;
    vector<int> valid2;
    vector<int> lru;
    ifstream source;
    source.open(filename);
    vector<string> lines;
    string line;
    
    while(getline(source, line)){
        lines.push_back(line);
    }
    
    for(int i=0;i<cst;i++){
        valid1.push_back(0);
        valid2.push_back(0);
        lru.push_back(0);
        bank1.push_back("");
        bank2.push_back("");
    }
    
    for(int i=0;i<lines.size();i++){
        if(lines[i].substr(9, 1) == "W"){
            writes=writes+1;}
        else{
            reads=reads+1;
        }
        char ch = lines[i][2];
        string index;
        string bin_ch;
        string tag;
        
        bin_ch= hex_to_bin(ch);
        index.append(binary_character_to_hexadecimal(bin_ch.substr(3,1)));
        index.append(lines[i].substr(3, 4));
        tag.append(lines[i].substr(0, 2));
        tag.append(binary_character_to_hexadecimal(bin_ch.substr(0,3)));
        int x=0;
        int y=0;
        stringstream stream;
        stream <<  index;
        stream >> hex >> y;
        if(valid1[y] == 0 || valid2[y] == 0){
            if(bank1[y] == tag || bank2[y] == tag){
                cache_hits=cache_hits+1;
                if(bank1[y] != tag) {
                    lru[y] = 1;
                    
                } else{
                    lru[y] = 2;
                }
                continue;
            }
            cache_misses=cache_misses+1;
            if(valid1[y] != 0){
                lru[y] = 1;
                bank2[y] = tag;
                valid2[y] = 1;
            } else {
                lru[y] = 2;
                bank1[y] = tag;
                valid1[y] = 1;
            }
        }
 
        else{
   
            if(bank1[y] == tag || bank2[y] == tag){
                cache_hits=cache_hits+1;
                if(bank1[y] != tag) {
                    lru[y] = 1;
                    
                } else{
                    lru[y] = 2;
                }
            }
        
            else{
                cache_misses=cache_misses+1;
                if(lru[y] == 2){
                    bank2[y] = tag;
                    lru[y] = 1;
                    
                } else if(lru[y] == 1){
                    bank1[y] = tag;
                    lru[y] = 2;
                }
            }
        }
 
    }
    cout << "Total traces: " << lines.size() << endl;
    cout << "Reads: " << reads << endl;
    cout << "Writes: " << writes << endl;
    cout << "Cache hits: " << cache_hits << endl;
    cout << "Cache misses: " << cache_misses << endl;
    
}








