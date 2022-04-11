#include "wordlists.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>

namespace codenames {

WordList::wordlist_not_found::wordlist_not_found(const std::string &wordlist)
    : std::runtime_error("Wordlist \"" + wordlist + "\" not found") {
}

WordList::WordList(const std::string &wordlist_name) {
    std::string file_path =
        WORDLISTS_PATH + path_sep() + wordlist_name + ".txt";
    if (!std::filesystem::exists(file_path)) {
        throw wordlist_not_found(wordlist_name);
    }
}
const std::vector<std::string> &WordList::all_words() const {
    return m_words;
}
int WordList::size() const {
    return static_cast<int>(m_words.size());
}
std::vector<std::string> WordList::sample(int n) const {
    std::vector<std::string> result(n);
    std::sample(m_words.begin(), m_words.end(), result.begin(), n, m_rnd);
    return result;
}

}  // namespace codenames
