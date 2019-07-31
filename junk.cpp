#include "SmartGuesser.hpp"
#include "DummyChoosers.hpp"
//#include "DummyGuessers.hpp"
#include "play.hpp"
#include "calculate.hpp"
#include <iostream>
#define AMOUNT 10000 // 10^4
#define ZERO '0'
#include <thread>
#include <chrono>
#include <vector>
using std::vector, bullpgia::bapResults, bullpgia::calculateBullAndPgia, bullpgia::SmartGuesser;

bapResults parse_results_string(string results_string);

struct opt
{
    char str[4];
};

struct filter_pack
{
    opt *options_ptr;
    unsigned int size;
};

void create_options(opt options[], unsigned int amount)
{
    unsigned int calc[4];
    for (int i = 0; i < amount; ++i)
    {
        calc[3] = i % 10;
        calc[2] = (i / 10) % 10;
        calc[1] = (i / 100) % 10;
        calc[0] = (i / 1000) % 10;

        for (int k = 0; k < 4; ++k)
            options[i].str[k] = ZERO + calc[k];
    }
}

filter_pack filter_options(opt options[], opt history[], bapResults results[], unsigned int amount, unsigned int h_len)
{
    //std::cout << amount << std::endl;
    vector<opt> *candidates = new vector<opt>;
    bool is_fit;
    for (int i = 0; i < amount; ++i)
    {
        is_fit = true;
        for (int j = 0; j < h_len; ++j)
        {
            opt o = options[i];
            std::string str(history[j].str, history[j].str + 4);
            if (o.str[0] == str[0] && o.str[1] == str[1] && o.str[2] == str[2] && o.str[3] == str[3])
            {
                is_fit = false;
                break;
            }
            //opt h = history[j];
            //std::cout<< h.str << std::endl;
            bapResults r = parse_results_string(calculateBullAndPgia(str, o.str));

            if (r.hit != results[j].hit || r.miss != results[j].miss)
            {
                is_fit = false;
                break;
            }
        }

        if (is_fit)
            candidates->push_back(options[i]);
    }

    unsigned int size = candidates->size();

    delete[] options;

    options = new opt[candidates->size()];
    std::copy(candidates->begin(), candidates->end(), options);

    delete candidates;

    filter_pack fp;

    fp.size = size;
    fp.options_ptr = options;

    return fp;
}

void simulation(opt *options)
{
    string secret = "1235";
    std::cout << "THE SECRET IS: " << secret << std::endl;
    string first_guess = "1335";
    vector<opt> history;
    vector<bapResults> results;
    unsigned int h_size = 0;
    unsigned int amount = AMOUNT;

    bapResults last_r = parse_results_string(calculateBullAndPgia(secret, first_guess));
    std::cout << "Feedback: " << last_r.hit << "," << last_r.miss << std::endl;
    //return;
    opt temp;
    std::copy(first_guess.begin(), first_guess.end(), temp.str);
    history.push_back(temp);
    results.push_back(last_r);
    ++h_size;

    while (last_r.hit != 4 && amount > 1)
    {
        std::cout << "Starting a round" << std::endl;
        filter_pack fp = filter_options(options, &history[0], &results[0], amount, h_size);
        options = fp.options_ptr;
        amount = fp.size;
        std::cout << amount << std::endl;

        opt o = options[0];
        std::cout << "Guess: " << o.str << std::endl;
        last_r = parse_results_string(calculateBullAndPgia(secret, o.str));
        history.push_back(o);
        results.push_back(last_r);
        ++h_size;
        std::cout << "Feedback: " << last_r.hit << "," << last_r.miss << std::endl;

        std::cout << "Finished a round" << std::endl;
    }

    std::cout << "Answer: " << options[0].str;
}

int main()
{
    std::cout << "starting...\n";
    std::cout << calculateBullAndPgia("9999","9997") << std::endl; // (3,0)
    std::cout << calculateBullAndPgia("9999","9799") << std::endl; // (3,0)
    std::cout << calculateBullAndPgia("9879","9997") << std::endl; // (1,2)
    //return 0;
    // vector<int> h;
    // for (size_t i = 0; i < 7; i++)
    // {
    //     h.push_back(i);

    // }
    
    //  vector<int>::iterator ptr;
    //  int i=0;
    // for (ptr = h.begin(); ptr < h.end(); ++ptr, ++i)
    // {
        
    //      if (i==1) 
    //     {
    //         ptr= h.erase(ptr-1);
    //     }
    //     std::cout << *ptr << std::endl;
    // }
    // return 0;
    //ConstantChooser randy("1782");
    std::cout << "----Playing..." << std::endl; 
    RandomChooser randy;
    SmartGuesser smarty;
    for (uint i = 0; i < 3; ++i)
    {
        std::cout <<play(randy, smarty, 6, 100) << std::endl;; // smarty should always win in at most 10 turns!
        //play(randy, smarty, 4, 100);
    }

    /*
    opt* options = new opt[AMOUNT];
    create_options(options,AMOUNT);

    simulation(options);
    return 0;

    opt h1[1];
    h1[0].str[0] = '1';
    h1[0].str[1] = '2';
    h1[0].str[2] = '3';
    h1[0].str[3] = '4';

    bapResults r1[1];
    r1[0].hit  = 1;
    r1[0].miss = 1;

    filter_pack fp = filter_options(options, h1, r1,AMOUNT, 1);
    options = fp.options_ptr;
    unsigned int size = fp.size;
    //std::cout << options[0].str;
    //1782
    opt h2[2];
    h2[0].str[0] = '1';
    h2[0].str[1] = '2';
    h2[0].str[2] = '3';
    h2[0].str[3] = '4';
    h2[1].str[0] = '5';
    h2[1].str[1] = '7';
    h2[1].str[2] = '3';
    h2[1].str[3] = '2';

    bapResults r2[2];
    r2[0].hit  = 1;
    r2[0].miss = 1;
    r2[1].hit  = 2;
    r2[1].miss = 0;

    fp = filter_options(options, h2, r2,size, 2);
    options = fp.options_ptr;
    size = fp.size;

    delete[] options;

    return 0;*/
}