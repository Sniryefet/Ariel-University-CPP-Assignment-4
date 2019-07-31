#pragma once
#include <string>
#include <iostream>
using std::string, std::ostream;
namespace bullpgia {

    /**
     * Function which calculates for a given game string and a guess
     * the amount of characters match in both strings
     * Input: str - as the game string
     *        guess - a string to check a match for.
     * Output: string of the format "<miss>,<hit>"
     **/
    string calculateBullAndPgia(string str, string guess);


};