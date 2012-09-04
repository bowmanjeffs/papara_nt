#ifndef __blast_partassign_h
#define __blast_partassign_h

#include <string>
#include <iostream>
#include <vector>
#include <map>

namespace partassign {

struct blast_hit {
    std::string qs_name;
    std::string ref_name;
    
    int qs_start;
    int qs_end;
    int ref_start;
    int ref_end;
    
    float bit_score;
//     std::string bit_score;
//     double evalue;
    
};

class partition {
public:
    partition() : start(-1), end(-1) {}
    int start;
    int end;
};


// static bool not_space( char x ) {
//     return !isspace(x); // doing the same with pre-c++11 functional is ridiculus
// }

blast_hit next_hit( std::istream &is );
partition next_partition( std::istream &is ); 


class part_assignment {
public:
    part_assignment( std::istream &blast_out, std::istream &part_file ) ;
    
//     const partassign::partition &partition( const std::string &name ) const ;
    const blast_hit &get_blast_hit( const std::string &qs_name ) const;
    const std::vector<partassign::partition> &partitions() const {
        return partitions_;
    }
    
private:
    std::vector<partassign::partition> partitions_;
    //std::map<std::string,int> assignments_;
    std::map<std::string,partassign::blast_hit> hits_;
    
    
};

}
#endif