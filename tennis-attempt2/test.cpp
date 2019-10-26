#include "catch2/catch.hpp"

enum class Score
{
    LoveAll,
    FifteenLove,
    LoveFifteen,
    FifteenAll,
    ThirtyLove,
    LoveThirty,
    ThirtyFifteen,
    FifteenThirty,
    ThirtyAll,
    FortyLove,
    LoveForty,
    FortyFifteen,
    FifteenForty,
    Game
};

enum class Player
{
    One,
    Two
};

class Tennis
{
public:
    Score score() const
    {
        return m_score;
    }

    void next_score(
        const Player scoring_player, const Score player1_scored, const Score player2_scored)
    {
        switch (scoring_player)
        {
        case Player::One:
            m_score = player1_scored;
            break;
        case Player::Two:
            m_score = player2_scored;
            break;
        }
    }

    void point(const Player player)
    {
        switch (m_score)
        {
        case Score::LoveAll:
            next_score(player, Score::FifteenLove, Score::LoveFifteen);
            break;
        case Score::FifteenLove:
            next_score(player, Score::ThirtyLove, Score::FifteenAll);
            break;
        case Score::LoveFifteen:
            next_score(player, Score::FifteenAll, Score::LoveThirty);
            break;
        case Score::ThirtyLove:
            next_score(player, Score::FortyLove, Score::ThirtyFifteen);
            break;
        case Score::LoveThirty:
            next_score(player, Score::FifteenThirty, Score::LoveForty);
            break;
        case Score::ThirtyFifteen:
            next_score(player, Score::FortyFifteen, Score::ThirtyAll);
            break;
        case Score::FifteenThirty:
            next_score(player, Score::ThirtyAll, Score::FifteenForty);
            break;
        case Score::FortyLove:
            next_score(player, Score::Game, Score::FortyFifteen);
            break;
        default:
            INFO("STATE WAS" << static_cast<int>(m_score));
            assert(false);
            break;
        }
    }

    Score m_score;
};

TEST_CASE("GameBeginsLoveAll", "[tennis]")
{
    Tennis tennis;
    CHECK(tennis.score() == Score::LoveAll);
}

TEST_CASE("PlayerOne_OnePoint_FifteenLove", "[tennis]")
{
    Tennis tennis;
    tennis.point(Player::One);
    CHECK(tennis.score() == Score::FifteenLove);
}

TEST_CASE("PlayerOne_TwoPoints_ThirtyLove", "[tennis]")
{
    Tennis tennis;
    tennis.point(Player::One);
    tennis.point(Player::One);
    CHECK(tennis.score() == Score::ThirtyLove);
}

TEST_CASE("PlayerOne_OnePoint_PlayerTwo_OnePoint_FifteenAll", "[tennis]")
{
    Tennis tennis;
    tennis.point(Player::One);
    tennis.point(Player::Two);
    CHECK(tennis.score() == Score::FifteenAll);
}

TEST_CASE("PlayerTwo_TwoPoints_LoveThirty", "[tennis]")
{
    Tennis tennis;
    tennis.point(Player::Two);
    tennis.point(Player::Two);
    CHECK(tennis.score() == Score::LoveThirty);
}

TEST_CASE("PlayerOne_TwoPoints_PlayerTwo_TwoPoints_ThirtyAll", "[tennis]")
{
    Tennis tennis;
    tennis.point(Player::Two);
    tennis.point(Player::Two);
    tennis.point(Player::One);
    tennis.point(Player::One);
    CHECK(tennis.score() == Score::ThirtyAll);
}

TEST_CASE("PlayerOne_ThreePoints_PlayerTwo_OnePoint_FortyFifteen", "[tennis]")
{
    Tennis tennis;
    tennis.point(Player::One);
    tennis.point(Player::One);
    tennis.point(Player::One);
    tennis.point(Player::Two);
    CHECK(tennis.score() == Score::FortyFifteen);
}
