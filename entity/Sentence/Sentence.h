//
// Created by zeng on 23-12-21.
//

#ifndef CPR_TEST_SENTENCE_H
#define CPR_TEST_SENTENCE_H


#include <string>

namespace Sentence {
    class Sentence {
    public:
        Sentence();

        //获取句子的中文前六个字符
        std::string_view GetFirstSixCharacters() const;

        //获取句子的六个字符后的所有字符
        std::string_view GetLastCharacters() const;

    private:
        std::string sentence;

        //api地址
        static constexpr std::string_view urlSentence = "https://v1.hitokoto.cn/?c=d&c=i&c=k&c=h&c=a&c=b&c=j&c=l&c=m&c=n&c=o&c=p&c=q&c=r&c=s&c=t&c=u&c=v&c=w&c=x&c=y&c=z&c=f&c=g&c=c&c=e&min_length=11&max_length=12";
    };
}
#endif //CPR_TEST_SENTENCE_H
