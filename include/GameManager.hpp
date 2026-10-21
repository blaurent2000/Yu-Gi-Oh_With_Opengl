
#pragma once

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
        Player getCurrentPlayer () const { return currentPlayer; };
        void setCurrentPhase(Phase phaseToSet) { currentPhase = phaseToSet; };
        void setCurrentPlayer(Player playerToSet) { currentPlayer = playerToSet; };
    private:
        Phase currentPhase;
        Player currentPlayer;
};