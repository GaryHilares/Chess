#include "../include/gameUI.hpp"

bool gameUI::existInterrumptions(square source, square destiny) const
{
    if(source.getCol() == destiny.getCol())
    {
        short int modifier = source.getRow() < destiny.getRow() ? 1:-1;
        for(short int i = source.getRow(); i != destiny.getRow(); i += modifier)
            if(this->readBoard(source.getCol(),i) != nullptr)
                return true;
        return false;
    }
    else if(source.getRow() == destiny.getRow())
    {
        short int modifier = source.getCol() < destiny.getCol() ? 1:-1;
        for(short int i = source.getCol(); i != destiny.getCol(); i += modifier)
            if(this->game.board[i*8+source.getRow()] != nullptr)
                return true;
        return false;
    }
    else if(abs(source.getRow()-destiny.getRow()) == abs(source.getCol()-destiny.getCol()))
    {
        short int modifierX = source.getCol() < destiny.getCol() ? 1:-1;
        short int modifierY = source.getRow() < destiny.getRow() ? 1:-1;
        for(short int i = source.getCol(), j = source.getRow(); i != destiny.getCol() || j != destiny.getRow(); i += modifierX, j += modifierY)
            if(this->game.board[i*8+j] != nullptr)
                return true;
        return false;
    }
    else
    {
        return true;
    }
}

piece* gameUI::readBoard(short int col, short int row) const
{
    if(col > 7 || col < 0 || row > 7 || row < 0)
        return nullptr;
    return this->game.board[col*8+row];
}

piece* gameUI::readBoard(square target) const
{
    return this->readBoard(target.getCol(),target.getRow());
}

bool gameUI::isLegalMove(const piece moving, const square source, const square destiny) const
{
    if(moving.color != this->game.turnColor)
        return false;
    if(source == destiny ||
       (this->readBoard(destiny.getCol(),destiny.getRow()) != nullptr && moving.color == this->readBoard(destiny.getCol(),destiny.getRow())->color))
        return false;
    const short int movedX = abs(source.getCol() - destiny.getCol());
    const short int movedY = abs(source.getRow() - destiny.getRow());
    const square kingPosition = [&]()
    {
        for(short int i = 1; i <= 8; i++)
        {
            for(short int j = 1; j <= 8; j++)
            {
                square cur(static_cast<square::column>(i),j);
                if(this->readBoard(cur) != nullptr && this->readBoard(cur)->type == piece::King && this->readBoard(cur)->color == moving.color)
                    return cur;
            }
        }
        return destiny;
    }();
    piece::Color oposingColor = moving.color == piece::Black ? piece::White:piece::Black;
    for(std::pair<short int,short int> knightPlaces: std::initializer_list<std::pair<short int, short int>>{{-2,-1},{-1,-2},{-1,2},{2,-1},{1,2},{2,1},{1,-2},{-2,1}})
    {
        if(this->readBoard(kingPosition.getCol()+knightPlaces.first,kingPosition.getRow()+knightPlaces.second) != nullptr && this->readBoard(kingPosition.getCol()+knightPlaces.first,kingPosition.getRow()+knightPlaces.second)->type == piece::Knight && this->readBoard((kingPosition.getCol()+knightPlaces.first),kingPosition.getRow()+knightPlaces.second)->color == oposingColor && !(destiny.getCol() == kingPosition.getCol()+knightPlaces.first && destiny.getRow() == kingPosition.getRow()+knightPlaces.second))
        {
            return false;
        }
    }
    for(std::pair<short int, short int> modifiers: std::initializer_list<std::pair<short int, short int>>{{-1,-1},{1,-1},{-1,1},{1,1},{0,1},{0,-1},{1,0},{-1,0}})
    {
        for(int x = kingPosition.getCol() + modifiers.first, y = kingPosition.getRow() + modifiers.second; x < 8 && x >= 0 && y < 8 && y >= 0; x += modifiers.first, y += modifiers.second)
        {
            if(this->readBoard(x,y) != nullptr)
            {
                if(this->readBoard(x,y)->color == oposingColor && !(destiny.getCol() == x && destiny.getRow() == y) && (this->readBoard(x,y)->type == piece::Queen || ((abs(modifiers.first) != abs(modifiers.second) && this->readBoard(x,y)->type == piece::Rook) || (abs(modifiers.first) == abs(modifiers.second) && this->readBoard(x,y)->type == piece::Bishop))))
                    return false;
                else
                    break;
            }
            else if(destiny.getCol() == x && destiny.getRow() == y)
            {
                break;
            }
        }
    }
    for(int i: {-1,1})
        if(this->readBoard(kingPosition.getCol()+i,kingPosition.getRow() + (oposingColor == piece::White ? 1:-1)) != nullptr && this->readBoard(kingPosition.getCol()+i,kingPosition.getRow() + (oposingColor == piece::White ? 1:-1))->color == oposingColor && this->readBoard(kingPosition.getCol()+i,kingPosition.getRow() + (oposingColor == piece::White ? 1:-1))->type == piece::Pawn)
            return false;

    switch(moving.type)
    {
    case piece::King:
        if(existInterrumptions(source,destiny))
            return false;
        if(destiny.getCol() == square::columnToInt(square::column::g) && game.board[square::columnToInt(square::column::h)*8+source.getRow()] != nullptr && !game.board[square::columnToInt(square::column::h)*8+source.getRow()]->moved && movedY == 0 && !moving.moved && isLegalMove(moving,source,square(square::column::f,moving.color == piece::White ? 1:8)))
            return true;
        if(destiny.getCol() == square::columnToInt(square::column::c) && game.board[square::columnToInt(square::column::a)*8+source.getRow()] != nullptr && !game.board[square::columnToInt(square::column::a)*8+source.getRow()]->moved && movedY == 0 && !moving.moved && isLegalMove(moving,source,square(square::column::d,moving.color == piece::White ? 1:8)))
            return true;
        if(movedX > 1 || movedY > 1)
            return false;
        return true;
    case piece::Queen:
        if(existInterrumptions(source,destiny))
            return false;
        return true;
    case piece::Bishop:
        if(movedX != movedY)
            return false;
        if(existInterrumptions(source,destiny))
            return false;
        return true;
    case piece::Knight:
        if(!((movedX == 2 && movedY == 1) || (movedX == 1 && movedY == 2)))
            return false;
        return true;
    case piece::Rook:
        if(movedX != 0 && movedY != 0)
            return false;
        if(existInterrumptions(source,destiny))
            return false;
        return true;
    case piece::Pawn:
        if((moving.color == piece::White && square::rowToInt(source.getRow()) > square::rowToInt(destiny.getRow())) || (moving.color == piece::Black && square::rowToInt(source.getRow()) < square::rowToInt(destiny.getRow())))
            return false;
        if(existInterrumptions(source,destiny))
            return false;
        if(((moving.color == piece::White && source.getRow() == square::rowToInt(2)) || (moving.color == piece::Black && source.getRow() == square::rowToInt(7)))
           && movedX == 0 && movedY == 2 && this->game.board[destiny.getCol()*8+destiny.getRow()] == nullptr)
            return true;
        else if(movedX == 1 && movedY == 1 && (this->game.board[destiny.getCol()*8+destiny.getRow()] != nullptr ||
                                                (source.getRow() == square::rowToInt(5) && moving.color == piece::White && this->game.board[destiny.getCol()*8+destiny.getRow()+1] != nullptr && this->game.board[destiny.getCol()*8+destiny.getRow()+1]->type == piece::Pawn && this->game.board[destiny.getCol()*8+destiny.getRow()+1] == this->game.pawnDoubleMovedLastTurn) ||
                                                (source.getRow() == square::rowToInt(4) && moving.color == piece::Black && this->game.board[destiny.getCol()*8+destiny.getRow()-1] != nullptr && this->game.board[destiny.getCol()*8+destiny.getRow()-1]->type == piece::Pawn && this->game.board[destiny.getCol()*8+destiny.getRow()-1] == this->game.pawnDoubleMovedLastTurn)))
            return true;
        else if(movedX != 0 || movedY != 1)
            return false;
        return true;
    default:
        return false;
    }
}

void gameUI::update(sf::RenderWindow& window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if((sf::Mouse::isButtonPressed(sf::Mouse::Left) && movingPiece == nullptr))
    {
        this->lastSquare = new square(getSquare(mousePos.x,mousePos.y));
        std::swap(this->movingPiece,this->game.board[this->lastSquare->getCol()*8+this->lastSquare->getRow()]);
    }
    else if((!sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->movingPiece != nullptr))
    {
        square pressedSquare = getSquare(mousePos.x,mousePos.y);
        if(this->isLegalMove(*this->movingPiece,*this->lastSquare,pressedSquare))
        {
            this->game.pawnDoubleMovedLastTurn = nullptr;
            this->movingPiece->moved = true;
            if(this->movingPiece->type == piece::Pawn && this->lastSquare->getCol() != pressedSquare.getCol() && this->game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow()] == nullptr)
            {
                if(this->movingPiece->color == piece::White && this->game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow()+1] != nullptr)
                {
                    delete this->game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow()+1];
                    this->game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow()+1] = nullptr;
                }
                else if(this->movingPiece->color == piece::Black && this->game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow()-1] != nullptr)
                {
                    delete this->game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow()-1];
                    this->game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow()-1] = nullptr;
                }
            }
            if(this->game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow()] != nullptr)
                delete this->game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow()];
            if(this->movingPiece->type == piece::King && this->lastSquare->getCol() == square::columnToInt(square::column::e) && pressedSquare.getCol() == square::columnToInt(square::column::c))
            {
                this->game.board[square::columnToInt(square::column::a) * 8 + this->lastSquare->getRow()]->moved = true;
                std::swap(this->game.board[square::columnToInt(square::column::a) * 8 + this->lastSquare->getRow()],this->game.board[square::columnToInt(square::column::d) * 8 + this->lastSquare->getRow()]);

            }
            else if(this->movingPiece->type == piece::King && this->lastSquare->getCol() == square::columnToInt(square::column::e) && pressedSquare.getCol() == square::columnToInt(square::column::g))
            {
                this->game.board[square::columnToInt(square::column::h) * 8 + this->lastSquare->getRow()]->moved = true;
                std::swap(this->game.board[square::columnToInt(square::column::h) * 8 + this->lastSquare->getRow()],this->game.board[square::columnToInt(square::column::f) * 8 + this->lastSquare->getRow()]);
            }
            else if(this->movingPiece->type == piece::Pawn && pressedSquare.getRow() == square::rowToInt(this->movingPiece->color == piece::White ? 8:1))
            {
                piece::Type toPromote;
                while(true)
                {
                    sf::Event event;
                    if(window.pollEvent(event))
                    {
                        switch(event.type)
                        {
                        case sf::Event::Closed:
                            window.close();
                            exit(0);
                        default:
                            break;
                        }
                    }
                    window.clear(movingPiece->color == piece::White ? sf::Color::White:sf::Color(100,100,100));
                    sf::Transformable transformer;
                    sf::RenderStates specificState;
                    transformer.setPosition(100,350);
                    for(piece::Type i: {piece::Queen,piece::Rook,piece::Bishop,piece::Knight})
                    {
                        transformer.move(100,0);
                        specificState.transform = transformer.getTransform();
                        window.draw(this->piecesSprites[movingPiece->color*6+i],specificState);
                    }
                    window.display();
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        sf::Vector2f newMousePosF = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        sf::Vector2i newMousePos(newMousePosF.x,newMousePosF.y);
                        if(newMousePos.y >= 350 && newMousePos.y <= 450 && newMousePos.x >= 200 && newMousePos.x <= 600)
                        {
                            switch((newMousePos.x-200)/100)
                            {
                            case 0:
                                toPromote = piece::Queen;
                                break;
                            case 1:
                                toPromote = piece::Rook;
                                break;
                            case 2:
                                toPromote = piece::Bishop;
                                break;
                            case 3:
                                toPromote = piece::Knight;
                                break;
                            case 4:
                                throw std::out_of_range("Out of promotable pieces");
                            }
                            break;
                        }
                    }
                }
                this->movingPiece->type = toPromote;
            }
            else if(this->movingPiece->type == piece::Pawn && abs(this->lastSquare->getRow() - pressedSquare.getRow()) == 2)
            {
                this->game.pawnDoubleMovedLastTurn = this->movingPiece;
            }
            this->game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow()] = this->movingPiece;
            this->movingPiece = nullptr;
            delete this->lastSquare;
            this->lastSquare = nullptr;
            if(this->game.turnColor == piece::White)
                this->game.turnColor = piece::Black;
            else
                this->game.turnColor = piece::White;
        }
        else
        {
            std::swap(this->movingPiece, this->game.board[this->lastSquare->getCol() * 8 + this->lastSquare->getRow()]);
            delete this->lastSquare;
            this->lastSquare = nullptr;
        }
    }
}

piece*& gameUI::accessBoard(short int col, short int row)
{
    if(col > 7 || col < 0 || row > 7 || row < 0)
        throw std::out_of_range("Out of board's range.");
    return this->game.board[col*8+row];
}

void gameUI::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    {
        bool rectangleIsWhite = false;
        sf::RectangleShape shape(sf::Vector2f(squareSize,squareSize));
        for(unsigned int i = boardPosX; i < boardPosX+squareSize*8; i += squareSize)
        {
            for(unsigned int j = boardPosY; j < boardPosY+squareSize*8; j += squareSize)
            {
                shape.setPosition(i,j);
                if(rectangleIsWhite)
                {
                    shape.setFillColor(sf::Color(100,100,100,255));
                    rectangleIsWhite = false;
                }
                else
                {
                    shape.setFillColor(sf::Color::White);
                    rectangleIsWhite = true;
                }
                target.draw(shape);
            }
            rectangleIsWhite = !rectangleIsWhite;
        }
    }
    {
        for(unsigned int i = 0; i < 8; i++)
        {
            for(unsigned int j = 0; j < 8; j++)
            {
                if(this->game.board[i*8+j] != nullptr)
                {
                    const sf::Sprite& currentPieceSprite = this->piecesSprites[this->game.board[i*8+j]->color*6+this->game.board[i*8+j]->type];
                    sf::Transformable transformer;
                    sf::RenderStates specificState = state;
                    transformer.setPosition(boardPosX+i*squareSize,boardPosY+j*squareSize);
                    specificState.transform *= transformer.getTransform();
                    target.draw(currentPieceSprite,specificState);
                }
            }
        }
        if(this->movingPiece != nullptr)
        {
            sf::Window* window = dynamic_cast<sf::Window*>(&target);
            if(window != nullptr)
            {
                const sf::Sprite& currentPieceSprite = this->piecesSprites[this->movingPiece->color*6+this->movingPiece->type];
                sf::Transformable transformer;
                sf::RenderStates specificState;
                sf::Vector2f mousePos = target.mapPixelToCoords(sf::Mouse::getPosition(*window));
                sf::FloatRect spriteRectangle = currentPieceSprite.getGlobalBounds();
                transformer.setPosition(mousePos.x-boardPosX-spriteRectangle.width/2,mousePos.y - boardPosY-spriteRectangle.height/2);
                specificState.transform = transformer.getTransform();
                target.draw(currentPieceSprite,specificState);
            }
        }
    }
}

gameUI::~gameUI()
{
    if(this->movingPiece != nullptr)
        delete movingPiece;
    if(this->lastSquare != nullptr)
        delete lastSquare;
}

square gameUI::getSquare(float posX, float posY)
{
    posX = (posX - boardPosX)/squareSize;
    posY = 8 - ((posY-boardPosY)/squareSize);
    int newPosX = posX + 1;
    int newPosY = posY + 1;
    return square(static_cast<square::column>(newPosX),newPosY);
}

gameUI::gameUI(gameController& _game): game(_game), movingPiece(nullptr), lastSquare(nullptr)
{
    if(!this->piecesTexture.loadFromFile("ChessPieces.png"))
        exit(1);
    for(unsigned int i = 0; i < 2; i++)
    {
        for(unsigned int j = 0; j < 6; j++)
        {
            this->piecesSprites[i*6+j].setTexture(piecesTexture);
            this->piecesSprites[i*6+j].setTextureRect(sf::IntRect(j*pieceSizeInTexture,i*pieceSizeInTexture,pieceSizeInTexture,pieceSizeInTexture));
            this->piecesSprites[i*6+j].setScale(float(squareSize)/pieceSizeInTexture,float(squareSize)/pieceSizeInTexture);
        }
    }
}

piece*& gameUI::accessBoard(square target)
{
    return this->accessBoard(target.getCol(),target.getRow());
}
