/**
 * A demo program for bull-pgia.
 * 
 * @author Erel Segal-Halevi
 * @since  2019-04
 */

#include <iostream>
using namespace std;

#include "play.hpp"
#include "DummyChoosers.hpp"
#include "DummyGuessers.hpp"
#include "SmartGuesser.hpp"
#include "badkan.hpp"

#define COMMA ,

using namespace bullpgia;

int main()
{

	badkan::TestCase testcase;
	int grade = 0;
	int signal = setjmp(badkan::longjmp_buffer);
	if (signal == 0)
	{

		// BASIC TESTS - DO NOT CHANGE
		ConstantChooser c1234{"1234"}, c12345{"12345"}, c9999{"9999"};
		ConstantGuesser g1234{"1234"}, g12345{"12345"}, g9999{"9999"};

		testcase.setname("Calculate bull and pgia")
				.CHECK_OUTPUT(calculateBullAndPgia("1234", "1234"), "4,0") // 4 bull, 0 pgia
				.CHECK_OUTPUT(calculateBullAndPgia("1234", "4321"), "0,4") // 0 bull, 4 pgia
				;

		testcase.setname("Play with dummy choosers and guessers")
				.CHECK_EQUAL(play(c1234, g1234, 4, 100), 1)		 // guesser wins in one turn.
				.CHECK_EQUAL(play(c1234, g9999, 4, 100), 101)	// guesser loses by running out of turns
				.CHECK_EQUAL(play(c1234, g12345, 4, 100), 101) // guesser loses technically by making an illegal guess (too long).
				.CHECK_EQUAL(play(c12345, g1234, 4, 100), 0)	 // chooser loses technically by choosing an illegal number (too long).
				;

		testcase.setname("Play with smart guesser");
		RandomChooser randy;
		SmartGuesser smarty;
		for (uint i = 0; i < 100; ++i)
		{
			testcase.CHECK_EQUAL(play(randy, smarty, 4, 100) <= 100, true); // smarty should always win in at most 10 turns!
		}

		testcase.setname("Play with smart guesser- STUDENTS VERSION")
				.CHECK_OUTPUT(calculateBullAndPgia("1231", "1142"), "1,2") // 1 bull, 2 pgia
				.CHECK_OUTPUT(calculateBullAndPgia("0000", "1111"), "0,0") // 0 bull, 0 pgia
				.CHECK_OUTPUT(calculateBullAndPgia("0000", "1110"), "1,0") // 1 bull, 0 pgia
				.CHECK_OUTPUT(calculateBullAndPgia("0000", "1101"), "1,0") // 1 bull, 0 pgia
				.CHECK_OUTPUT(calculateBullAndPgia("0000", "1011"), "1,0") // 1 bull, 0 pgia
				.CHECK_OUTPUT(calculateBullAndPgia("0000", "0111"), "1,0") // 1 bull, 0 pgia
				.CHECK_OUTPUT(calculateBullAndPgia("1221", "1212"), "2,2") // 2 bull, 2 pgia
				.CHECK_OUTPUT(calculateBullAndPgia("9919", "8199"), "1,2") // 1 bull, 2 pgia
				;
		ConstantGuesser g012{"012"}, g012345{"012345"};
		ConstantChooser c01234{"01234"};

		testcase.setname("Play WRONG length - STUDENTS VERSION")
				.CHECK_EQUAL(play(c01234, g012,5 , 100), 101)				// chooser wins, maxturns+1
				.CHECK_EQUAL(play(c01234, g012345,5 , 100), 101)		// chooser wins, maxturns+1
				;

		testcase.setname("Play - STUDENTS VERSION");
		for (uint i = 0; i < 10; ++i)
		{
			testcase.CHECK_EQUAL(play(randy, smarty, 3, 100) <= 100, true); // smarty should always win in at most 10 turns!
		}
		for (uint i = 0; i < 10; ++i)
		{
			testcase.CHECK_EQUAL(play(randy, smarty, 2, 100) <= 100, true); // smarty should always win in at most 10 turns!
		}
		for (uint i = 0; i < 10; ++i)
		{
			testcase.CHECK_EQUAL(play(randy, smarty, 1, 100) <= 100, true); // smarty should always win in at most 10 turns!
		}
		for (uint i = 0; i < 10; ++i)
		{
			testcase.CHECK_EQUAL(play(randy, smarty, 7, 100) <= 50, true); // smarty should always win in at most 50 turns!
		}

		for (uint i = 0; i < 10; ++i)
		{
			testcase.CHECK_EQUAL(play(randy, smarty, 12, 100) <= 60, true); // smarty should always win in at most 60 turns!
		}

		grade = testcase.grade();
	}
	else
	{
		testcase.print_signal(signal);
		grade = 0;
	}
	cout << "Your grade is: " << grade << endl;
	return 0;
}
