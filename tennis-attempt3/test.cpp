#include "catch2/catch.hpp"

#include <string>
#include <variant>
#include <optional>

template<typename... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template<typename... Ts> overloaded(Ts...) -> overloaded<Ts...>;

enum class Player
{
    One,
    Two
};

Player opposite_player(Player player) {
    Player players[] = { Player::Two, Player::One };
    return players[static_cast<int>(player)];
}

enum class Points
{
    Love,
    Fifteen,
    Thirty,
};

struct Normal
{
    Points points(Player player) const {
        return players[static_cast<int>(player)];
    }

    void set_points(Player player, Points points) {
        players[static_cast<int>(player)] = points;
    }

    bool equal() const { return players[0] == players[1]; }
private:
    Points players[2];
};

struct Forty
{
    Player fortyPlayer;
    Points otherPlayer;
};

struct Deuce {};
struct Advantage { Player player; };
struct Game { Player winner; };

std::string point_display(const Points point) {
    switch (point) {
        case Points::Love:
            return "Love";
        case Points::Fifteen:
            return "Fifteen";
        case Points::Thirty:
            return "Thirty";
        default:
            return "";
    }
}

std::string player_display(const Player player) {
    switch (player) {
        case Player::One:
            return "Player One";
        case Player::Two:
            return "Player Two";
    }
}

class Tennis
{
public:
    std::string show_score() const
    {
        return std::visit(
            overloaded {
                [this](const Normal& normal)
                {
                    if (normal.equal()) {
                        return point_display(normal.points(Player::One)) + std::string(" All");
                    } else {
                        return point_display(normal.points(Player::One)) + ":" + point_display(normal.points(Player::Two));
                    }
                },
                [this](const Forty& forty)
                {
                    switch (forty.fortyPlayer) {
                        case Player::One:
                            return "Forty:" + point_display(forty.otherPlayer);
                        case Player::Two:
                            return point_display(forty.otherPlayer) + ":Forty";
                    }
                },
                [this](const Deuce& deuce)
                {
                    return std::string("Deuce");
                },
                [this](const Advantage& advantage)
                {
                    return "Advantage " + player_display(advantage.player);
                },
                [this](const Game& game)
                {
                    return std::string("Game ") + player_display(game.winner);
                },
            }, m_state);
    }

    std::optional<Points> next_normal_score(const Points& points) {
        switch (points) {
        case Points::Love:
            return { Points::Fifteen };
        case Points::Fifteen:
            return { Points::Thirty };
        default:
            return {};
        }
    }

    void point(Player player)
    {
        std::visit(
            overloaded {
                [this, player](const Normal& normal)
                {
                    Normal next_normal = normal;
                    auto next = next_normal_score(normal.points(player));
                    if (next.has_value()) {
                        next_normal.set_points(player, next.value());
                        m_state = next_normal;
                    } else {
                        m_state = Forty { player, next_normal.points(opposite_player(player)) };
                    }
                },
                [this, player](const Forty& forty)
                {
                    if (player == forty.fortyPlayer) {
                        m_state = Game { player };
                    } else {
                        auto next = next_normal_score(forty.otherPlayer);
                        if (next.has_value()) {
                            m_state = Forty { forty.fortyPlayer, next.value() };
                        } else {
                            m_state = Deuce{};
                        }
                    }
                },
                [this, player](const Deuce& deuce)
                {
                    m_state = Advantage { player };
                },
                [this, player](const Advantage& advantage)
                {
                    if (player == advantage.player) {
                        m_state = Game { player };
                    } else {
                        m_state = Deuce{};
                    }
                },
                [this, player](const Game& game)
                {
                    // do nothing
                },
            }, m_state);
    }

    std::variant<Normal, Forty, Deuce, Advantage, Game> m_state;
    std::string m_score_display { "Love All" };
};

TEST_CASE("InitialScoreLoveAll", "[tennis]")
{
    Tennis tennis;

    if (auto score = std::get_if<Normal>(&tennis.m_state))
    {
        CHECK(score->points(Player::One) == Points::Love);
        CHECK(score->points(Player::Two) == Points::Love);
    }
}

TEST_CASE("PlayerOne_OnePoint", "[tennis]")
{
    Tennis tennis;
    tennis.point(Player::One);

    if (auto score = std::get_if<Normal>(&tennis.m_state))
    {
        CHECK(score->points(Player::One) == Points::Fifteen);
        CHECK(score->points(Player::Two) == Points::Love);
    }
}

TEST_CASE("PlayerOne_OnePoint_PlayerTwo_OnePoint", "[tennis]")
{
    Tennis tennis;
    tennis.point(Player::One);
    tennis.point(Player::Two);

    if (auto score = std::get_if<Normal>(&tennis.m_state))
    {
        CHECK(score->points(Player::One) == Points::Fifteen);
        CHECK(score->points(Player::Two) == Points::Fifteen);
    }
}

TEST_CASE("PlayerOne_ThreePoints", "[tennis]")
{
    Tennis tennis;
    tennis.point(Player::One);
    tennis.point(Player::One);
    tennis.point(Player::One);

    if (auto score = std::get_if<Forty>(&tennis.m_state))
    {
        CHECK(score->fortyPlayer == Player::One);
        CHECK(score->otherPlayer == Points::Love);
    }
}

TEST_CASE("PlayerOne_FourPoints", "[tennis]")
{
    Tennis tennis;
    tennis.point(Player::One);
    tennis.point(Player::One);
    tennis.point(Player::One);
    tennis.point(Player::One);

    if (auto game = std::get_if<Game>(&tennis.m_state))
    {
        CHECK(game->winner == Player::One);
    }
}

TEST_CASE("PlayerOne_ThreePoints_PlayerTwo_TwoPoints", "[tennis]")
{
    Tennis tennis;
    tennis.point(Player::One);
    tennis.point(Player::Two);
    tennis.point(Player::One);
    tennis.point(Player::One);
    tennis.point(Player::Two);

    if (auto score = std::get_if<Forty>(&tennis.m_state))
    {
        CHECK(score->fortyPlayer == Player::One);
        CHECK(score->otherPlayer == Points::Thirty);
    }
}

TEST_CASE("PlayerOne_ThreePoints_PlayerTwo_ThreePoints", "[tennis]")
{
    Tennis tennis;
    tennis.point(Player::One);
    tennis.point(Player::Two);
    tennis.point(Player::One);
    tennis.point(Player::One);
    tennis.point(Player::Two);
    tennis.point(Player::Two);

    auto score = std::get_if<Deuce>(&tennis.m_state);
    CHECK(score != nullptr);
}

TEST_CASE("PlayerOne_FourPoints_PlayerTwo_ThreePoints", "[tennis]")
{
    Tennis tennis;
    tennis.point(Player::One);
    tennis.point(Player::Two);
    tennis.point(Player::One);
    tennis.point(Player::One);
    tennis.point(Player::Two);
    tennis.point(Player::Two);
    tennis.point(Player::One);

    if (auto advantage = std::get_if<Advantage>(&tennis.m_state))
    {
        CHECK(advantage->player == Player::One);
    }
}

TEST_CASE("PlayerOne_FourPoints_PlayerTwo_FourPoints", "[tennis]")
{
    Tennis tennis;
    tennis.point(Player::One);
    tennis.point(Player::Two);
    tennis.point(Player::One);
    tennis.point(Player::One);
    tennis.point(Player::Two);
    tennis.point(Player::Two);
    tennis.point(Player::One);
    tennis.point(Player::Two);

    auto score = std::get_if<Deuce>(&tennis.m_state);
    CHECK(score != nullptr);
}

TEST_CASE("TestScoreOutput", "[tennis]")
{
    Tennis tennis;
    tennis.point(Player::One);
    tennis.point(Player::Two);
    tennis.point(Player::One);
    tennis.point(Player::One);
    tennis.point(Player::Two);
    tennis.point(Player::Two);
    tennis.point(Player::One);
    tennis.point(Player::Two);

    CHECK(std::string{"Deuce"} == tennis.show_score());
}
