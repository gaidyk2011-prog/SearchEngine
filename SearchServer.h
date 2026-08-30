#pragma once
#include <algorithm>
#include "InvertedIndex.h"
#include <string>

struct RelativeIndex {
    size_t doc_id;
    float rank;
};

class SearchServer {
public:
    SearchServer(InvertedIndex& index, int max_responses) : _index(index), _max_responses(max_responses) {};

private:
    InvertedIndex& _index;
    int _max_responses;
public:
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);
};