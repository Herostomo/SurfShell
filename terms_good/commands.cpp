#include <iostream>
#include <filesystem>
#include "commands.h"
#include <fstream>
#include <string>
#include<algorithm>
#include <windows.h>
#include <vector>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <limits>
using json = nlohmann::json;
size_t WriteCallback(
    void* contents,
    size_t size,
    size_t nmemb,
    void* userp)
{
    ((std::string*)userp)->append(
        (char*)contents,
        size * nmemb);

    return size * nmemb;
}

std::vector<SearchResult>
search_tavily(const std::string& query)
{
    std::vector<SearchResult> results;

    CURL* curl = curl_easy_init();

    if(!curl)
    {
        std::cout << "Failed to initialize CURL\n";
        return results;
    }

    std::string response;

    struct curl_slist* headers = nullptr;

    headers = curl_slist_append(
        headers,
        "Content-Type: application/json");

    headers = curl_slist_append(
        headers,
        "User-Agent: TerminalProject/1.0");

    json body =
    {
        {"api_key", "Enter-your-api-key"},
        {"query", query},
        {"max_results", 5},
        {"search_depth", "basic"}
    };

    std::string request_body = body.dump();

    std::cout
        << "\nRequest Body:\n"
        << request_body
        << "\n\n";

    curl_easy_setopt(
        curl,
        CURLOPT_URL,
        "https://api.tavily.com/search");

    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    std::filesystem::path certPath =
        std::filesystem::path(exePath).parent_path()
        / "ca-bundle.crt";

    curl_easy_setopt(
        curl,
        CURLOPT_CAINFO,
        certPath.string().c_str());

    curl_easy_setopt(
        curl,
        CURLOPT_HTTPHEADER,
        headers);

    curl_easy_setopt(
        curl,
        CURLOPT_POSTFIELDS,
        request_body.c_str());

    curl_easy_setopt(
        curl,
        CURLOPT_POSTFIELDSIZE,
        request_body.size());

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        WriteCallback);

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEDATA,
        &response);

    CURLcode res =
        curl_easy_perform(curl);

    if(res != CURLE_OK)
    {
        std::cout
            << "Curl Error: "
            << curl_easy_strerror(res)
            << '\n';
    }

    std::cout
        << "\nResponse:\n"
        << response
        << "\n\n";

    try
    {
        auto parsed = json::parse(response);

        if(parsed.contains("results"))
        {
            for(const auto& item :
                parsed["results"])
            {
                SearchResult r;

                r.title =
                    item.value(
                        "title",
                        "Untitled");

                r.url =
                    item.value(
                        "url",
                        "");

                results.push_back(r);
            }
        }
        else
        {
            std::cout
                << "No 'results' field found.\n";
        }
    }
    catch(const std::exception& e)
    {
        std::cout
            << "JSON Parse Error: "
            << e.what()
            << '\n';
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return results;
}


void ls(const std::vector<std::string>& args)
{
    try{
        std::filesystem::path path=".";

        if(!args.empty()){
            path=args[0];
        }

        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            std::cout 
                << entry.path().filename().string() <<std::endl;
        }

    }
    catch(const std::exception& e){
        std::cerr
            <<"ls :"
            <<e.what()
            <<"\n";
    }
}

void cd(const std::vector<std::string>& args){
    try{
        if(args.empty()){
            std::cout<<"cd : missing path";
            return;
        }
        std::filesystem::current_path(args[0]);
        std::cout
            << std::filesystem::current_path()
            <<"\n";
    }
    catch(const std::exception& e){
        std::cerr
            <<"cd :"
            <<e.what()
            <<"\n";
    }
}

void pwd(const std::vector<std::string>&)
{
    std::cout
        << std::filesystem::current_path()
        << '\n';
}

void mkdir(const std::vector<std::string>& args){
    try{
        if(args.empty()){
            std::cout<<"mkdir <path> missing";
            return;
        }
        if(!std::filesystem::create_directory(args[0]))
        {
            std::cerr
                << "mkdir: directory already exists\n";
        }

    }
    catch(const std::exception& e){
        std::cerr
            <<"mkdir"
            <<e.what()
            <<"\n";
    }
}

void rm(const std::vector<std::string>& args){
    try{
        if(args.empty()){
            std::cout<<"rm <path> missing";
            return;
        }
        std::filesystem::remove(args[0]);
    }
    catch(const std::exception& e){
        std::cerr
            <<"rm"
            <<e.what()
            <<"\n";
    }
}

void touch(const std::vector<std::string>& args){
    try{
        if(args.empty()){
            std::cout<<"touch <path> missing";
            return;
        }
        std::ofstream file(args[0], std::ios::app);
        file.close();
    }
    catch(const std::exception& e){
        std::cerr
            <<"touch"
            <<e.what()
            <<"\n";
    }
}

void echo(const std::vector<std::string>& args)
{
    if (args.empty())
    {
        std::cout << '\n';
        return;
    }

    for (size_t i = 0; i < args.size(); ++i)
    {
        if (args[i] == ">" || args[i] == ">>")
        {
            if (i + 1 >= args.size())
            {
                std::cerr << "echo: missing filename\n";
                return;
            }

            const std::string& filename = args[i + 1];

            std::ofstream file(
                filename,
                args[i] == ">>" ? std::ios::app : std::ios::out
            );

            if (!file)
            {
                std::cerr << "echo: cannot open file\n";
                return;
            }

            for (size_t j = 0; j < i; ++j)
            {
                file << args[j];

                if (j != i - 1)
                    file << ' ';
            }

            file << '\n';
            return;
        }
    }

    for (size_t i = 0; i < args.size(); ++i)
    {
        std::cout << args[i];

        if (i != args.size() - 1)
            std::cout << ' ';
    }

    std::cout << '\n';
}

void cat(const std::vector<std::string>& args){
    try{
        if(args.empty()){
            std::cout<<"cat <path> missing";
            return;
        }
        
        std::ifstream file(args[0]);
        if(!file)
        {
            std::cerr
                << "cat: cannot open file\n";
            return;
        }
        std::string line;
        while(std::getline(file,line)){
            std::cout<<line<<std::endl;
        }
        
    }
    catch(std:: exception& e){
        std::cerr
            <<"cat"
            <<e.what()
            <<"\n";
    }
}

void head(const std::vector<std::string>& args){
    try{
        if(args.empty()){
            std::cerr << "head: missing filename\n";
            return;
        }

        std::ifstream file(args[0]);

        if(!file){
            std::cerr << "head: cannot open file\n";
            return;
        }

        size_t n = 10; // default number of lines

        if(args.size() > 1){
            n = std::stoi(args[1]);
        }

        std::string line;

        for(size_t i = 0; i < n; i++){
            if(std::getline(file, line)){
                std::cout << line << '\n';
            }
            else{
                break; // reached end of file
            }
        }
    }
    catch(const std::exception& e){
        std::cerr
            << "head: "
            << e.what()
            << '\n';
    }
}

void tail(const std::vector<std::string>& args){
    try{
        if(args.empty()){
            std::cerr << "tail: missing filename\n";
            return;
        }

        std::ifstream file(args[0]);

        if(!file){
            std::cerr << "tail: cannot open file\n";
            return;
        }

        size_t n = 10; // default

        if(args.size() > 1){
            n = std::stoi(args[1]);
        }

        std::vector<std::string> lines;
        std::string line;

        while(std::getline(file, line)){
            lines.push_back(line);
        }

        size_t start = 0;

        if(lines.size() > n){
            start = lines.size() - n;
        }

        for(size_t i = start; i < lines.size(); i++){
            std::cout << lines[i] << '\n';
        }
    }
    catch(const std::exception& e){
        std::cerr
            << "tail: "
            << e.what()
            << '\n';
    }
}

void clear(){
    std::cout << "\033[3J\033[2J\033[H";
    std::cout.flush();
}

void mv(const std::vector<std::string>& args) {
    try {
        if (args.size() < 2) {
            std::cerr << "mv: missing source or destination\n";
            return;
        }

        std::filesystem::rename(args[0], args[1]);

        std::cout << "Moved/Renamed successfully.\n";
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "mv: " << e.what() << '\n';
    }
    catch (const std::exception& e) {
        std::cerr << "mv: " << e.what() << '\n';
    }
}
void diff(const std::vector<std::string>& args)
{
    try
    {
        if(args.size() < 2)
        {
            std::cerr << "diff: missing file operands\n";
            return;
        }

        std::ifstream file1(args[0]);
        std::ifstream file2(args[1]);

        if(!file1)
        {
            std::cerr << "diff: cannot open " << args[0] << '\n';
            return;
        }

        if(!file2)
        {
            std::cerr << "diff: cannot open " << args[1] << '\n';
            return;
        }

        std::vector<std::string> A;
        std::vector<std::string> B;

        std::string line;

        while(std::getline(file1, line))
            A.push_back(line);

        while(std::getline(file2, line))
            B.push_back(line);

        int n = static_cast<int>(A.size());
        int m = static_cast<int>(B.size());

        std::vector<std::vector<int>> dp(
            n + 1,
            std::vector<int>(m + 1, 0)
        );

        for(int i = 1; i <= n; i++)
        {
            for(int j = 1; j <= m; j++)
            {
                if(A[i - 1] == B[j - 1])
                {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                }
                else
                {
                    dp[i][j] = std::max(
                        dp[i - 1][j],
                        dp[i][j - 1]
                    );
                }
            }
        }

        std::vector<std::string> result;

        int i = n;
        int j = m;

        while(i > 0 && j > 0)
        {
            if(A[i - 1] == B[j - 1])
            {
                result.push_back("  " + A[i - 1]);
                i--;
                j--;
            }
            else if(dp[i - 1][j] >= dp[i][j - 1])
            {
                result.push_back("- " + A[i - 1]);
                i--;
            }
            else
            {
                result.push_back("+ " + B[j - 1]);
                j--;
            }
        }

        while(i > 0)
        {
            result.push_back("- " + A[i - 1]);
            i--;
        }

        while(j > 0)
        {
            result.push_back("+ " + B[j - 1]);
            j--;
        }

        std::reverse(result.begin(), result.end());

        for(const auto& entry : result)
        {
            std::cout << entry << '\n';
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "diff: " << e.what() << '\n';
    }
}
void cp(const std::vector<std::string>& args)
{
    try
    {
        if(args.size() < 2)
        {
            std::cerr << "cp: missing source or destination\n";
            return;
        }

        std::filesystem::copy_file(
            args[0],
            args[1],
            std::filesystem::copy_options::overwrite_existing
        );

        std::cout << "File copied successfully.\n";
    }
    catch(const std::filesystem::filesystem_error& e)
    {
        std::cerr << "cp: " << e.what() << '\n';
    }
    catch(const std::exception& e)
    {
        std::cerr << "cp: " << e.what() << '\n';
    }
}
struct CommandInfo
{
    std::string name;
    std::string description;
    std::string usage;
    std::string details;
};

std::vector<CommandInfo> commands =
{
    {"ls",
     "List files and directories",
     "ls [path]",
     "Displays all files and folders in the specified directory. If no path is given, current directory is used."},

    {"cd",
     "Change current directory",
     "cd <directory>",
     "Changes the current working directory to the specified path."},

    {"pwd",
     "Print current working directory",
     "pwd",
     "Displays the full path of the current directory."},

    {"touch",
     "Create a file",
     "touch <filename>",
     "Creates a new empty file. If the file already exists, its timestamp is updated."},

    {"rm",
     "Remove a file",
     "rm <filename>",
     "Deletes the specified file."},

    {"cat",
     "Display file contents",
     "cat <filename>",
     "Prints the entire contents of a file."},

    {"head",
     "Display first lines of a file",
     "head <filename> <n>",
     "Displays the first n lines of the file."},

    {"tail",
     "Display last lines of a file",
     "tail <filename> <n>",
     "Displays the last n lines of the file."},

    {"echo",
     "Print text",
     "echo <text>",
     "Prints text to the terminal or writes text to a file depending on implementation."},

    {"mkdir",
     "Create a directory",
     "mkdir <directory_name>",
     "Creates a new directory."},

    {"cp",
     "Copy files",
     "cp <source> <destination>",
     "Copies a file from source to destination."},

    {"mv",
     "Move/Rename files",
     "mv <source> <destination>",
     "Moves a file or renames it."},

    {"diff",
     "Compare two files",
     "diff <file1> <file2>",
     "Compares two files line-by-line and displays differences."},

    {"find",
     "Search for files",
     "find <directory> <filename>",
     "Recursively searches for a file inside a directory."},

    {"grep",
     "Search text patterns",
     "grep <pattern> <filename>",
     "Searches for a pattern inside a file."},

    {"wc",
     "Count words, lines and characters",
     "wc <filename>",
     "Displays word count, line count and character count."},

    {"tree",
     "Display directory structure",
     "tree [path]",
     "Displays files and folders in a tree-like format."},

    {"history",
     "Display command history",
     "history",
     "Shows previously executed commands."},

    {"search",
     "Search the web",
     "search <query>",
     "Performs a web search and displays results."},

    {"neofetch",
     "Display system information",
     "neofetch",
     "Shows OS, CPU, memory and system information."},

    {"clear",
     "Clear terminal screen",
     "clear",
     "Clears the terminal window."},

    {"help",
     "Show help information",
     "help [command]",
     "Displays all commands or detailed help for a specific command."}
};

void help(const std::vector<std::string>& args)
{
    if(args.empty())
    {
        std::cout << "Available Commands:\n\n";

        for(const auto& cmd : commands)
        {
            std::cout
                << cmd.name
                << " - "
                << cmd.description
                << '\n';
        }

        std::cout << "\nType 'help <command>' for detailed information.\n";
        return;
    }

    std::string commandName = args[0];

    for(const auto& cmd : commands)
    {
        if(cmd.name == commandName)
        {
            std::cout << "\nCommand : " << cmd.name << '\n';
            std::cout << "Description : " << cmd.description << '\n';
            std::cout << "Usage : " << cmd.usage << '\n';
            std::cout << "Details : " << cmd.details << '\n';
            return;
        }
    }

    std::cout << "help: command '" << commandName
              << "' not found\n";
}

void stat(const std::vector<std::string>& args){
    if(args.empty()) {
        std::cerr << "stat: missing file operand\n";
        return;
    }

    std::filesystem::path p(args[0]);

    if(!std::filesystem::exists(p)) {
        std::cerr << "stat: file does not exist\n";
        return;
    }

    auto status = std::filesystem::status(p);

    std::cout << "Path: " << p << '\n';

    if(std::filesystem::is_regular_file(status))
    {
        std::cout << "Type: Regular File\n";
        std::cout << "Size: "
                << std::filesystem::file_size(p)
                << " bytes\n";
    }
    else if(std::filesystem::is_directory(status))
    {
        std::cout << "Type: Directory\n";
    }
    else if(std::filesystem::is_symlink(status))
    {
        std::cout << "Type: Symbolic Link\n";
    }
    else
    {
        std::cout << "Type: Other\n";
    }

}
void find(const std::vector<std::string>& args){
    if(args.empty()){
        std::cerr<<"Command argument not specified"<<std::endl;
        return;
    }

    std::string target = args[0];
    for(const auto& entry : std::filesystem::recursive_directory_iterator("." ,
         std::filesystem::directory_options::skip_permission_denied)){
        if(entry.path().filename() == target){
            std::cout<<std::filesystem::absolute(entry.path())<<'\n';
        }
    }
}
void grep(const std::vector<std::string>& args)
{
    if(args.size() < 2)
    {
        std::cerr << "grep: usage: grep <pattern> <file>\n";
        return;
    }

    std::string pattern = args[0];
    std::string filename = args[1];

    std::ifstream file(filename);

    if(!file)
    {
        std::cerr << "grep: cannot open file '" << filename << "'\n";
        return;
    }

    std::string line;
    size_t line_number = 1;

    while(std::getline(file, line))
    {
        if(line.find(pattern) != std::string::npos)
        {
            std::cout << line_number << ": " << line << '\n';
        }

        ++line_number;
    }
}
void wc(const std::vector<std::string>& args)
{
    if(args.empty())
    {
        std::cerr << "wc: missing filename\n";
        return;
    }

    std::ifstream file(args[0]);

    if(!file)
    {
        std::cerr << "wc: cannot open file\n";
        return;
    }

    size_t lines = 0;
    size_t words = 0;
    size_t characters = 0;

    std::string line;

    while(std::getline(file, line))
    {
        lines++;

        characters += line.length() + 1; // +1 for '\n'

        std::stringstream ss(line);
        std::string word;

        while(ss >> word)
        {
            words++;
        }
    }

    std::cout << "Lines: " << lines << '\n';
    std::cout << "Words: " << words << '\n';
    std::cout << "Characters: " << characters << '\n';
}
extern std::vector<std::string> command_history;

void history_command(const std::vector<std::string>&)
{
    if(command_history.empty())
    {
        std::cout << "No commands in history.\n";
        return;
    }

    for(size_t i = 0; i < command_history.size(); i++)
    {
        std::cout << i + 1 << " "
                  << command_history[i]
                  << '\n';
    }
}

void printTree(const std::filesystem::path& path,
               const std::string& prefix = "")
{
    for(const auto& entry : std::filesystem::directory_iterator(path))
    {
        std::cout << prefix
                  << "├── "
                  << entry.path().filename().string()
                  << '\n';

        if(std::filesystem::is_directory(entry) && !std::filesystem::is_symlink(entry))
        {
            printTree(entry.path(), prefix + "│   ");
        }
    }
}

void tree(const std::vector<std::string>& args)
{
    std::filesystem::path root = ".";

    if(!args.empty())
    {
        root = args[0];
    }

    if(!std::filesystem::exists(root))
    {
        std::cerr << "tree: path does not exist\n";
        return;
    }

    std::cout << root.string() << '\n';

    printTree(root);
}
// void search(const std::vector<std::string>& args)
// {
//     if(args.empty())
//     {
//         std::cerr << "Usage: watch <search terms>\n";
//         return;
//     }

//     std::string query;

//     for(const auto& arg : args)
//     {
//         query += arg + " ";
//     }

//     for(char& c : query)
//     {
//         if(c == ' ')
//             c = '+';
//     }

//     std::string url =
//         "https://www.google.com/search?q=" + query;

//     ShellExecuteA(
//         nullptr,
//         "open",
//         url.c_str(),
//         nullptr,
//         nullptr,
//         SW_SHOWNORMAL
//     );

//     std::cout << "Opening search: " << query << '\n';
// }

void search(
    const std::vector<std::string>& args)
{
    if(args.empty())
    {
        std::cout
            << "Usage: search <query>\n";

        return;
    }

    std::string query;

    for(const auto& arg : args)
    {
        query += arg + " ";
    }

    std::cout
        << "Searching...\n";

    auto results =
        search_tavily(query);

    if(results.empty())
    {
        std::cout
            << "No results found.\n";

        return;
    }

    std::cout
        << "\nResults:\n\n";

    for(size_t i = 0;
        i < results.size();
        i++)
    {
        std::cout
            << "\033[36m"
            << "["
            << i + 1
            << "] "
            << "\033[0m"
            << results[i].title
            << '\n';
    }

    int choice;

    std::cout
        << "\nSelect result: ";

    if(!(std::cin >> choice))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n');

        std::cout << "Invalid input\n";
        return;
    }

    if(choice < 1 ||
       choice >
       static_cast<int>(
           results.size()))
    {
        std::cout
            << "Invalid choice\n";

        return;
    }

    ShellExecuteA(
        nullptr,
        "open",
        results[choice - 1]
            .url.c_str(),
        nullptr,
        nullptr,
        SW_SHOWNORMAL);
}

void neofetch(const std::vector<std::string>&)
{
    std::cout
    << "\033[36m"
    << R"(

       /\
      /  \
     /++++\
    /  ++  \
   /++++++++\
  /          \

)"
    << "\033[0m";

    std::cout
    << "\033[1;32mOS:\033[0m Kshitij Terminal\n"
    << "\033[1;32mVersion:\033[0m 1.0\n"
    << "\033[1;32mCompiler:\033[0m GCC\n"
    << "\033[1;32mDirectory:\033[0m "
    << std::filesystem::current_path()
    << "\n";
}