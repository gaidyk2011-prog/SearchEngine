#include "InvertedIndex.h"

void indexDocument(std::string doc, std::map<std::string, std::vector<Entry>> &freq_dictionary, size_t doc_id, std::mutex &mtx) {
    std::string tempWord;
    std::vector<std::string> words;
    for (int j = 0; j < doc.length(); j++) {
        if (doc[j] != ' ') {
            tempWord.clear();
            while (j < (int)doc.length() && doc[j] != ' ') {
                tempWord += doc[j];
                ++j;
            }
            words.push_back(tempWord);
        }
    }

    std::map<std::string, size_t> temp_freq_dictionary;

    for (int j = 0; j < words.size(); j++) {
        temp_freq_dictionary[words[j]]++;
    }

    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& [word, count] : temp_freq_dictionary) {
        freq_dictionary[word].push_back({doc_id, count});
    }
}

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs.clear();
    docs = input_docs;
    freq_dictionary.clear();

    std::vector<std::thread> threads;

    for (size_t i = 0; i < docs.size(); ++i) {
        threads.emplace_back(indexDocument, docs[i], std::ref(freq_dictionary), i, std::ref(mtx));
    }

    for (auto& t : threads) {
        t.join();
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
    auto it = freq_dictionary.find(word);
    if (it != freq_dictionary.end()) {
        return it->second;
    }
    return {};
}

std::map<std::string, std::vector<Entry>> InvertedIndex::GetFreqDictionary() {
    return freq_dictionary;
}

