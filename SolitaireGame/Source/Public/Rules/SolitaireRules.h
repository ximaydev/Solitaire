#pragma once
#include "Globals.h"

/** Forward Declarations */
class SACard;

class SSolitaireRules
{
public:
    /** Constructor */
    SSolitaireRules();

    /** Move card(s) from Stock Pile to Foundation Lists */
    void MoveStockToFoundation(const SACard& StockCard, const SACard& FoundationCard);

    /** Move card(s) from Stock Pile to Board Lists */
    void MoveStockToBoard(const SACard& StockCard, const SACard& BoardCard);

    /** Move card(s) within Board Lists */
    void MoveBoardToBoard(const SACard& BoardCard1, const SACard& BoardCard2);

    /** Move card(s) from Board Lists to Foundation Lists */
    void MoveBoardToFoundation(const SACard& BoardCard, const SACard& FoundationCard);

    /** Number of cards to reveal each time Stock Pile is used */
    const SUInt8 CardsToRevealPerStockUse = {};
};