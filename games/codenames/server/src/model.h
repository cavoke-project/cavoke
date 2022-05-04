#ifndef CAVOKE_MODEL_H
#define CAVOKE_MODEL_H

#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <vector>
#include "wordlists.h"

namespace codenames {

class CodenamesModel {  // NOLINT(cppcoreguidelines-pro-type-member-init)
public:
    struct invalid_move : std::runtime_error {
        invalid_move();
    };

    enum class GAME_STAGE {
        BLUE_CAPTAIN = 0,
        BLUE_TEAM = 1,
        RED_CAPTAIN = 2,
        RED_TEAM = 3,
        FINISHED = 4
    };

    enum class CARD_STATE {
        BLUE = 0,
        RED = 1,
        BLACK = 2,
        NEUTRAL = 3,
        CLOSED = 4
    };

    enum class GAME_RESULT { IN_PROGRESS = 0, BLUE_WINS = 1, RED_WINS = 2 };

private:
    void generate_cards();

    GAME_STAGE m_stage;
    std::string m_last_hint;
    int m_attempts_left;
    std::vector<std::string> m_words;
    std::vector<CARD_STATE> m_card_states;
    std::vector<bool> m_opened;
    int m_height;
    int m_width;
    int m_blue_closed;
    int m_red_closed;
    GAME_RESULT m_result;

    void next_stage();

public:
    [[nodiscard]] GAME_STAGE stage() const;
    [[nodiscard]] GAME_RESULT result() const;

    void open_card(int pos);
    void make_hint(std::string hint, int attempts);
    void skip();

    [[nodiscard]] std::string get_player_state(int player) const;
    [[nodiscard]] std::string get_global_state() const;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CodenamesModel,
                                   m_stage,
                                   m_last_hint,
                                   m_attempts_left,
                                   m_words,
                                   m_card_states,
                                   m_opened,
                                   m_height,
                                   m_width,
                                   m_result,
                                   m_blue_closed,
                                   m_red_closed)

    CodenamesModel(int height_, int width_, const WordList &wordlist_);
    CodenamesModel() = default;
};

}  // namespace codenames

#endif  // CAVOKE_MODEL_H
