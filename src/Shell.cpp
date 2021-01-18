/*
 * SAML 0.0.1
 * Author - Kidus W. (hello@kidusw.com)
 * License - GNU GPL v.3
 */

#include <iostream>
#include <fstream>
#include <string>

#include "SAML.h"
#include "Shell.h"

bool msg_displayed = false;

int main(int argc, char ** argv)
{
    if (argc < 2)
    {
        system("title SAML Shell");
        if (!msg_displayed)
        {
            msg_displayed = true;
            std::cout << "SAML Shell " << SAML_VER_STRING << " - Call 'HELP' for help, 'QUIT' to exit the program at any time.\n\n";
        }

        std::cout << "$ ";
        std::string line;
        getline(std::cin, line);

        if (line[0] == '#' || line.empty())
        {
            std::cout << std::endl;
            main(argc, argv);
        }

        else if (line == "CLEAR")
        {
            system("cls");
            main(argc, argv);
        }

        else if (line == "QUIT" || line == "EXIT")
        {
            exit(0);
            return 0;
        }

        else if (line == "HELP")
        {
            std::cout << "__________________________________ SAML HELP __________________________________\n\n";
            std::cout << "Welcome to SAML!\n"
                      << "SAML is a statistical unstructured programming language built for educational purposes.\n"
                      << "To use SAML run saml.exe, the shell, or you can run a script using 'saml.exe [Filename]'\n"
                      << "For documentation, check out the /docs folder included within the source or doc.pdf \nincluded with "
                      << "the installation package. SAML is a relatively easy language that can be learned in\nless than an hour.\n";
            std::cout << "_______________________________________________________________________________\n\n";
            main(argc, argv);
        }

        else
        {
        	//Use the following line to check if the DLL loaded successfully
        	//SAML::API_Test();
            SAML::Lexer lexer;
            lexer.set_line(line);
            lexer.lex();
            SAML::Parser parser;
            parser.set_tokens(lexer);
            SAML::Tree tree = parser.parse_tokens();
            bool state = SAML::evaluate_ast(tree);

            if (!state)
            {
                std::cerr << "Occurrence: In <stdin>\n";
            }
        }

        std::cout << std::endl;
        main(argc, argv);
    }

    else
    {
        if (argc != 2)
        {
            std::cerr << "SAML: Error: Too many arguments supplied\n"
                      << "(in this context): Usage: saml [Filename]\n";
            exit(0);
        }

        else
        {
            const std::string filename = argv[1];
            std::ifstream _stream;
            std::string _buffer;
            unsigned line_count = 0;

            _stream.open(filename.c_str());

            if (!_stream.is_open())
            {
                std::cerr << "SAML: Error: Failed to open file '" << filename << "'.\n"
                          << "(in this context): Check file existence and permissions\n";
                exit(0);
            }

            SAML::Lexer lexer;
            SAML::Parser parser;
            SAML::Tree _tree;
            bool state;

            while (getline(_stream, _buffer))
            {
                if (!(_buffer[0] == '#'))
                {
                    ++line_count;
                    lexer.set_line(_buffer);
                    lexer.lex();
                    parser.set_tokens(lexer);
                    _tree = parser.parse_tokens();
                    state = SAML::evaluate_ast(_tree);

                    if (state)
                    {
                        continue;
                    }

                    else
                    {
                        std::cout << "Occurrence: Line " << line_count << " of '" << filename << "'\n";
                        break;
                    }
                }

                lexer._clear();
            }

        }
    }

    return 0;
}
