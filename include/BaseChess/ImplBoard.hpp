#pragma once

#include <vector>
#include <stdexcept>
#include <memory>

#include "BaseChess/Piece/Pieces.hpp"
#include "BaseChess/Board.hpp"
#include "BaseChess/Piece/MovablePiece.hpp"
#include "BaseChess/Piece/Piece.hpp"
#include "BaseChess/Move.hpp"
#include "BaseChess/Piece/Enums.hpp"
#include "BaseChess/Position.hpp"

namespace Chess
{
    class GameBoardImpl : public GameBoard
    {
    public:
        GameBoardImpl();
        GameBoardImpl(const GameBoardImpl& other);

        std::shared_ptr<GameBoardImpl> getInstance() const;

        int getPieceId(const GridPoint& pos, int moveTick = -1, bool isCaptured = false) const;
        std::shared_ptr<GamePiece> getMovablePiece(const GridPoint& pos, int moveTick = -1, bool isCaptured = false) const;

        // Overridden functions
        const std::shared_ptr<GameEntity> getPieceAt(GridPoint pos) const override;
        std::vector<std::shared_ptr<GameEntity>> getPiecesByType(PieceType type = PieceType::Any, PlayerSide team = PlayerSide::Any) const override;
        const std::shared_ptr<GamePiece> getMovablePieceAt(GridPoint pos) const override;

        std::shared_ptr<GameMove> getMove(const GridPoint& startPos, const GridPoint& endPos) const;
        void getMoves(const GridPoint& pos, std::vector<std::shared_ptr<GameMove>>& moveList) const;
        void computeMoves(std::vector<std::shared_ptr<GameMove>>& moveList, PlayerSide team = PlayerSide::Any, bool onlyCaptures = false) const override;

        PlayerSide getActivePlayer() const override;
        int getMoveCount() const override;

        void executeMove(std::shared_ptr<GameMove> move) override;
        void undoMove(std::shared_ptr<GameMove> move) override;

    private:
        void initializePiece(GamePiece* piece);
        void changeType(const GridPoint& pos, const PieceType type);
        
        std::vector<std::shared_ptr<GamePiece>> pieces;
        int moveCounter;
        std::shared_ptr<GameBoardImpl> selfRef;
    };
}