#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <filesystem>
#include <string_view>

struct SignalInfo
{
  std::string identifier1; // struct name
  std::string identifier2; // signal name
  std::string filePath;
  int lineNumber;
  std::string identifier3; // type of second parameter
};

std::string count_braces(std::string const& line, ssize_t& open_brace_count)
{
  std::string result;
  open_brace_count;
  bool in_string = false;
  bool in_char = false;
  bool in_line_comment = false;
  bool in_block_comment = false;
  bool escaped = false;

  for (size_t i = 0; i < line.size(); ++i)
  {
    char const current = line[i];
    char const next = (i + 1 < line.size()) ? line[i + 1] : '\0';

    // Handle escape sequences in string/char literals.
    if (escaped)
    {
      escaped = false;
      result += current;
      continue;
    }

    // Check for escape character.
    if ((in_string || in_char) && current == '\\')
    {
      escaped = true;
      result += current;
      continue;
    }

    // Check for comment start/end.
    if (!in_string && !in_char && !in_line_comment && !in_block_comment)
    {
      if (current == '/' && next == '/')
      {
        in_line_comment = true;
        ++i;       // Skip the next character.
        continue;
      }

      if (current == '/' && next == '*')
      {
        in_block_comment = true;
        ++i; // Skip the next character
        continue;
      }
    }
    else if (in_block_comment && current == '*' && next == '/')
    {
      in_block_comment = false;
      ++i; // Skip the next character
      continue;
    }

    // Skip processing if in a comment.
    if (in_line_comment || in_block_comment)
      continue;

    result += current;

    // Handle string literals.
    if (!in_char && current == '"')
    {
      in_string = !in_string;
      continue;
    }

    // Handle character literals.
    if (!in_string && current == '\'')
    {
      in_char = !in_char;
      continue;
    }

    // Count braces only if not in a string or character literal.
    if (!in_string && !in_char)
    {
      if (current == '(')
        ++open_brace_count;
      else if (current == ')')
        --open_brace_count;
    }
  }

  return result;
}

std::string extractSecondParameter(std::vector<std::string> const& fileLines, int signalLine)
{
  // Check if line exists and contains wl_signal_emit_mutable.
  if (signalLine >= fileLines.size() || fileLines[signalLine].find("wl_signal_emit_mutable") == std::string::npos)
    return "";

  ssize_t braceCount = 0;

  // Extract the entire function call which might span multiple lines.
  // Count opening and closing parentheses.
  std::string line = count_braces(fileLines[signalLine], braceCount);

  // If the call spans multiple lines, add more lines until we have balanced parentheses.
  int currentLine = signalLine + 1;
  while (braceCount > 0 && currentLine < fileLines.size())
  {
    line += " " + count_braces(fileLines[currentLine], braceCount);
    ++currentLine;
  }

  // Find second parameter by first finding the comma after the first parameter.
  size_t firstComma = line.find(',', line.find("events.", line.find("wl_signal_emit_mutable")));
  if (firstComma == std::string::npos)
    return "";

  // Extract from after comma to before closing parenthesis.
  std::string secondParam = line.substr(firstComma + 1);
  size_t closeParen = secondParam.find_last_of(')');
  if (closeParen != std::string::npos)
    secondParam = secondParam.substr(0, closeParen);

  // Trim whitespace.
  secondParam = std::regex_replace(secondParam, std::regex("^\\s+|\\s+$"), "");

  return secondParam;
}

void parse_string(std::string const& input, bool& saw_ampersand, bool& saw_arrow, std::string& var, std::string& member)
{
  saw_ampersand = false;
  saw_arrow = false;

  // Check if it starts with ampersand.
  size_t start_pos = 0;
  if (!input.empty() && input[0] == '&')
  {
    saw_ampersand = true;
    start_pos = 1;
  }

  // Check if it contains "->".
  size_t arrow_pos = input.find("->");
  if (arrow_pos != std::string::npos)
  {
    saw_arrow = true;

    // Extract variable name (from after & to before ->).
    var = input.substr(start_pos, arrow_pos - start_pos);

    // Extract member name (everything after ->).
    member = input.substr(arrow_pos + 2);
  }
  else
  {
    // No arrow, just extract variable.
    var = input.substr(start_pos);
    // Member is not initialized in this case.
  }
}

std::string findVariableType(std::vector<std::string> const& fileLines, int signalLine, std::string const& secondArgument, std::string const& path)
{
  // If NULL, return nullptr_t.
  if (secondArgument == "NULL")
    return "nullptr_t";

  // A few hard-coded types because it is too hard to lookup the types of members of structs.
  if (secondArgument == "surface->toplevel")
    return "wlr_xdg_toplevel";
  else if (secondArgument == "surface->popup")
    return "wlr_xdg_popup";
  else if (secondArgument == "device->cursor")
    return "wlr_cursor";
  else if (secondArgument == "inhibitor->surface")
    return "wlr_surface";
  else if (secondArgument == "&seat->wlr_tablet_tool")
    return "wlr_tablet_tool";
  else if (secondArgument == "&tool->wlr_tool")
    return "wlr_tablet_tool";
  else if (secondArgument.ends_with(".base"))
    return "wlr_input_device";
  else if (secondArgument == "&conn->output")
    return "wlr_output";

  bool saw_ampersand, saw_arrow;
  std::string varName, member;

  parse_string(secondArgument, saw_ampersand, saw_arrow, varName, member);

  std::string optPtrRegex = (saw_ampersand && !saw_arrow) ? "" : "\\*\\s*";

  // Look for variable declaration in the file.
  std::regex structRegex("struct\\s+(\\w+)\\s*" + optPtrRegex + varName + "\\b");

  // Scan backwards, starting at signalLine.
  for (int line = signalLine - 1; line >= 0; --line)
  {
    std::smatch matches;

    // Check for pointer declarations (struct type *var).
    if (std::regex_search(fileLines[line], matches, structRegex) && matches.size() > 1)
      return matches[1].str();
  }

  return path;
}

bool processSignalLine(std::string const& line, SignalInfo& info)
{
  std::regex pattern(R"((\w+),\s*(\w+),\s*([^:]+):(\d+))");
  std::smatch matches;

  if (std::regex_search(line, matches, pattern) && matches.size() > 4)
  {
    info.identifier1 = matches[1].str();
    info.identifier2 = matches[2].str();
    info.filePath = matches[3].str();
    info.lineNumber = std::stoi(matches[4].str()) - 1; // Convert to 0-based index
    return true;
  }

  return false;
}

std::vector<std::string> readFile(std::string const& filePath)
{
  std::vector<std::string> lines;
  std::ifstream file(filePath);

  if (!file.is_open())
  {
    std::cerr << "Failed to open file: " << filePath << std::endl;
    return lines;
  }

  std::string line;
  while (std::getline(file, line))
    lines.push_back(line);

  return lines;
}

int main(int argc, char* argv[])
{
  if (argc < 3)
  {
    std::cerr << "Usage: " << argv[0] << " <wlroots_builddir> <input_file>" << std::endl;
    return 1;
  }

  std::string builddir = argv[1];
  std::string inputPath = argv[2];
  std::vector<std::string> inputLines = readFile(inputPath);

  if (inputLines.empty())
  {
    std::cerr << "Input file is empty or could not be read" << std::endl;
    return 1;
  }

  for (auto const& inputLine : inputLines)
  {
    SignalInfo info;

    if (!processSignalLine(inputLine, info))
    {
      std::cerr << "Failed to parse line: " << inputLine << std::endl;
      continue;
    }

    // Get the absolute path to the source file.
    std::string sourcePath = info.filePath;
    if (sourcePath.substr(0, 3) == "../")
    {
      // Remove ../ prefix if present and convert to absolute path based on builddir.
      std::filesystem::path basePath(builddir);
      std::filesystem::path relativePath(sourcePath);
      sourcePath = (basePath / relativePath).lexically_normal().string();
    }

    std::vector<std::string> sourceLines = readFile(sourcePath);
    if (sourceLines.empty())
    {
      std::cerr << "Failed to read source file: " << sourcePath << std::endl;
      continue;
    }

    // Extract the second parameter from the signal emission line.
    std::string secondParam = extractSecondParameter(sourceLines, info.lineNumber);
    if (secondParam.empty())
    {
      std::cerr << "Failed to extract second parameter at " << info.filePath << ":" << (info.lineNumber + 1) << std::endl;
      continue;
    }

    std::stringstream ss;
    ss << info.filePath << ":" << (info.lineNumber + 1);

    // Find the type of the second parameter.
    info.identifier3 = findVariableType(sourceLines, info.lineNumber, secondParam, ss.str());

    // Output the results.
    std::cout << info.identifier1 << ", " << info.identifier2 << ", " << info.identifier3 << std::endl;
  }
}
