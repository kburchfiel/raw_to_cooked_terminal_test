//Raw to cooked terminal test:
// A simple typing test script for testing raw-to-cooked
// transitions using the cpp-terminal library 
// on Linux and Mac
// Released under the MIT license


#include <iostream>
#include <chrono>
#include "cpp-terminal/terminal.hpp"
#include "cpp-terminal/color.hpp"
#include "cpp-terminal/cursor.hpp"
#include "cpp-terminal/exception.hpp"
#include "cpp-terminal/input.hpp"
#include "cpp-terminal/iostream.hpp"
#include "cpp-terminal/key.hpp"
#include "cpp-terminal/options.hpp"
#include "cpp-terminal/screen.hpp"
#include "cpp-terminal/terminal.hpp"
#include "cpp-terminal/tty.hpp"
#include "cpp-terminal/version.hpp"

// Defining colors that represent correct and incorrect output:
// (This code is based on the Hello World example for 
// cpp-terminal at https://github.com/jupyter-xeus/cpp-terminal .)

std::string correct_output_color_code = Term::color_fg(
    Term::Color::Name::Green);
std::string incorrect_output_color_code = Term::color_fg(
    Term::Color::Name::Red);
std::string default_output_color_code = Term::color_fg(
    Term::Color::Name::Default);
std::string print_color_code = default_output_color_code; // This
// variable will get updated within typing tests to reflect either
// correct or incorrect output.

void type_phrase(std::string gameplay_option)
{
    std::string phrase;
    if (gameplay_option == "100")
    {phrase = "It's SO over.";}
    else if (gameplay_option == "200")
    {phrase = "We're SO back!";}
    else
    {Term::cout << "Unknown phrase." << std::endl;
    return;}

    int phrase_length = phrase.size();
    
    std::string user_string;

    // // Clearing the console and displaying the verse to type:
    Term::cout << Term::clear_screen() << Term::terminal.clear() 
        << Term::cursor_move(
    1, 1) << phrase << std::endl;

    // Defining a string that will allow the cursor to get moved
    // directly below the phrase: (This assumes that the phrase
    // is only one line long, which is indeed the case within
    // this example.)
    std::string cursor_reposition_code = Term::cursor_move(
        2, 1);

    Term::cout << cursor_reposition_code << "\033[J" << std::endl;

    Term::terminal.setOptions(Term::Option::Raw);
    auto start_time = std::chrono::high_resolution_clock::now();
    while (user_string != phrase)
    {
        Term::Event event = Term::read_event();
        switch (event.type())
        {
        case Term::Event::Type::Key:
        {
            Term::Key key(event);

            std::string char_to_add = ""; 
            std::string keyname = key.name();
                if (keyname == "Space")
            {
                char_to_add = " ";
            }
            else if (keyname == "Backspace") // We'll need to remove the
            // last character from our string.
            {
                user_string = user_string.substr(
                    0, user_string.length() - 1);
            }
            else
            {
                char_to_add = keyname;
            }
            user_string += char_to_add;
                /* Determining how to color the output: (If the output is correct
            so far, it will be colored green; if there is a mistake, it will
            instead be colored red.*/
            if (user_string == phrase.substr(0, user_string.length()))
            {print_color_code = correct_output_color_code;}
            else
            {print_color_code = incorrect_output_color_code;}

            Term::cout << cursor_reposition_code << "\033[J" <<
            print_color_code << user_string 
            << default_output_color_code <<std::endl;
            break;
        }
        default:
        break;
        }
}

    auto end_time = std::chrono::high_resolution_clock::now();
    auto test_seconds = std::chrono::duration<double>(
end_time - start_time).count();

    double wpm = (phrase_length / test_seconds) * 12; /* To calculate WPM,
              we begin with characters per second, then multiply by 60 (to go
              from seconds to minutes) and divide by 5 (to go from characters
              to words using the latter's standard definition).*/

Term::cout << "You typed the phrase at " 
<< wpm << " words per minute." << std::endl;

}

int main() {

std::string gameplay_option;
while (gameplay_option != "quit")
{
Term::cout << "Please enter 100 or 200 to choose which phrase \
to type, followed by Enter. Your typing test will then begin; \
once you enter the text correctly, the test will finish \
automatically. You may also enter 'quit' to quit." << std::endl;
Term::terminal.setOptions(Term::Option::Cooked);
gameplay_option.clear();
Term::cin >> gameplay_option;
if (gameplay_option != "quit")
{type_phrase(gameplay_option);}
}


Term::cout << "Finished running test." << std::endl;



}