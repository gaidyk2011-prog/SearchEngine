#pragma once
#include <vector>
#include <string>
#include "SearchServer.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

class JsonConverter {
public:
    JsonConverter() = default;

    std::vector<std::string> GetTextDocuments();

    int GetResponsesLimit();

    std::vector<std::string> GetRequests();

    void putAnswers(std::vector<std::vector<RelativeIndex>> answers);
};