#include "BaseChess/ImplBoard.hpp"

namespace Chess
{
    GameBoardImpl::GameBoardImpl() :
        moveCounter(0),
        selfRef(std::shared_ptr<GameBoardImpl>(this))
    {
        std::weak_ptr<GameBoard> board(selfRef);

        for (int i = 0; i < 8; ++i)
        {
            initializePiece(new Soldier(GridPoint(i, 1), PlayerSide::White, board));
            initializePiece(new Soldier(GridPoint(i, 6), PlayerSide::Black, board));
        }

        initializePiece(new Tower(GridPoint(0, 0), PlayerSide::White, board));
        initializePiece(new Tower(GridPoint(7, 0), PlayerSide::White, board));
        initializePiece(new Steed(GridPoint(1, 0), PlayerSide::White, board));
        initializePiece(new Steed(GridPoint(6, 0), PlayerSide::White, board));
        initializePiece(new Diagonal(GridPoint(2, 0), PlayerSide::White, board));
        initializePiece(new Diagonal(GridPoint(5, 0), PlayerSide::White, board));
        initializePiece(new Regent(GridPoint(3, 0), PlayerSide::White, board));
        initializePiece(new Sovereign(GridPoint(4, 0), PlayerSide::White, board));

        initializePiece(new Tower(GridPoint(0, 7), PlayerSide::Black, board));
        initializePiece(new Tower(GridPoint(7, 7), PlayerSide::Black, board));
        initializePiece(new Steed(GridPoint(1, 7), PlayerSide::Black, board));
        initializePiece(new Steed(GridPoint(6, 7), PlayerSide::Black, board));
        initializePiece(new Diagonal(GridPoint(2, 7), PlayerSide::Black, board));
        initializePiece(new Diagonal(GridPoint(5, 7), PlayerSide::Black, board));
        initializePiece(new Regent(GridPoint(3, 7), PlayerSide::Black, board));
        initializePiece(new Sovereign(GridPoint(4, 7), PlayerSide::Black, board));
    }

    GameBoardImpl::GameBoardImpl(const GameBoardImpl& other) :
        moveCounter(other.moveCounter),
        selfRef(std::shared_ptr<GameBoardImpl>(this))
    {
        for (const auto& piece : other.pieces)
        {
            pieces.push_back(piece->clonePiece(std::shared_ptr<GameBoard>(this)));
        }
    }

    std::shared_ptr<GameBoardImpl> GameBoardImpl::getInstance() const
    {
        return selfRef;
    }

    int GameBoardImpl::getPieceId(const GridPoint& pos, int moveTick, bool isCaptured) const
    {
        for (int i = 0; i < pieces.size(); ++i)
        {
            if (pieces[i]->isCaptured != isCaptured) continue;
            if (pieces[i]->pos != pos) continue;
            if (moveTick != -1)
            {
                if (pieces[i]->getLastMoveTick() != moveTick) continue;
            }
            return i;
        }
        return -1;
    }

    std::shared_ptr<GamePiece> GameBoardImpl::getMovablePiece(const GridPoint& pos, int moveTick, bool isCaptured) const
    {
        int id = getPieceId(pos, moveTick, isCaptured);
        return (id != -1) ? pieces.at(id) : nullptr;
    }

    const std::shared_ptr<GameEntity> GameBoardImpl::getPieceAt(GridPoint pos) const
    {
        return getMovablePiece(pos);
    }

    const std::shared_ptr<GamePiece> GameBoardImpl::getMovablePieceAt(GridPoint pos) const
    {
        return getMovablePiece(pos);
    }

    std::vector<std::shared_ptr<GameEntity>> GameBoardImpl::getPiecesByType(PieceType type, PlayerSide team) const
    {
        std::vector<std::shared_ptr<GameEntity>> vec;
        for (const auto& piece : pieces)
        {
            if (piece == nullptr) continue;
            if (piece->isCaptured) continue;
            if (team != piece->team && team != PlayerSide::Any) continue;
            if (type != piece->type && type != PieceType::Any) continue;

            vec.push_back(piece);
        }
        return vec;
    }

    std::shared_ptr<GameMove> GameBoardImpl::getMove(const GridPoint& startPos, const GridPoint& endPos) const
    {
        std::shared_ptr<GamePiece> piece = getMovablePiece(startPos);

        std::vector<std::shared_ptr<GameMove>> moves;
        piece->computeMoves(moves);

        for (const auto& move : moves)
        {
            try
            {
                if (move->getMoveSequence().at(startPos) == endPos)
                {
                    return move;
                }
            }
            catch (std::out_of_range&) {}
        }
        return nullptr;
    }

    void GameBoardImpl::getMoves(const GridPoint& pos, std::vector<std::shared_ptr<GameMove>>& moveList) const
    {
        auto piece = getMovablePiece(pos);
        if (piece) {
            piece->computeMoves(moveList);
        }
    }

    void GameBoardImpl::computeMoves(std::vector<std::shared_ptr<GameMove>>& moveList, PlayerSide team, bool onlyCaptures) const
    {
        for (const auto& piece : pieces)
        {
            if (piece->isCaptured) continue;
            if (team != PlayerSide::Any && piece->team != team) continue;
            piece->computeMoves(moveList, onlyCaptures);
        }
    }

    PlayerSide GameBoardImpl::getActivePlayer() const
    {
        return (moveCounter & 1) ? PlayerSide::Black : PlayerSide::White;
    }

    int GameBoardImpl::getMoveCount() const
    {
        return moveCounter;
    }

    void GameBoardImpl::executeMove(std::shared_ptr<GameMove> move)
    {
        if (moveCounter != move->getMoveCount())
            return;

        for (const GridPoint& capturePos : move->getCapturePoints())
        {
            std::shared_ptr<GameEntity> capturedPiece = getPieceAt(capturePos);
            if (!capturedPiece) continue;

            if (capturedPiece->isCaptured)
            {
                throw std::logic_error("Failure attacking");
            }

            capturedPiece->isCaptured = true;
            capturedPiece->incrementMoveCount(moveCounter);
        }

        for (const auto& transformation : move->getTransformations())
        {
            std::shared_ptr<GameEntity> piece = getPieceAt(transformation.first);
            if (!piece)
            {
                throw std::logic_error("Type changing failure");
            }

            changeType(transformation.first, transformation.second.second);
            piece->incrementMoveCount(moveCounter);
        }

        for (const auto& step : move->getMoveSequence())
        {
            std::shared_ptr<GameEntity> piece = getPieceAt(step.first);
            if (!piece)
            {
                throw std::logic_error("Piece isn't found");
            }

            piece->pos = step.second;
            piece->incrementMoveCount(moveCounter);
        }

        moveCounter++;
    }

    void GameBoardImpl::undoMove(std::shared_ptr<GameMove> move)
    {
        if (moveCounter - 1 != move->getMoveCount())
            return;

        for (const auto& step : move->getMoveSequence())
        {
            std::shared_ptr<GameEntity> piece = getPieceAt(step.second);
            if (!piece)
            {
                throw std::logic_error("Piece isn't found");
            }

            if (piece->getLastMoveTick() != moveCounter - 1)
                continue;

            piece->pos = step.first;
            piece->decrementMoveCount();
        }

        for (const auto& transformation : move->getTransformations())
        {
            std::shared_ptr<GameEntity> piece = getPieceAt(transformation.first);
            if (!piece)
            {
                throw std::logic_error("Type changing failure");
            }

            if (piece->getLastMoveTick() != moveCounter - 1)
                continue;

            changeType(transformation.first, transformation.second.first);
            piece->decrementMoveCount();
        }

        for (const GridPoint& capturePos : move->getCapturePoints())
        {
            std::shared_ptr<GamePiece> capturedPiece = getMovablePiece(capturePos, moveCounter - 1, true);
            if (!capturedPiece) continue;

            if (capturedPiece->getLastMoveTick() != moveCounter - 1)
                continue;

            if (!capturedPiece->isCaptured)
            {
                throw std::logic_error("Failure attacking reset");
            }

            capturedPiece->isCaptured = false;
            capturedPiece->decrementMoveCount();
        }

        moveCounter--;
    }

    void GameBoardImpl::changeType(const GridPoint& pos, const PieceType type)
    {
        int pieceId = getPieceId(pos);
        if (pieceId == -1)
            throw std::logic_error("Type changing failure");

        std::shared_ptr<GamePiece>& piece = pieces[pieceId];
        std::shared_ptr<GamePiece> newPiece;

        switch (type)
        {
        case PieceType::Soldier:
            newPiece = std::make_shared<Soldier>(*piece);
            break;
        case PieceType::Tower:
            newPiece = std::make_shared<Tower>(*piece);
            break;
        case PieceType::Steed:
            newPiece = std::make_shared<Steed>(*piece);
            break;
        case PieceType::Diagonal:
            newPiece = std::make_shared<Diagonal>(*piece);
            break;
        case PieceType::Regent:
            newPiece = std::make_shared<Regent>(*piece);
            break;
        case PieceType::Sovereign:
            newPiece = std::make_shared<Sovereign>(*piece);
            break;
        default:
            throw std::logic_error("Type changing failure: wrong type");
        }

        pieces[pieceId] = newPiece;
    }

    void GameBoardImpl::initializePiece(GamePiece* piece)
    {
        pieces.push_back(std::shared_ptr<GamePiece>(piece));
    }
}