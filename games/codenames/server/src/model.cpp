#include "model.h"
#include <utility>

namespace codenames {

CodenamesModel::GAME_STAGE CodenamesModel::stage() const {
    return m_stage;
}

const std::string &CodenamesModel::last_hint() const {
    return m_last_hint;
}

int CodenamesModel::height() const {
    return m_height;
}

int CodenamesModel::width() const {
    return m_width;
}

CodenamesModel::GAME_RESULT CodenamesModel::result() const {
    return m_result;
}

CodenamesModel::CodenamesModel(int height_,
                               int width_,
                               const WordList &wordlist_)
    : m_height(height_),
      m_width(width_),
      m_stage(GAME_STAGE::BLUE_CAPTAIN),
      m_result(GAME_RESULT::IN_PROGRESS) {
    int cards_cnt = height_ * width_;
    m_opened = std::vector<bool>(cards_cnt, false);
    m_words = wordlist_.sample(cards_cnt);
    generate_cards();
}

void CodenamesModel::generate_cards() {
    std::mt19937 rnd{std::random_device{}()};

    int red_cnt = m_height * m_width / 3;
    int blue_cnt = red_cnt + 1;
    int black_cnt = m_height * m_width / 15;

    m_blue_closed = blue_cnt;
    m_red_closed = red_cnt;

    m_card_states =
        std::vector<CARD_STATE>(m_height * m_width, CARD_STATE::NEUTRAL);

    for (int i = 0; i < red_cnt; ++i) {
        size_t pos = rnd() % m_card_states.size();
        while (m_card_states[pos] != CARD_STATE::NEUTRAL) {
            pos = rnd() % m_card_states.size();
        }
        m_card_states[pos] = CARD_STATE::RED;
    }

    for (int i = 0; i < blue_cnt; ++i) {
        size_t pos = rnd() % m_card_states.size();
        while (m_card_states[pos] != CARD_STATE::NEUTRAL) {
            pos = rnd() % m_card_states.size();
        }
        m_card_states[pos] = CARD_STATE::BLUE;
    }

    for (int i = 0; i < black_cnt; ++i) {
        size_t pos = rnd() % m_card_states.size();
        while (m_card_states[pos] != CARD_STATE::NEUTRAL) {
            pos = rnd() % m_card_states.size();
        }
        m_card_states[pos] = CARD_STATE::BLACK;
    }
}

std::string CodenamesModel::get_global_state() const {
    nlohmann::json state_json = *this;
    return state_json.dump();
}

std::string CodenamesModel::get_player_state(int player) const {
    nlohmann::json result = *this;
    result["role"] = player;
    if (player == 0 || player == 1) {
        return result.dump();
    }

    result.erase("m_opened");

    std::vector<CARD_STATE> state(m_width * m_height);

    for (int i = 0; i < m_height * m_width; ++i) {
        if (m_opened[i]) {
            state[i] = m_card_states[i];
        } else {
            state[i] = CARD_STATE::CLOSED;
        }
    }

    result["m_card_states"] = std::move(state);

    return result.dump();
}

void CodenamesModel::open_card(int pos) {
    if (m_stage == GAME_STAGE::BLUE_CAPTAIN ||
        m_stage == GAME_STAGE::RED_CAPTAIN) {
        throw invalid_move();
    }

    if (m_opened[pos]) {
        return;
    }

    m_opened[pos] = true;

    if (m_card_states[pos] == CARD_STATE::BLACK) {
        m_last_hint = "";

        if (m_stage == GAME_STAGE::BLUE_TEAM) {
            m_result = GAME_RESULT::RED_WINS;
        } else {
            m_result = GAME_RESULT::BLUE_WINS;
        }
    } else if (m_card_states[pos] == CARD_STATE::NEUTRAL) {
        m_last_hint = "";

        if (m_stage == GAME_STAGE::BLUE_TEAM) {
            m_stage = GAME_STAGE::RED_CAPTAIN;
        } else {
            m_stage = GAME_STAGE::BLUE_CAPTAIN;
        }
    } else {
        if ((m_card_states[pos] == CARD_STATE::BLUE &&
             m_stage == GAME_STAGE::RED_TEAM)) {
            m_last_hint = "";

            m_stage = GAME_STAGE::BLUE_CAPTAIN;
        } else if (m_card_states[pos] == CARD_STATE::RED &&
                   m_stage == GAME_STAGE::BLUE_TEAM) {
            m_last_hint = "";

            m_stage = GAME_STAGE::RED_CAPTAIN;
        }

        if (m_card_states[pos] == CARD_STATE::BLUE) {
            m_blue_closed--;
        } else if (m_card_states[pos] == CARD_STATE::RED) {
            m_red_closed--;
        }

        if (m_red_closed == 0) {
            m_result = GAME_RESULT::RED_WINS;
            m_stage = GAME_STAGE::FINISHED;
        } else if (m_blue_closed == 0) {
            m_result = GAME_RESULT::BLUE_WINS;
            m_stage = GAME_STAGE::FINISHED;
        }
    }
}

void CodenamesModel::make_hint(std::string hint) {
    if (m_stage != GAME_STAGE::BLUE_CAPTAIN &&
        m_stage != GAME_STAGE::RED_CAPTAIN) {
        throw invalid_move();
    }

    m_last_hint = std::move(hint);

    if (m_stage == GAME_STAGE::BLUE_CAPTAIN) {
        m_stage = GAME_STAGE::BLUE_TEAM;
    } else {
        m_stage = GAME_STAGE::RED_TEAM;
    }
}

CodenamesModel::invalid_move::invalid_move()
    : std::runtime_error("Invalid move") {
}

}  // namespace codenames
