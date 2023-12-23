//
// Created by zeng on 23-12-21.
//

#include <iostream>
#include "Sentence.h"
#include "cpr/cpr.h"
#include "nlohmann/json.hpp"

Sentence::Sentence::Sentence() {
    // 从api获取句子
    auto response{cpr::Get(cpr::Url{urlSentence})};
    // 解析json
    auto json{nlohmann::json::parse(response.text)};
    std::cout << json << std::endl;
    // 获取句子
    this->sentence = json.at(0).at("hitokoto").get<std::string>();
    // 打印
    std::cout << this->sentence << std::endl;
}

std::string_view Sentence::Sentence::GetFirstSixCharacters() const {
    return this->sentence.substr(0, 6);
}

std::string_view Sentence::Sentence::GetLastCharacters() const {
    return this->sentence.substr(6);
}
