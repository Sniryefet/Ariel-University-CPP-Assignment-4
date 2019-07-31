
#include "Guesser.hpp"
#include "calculate.hpp"
#include <vector>
#include <iostream>
using std::vector, bullpgia::Guesser;

namespace bullpgia
{

struct bapResults
{
    unsigned int hit;
    unsigned int miss;
};

enum Stage
{
    countTo10,
    digitScan
};

/**
 * SmartGuesser is a guesser that guesses correctly in less than 10 tries!
 */
class SmartGuesser : public Guesser
{
private:
    unsigned int length;

    //short algorithem
    vector<string> guess_history;
    vector<bapResults> guess_history_results;
    vector<string> *optional_guesses;
    string next_guess;
    bool first_time;

    //long algorithem
    string finalString; //need to change to a more meaningfull name (string to hold the result of the long algorithem)
    int IcountTo10;
    int appearances[10] = {0}; //the index is the digit and the value is the number of apearences in the string
    unsigned short movingDigPos;
    unsigned short matchesDigitsFound;
    char baseDig;
    char movingDig;

    // temp
    Stage stage;

    //long algorithm methodsssssssssss
    string create_long_guess();
    void advance_moving_digit();
    void longStrategy(bapResults);
    void countDigitOccurrences(bapResults);
    void choose2digits();
    void checkResult(bapResults);
    bool is2newDigitsExists();
    void find2newDigits();

    //short algorithm
    void shortStrategy(bapResults);

    // private methods
    void first_filter();
    void filter();

public:
    SmartGuesser() : optional_guesses(nullptr){};
    ~SmartGuesser()
    {
        if (optional_guesses)
            delete optional_guesses;
    };
    string guess() override;

    void startNewGame(uint length) override;

    void learn(string results) override;
};

}; // namespace bullpgia