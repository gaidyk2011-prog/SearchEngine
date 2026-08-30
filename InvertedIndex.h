#pragma once
#include <map>
#include <vector>
#include <string>
#include <thread>
#include <mutex>

struct Entry {
    size_t doc_id, count;
};

class InvertedIndex {
private:
    std::map<std::string, std::vector<Entry>> freq_dictionary;
    std::vector<std::string> docs;
    std::mutex mtx;
public:
    std::map<std::string, std::vector<Entry>> GetFreqDictionary();

    InvertedIndex() = default;

    void UpdateDocumentBase(std::vector<std::string> input_docs);

    std::vector<Entry> GetWordCount(const std::string &word);
};