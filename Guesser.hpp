#pragma once

#include <string>
using std::string;

namespace bullpgia {

    class Guesser {
        protected:
        unsigned int length;

        public:
        virtual string guess()=0;
        virtual void startNewGame(uint length) {this->length = length;};
        virtual void learn(string results) {};
    };
};