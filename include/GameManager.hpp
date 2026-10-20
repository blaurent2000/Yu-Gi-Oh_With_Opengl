
enum class Phase {
    Draw,
    Standby,
    Main1,
    Battle,
    Main2,
    End
};

enum class Player {
    Player1,
    Player2
};

class GameManager {
    public:
        GameManager() : currentPhase(Phase::Draw), currentPlayer(Player::Player1) {};
        Phase getCurrentPhase() const { return currentPhase; };
        Player getCurrentplayer () const { return currentPlayer; };
    private:
        Phase currentPhase;
        Player currentPlayer;
};