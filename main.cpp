#include <iostream>
#include "optionparser.h"

enum optionIndex {
  UNKNOWN, HELP, COPY, ISS, EXP
};

const option::Descriptor usage[] = {
    {UNKNOWN, 0, "",      "",      option::Arg::None, "\nUsage:  \tjwtgen [options]\n\n" "Options:"},
    {HELP,    0, "h",     "help",  option::Arg::None, "  -h, --help  \tPrint usage and exit."},
    {COPY,    0, "c",     "copy",  option::Arg::None, "  -c, --copy  \tCopy generated token to clipboard automatically."},
    {ISS,     0, "i",     "iss",   option::Arg::None, "  -i, --iss  \tThe issuer name of the jwt."},
    {EXP,     0, "e",     "exp",   option::Arg::None, "  -e, --exp  \tThe jwt's expiration date in numeric date format, meaning the amount of SECONDS SINCE 1970-01-01T00:00:00Z UTC (according to RFC7519 standard https://tools.ietf.org/html/rfc7519#section-4.1.4)"},
    {UNKNOWN, 0, "",      "",      option::Arg::None, "\nExamples:"
                                                      "\n  jwtgen -iss glitchedpolygons -c "
                                                      "\n  jwtgen -i glitchedtime --exp 1587399600"},

    {0,       0, nullptr, nullptr, nullptr,           nullptr}
};

int main(int argc, char** argv) {
  argc -= (argc > 0);
  argv += (argc > 0); // skip program name argv[0] if present.

  option::Stats stats(usage, argc, argv);
  option::Option options[stats.options_max], buffer[stats.buffer_max];
  option::Parser parse(usage, argc, argv, options, buffer);

  if (parse.error()) {
    return 1;
  }

  if (options[HELP] || argc == 0) {
    option::printUsage(std::cout, usage);
    return 0;
  }

  for (option::Option* opt = options[UNKNOWN]; opt; opt = opt->next()) {
    std::cout << "Unknown option: " << opt->name << "\n";
  }

  for (int i = 0; i < parse.nonOptionsCount(); ++i) {
    std::cout << "Non-option #" << i << ": " << parse.nonOption(i) << "\n";
  }

  return 0;
}
