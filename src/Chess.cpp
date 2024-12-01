#include "BaseChess/Chess.hpp"

namespace Chess
{
    GameState::GameState(PieceType (*getPromotionTypeWhite)(), PieceType (*getPromotionTypeBlack)())
        : 
        gameBoard((new GameBoardImpl())->getInstance()),
        gameResult(PlayerSide::None),
        getPromotionTypeWhite(getPromotionTypeWhite),
        getPromotionTypeBlack(getPromotionTypeBlack)
    {}

    GameState::GameState(const GameState& other)
        : 
        gameBoard((new GameBoardImpl(*(other.gameBoard)))->getInstance()),
        gameResult(other.gameResult),
        getPromotionTypeWhite(other.getPromotionTypeWhite),
        getPromotionTypeBlack(other.getPromotionTypeBlack)
    {}

    const std::shared_ptr<GameEntity> GameState::getPieceAt(GridPoint position) const
    {
        return gameBoard->getPieceAt(position);
    }

    PlayerSide GameState::getGameResult() const
    {
        return gameResult;
    }

    const std::shared_ptr<GameBoard> GameState::getGameBoard() const
    {
        return gameBoard;
    }

    GameState& GameState::operator=(const GameState& other)
    {
        gameBoard = std::shared_ptr<GameBoardImpl>(new GameBoardImpl(*other.gameBoard));
        this->getPromotionTypeWhite = other.getPromotionTypeWhite;
        this->getPromotionTypeBlack = other.getPromotionTypeBlack;

        return *this;
    }

    bool GameState::executeMove(GridPoint source, GridPoint target)
    {
        // Checking for correct positions
        if (!source.isValid() || !target.isValid()) return false;

        std::shared_ptr<GamePiece> movingPiece = gameBoard->getMovablePieceAt(source);
        if (movingPiece == nullptr) return false;
        if (movingPiece->team != gameBoard->getActivePlayer()) return false;

        // Search for a move
        std::vector<std::shared_ptr<GameMove>> possibleMoves;
        movingPiece->computeMoves(possibleMoves);

        std::shared_ptr<GameMove> move;
        for (std::shared_ptr<GameMove> currentMove : possibleMoves)
        {
            try
            {
                if (currentMove->getMoveSequence().at(source) != target) 
                    continue;

                move = currentMove;
                break;
            }
            catch (std::out_of_range&) {}
        }

        if (!move) return false;

        PlayerSide currentTeam = gameBoard->getActivePlayer();
        std::shared_ptr<GameEntity> currentKing = gameBoard->getPiecesByType(PieceType::Sovereign, currentTeam).at(0);

        gameBoard->executeMove(move);
        
        // Checking the move
        PlayerSide enemyTeam = gameBoard->getActivePlayer();

        std::vector<std::shared_ptr<GameMove>> enemyMoves;
        gameBoard->computeMoves(enemyMoves, enemyTeam);

        for (std::shared_ptr<GameMove>& enemyMove : enemyMoves)
        {
            for (const GridPoint& capturePosition : enemyMove->getCapturePoints())
            {
                if (capturePosition == currentKing->pos)
                {
                    gameBoard->undoMove(move);
                    return false;
                }
            }
        }

        // End-of-game check
        bool isEndGame = true;

        for (std::shared_ptr<GameMove>& enemyMove : enemyMoves)
        {
            if (validateMove(enemyMove))
            {
                isEndGame = false;
                break;
            }
        }
        
        if (!isEndGame) return true;

        std::vector<std::shared_ptr<GameMove>> currentTeamAttacks;
        gameBoard->computeMoves(currentTeamAttacks, currentTeam, true);

        std::shared_ptr<GameEntity> enemyKing = gameBoard->getPiecesByType(PieceType::Sovereign, enemyTeam).at(0);

        PlayerSide finalResult = PlayerSide::Any;
        for (std::shared_ptr<GameMove>& attackMove : currentTeamAttacks)
        {
            for (const GridPoint& capturePosition : attackMove->getCapturePoints())
            {
                if (capturePosition == enemyKing->pos)
                {
                    finalResult = currentTeam;
                    break;
                }
            }

            if (finalResult != PlayerSide::Any)
                break;
        }
        this->gameResult = finalResult;
        
        return true;
    }

    bool GameState::validateMove(std::shared_ptr<GameMove> move)
    {
        // Getting our king
        std::shared_ptr<GameEntity> currentKing = gameBoard->getPiecesByType(PieceType::Sovereign, gameBoard->getActivePlayer()).at(0);

        // Making the move
        gameBoard->executeMove(move);

        // Checking the move
        std::vector<std::shared_ptr<GameMove>> enemyMoves;
        PlayerSide enemyTeam = gameBoard->getActivePlayer();
        gameBoard->computeMoves(enemyMoves, enemyTeam, true);

        bool isValid = true;
        for (std::shared_ptr<GameMove>& enemyMove : enemyMoves)
        {
            for (const GridPoint& capturePosition : enemyMove->getCapturePoints())
            {
                if (capturePosition == currentKing->pos)
                {
                    isValid = false;
                    break;
                }
            }

            if (!isValid) break;
        }

        // Canceling the move
        gameBoard->undoMove(move);

        return isValid;
    }
}