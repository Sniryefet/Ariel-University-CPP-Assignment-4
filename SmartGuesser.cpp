#include "SmartGuesser.hpp"
#include "calculate.hpp"
#include <string>
#include <cmath>
#include <algorithm>
#include <iostream>
#define print(x) std::cout << x
#define println(x) std::cout << x << std::endl
#define ZERO '0'
#define ALGO_SWITCH_LENGTH_THRESHOLD 4
using std::string, std::reverse, bullpgia::SmartGuesser, bullpgia::bapResults, bullpgia::calculateBullAndPgia;

// Accepts input of the format "<number>,<number>"
bapResults parse_results_string(string results_string)
{
    bapResults results;

    unsigned long long comma_pos = results_string.find(',');
    if (comma_pos == string::npos)
        throw "err"; //TODO: IMPLEMEMNT

    string hit_str = results_string.substr(0, comma_pos);
    string miss_str = results_string.substr(comma_pos + 1, string::npos);

    results.hit = std::stoi(hit_str);
    results.miss = std::stoi(miss_str);
    return results;
}

void bullpgia::SmartGuesser::first_filter()
{

    unsigned long max_options = pow(10.0, length);

    for (long i = 0; i < max_options; i++)
    {
        string opt_guess;
        for (size_t k = 0; k < length; k++)
            opt_guess += ZERO + (i / (int)(pow(10.0, k))) % 10;

        reverse(opt_guess.begin(), opt_guess.end());

        bapResults result = parse_results_string(calculateBullAndPgia(guess_history[0], opt_guess));

        if (result.hit == guess_history_results[0].hit && result.miss == guess_history_results[0].miss)
            optional_guesses->push_back(opt_guess);
    }
}

void bullpgia::SmartGuesser::filter()
{
    string opt_guess;

    vector<string> *temp = new vector<string>(optional_guesses->size());

    vector<string>::iterator ptr;
    for (ptr = optional_guesses->begin(); ptr < optional_guesses->end(); ++ptr)
    {
        opt_guess = *ptr;
        //std::cout << "the opt guess: " << opt_guess << std::endl;
        bool valid = true;
        for (size_t i = 0; i < guess_history.size() && valid; i++)
        {
            if (guess_history[i] == opt_guess)
                valid = false;

            bapResults result = parse_results_string(calculateBullAndPgia(guess_history[i], opt_guess));
            if (result.hit != guess_history_results[i].hit || result.miss != guess_history_results[i].miss)
                valid = false;
        }

        if (valid)
        {
            temp->push_back(opt_guess);
        }
    }

    vector<string> *vptr = optional_guesses;
    optional_guesses = temp;
    delete vptr;
}

string bullpgia::SmartGuesser::guess()
{
    if (this->length < ALGO_SWITCH_LENGTH_THRESHOLD)
    {
        string guess = this->next_guess;
        this->guess_history.push_back(guess);

        return guess;
    }
    else
        return this->next_guess;
    
}

/* This function creates a guess for a game with higher length of string
*  this creates a guess of the form "xxxxxyx" where x is a base digit and 
*  moving digit is a y digits. Remark: y can be place anywhere in the string and not only at the example above.
*       Input: None. (using class fields)
*       Output: the guess as a string.
*/
string SmartGuesser::create_long_guess()
{
    if (matchesDigitsFound == length)
        return finalString;

    string str(length, baseDig);
    str[movingDigPos] = movingDig;

    return str;
}

/*
Notify the gusser about starting a new game.
    input : unsigned length as the secret code length
    Output: None.
*/
void bullpgia::SmartGuesser::startNewGame(uint length)
{
    //reset 
    this->length       = length;
    this->movingDigPos = length;
    baseDig            = 0;                 // reset
    movingDig          = 0;                 // reset
    matchesDigitsFound = 0;                 // reset
    stage              = Stage::countTo10;

    if (length < ALGO_SWITCH_LENGTH_THRESHOLD)
    {                          //short algo
        this->next_guess = ""; // "1234";
        for (int i = 1; i <= length; ++i)
        {
            char c = ZERO + i % 10;
            this->next_guess += c;
        }
        first_time = true;

        if (optional_guesses)
        {
            optional_guesses->clear();
            optional_guesses->reserve(pow(10, length));
        }
        else
        {
            optional_guesses = new vector<string>(pow(10, length));
        }

        guess_history.clear();
        guess_history_results.clear();
    }
    else
    { //long algo
        this->IcountTo10  = 0;
        //setting first guess to be "00000...."
        this->next_guess  = string(this->length, ZERO);
        this->finalString = string(this->length, ZERO - 1);
    }
}

void SmartGuesser::advance_moving_digit()
{

    for (; movingDigPos < finalString.length(); movingDigPos++)
    { // choose first position for moving digit
        if (finalString[movingDigPos] == ZERO - 1)
            break;
    }
}

void SmartGuesser::countDigitOccurrences(bapResults last_guess_results)
{
    this->appearances[IcountTo10++] = last_guess_results.hit; //appearances

    if (IcountTo10 == 10)
    {
        stage = Stage::digitScan;
        return;
    }
    else
        next_guess = string(this->length, ZERO + this->IcountTo10);
}

void SmartGuesser::choose2digits()
{
    if (is2newDigitsExists())
    {
        if (baseDig != 0 && movingDig != 0)
        {
            appearances[baseDig   - ZERO] = -1; // the appearances are no longer needed
            appearances[movingDig - ZERO] = -1;
        }

        find2newDigits();
    }
    else
    {
        appearances[movingDig - ZERO] = -1; // reset the moving digit

        for (size_t i = 0; i < 10; i++)
        { // find the last movingDigit by finding the last digit in appearances array

            if (appearances[i] > 0)
            { // if found the last movingDigit
                movingDig = ZERO + i;
                break;
            }
        }
    }
}

void SmartGuesser::find2newDigits()
{
    baseDig = 0;   // reset
    movingDig = 0; // reset

    for (size_t i = 0; i < 10; i++)
    {
        // choose two digits

        if (appearances[i] > 0)
        {
            if (baseDig == 0) // if  basedig hadnt been chosed.
                baseDig = ZERO + i;
            else
            {
                movingDig = ZERO + i;
                break;
            }
        }
    } // for
}

bool SmartGuesser::is2newDigitsExists()
{
    unsigned short digitsFound = 0;
    for (size_t i = 0; i < 10 && digitsFound < 2; i++)
    {
        if (appearances[i] > 0)
            digitsFound++;
    }

    return (digitsFound >= 2);
}

void SmartGuesser::checkResult(bapResults last_guess_results)
{
    const unsigned int amount_of_base_digit = appearances[baseDig - ZERO];

    if (last_guess_results.hit == amount_of_base_digit + 1)
    {
        this->finalString[movingDigPos] = movingDig;
        matchesDigitsFound++;
    }

    if (last_guess_results.hit == amount_of_base_digit - 1)
    {
        this->finalString[movingDigPos] = baseDig;
        matchesDigitsFound++;
    }

    // movingDigPos++;
}

void SmartGuesser::shortStrategy(bapResults last_guess_results)
{
    guess_history_results.push_back(last_guess_results);

    if (first_time)
    {
        first_filter();
        first_time = false;
    }
    else
        filter();

    this->next_guess = optional_guesses->back();
    optional_guesses->pop_back();
}

void SmartGuesser::longStrategy(bapResults last_guess_results)
{
    if (stage == Stage::countTo10) //if(IcountTo10 <= 10)
    {
        countDigitOccurrences(last_guess_results);
    }

    if (stage == Stage::digitScan)
    {
        if (movingDigPos == length)
        {
            choose2digits();
            movingDigPos = 0; // reset
        }
        else
        {
            checkResult(last_guess_results);
            movingDigPos++;
        }

        advance_moving_digit();
        this->next_guess = create_long_guess();
    }
}

void bullpgia::SmartGuesser::learn(string results)
{
    bapResults last_guess_results = parse_results_string(results); // parse string
    
    //short algo
    if (this->length < ALGO_SWITCH_LENGTH_THRESHOLD)
    {
        shortStrategy(last_guess_results);
    }
    //Long algo
    else
    {
        longStrategy(last_guess_results);
    }
    //end Long algo
}
