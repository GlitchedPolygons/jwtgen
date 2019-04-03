#include <iostream>
#include "jwt-cpp/jwt.h"
#include "optionparser.h"

enum optionIndex {
  UNKNOWN, HELP, COPY, ISS, EXP
};

const option::Descriptor usage[] = {
    {UNKNOWN, 0, "",      "",      option::Arg::None,     "\nUsage:  \tjwtgen [options]\n\n" "Options:"},
    {HELP,    0, "h",     "help",  option::Arg::Optional, "  -h, --help  \tPrint usage and exit."},
    {COPY,    0, "c",     "copy",  option::Arg::Optional, "  -c, --copy  \tCopy generated token to clipboard automatically."},
    {ISS,     0, "i",     "iss",   option::Arg::Optional, "  -i, --iss  \tThe issuer name of the jwt."},
    {EXP,     0, "e",     "exp",   option::Arg::Optional, "  -e, --exp  \tThe jwt's expiration date in numeric date format, meaning the amount of SECONDS SINCE 1970-01-01T00:00:00Z UTC (according to RFC7519 standard https://tools.ietf.org/html/rfc7519#section-4.1.4). You can use https://unixtimestamp.com to your advantage."},
    {UNKNOWN, 0, "",      "",      option::Arg::None,     "\nExamples:"
                                                          "\n  jwtgen --iss=glitchedpolygons --copy"
                                                          "\n  jwtgen -iglitchedtime --exp=1587399600"},

    {0,       0, nullptr, nullptr, nullptr,               nullptr}
};

/**
 * Parses the passed jwt generation parameters and outputs the final token (in its encoded form ready for usage).<p>
 *
 * @param argc The amount of passed CLI arguments.
 * @param argv The passed program's CLI arguments.
 * @return 0 if generation was successful; 1 if the program arguments couldn't be parsed; 2 if you passed more than one issuer argument.
 */
int main(int argc, char** argv) {
  argc -= (argc > 0);
  argv += (argc > 0); // skip program name argv[0] if present.

  option::Stats stats(usage, argc, argv);
  option::Option options[128], buffer[4096];
  option::Parser parse(usage, argc, argv, options, buffer);

  if (parse.error()) {
    std::cout << "\nERROR: Failed to parse jwtgen command line arguments.";
    return 1;
  }

  if (options[HELP] || argc == 0) {
    option::printUsage(std::cout, usage);
    return 0;
  }

  for (option::Option* opt = options[UNKNOWN]; opt; opt = opt->next()) {
    std::cout << "Unknown option: " << opt->name << "\n";
  }

  jwt::builder token = jwt::create();

  option::Option* iss = options[ISS];
  if (iss != nullptr) {
    if (iss->count() > 1) {
      std::cout << "\nERROR: You passed more than one issuer. Only one issuer per jwt is allowed!\n";
      return 2;
    }
    token.set_issuer(iss->arg);
  }

  const std::string& output = token.sign(jwt::algorithm::hs256 {"secret"});
  std::cout << std::endl << output;

  if (options[COPY]) {
    // TODO: copy the jwt to the clipboard here
  }

  return 0;
}
