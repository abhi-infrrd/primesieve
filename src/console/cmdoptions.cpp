///
/// @file   cmdoptions.cpp
/// @brief  Parse command-line options for the primesieve console
///         (terminal) application.
///
/// Copyright (C) 2017 Kim Walisch, <kim.walisch@gmail.com>
///
/// This file is distributed under the BSD License. See the COPYING
/// file in the top level directory.
///

#include <primesieve.hpp>
#include "cmdoptions.hpp"
#include "calculator.hpp"

#include <string>
#include <map>
#include <cstddef>
#include <stdint.h>

void help();
void version();

using namespace std;
using namespace primesieve;

namespace {

/// Command-line option
/// e.g. str = "--threads", value = "4"
struct Option
{
  string argv;
  string str;
  string value;
  template <typename T>
  T getValue() const
  {
    if (value.empty())
      throw primesieve_error("missing value for option " + argv);
    return calculator::eval<T>(value);
  }
};

enum OptionID
{
  OPTION_COUNT,
  OPTION_HELP,
  OPTION_NTHPRIME,
  OPTION_NO_STATUS,
  OPTION_NUMBER,
  OPTION_DISTANCE,
  OPTION_PRINT,
  OPTION_QUIET,
  OPTION_SIZE,
  OPTION_THREADS,
  OPTION_TIME,
  OPTION_VERSION
};

/// Command-line options
map<string, OptionID> optionMap =
{
  { "-c",          OPTION_COUNT },
  { "--count",     OPTION_COUNT },
  { "-h",          OPTION_HELP },
  { "--help",      OPTION_HELP },
  { "-n",          OPTION_NTHPRIME },
  { "--nthprime",  OPTION_NTHPRIME },
  { "--no-status", OPTION_NO_STATUS },
  { "--number",    OPTION_NUMBER },
  { "-d",          OPTION_DISTANCE },
  { "--dist",      OPTION_DISTANCE },
  { "-p",          OPTION_PRINT },
  { "--print",     OPTION_PRINT },
  { "-q",          OPTION_QUIET },
  { "--quiet",     OPTION_QUIET },
  { "-s",          OPTION_SIZE },
  { "--size",      OPTION_SIZE },
  { "-t",          OPTION_THREADS },
  { "--threads",   OPTION_THREADS },
  { "--time",      OPTION_TIME },
  { "-v",          OPTION_VERSION },
  { "--version",   OPTION_VERSION }
};

void optionPrint(Option& opt,
                 CmdOptions& opts)
{
  opts.quiet = true;

  // by default print primes
  if (opt.value.empty())
    opt.value = "1";

  switch (opt.getValue<int>())
  {
    case 1: opts.flags |= PrimeSieve::PRINT_PRIMES; break;
    case 2: opts.flags |= PrimeSieve::PRINT_TWINS; break;
    case 3: opts.flags |= PrimeSieve::PRINT_TRIPLETS; break;
    case 4: opts.flags |= PrimeSieve::PRINT_QUADRUPLETS; break;
    case 5: opts.flags |= PrimeSieve::PRINT_QUINTUPLETS; break;
    case 6: opts.flags |= PrimeSieve::PRINT_SEXTUPLETS; break;
    default: throw primesieve_error("invalid option " + opt.argv);
  }
}

void optionCount(Option& opt,
                 CmdOptions& opts)
{
  // by default count primes
  if (opt.value.empty())
    opt.value = "1";

  int n = opt.getValue<int>();

  for (; n > 0; n /= 10)
  {
    switch (n % 10)
    {
      case 1: opts.flags |= PrimeSieve::COUNT_PRIMES; break;
      case 2: opts.flags |= PrimeSieve::COUNT_TWINS; break;
      case 3: opts.flags |= PrimeSieve::COUNT_TRIPLETS; break;
      case 4: opts.flags |= PrimeSieve::COUNT_QUADRUPLETS; break;
      case 5: opts.flags |= PrimeSieve::COUNT_QUINTUPLETS; break;
      case 6: opts.flags |= PrimeSieve::COUNT_SEXTUPLETS; break;
      default: throw primesieve_error("invalid option " + opt.argv);
    }
  }
}

/// e.g. "--threads=8"
/// -> opt.str = "--threads"
/// -> opt.value = "8"
///
Option makeOption(const string& argv)
{
  Option opt;
  opt.argv = argv;
  size_t delimiter = argv.find_first_of("=0123456789");

  if (delimiter == string::npos)
    opt.str = argv;
  else
  {
    opt.str = argv.substr(0, delimiter);
    opt.value = argv.substr(delimiter + (argv.at(delimiter) == '=' ? 1 : 0));
  }

  if (opt.str.empty() && !opt.value.empty())
    opt.str = "--number";

  if (!optionMap.count(opt.str))
    throw primesieve_error("unknown option " + argv);

  return opt;
}

} // namespace

CmdOptions parseOptions(int argc, char* argv[])
{
  CmdOptions opts;

  for (int i = 1; i < argc; i++)
  {
    Option opt = makeOption(argv[i]);

    switch (optionMap[opt.str])
    {
      case OPTION_COUNT:     optionCount(opt, opts); break;
      case OPTION_PRINT:     optionPrint(opt, opts); break;
      case OPTION_SIZE:      opts.sieveSize = opt.getValue<int>(); break;
      case OPTION_THREADS:   opts.threads = opt.getValue<int>(); break;
      case OPTION_QUIET:     opts.quiet = true; break;
      case OPTION_NTHPRIME:  opts.nthPrime = true; break;
      case OPTION_NO_STATUS: opts.status = false; break;
      case OPTION_TIME:      opts.time = true; break;
      case OPTION_NUMBER:    opts.numbers.push_back(opt.getValue<uint64_t>()); break;
      case OPTION_DISTANCE:  opts.numbers.push_back(opt.getValue<uint64_t>() + opts.numbers[0]); break;
      case OPTION_VERSION:   version(); break;
      case OPTION_HELP:      help(); break;
    }
  }

  if (opts.numbers.empty())
    throw primesieve_error("missing STOP number");

  if (opts.quiet)
    opts.status = false;
  else
    opts.time = true;

  return opts;
}
