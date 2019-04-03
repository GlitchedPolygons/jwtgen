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
    EXP,
    CLAIM
};

const option::Descriptor usage[] = {
    {UNKNOWN, 0, "",      "",      option::Arg::None,     "\nUsage:  \tjwtgen [options]\n\n" "Options:"},
    {HELP,    0, "h",     "help",  option::Arg::Optional, "  -h, --help  \tPrint usage and exit."},
    {COPY,    0, "c",     "copy",  option::Arg::Optional, "  -c, --copy  \tCopy generated token to clipboard automatically."},
    {ISS,     0, "i",     "iss",   option::Arg::Optional, "  -i, --iss  \tThe issuer name of the jwt."},
    {EXP,     0, "e",     "exp",   option::Arg::Optional, "  -e, --exp  \tThe jwt's expiration date in numeric date format, meaning the amount of SECONDS SINCE 1970-01-01T00:00:00Z UTC (according to RFC7519 standard https://tools.ietf.org/html/rfc7519#section-4.1.4). You can use https://unixtimestamp.com to your advantage."},
    {CLAIM,   0, "",      "claim", option::Arg::Optional, "  --claim    \tPut as many claims in as you need. Specify them with the syntax \"--claim=CLAIM_NAME:CLAIM_VALUE\" (without quotation marks)."},
    {UNKNOWN, 0, "",      "",      option::Arg::None,     "\nExamples:"
                                                          "\n  jwtgen --iss=glitchedpolygons --copy"
                                                          "\n  jwtgen --iss=otherIssuerName --claim=role:admin --claim=projectId:7"
                                                          "\n  jwtgen -iglitchedtime --exp=1587399600"},

    {0,       0, nullptr, nullptr, nullptr,               nullptr}
};

using std::cout;
using std::endl;
using std::vector;
using std::string;

static vector<string> split(const string& inputString, char delimiter)
{
    using std::istringstream;
    string token;
    vector<string> tokens;
    istringstream tokenStream(inputString);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

/**
 * Parses the passed jwt generation parameters and outputs the final token (in its encoded form ready for usage).<p>
 *
 * @param argc The amount of passed CLI arguments.
 * @param argv The passed program's CLI arguments.
 * @return 0 if generation was successful; 1 if the program arguments couldn't be parsed; 2 if you passed more than one issuer argument.
 */
int main(int argc, char** argv)
{
    argc -= (argc > 0);
    argv += (argc > 0); // skip program name argv[0] if present.

    using option::Stats;
    using option::Option;
    using option::Parser;

    Stats stats(usage, argc, argv);
    Option options[128], buffer[4096];
    Parser parse(usage, argc, argv, options, buffer);

    if (parse.error())
    {
        cout << "\nERROR: Failed to parse jwtgen command line arguments.";
        return 1;
    }

    if (options[HELP] || argc == 0)
    {
        option::printUsage(cout, usage);
        return 0;
    }

    for (Option* opt = options[UNKNOWN]; opt; opt = opt->next())
    {
        cout << "Unknown option: " << opt->name << "\n";
    }

    jwt::builder token = jwt::create();

    Option* iss = options[ISS];
    if (iss != nullptr)
    {
        if (iss->count() > 1)
        {
            cout << "\nERROR: You passed more than one issuer. Only one issuer per jwt is allowed!\n";
            return 2;
        }
        token.set_issuer(iss->arg);
    }

    for (Option* opt = options[CLAIM]; opt; opt = opt->next())
    {
        const string& s(opt->arg);
        if (s.empty())
        {
            continue;
        }
        const vector<string>& spl = split(s, ':');
        if (spl.size() != 2)
        {
            cout << "ERROR: invalid claim argument \"" << s
                 << "\" - please use the correct syntax:  --claim=CLAIM_NAME:CLAIM_VALUE   the colon : delimiter is important. No spaces!";
            continue;
        }
        if (spl[0].empty() || spl[1].empty())
        {
            cout << "ERROR: invalid claim argument \"" << s
                 << "\" - claim name or claim value is an empty string. Please use the correct syntax:  --claim=CLAIM_NAME:CLAIM_VALUE   the colon : delimiter is important. No spaces!";
            continue;
        }
        token.set_payload_claim(spl[0], jwt::claim(spl[1]));
    }

    const string& output = token.sign(jwt::algorithm::hs256 {"secret"});
    cout << endl << output;

    if (options[COPY])
    {
        // TODO: copy the jwt to the clipboard here
    }

    return 0;
}

