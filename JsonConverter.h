#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "SearchServer.h"

class JsonConverter {
public:
    JsonConverter() = default;

    std::vector<std::string> GetTextDocuments();

    int GetResponsesLimit();

    std::vector<std::string> GetRequests();

    void putAnswers(std::vector<std::vector<RelativeIndex>> answers);
};