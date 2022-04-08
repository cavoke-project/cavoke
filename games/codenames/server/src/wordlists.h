#ifndef CAVOKE_WORDLISTS_H
#define CAVOKE_WORDLISTS_H

#include <filesystem>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

namespace codenames {

inline std::string path_sep() {
    return {
        std::filesystem::path::preferred_separator
    }
}

const std::string WORDLISTS_PATH = "assets" + path_sep() + "wordlists";

struct WordList {
    explicit WordList(const std::string &wordlist_name);

    struct wordlist_not_found : std::runtime_error {
        explicit wordlist_not_found(const std::string &wordlist);
    };

    const std::vector<std::string> &all_words();
    int size();
    std::vector<std::string> sample(int n);

private:
    std::vector<std::string> m_words;
    std::mt19937 m_rnd;
};

}  // namespace codenames

#endif  // CAVOKE_WORDLISTS_H
