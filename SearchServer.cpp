#include "SearchServer.h"

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queries_input) {
    std::string temp_string;
    std::string words;
    std::vector<std::string> temp_vector;
    std::vector<std::vector<RelativeIndex>> allResults;
    for (int i = 0; i < queries_input.size(); i++) {
        temp_vector.clear();
        temp_string = queries_input[i];
        for (int j = 0; j < temp_string.length(); ) {
            while (j < temp_string.length() && temp_string[j] == ' ') {
                ++j;
            }
            if (j >= temp_string.length()) break;
            words.clear();
            while (j < temp_string.length() && temp_string[j] != ' ') {
                words += temp_string[j];
                ++j;
            }
            temp_vector.push_back(words);
        }
        std::sort(temp_vector.begin(), temp_vector.end());
        temp_vector.erase(std::unique(temp_vector.begin(), temp_vector.end()), temp_vector.end());

        for (int j = 0; j < temp_vector.size(); j++) {
            for (int k = j + 1; k < temp_vector.size(); k++) {
                auto entryWordJ = _index.GetWordCount(temp_vector[j]);
                auto entryWordK = _index.GetWordCount(temp_vector[k]);
                if (entryWordJ.size() > entryWordK.size()) {
                    std::swap(temp_vector[j], temp_vector[k]);
                }
            }
        }

        std::vector<size_t> result_doc_id;
        for (int j = 0; j < temp_vector.size(); j++) {
            auto entries = _index.GetWordCount(temp_vector[j]);
            if (entries.empty()) {
                result_doc_id.clear();
                break;
            }
            if (j == 0) {
                for (const auto& e : entries) {
                    result_doc_id.push_back(e.doc_id);
                }
            } else {
                std::vector<size_t> result_doc_id_temp;
                for (const auto& e : entries) {
                    auto it = std::find(result_doc_id.begin(), result_doc_id.end(), e.doc_id);
                    if (it != result_doc_id.end()) {
                        result_doc_id_temp.push_back(e.doc_id);
                    }
                }
                result_doc_id = result_doc_id_temp;
                if (result_doc_id.empty()) {
                    break;
                }
            }
        }

        std::vector<RelativeIndex> result;
        if (result_doc_id.empty()) {

        }
        else {
            std::vector<size_t> absoluteRev;
            for (int j = 0; j < result_doc_id.size(); j++) {
                size_t sum = 0;
                for (int k = 0; k < temp_vector.size(); k++) {
                    auto entryWord = _index.GetWordCount(temp_vector[k]);
                    for (int f = 0; f < entryWord.size(); f++) {
                        if (entryWord[f].doc_id == result_doc_id[j]) {
                            sum+= entryWord[f].count;
                            break;
                        }
                    }
                }
                absoluteRev.push_back(sum);
            }

            size_t maxRev = 0;
            for (int j = 0; j < absoluteRev.size(); j++) {
                if (absoluteRev[j] > maxRev) {
                    maxRev = absoluteRev[j];
                }
            }

            std::vector<float> relativeRev;
            for (int j = 0; j < absoluteRev.size(); j++) {
                relativeRev.push_back(static_cast<float>(absoluteRev[j])/maxRev);
            }

            for (int j = 0; j < relativeRev.size(); j++) {
                for (int k = 0; k < relativeRev.size(); k++) {
                    if (relativeRev[j] > relativeRev[k]) {
                        float temp = relativeRev[j];
                        relativeRev[j] = relativeRev[k];
                        relativeRev[k] = temp;

                        size_t temp1 = result_doc_id[j];
                        result_doc_id[j] = result_doc_id[k];
                        result_doc_id[k] = temp1;
                    }
                }
            }

            if (relativeRev.size() > _max_responses) {
                relativeRev.resize(_max_responses);
                result_doc_id.resize(_max_responses);
            }

            for (int j = 0; j < result_doc_id.size(); j++) {
                RelativeIndex temp;
                temp.doc_id = result_doc_id[j];
                temp.rank = relativeRev[j];
                result.push_back(temp);
            }
        }
        allResults.push_back(result);
    }
    return allResults;
}