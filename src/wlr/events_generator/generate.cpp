#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>

void print_usage(std::string program_name)
{
  std::cerr << "Usage: " << program_name << " [--events|--listener] <input_file>\n";
}

enum Output
{
  events,
  listener
};

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    print_usage(argv[0]);
    return 1;
  }

  std::string option = argv[1];
  std::string input_filename = argv[2];

  if (option != "--events" && option != "--listener")
  {
    print_usage(argv[0]);
    return 1;
  }

  Output output = option == "--events" ? events : listener;

  std::set<std::string> wlr_structs;

  for (int i = 0; i < 2; ++i)
  {
    std::ifstream input_file(input_filename);
    if (!input_file)
    {
      std::cerr << "Error: Cannot open input file " << input_filename << "\n";
      return 1;
    }

    std::string line;
    std::string prev_id1;

    while (std::getline(input_file, line))
    {
      std::istringstream iss(line);
      std::string id1, id2, id3;

      // Parse the line.
      if (!std::getline(iss, id1, ',')) continue;
      if (!std::getline(iss, id2, ',')) continue;
      if (!std::getline(iss, id3)) continue;

      // Trim whitespace.
      id1.erase(0, id1.find_first_not_of(" \t"));
      id1.erase(id1.find_last_not_of(" \t") + 1);
      id2.erase(0, id2.find_first_not_of(" \t"));
      id2.erase(id2.find_last_not_of(" \t") + 1);
      id3.erase(0, id3.find_first_not_of(" \t"));
      id3.erase(id3.find_last_not_of(" \t") + 1);

      // If id1 changes, close previous namespace and open new one.
      if (id1 != prev_id1)
      {
        if (output == listener)
        {
          if (!prev_id1.empty())
          {
            // Terminate the sequence.
            std::cout << ");\n";
          }
          std::cout << "__WAYARC_DECLARE_LISTENERS(" << id1 << ", ";
        }
        else if (i == 1)
        {
          if (!prev_id1.empty())
          {
            // Close the last namespace if there was one.
            std::cout << "} // namespace " << prev_id1 << '\n';
          }
          std::cout << "namespace " << id1 << " {\n";
        }

        prev_id1 = id1;
      }

      if (output == listener)
      {
        // Output the sequence.
        std::cout << "(" << id2 << ")";
      }
      else if (i == 1)
      {
        // Output the 'using' line.
        std::cout << "  using " << id2 << " = ";
        if (id3 == "nullptr_t")
          std::cout << "std::nullptr_t;\n";
        else
          std::cout << "struct " << id3 << ";\n";
      }
      else if (id3 != "nullptr_t")
      {
        if (wlr_structs.insert(id3).second)
          std::cout << "struct " << id3 << ";\n";
      }
    }

    if (output == listener)
    {
      // Terminate the sequence.
      std::cout << ");\n";
    }
    else if (i == 1)
    {
      // Close the last namespace if there was one.
      std::cout << "} // namespace " << prev_id1 << '\n';
    }

    input_file.close();

    if (output == listener)
    {
      // Print footer of listener.h.
      std::cout << "\n";
      std::cout << "} // namespace listener\n";
      std::cout << "\n";
      std::cout << "#undef __WAYARC_DECLARE_LISTENER\n";
      std::cout << "#undef __WAYARC_DECLARE_LISTENERS\n";
      break;
    }
    else if (i == 0)
    {
      // Print footer of part 1, and header of part 2, of events.h.
      std::cout << "} // extern \"C\"\n";
      std::cout << "\n";
      std::cout << "namespace wlr::events {\n";
    }
    else
    {
      // Print footer of events.h.
      std::cout << "} // namespace wlr::events\n";
    }
  }
}
