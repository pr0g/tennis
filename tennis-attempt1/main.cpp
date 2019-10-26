#include <string>
#include <iostream>

struct tennis_t
{
    static const int p1 = 0;
    static const int p2 = 1;

    int m_score[2] = {};
    int m_game = -1;

    void point(int player)
    {
        if (game()) {
            return;
        }

        if (deuce()) {
            m_score[player]++;
        } else if (adv()) {
            if (m_score[player] == 4) {
                m_game = player;
            } else {
                m_score[!player] = 3;
            }
        } else {
            m_score[player]++;
            if (m_score[player] > 3) {
                m_game = player;
            }
        }
    }

    bool deuce() const {
        return m_score[p1] == 3 && m_score[p1] == m_score[p2];
    }

    bool adv() const {
        return (m_score[p1] == 4 && m_score[p2] == 3)
            || (m_score[p2] == 4 && m_score[p1] == 3);
    }

    bool game() const { return m_game != -1; }

    std::string score() const {
        if (game()) {
            std::string player[] = { "p1", "p2" };
            return "game " + player[m_game];
        } else if (deuce()) {
            return "deuce";
        } else if (adv()) {
            if (m_score[p1] > m_score[p2]) {
                return "p1 adv";
            } else {
                return "p2 adv";
            }
        } else {
            const std::string normal_scores[] {
                "love",
                "fifteen",
                "thirty",
                "forty"
            };

            return normal_scores[m_score[p1]] + " " + normal_scores[m_score[p2]];
        }
    }
};

int main(int argc, char** argv) {

    tennis_t tennis;

    tennis.point(tennis_t::p1);
    tennis.point(tennis_t::p1);
    tennis.point(tennis_t::p1);
    tennis.point(tennis_t::p2);
    tennis.point(tennis_t::p2);
    tennis.point(tennis_t::p2);
    tennis.point(tennis_t::p2);
    tennis.point(tennis_t::p1);
    tennis.point(tennis_t::p1);
    tennis.point(tennis_t::p2);
    tennis.point(tennis_t::p2);
    tennis.point(tennis_t::p2);

    std::cout << tennis.score() << "\n";

    return 0;
}
