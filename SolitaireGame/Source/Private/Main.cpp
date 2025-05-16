#include "SolitaireGamePCH.h"
#include "Card/Card.h"
#include "Framework/World.h"
int main()
{
    SSolitaireEngine Engine;
    SSharedPtr<SWorld> World = std::make_shared<SWorld>();

    SACard* Card = new SACard(SGridPositionU32(68, 24), FCardInfo(ECardRank::Jack, ECardSuit::Spades, false));
    World->AddActor(Card);

    Engine.Initialize(World);
    Engine.Run();
    
    delete Card;
    return 0;
}