#include <iostream>
#include <map>
#include "BaseChess/Chess.hpp"

using namespace std;

void drawBoard(Chess::GameState& game)
{
    map<int, string> pieceSymbols = {
        {static_cast<int>(Chess::PieceType::Soldier), "P "},
        {static_cast<int>(Chess::PieceType::Tower), "R "},
        {static_cast<int>(Chess::PieceType::Steed), "Kn"},
        {static_cast<int>(Chess::PieceType::Diagonal), "B "},
        {static_cast<int>(Chess::PieceType::Regent), "Q "},
        {static_cast<int>(Chess::PieceType::Sovereign), "K "}
    };
    
    system("cls");

    std::cout << "  ";
    for (int i = 0; i < 8; ++i)
    {
        std::cout << ' ' << i << ' ';
    }
    std::cout << "\t Active Player: "
              << ((game.getGameBoard()->getActivePlayer() == Chess::PlayerSide::Black) ? "Black" : "White")
              << "\n\n";

    for (int i = 0; i < 8; ++i)
    {
        std::cout << i << " ";
        for (int j = 0; j < 8; ++j)
        {
            const std::shared_ptr<Chess::GameEntity> piece = 
                game.getPieceAt(Chess::GridPoint(j, i));
            
            if (piece != nullptr)
            {
                std::cout << ((piece->team == Chess::PlayerSide::Black) ? " \x1B[37m" : " \033[2;47;30m")
                          << pieceSymbols[static_cast<int>(piece->type)]
                          << "\033[0m";
            }
            else
            {
                std::cout << " - ";
            }
        }
        std::cout << "\n";
    }
}

int main()
{
    Chess::PieceType (*getPromotionTypeWhite)() = []() -> Chess::PieceType { 
        return Chess::PieceType::Regent; 
    };
    Chess::PieceType (*getPromotionTypeBlack)() = []() -> Chess::PieceType { 
        return Chess::PieceType::Regent; 
    };

    Chess::GameState game(getPromotionTypeWhite, getPromotionTypeBlack);
    
    while (game.getGameResult() == Chess::PlayerSide::None)
    {
        drawBoard(game);
        int x1, y1, x2, y2;
        std::cout << "\nEnter a move (x1, y1, x2, y2)\n";
        std::cin >> x1 >> y1 >> x2 >> y2;
        
        Chess::GridPoint startPos(x1, y1);
        Chess::GridPoint endPos(x2, y2);
        game.executeMove(startPos, endPos);
    }
    
    drawBoard(game);

    std::cout << "\nWinner: "
              << ((game.getGameResult() == Chess::PlayerSide::Black) ? "Black" : "White")
              << "\n";

    return 0;
}