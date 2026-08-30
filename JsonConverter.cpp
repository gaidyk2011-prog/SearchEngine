#include "JsonConverter.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

std::vector<std::string> JsonConverter::GetTextDocuments() {
    std::ifstream config_file("config.json");
    if (!config_file.is_open()) {
        throw std::runtime_error("config file if missing");
    }

    json config_json;
    try {
        config_file >> config_json;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    std::vector<std::string> files_path;
    auto files = config_json["files"];
    for (auto file : files) {
        files_path.push_back(file.get<std::string>());
    }

    std::vector<std::string> textDocuments;
    for (int i = 0; i < files_path.size(); i++) {
        std::ifstream textInFile(files_path[i]);
        if (!textInFile.is_open()) {
            throw std::runtime_error("file " + files_path[i] + " not found");
        }
        std::string text;
        try {
            std::stringstream buffer;
            buffer << textInFile.rdbuf();
            textDocuments.push_back(buffer.str());
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    return textDocuments;
}

int JsonConverter::GetResponsesLimit() {
    std::ifstream config_file("config.json");
    if (!config_file.is_open()) {
        throw std::runtime_error("config file if missing");
    }
    json config_json;
    try {
        config_file >> config_json;
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }

    int max_responses;
    auto config = config_json["config"];
    if (config["max_responses"].empty()) max_responses = 5;
    else max_responses = config["max_responses"];

    return max_responses;
}

std::vector<std::string> JsonConverter::GetRequests() {
    std::vector<std::string> requests;
    std::ifstream requests_file("requests.json");

    if (!requests_file.is_open()) {
        throw std::runtime_error("requests file if missing");
    }
    json requests_json;
    try {
        requests_file >> requests_json;
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }

    auto requestsInJson = requests_json["requests"];
    for (auto request : requestsInJson) {
        requests.push_back(request.get<std::string>());
    }

    return requests;
}

void JsonConverter::putAnswers(std::vector<std::vector<RelativeIndex>> allResults) {
    std::ofstream answers_file("answers.json");
    json answers_json;
    answers_json["answers"] = json::object();

    for (size_t i = 0; i < allResults.size(); ++i) {
        std::string key = "request";
        int num = i + 1;
        if (num < 10) {
            key += "00";
        } else if (num < 100) {
            key += "0";
        }
        key += std::to_string(num);

        json requestObj;

        if (allResults[i].empty()) {
            requestObj["result"] = "false";
        } else {
            requestObj["result"] = "true";
            json relevanceArray = json::array();
            for (const auto& rel : allResults[i]) {
                json entry;
                entry["docid"] = rel.doc_id;
                entry["rank"] = rel.rank;
                relevanceArray.push_back(entry);
            }
            requestObj["relevance"] = relevanceArray;
        }

        answers_json["answers"][key] = requestObj;
    }
    answers_file << answers_json.dump(4);
    answers_file.close();
}