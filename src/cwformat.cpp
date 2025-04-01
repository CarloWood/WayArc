//=============================================================================
// Command Line Options.
//
// See https://llvm.org/docs/CommandLine.html
#include "llvm/Support/CommandLine.h"

namespace cl = llvm::cl;

// Create a custom category for our options.
cl::OptionCategory cwformat_category("cwformat options");

cl::opt<std::string> OutputFilename("o",
    cl::desc("Specify output filename"),
    cl::value_desc("filename"),
    cl::cat(cwformat_category));

// Collect input files as positional arguments.
cl::list<std::string> InputFiles(cl::Positional,
    cl::desc("<file> [<file> ...]"),
    cl::OneOrMore,
    cl::cat(cwformat_category));

// Override the default --version behavior.
static void print_version(llvm::raw_ostream& ros)
{
  ros << "cwformat version 0.1.0, written in 2025 by Carlo Wood.\n";
}

//=============================================================================

int main(int argc, char* argv[])
{
  // Set the program name.
  cl::SetVersionPrinter(&print_version);

  // Hide all options except those in our category.
  cl::HideUnrelatedOptions(cwformat_category);

  // Parse command line options.
  cl::ParseCommandLineOptions(argc, argv);

  // Now we can access the files from InputFiles.
  llvm::outs() << "Processing " << InputFiles.size() << " files:\n";
  for (auto const& file : InputFiles)
    llvm::outs() << "  " << file << "\n";
}
