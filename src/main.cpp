#include <iostream>
#include <sstream>
#include "jwt-cpp/jwt.h"
#include "optionparser.h"

enum optionIndex
{
    UNKNOWN,
    HELP,
    COPY,
    ISS,
    SUB,
    EXP,
    IAT,
    NBF,
    CLAIM
};

const option::Descriptor usage[] = {
    {UNKNOWN, 0, "",      "",      option::Arg::None,     "\nUsage:  \tjwtgen [options]\n\n" "Options:"},
    {HELP,    0, "h",     "help",  option::Arg::Optional, "  -h, --help  \tPrint usage and exit."},
    {COPY,    0, "c",     "copy",  option::Arg::Optional, "  -c, --copy  \tCopy generated token to clipboard automatically."},
    {ISS,     0, "i",     "iss",   option::Arg::Optional, "  -i, --iss  \tThe issuer name of the jwt."},
    {SUB,     0, "s",     "sub",   option::Arg::Optional, "  -s, --sub  \tThe jwt's sub claim (usually whom this token refers to)."},
    {EXP,     0, "",      "exp",   option::Arg::Optional, "  --exp  \tThe jwt's expiration date in numeric date format, meaning the amount of SECONDS SINCE 1970-01-01T00:00:00Z UTC (according to RFC7519 standard https://tools.ietf.org/html/rfc7519#section-4.1.4). You can use https://unixtimestamp.com to your advantage."},
    {IAT,     0, "",      "iat",   option::Arg::Optional, "  --iat  \tThe numeric date format of when this token was issued. If you don't pass this argument, it defaults to the current time in UTC."},
    {NBF,     0, "",      "nbf",   option::Arg::Optional, "  --nbf  \tDatetime of when the jwt starts being valid (in numeric date format, just as in the --exp argument)."},
    {CLAIM,   0, "",      "claim", option::Arg::Optional, "  --claim    \tPut as many claims in as you need. Specify them with the syntax \"--claim=CLAIM_NAME:CLAIM_VALUE\" (without quotation marks)."},
    {UNKNOWN, 0, "",      "",      option::Arg::None,     "\nExamples:"
                                                          "\n  jwtgen --iss=glitchedpolygons --copy"
                                                          "\n  jwtgen --iss=otherIssuerName --claim=role:admin --claim=projectId:7"
                                                          "\n  jwtgen -iglitchedtime -c --exp=1587399600\n\nFully qualified arguments (double-dash) need to have the equals sign '=' between them and their values."},

    {0,       0, nullptr, nullptr, nullptr,               nullptr}
};

using std::cout;
using std::endl;
using std::vector;
using std::string;

/**
 * Splits a string using a delimiter character (e.g. comma-separated values) and returns a vector of the found substrings.
 * @param inputString The string to split.
 * @param delimiter The delimiter character to use for splitting (e.g. ',' or ':' or whatever).
 * @return std::vector of the found substrings.
 */
static vector<string> split(const string& inputString, char delimiter)
{
    using std::istringstream;

    string substring;
    vector<string> substrings;
    istringstream inputStringStream(inputString);

    while (std::getline(inputStringStream, substring, delimiter))
    {
        substrings.push_back(substring);
    }

    return substrings;
}

/**
 * Tries to convert a string (raw char*) into a time_point (parsing it as long int).
 * @param str The unix time string (seconds since 1970-01-01T00:00:00Z) to convert.
 * @return The converted time_point.
 */
inline static std::chrono::system_clock::time_point string_to_time_point(const char* str)
{
    const long int s = std::strtol(str, nullptr, 10);
    return std::chrono::system_clock::from_time_t(std::abs(s));
}

/**
 * Parses the passed jwt generation parameters and outputs the final token (in its encoded form ready for usage).<p>
 * @param argc The amount of passed CLI arguments.
 * @param argv The passed program's CLI arguments.
 * @return 0 if generation was successful or the user asked for help; 1 if the program's arguments couldn't be parsed; 2 if the passed arguments are invalid.
 */
int main(int argc, char** argv)
{
    argc -= (argc > 0);
    argv += (argc > 0); // skip program name argv[0] if present.

    using option::Stats;
    using option::Option;
    using option::Parser;

    Stats stats(usage, argc, argv);
    Option options[stats.options_max], buffer[stats.buffer_max];
    Parser parser(usage, argc, argv, options, buffer);

    if (parser.error())
    {
        cout << "\nERROR: Failed to parse jwtgen command line arguments.";
        return 1;
    }

    if (options[HELP] || argc == 0)
    {
        option::printUsage(cout, usage);
        return 0;
    }

    for (const Option* opt = options[UNKNOWN]; opt; opt = opt->next())
    {
        cout << "Unknown option: " << opt->name << "\n";
    }

    jwt::builder token = jwt::create();

    const Option* iss = options[ISS];
    if (iss != nullptr)
    {
        if (iss->count() > 1)
        {
            cout << "\nERROR: You passed more than one issuer. Only one issuer per jwt is allowed!\n";
            return 2;
        }
        token.set_issuer(iss->arg);
    }

    const Option* sub = options[SUB];
    if (sub != nullptr)
    {
        if (sub->count() > 1)
        {
            cout << "\nERROR: You passed more than one subject. Only one --sub per jwt is allowed!\n";
            return 2;
        }
        token.set_subject(sub->arg);
    }

    const Option* exp = options[EXP];
    if (exp != nullptr)
    {
        if (exp->count() > 1)
        {
            cout << "\nERROR: You passed more than one expiration datetime. Only one --exp argument per jwt is allowed!\n";
            return 2;
        }
        token.set_expires_at(string_to_time_point(exp->arg));
    }

    const Option* iat = options[IAT];
    if (iat != nullptr)
    {
        if (iat->count() > 1)
        {
            cout << "\nERROR: You passed more than one issued-at datetime. Only one --iat argument per jwt is allowed!\n";
            return 2;
        }
        token.set_issued_at(string_to_time_point(iat->arg));
    }
    else
    {
        token.set_issued_at(std::chrono::system_clock::now());
    }

    const Option* nbf = options[NBF];
    if (nbf != nullptr)
    {
        if (nbf->count() > 1)
        {
            cout << "\nERROR: You passed more than one not-before (nbf) datetime. Only one --nbf argument per jwt is allowed!\n";
            return 2;
        }
        token.set_not_before(string_to_time_point(nbf->arg));
    }

    for (const Option* opt = options[CLAIM]; opt; opt = opt->next())
    {
        const string& claim(opt->arg);
        if (claim.empty())
        {
            cout << "WARNING: One or more claims were left empty (--claim= )";
            continue;
        }

        const vector<string>& kvp = split(claim, ':');
        if (kvp.size() != 2)
        {
            cout << "ERROR: Invalid claim argument \"" << claim << "\" - please use the correct syntax:  --claim=CLAIM_NAME:CLAIM_VALUE   the colon : delimiter is important. No spaces! No weird characters!";
            return 2;
        }

        if (kvp[0].empty() || kvp[1].empty())
        {
            cout << "ERROR: Invalid claim argument \"" << claim << "\" - claim name or claim value is an empty string. Please use the correct syntax:  --claim=CLAIM_NAME:CLAIM_VALUE   the colon delimiter is important. No spaces! No weird characters!";
            return 2;
        }

        token.set_payload_claim(kvp[0], jwt::claim(kvp[1]));
    }

    const string& output = token.sign(jwt::algorithm::hs256 {"secret"});
    cout << endl << output;

    if (options[COPY])
    {
        // TODO: copy the jwt to the clipboard here
    }

    return 0;
}

