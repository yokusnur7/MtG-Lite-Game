#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>


// CARD CLASS START
class Card {
protected:
    std::string name;
    int manaCost;
    std::string color;
    bool isDiscarded;
    std::string typeOfCard;
public:
    // yeni typeOfCard
    Card(std::string nm, int mnCt, std::string clr, std::string typeOfCard) : name(nm), manaCost(mnCt),
                                                                              color(clr), typeOfCard(typeOfCard) {
        isDiscarded = false;
    }

    Card() : name(""), manaCost(0), color(""), isDiscarded(false), typeOfCard("") {}

    Card(const Card &a) {
        name = a.name;
        manaCost = a.manaCost;
        color = a.color;
        isDiscarded = a.isDiscarded;
        typeOfCard = a.typeOfCard;
    }


    void isStillOperational() {
        if (!isDiscarded) {
            std::cout << name << ": is still operational" << std::endl;
        } else {
            std::cout << name << ": is destroyed" << std::endl;
        }
    }

    void setName(std::string &name) {
        this->name = name;
    }

    std::string getName() {
        return name;
    }

    void setManaCost(int b) {
        manaCost = b;
    }

    int getManaCost() {
        return manaCost;
    }

    void setColor(std::string c) {
        color = c;
    }

    std::string getColor() {
        return color;
    }

    std::string getTypeofCard() {
        return typeOfCard;
    }

    ~Card() = default;
};

void randomizeCardInLibrary(std::vector<Card> &a, std::vector<Card> &b) {
    for (int i = 0; i < a.size(); i++) {
        int j = i + rand() % (a.size() - i);
        std::swap(a[i], a[j]);
    }
    for (int i = 0; i < b.size(); i++) {
        int j = i + rand() % (b.size() - i);
        std::swap(b[i], b[j]);
    }
    std::cout << "Cards in library are randomized." << std::endl;
}


class creatureCard : public Card {
protected:
    int hP;
    int attackPower;
    bool isTapped;
    bool hasFirstStr;
    bool hasTrmb;

public:
    creatureCard(std::string nam, int mC, std::string cl, int ap, int hp, bool fstStk, bool trmb) :Card(nam, mC, cl, "Creature") {

        if (hp >= 0)
            hP = hp;
        else
            hP = 0;
        if (ap >= 0)
            attackPower = ap;
        else
            attackPower = 0;

        hasFirstStr = fstStk;
        hasTrmb = trmb;
        isTapped = true;
    }

    creatureCard() : Card(), hP(0), attackPower(0), isTapped(true), hasFirstStr(false), hasTrmb(false) {}

    creatureCard(Card &card): Card(card.getName(), card.getManaCost(), card.getColor(), "Creature") {}

    creatureCard(const creatureCard &c): Card(c){
        hasFirstStr = c.hasFirstStr;
        hasTrmb = c.hasTrmb;
        isTapped = c.isTapped;
        attackPower = c.attackPower;
        hP = c.hP;
    }

    void attack() {
        tapTheCard();
    }

    void tapTheCard() {
        isTapped = true;
    }

    int getHp() {
        return hP;
    }

    int getAttackPower() {
        return attackPower;
    }

    bool getIisTapped1() {
        return isTapped;
    }

    bool getIsHasFirstStr() {
        return hasFirstStr;
    }

    bool getIsHasTrmb() {
        return hasTrmb;
    }

    void setHp(int hP) {
        this->hP = hP;
    }

    void setAttackPower(int attackPower) {
        this->attackPower = attackPower;
    }

    void setIsTapped(bool isTapped) {
        this->isTapped = isTapped;
    }

    void setHasFirstStr(bool hasFirstStr) {
        this->hasFirstStr = hasFirstStr;
    }

    void setHasTrmb(bool hasTrmb) {
        this->hasTrmb = hasTrmb;
    }
};



class Player {
protected:
    bool isAlive;
    int currentHp;
    int baseHp;
    std::vector<Card> inplayCards;
    std::vector<Card> handPile;
    std::vector<Card> deckPile; //library for every player
    std::vector<Card> playableCards;
    std::vector<Card> disCards;
    int landPower; // new
    bool isLandCardPlayed;
    int landPowerDecreaser;

public:

    Player(std::vector<Card> &dckPile): currentHp(15), baseHp(15), landPower(0) {

        for (int i = 0; i < dckPile.size(); i++) {
            deckPile.push_back(dckPile[i]);
        }
        isAlive = true;
        isLandCardPlayed = false;
        assign5Cards();
        setPlayableCards();
        landPowerDecreaser = landPower;
    }

    Player() : currentHp(0), baseHp(0), landPower(0), landPowerDecreaser(0) {
        isAlive = true;
        isLandCardPlayed = false;
    }

    Player(const Player &p) : isAlive(p.isAlive), currentHp(p.currentHp), landPower(p.landPower), isLandCardPlayed(p.isLandCardPlayed) {
        baseHp = 15;
        landPowerDecreaser = p.landPowerDecreaser;
        for (auto &inplayCard : p.inplayCards) {
            this->inplayCards.push_back(inplayCard);
        }

        for (auto &handCard : p.handPile) {
            this->handPile.push_back(handCard);
        }

        for (auto &deckCard : p.deckPile) {
            this->deckPile.push_back(deckCard);
        }

        for (auto &playableCard : p.playableCards) {
            this->playableCards.push_back(playableCard);
        }

        for (auto & disCard : p.disCards) {
            this->disCards.push_back(disCard);
        }
    }

    std::vector<Card> &getDisCards() {
        return disCards;
    }

    void getACardFromDiscardToHand(std::string name) {
        for (int i = 0; i < disCards.size(); i++) {
            if (disCards[i].getName() == name){
                handPile.push_back(disCards[i]); // move it into hand pile
                disCards.erase(disCards.begin()+i);  // remove a card from deck pile
                break;
            }
        }
        setHandPile(handPile);
        setDisCards(disCards);
    }

    void discardTheCard(int indexAtInplayVector) {

        disCards.push_back(inplayCards[indexAtInplayVector]);
        std::vector<Card>::iterator it;

        for (it = inplayCards.begin(); it < inplayCards.end(); it++) {
            if (it->getName() == disCards[indexAtInplayVector].getName()) {
                inplayCards.erase(it);
                break;
            }
        }
        setInplayCards(inplayCards);
        setDisCards(disCards);
    }


    void setDisCards(std::vector<Card> &dsCrds) {
        disCards = dsCrds;
    }


    void setInplayCards(std::vector<Card> &iP) {
        inplayCards = iP;
    }


    std::vector<Card> creatureCardsInplay() {

        std::vector<Card> creatureCardInplay;
        for (int i = 0; i < inplayCards.size(); i++) {
            if (inplayCards[i].getTypeofCard() == "Creature") {
                creatureCardInplay.push_back(inplayCards[i]);
            }
        }

        return creatureCardInplay;
    }

    void printCreatureCardsInplay(std::vector<Card> tmp) {
        for (int i = 0; i < tmp.size(); i++) {
            if (tmp[i].getTypeofCard() == "Creature") {
                std::cout << tmp[i].getName() << " ";
            }
        }
        std::cout << std::endl;
    }

    int getLandPower() {
        return landPower;
    }

    void setLandPower(int lP) {
        landPower = lP;
    }

    int getLandPowerDecreaser() {
        return landPowerDecreaser;
    }

    void setLandPowerDecreaser() {
        landPowerDecreaser = landPower;
    }

    void decreaseLandPowerDecreaser(int manaCost) {
        landPowerDecreaser -= manaCost;
    }

    void increaseLandPowerDecreaser() {
        landPowerDecreaser++;
    }

    void setIsLandCardPlayed(bool played) {
        isLandCardPlayed = played;
    }

    std::vector<Card> &getHandPile() {
        return handPile;
    }

    void setHandPile(std::vector<Card> &hndPl) {
        Player::handPile = hndPl;
    }

    void setCurrentHp(int cH) {
        currentHp = cH;
    }

    void dealDamageToThisPlayer(int damageValue) {
        currentHp -= damageValue;
    }

    void addToInplayCardsAndRemoveFromHand(int indexAtPlayableVector) {

        inplayCards.push_back(playableCards[indexAtPlayableVector]);

        if (playableCards[indexAtPlayableVector].getTypeofCard() == "Land") {
            landPower++;
            increaseLandPowerDecreaser();
        }

        std::vector<Card>::iterator it;

        for (it = handPile.begin(); it < handPile.end(); it++) {
            if (it->getName() == playableCards[indexAtPlayableVector].getName()) {
                handPile.erase(it);
                break;
            }
        }

        setHandPile(handPile);
        setPlayableCards();
    }

    std::string getDefenderCreatureCardName() {
        std::cout << "With which card would you like to defend yourself?" << std::endl;
        printCreatureCardsInInPlay();
        std::string answer;
        std::cin >> answer;
        return answer;
    }

    void printCreatureCardsInInPlay() {
        std::vector<Card>::iterator it;
        for (it = inplayCards.begin(); it < inplayCards.end(); it++) { // traverse inplayCards
            if (it->getTypeofCard() == "Creature") {
                std::cout << it->getName() << " ";
            }
        }
        std::cout << std::endl;
    }

    Card &getCreatureCardFromInInPlay(std::string wantedCardName) {
        std::vector<Card>::iterator it;
        for (it = inplayCards.begin(); it < inplayCards.end(); it++) { // traverse inplayCards
            if (it->getName() == wantedCardName) {
                break;
            }
        }
        return (*it); // it for Card class, want to return creatureCard
    }

    void destroyCardFromInplay(std::string cardName) {
        std::vector<Card>::iterator it;
        for (it = inplayCards.begin(); it < inplayCards.end(); it++) { // traverse inplayCards
            if (it->getName() == cardName) {
//                inplayCard // remove card from inplay
                // insert into discard pile
            }
        }
    }

    std::vector<Card> &getPlayableCards() {
        setPlayableCards();
        return playableCards;
    }

    void assign5Cards() {
        for (int i = 0; i < 5; i++) {
            handPile.push_back(deckPile[i]);
        }
        for (int i = 0; i < 5; i++) {
            deckPile.erase(deckPile.begin());
        }
    }

    bool getIsAlive() {
        return isAlive;
    }

    void dead() {
        isAlive = false;
    }

    int getCurrentHp() {
        return currentHp;
    }

    int getBaseHp() {
        return baseHp;
    }

    void setIsAlive(bool isAlive) {
        Player::isAlive = isAlive;
    }

    std::vector<Card> &getInplayCards() {
        return inplayCards;
    }

    void printHandPile() {
        for (int i = 0; i < handPile.size(); i++) {
            if (i < handPile.size() - 1)
                std::cout << handPile[i].getName() << ", ";
            else
                std::cout << handPile[i].getName() << ".";
        }
        std::cout << std::endl;
    }

    std::vector<Card> &getDeckPile() {
        return deckPile;
    }

    bool getIsLandCardPlayed() {
        return isLandCardPlayed;
    }

    void landCardPlayed() {
        isLandCardPlayed = true;
    }

    void addCardIntoHandPile() {
        if (handPile.size() < 7) {
            handPile.push_back(deckPile[0]); // move it into hand pile
            deckPile.erase(deckPile.begin()+0);  // remove a card from deck pile
        }
    }

    void setPlayableCards() {

        playableCards.clear();
        creatureCard c;

        for (int i = 0; i < handPile.size(); i++) { // traverse hand list
            if (handPile[i].getManaCost() <= landPowerDecreaser) { // cards that i can play
                playableCards.push_back(handPile[i]);
            }
        }
        for (int i = 0; i < playableCards.size(); i++) {
            if (isLandCardPlayed) {
                if (playableCards[i].getTypeofCard() == "Land") {
                    playableCards.erase(playableCards.begin() + i);
                    i--;
                }
            }
        }
    }

    void printIfPlayableCards() {

        setPlayableCards();
        auto it = playableCards.begin();

        if (it != playableCards.end()) {
            std::cout << "The cards you can play:" << std::endl;

            for (it = playableCards.begin(); it < playableCards.end(); it++) { // traverse hand list
                std::cout << it->getName() << " ";
            }
            std::cout << std::endl;
        }
        else {
            std::cout << "No cards can be played!!!";
        }
    }

    ~Player() {
        std::cout << "The end!" << std::endl;
    }

};






class Soldier : public creatureCard {
public:
    Soldier() : creatureCard("Soldier", 1, "White", 1, 1, false, false) {}
};

class ArmoredPegasus : public creatureCard {
public:
    ArmoredPegasus() : creatureCard("ArmoredPegasus", 2, "White", 1, 2, false, false) {}
};

class WhiteKnight : public creatureCard {
public:
    WhiteKnight() : creatureCard("WhiteKnight", 2, "White", 2, 2, true, false) {}
};

class AngryBear : public creatureCard {
public:
    AngryBear() : creatureCard("AngryBear", 3, "Green", 3, 2, false, true) {}
};

class Guard : public creatureCard {
public:
    Guard() : creatureCard("Guard", 4, "White", 2, 5, false, false) {}
};

class Werewolf : public creatureCard {  //TRAMPLE
public:
    Werewolf() : creatureCard("Werewolf", 4, "Green", 4, 6, false, true) {}
};

class Skeleton : public creatureCard {
public:
    Skeleton() : creatureCard("Skeleton", 1, "Black", 1, 1, false, false) {}
};

class Ghost : public creatureCard {
public:
    Ghost() : creatureCard("Ghost", 2, "Black", 2, 1, false, false) {}
};

class BlackKnight : public creatureCard {
public:
    BlackKnight() : creatureCard("BlackKnight", 2, "Black", 2, 2, true, false) {}
};

class OrcManiac : public creatureCard {
public:
    OrcManiac() : creatureCard("OrcManiac", 3, "Red", 4, 1, false, false) {}
};

class Hobgoblin : public creatureCard {
public:
    Hobgoblin() : creatureCard("Hobgoblin", 3, "Red", 3, 3, false, false) {}
};

class Vampire : public creatureCard {
public:
    Vampire() : creatureCard("Vampire", 4, "Black", 6, 3, false, false) {}
};


std::vector<creatureCard> creatureCards{Soldier(), ArmoredPegasus(), WhiteKnight(), AngryBear(), Guard(), Werewolf(),Skeleton(),Ghost(), BlackKnight(),OrcManiac(), Hobgoblin(), Vampire()};



class landCard : public Card {
protected:
    bool isTapped;
    char letter;

public:
    landCard(std::string nm, int mnCt, std::string clr) : Card(nm, mnCt, clr, "Land") {
        isTapped = false;
    }

    landCard(): isTapped(false), letter(' ') {}

    virtual void tapTheCard() {
        isTapped = true;
    }

    bool isTapped1() {
        return isTapped;
    }

    void setIsTapped(bool tap) {
        isTapped = tap;
    }

    char getLetter() {
        return letter;
    }

    std::string getTypeOfCard() {
        return typeOfCard;
    }

    char returnManaValue() {
        if (Card::name == "Forest")
            letter = 'G';

        else if (Card::name == "Island")
            letter = 'L';

        else if (Card::name == "Mountain")
            letter = 'R';

        else if (Card::name == "Plains")
            letter = 'W';

        else if (Card::name == "Swamp")
            letter = 'B';
        else
            letter = ' ';
        return letter;
    }
};


class Forest : public landCard {
public:
    Forest() : landCard("Forest", 0, "") {tapTheCard();}

    void tapTheCard() override {
        landCard::isTapped = true;
        landCard::letter = returnManaValue();
    }
};

class Island : public landCard {
public:
    Island() : landCard("Island", 0, "") {tapTheCard();}

    void tapTheCard() override {
        landCard::isTapped = true;
        landCard::letter = returnManaValue();
    }
};

class Mountain : public landCard {
public:
    Mountain() : landCard("Mountain", 0, "") {tapTheCard();}

    void tapTheCard() override {
        landCard::isTapped = true;
        landCard::letter = returnManaValue();
    }
};

class Plain : public landCard {
public:
    Plain() : landCard("Plain", 0, "") {tapTheCard();}

    void tapTheCard() override {
        landCard::isTapped = true;
        landCard::letter = returnManaValue();
    }
};

class Swamp : public landCard {
public:
    Swamp() : landCard("Swamp", 0, "") {tapTheCard();}

    void tapTheCard() override {
        landCard::isTapped = true;
        landCard::letter = returnManaValue();
    }
};

std::vector<landCard> landCards{Forest(), Island(), Mountain(), Plain(), Swamp()};



class SorceryCard : public Card {
public:
    SorceryCard(std::string nm, int mnCt, std::string clr) :Card(nm, mnCt, clr, "Sorcery") {}
    SorceryCard() : Card() {}
    SorceryCard(const SorceryCard& s) : Card(s) {
        color = s.color;
        isDiscarded = s.isDiscarded;
        manaCost = s.manaCost;
        name = s.name;
        typeOfCard = s.typeOfCard;
    }
};

class Disenchant : public SorceryCard {  //Destroy 1 Target Enchantment
public:
    Disenchant(std::string name, Player &p) : SorceryCard("Disenchant", 2, "White") {
        for (int i = 0; i < p.getInplayCards().size(); i++) {
            if ((p.getInplayCards()[i].getTypeofCard() == "Enchantment") && (p.getInplayCards()[i].getName() == name)){
                p.discardTheCard(i);
                std::cout << p.getInplayCards()[i].getName() << " card has been destroyed!!!" << std::endl;
                break;
            }
        }
    }

    Disenchant(): SorceryCard("Disenchant", 2, "White") {}
};

class LightningBolt : public SorceryCard {   //Deal 2 dmg to target creature or player
public:
    LightningBolt(Player &p) : SorceryCard("LightningBolt", 2, "Green") {
        p.setCurrentHp(p.getCurrentHp()-2);
    }

    LightningBolt(creatureCard &crtCrd) : SorceryCard("LightningBolt", 2, "Green") {
        crtCrd.setHp(crtCrd.getHp()-2);
        // SADECE 1 TURN ICIN OLCAK
    }

    LightningBolt(): SorceryCard("LightningBolt", 2, "Green") {}
};


class Flood : public SorceryCard {   //Destroy 1 Target Land
public:
    Flood(std::string name, Player &p) : SorceryCard("Flood", 3, "Green") {
        for (int i = 0; i < p.getInplayCards().size(); i++) {
            if ((p.getInplayCards()[i].getTypeofCard() == "Land") && (p.getInplayCards()[i].getName() == name)){
                p.discardTheCard(i);
                std::cout << p.getInplayCards()[i].getName() << " card has been destroyed!!!" << std::endl;
                break;
            }
        }
    }
    Flood(): SorceryCard("Flood", 3, "Green") {}
};


class Reanimate : public SorceryCard {  //Return 1 character card from discard to hand
public:
    Reanimate(std::string name, Player &p) : SorceryCard("Reanimate", 1, "Black") {
        p.getACardFromDiscardToHand(name);
        std::cout << name << " card is removed from discard pile to hand pile." << std::endl;
    }
    Reanimate(): SorceryCard("Reanimate", 1, "Black") {}
};


class Plague : public SorceryCard {  //Deal 1 dmg to all creatures
public:
    Plague(Player &player1, Player &player2) : SorceryCard("Plague", 3, "Black") {

        creatureCard c1, c2;
        for (int i = 0; i < player1.getInplayCards().size(); i++) {
            if (player1.getInplayCards()[i].getTypeofCard() == "Creature"){
                c1 = player1.getInplayCards()[i];

                c1.setHp(c1.getHp()-2);

            }
            if (player2.getInplayCards()[i].getTypeofCard() == "Creature"){
                c2 = player2.getInplayCards()[i];

                c2.setHp(c2.getHp()-2);

            }
        }
    }
    Plague(): SorceryCard("Plague", 3, "Black") {}
};


class Terror : public SorceryCard {   //Destroy 1 Target Creature
public:
    Terror(std::string name, Player &p) : SorceryCard("Terror", 2, "Black") {
        for (int i = 0; i < p.getInplayCards().size(); i++) {
            if ((p.getInplayCards()[i].getTypeofCard() == "Creature") && (p.getInplayCards()[i].getName() == name)){
                p.discardTheCard(i);
                std::cout << p.getInplayCards()[i].getName() << " card has been destroyed!!!" << std::endl;
                break;
            }
        }
    }
    Terror(): SorceryCard("Terror", 2, "Black") {}
};

std::vector<SorceryCard> sorceryCards{Disenchant(), LightningBolt(), Flood(), Reanimate(), Plague(), Terror()};


class EnchanmentCard : public Card {
protected:
    std::string ability;

public:
    EnchanmentCard(std::string nm, int mnCt, std::string clr) : Card(nm,mnCt,clr, "Enchanment") {
        ability = "";
    }
    EnchanmentCard(): Card(),ability("") {}
    EnchanmentCard(const EnchanmentCard& s) : Card(s) {
        ability = s.ability;
    }
};


class Rage : public EnchanmentCard {   // 1 Target creature gains Trample
protected:
    bool isDefended;

public:
    Rage(bool isDFD) : EnchanmentCard("Rage", 1, "Green"), isDefended(isDFD) {}
    Rage(): EnchanmentCard("Rage", 1, "Green"), isDefended(false) {}

    void use_Rage_card(Player &player, std::string creatureName) {
        for (auto & i : player.getInplayCards()) {
            if (i.getName() == creatureName){
                creatureCard cr = i;
                cr.setHasTrmb(true);
                break;
            }
        }
    }
};

class HolyWar : public EnchanmentCard {  // All white creatures gain +1/+1
public:
    HolyWar(Player &player1, Player &player2) : EnchanmentCard("HolyWar", 2, "White") {

        creatureCard c1, c2;
        for (int i = 0; i < player1.getInplayCards().size(); i++) {
            if ((player1.getInplayCards()[i].getTypeofCard() == "Creature") && (player1.getInplayCards()[i].getColor() == "White")){
                c1 = player1.getInplayCards()[i];
                c1.setHp(c1.getHp()+1);
                c1.setAttackPower(c1.getAttackPower()+1);
            }
            if ((player2.getInplayCards()[i].getTypeofCard() == "Creature") && (player2.getInplayCards()[i].getColor() == "White")){
                c2 = player2.getInplayCards()[i];
                c2.setHp(c2.getHp()+1);
                c2.setAttackPower(c2.getAttackPower()+1);
            }
        }
    }
    HolyWar(): EnchanmentCard("HolyWar", 2, "White") {}
};


class HolyLight : public EnchanmentCard {  // All black creatures gain -1/-1
public:
    HolyLight(Player &player1, Player &player2) : EnchanmentCard("HolyLight", 2, "White") {

        creatureCard c1, c2;
        for (int i = 0; i < player1.getInplayCards().size(); i++) {
            if ((player1.getInplayCards()[i].getTypeofCard() == "Creature") && (player1.getInplayCards()[i].getColor() == "Black")){
                c1 = player1.getInplayCards()[i];
                c1.setHp(c1.getHp()-1);
                c1.setAttackPower(c1.getAttackPower()-1);
            }
            if ((player2.getInplayCards()[i].getTypeofCard() == "Creature") && (player2.getInplayCards()[i].getColor() == "Black")){
                c2 = player2.getInplayCards()[i];
                c2.setHp(c2.getHp()-1);
                c2.setAttackPower(c2.getAttackPower()-1);
            }
        }
    }
    HolyLight(): EnchanmentCard("HolyLight", 2, "White") {}
};


class UnholyWar : public EnchanmentCard {  // All black creatures gain +2/+0
public:
    UnholyWar(Player &player1, Player &player2) : EnchanmentCard("UnholyWar", 2, "Black") {

        creatureCard c1, c2;
        for (int i = 0; i < player1.getInplayCards().size(); i++) {
            if ((player1.getInplayCards()[i].getTypeofCard() == "Creature") && (player1.getInplayCards()[i].getColor() == "Black")){
                c1 = player1.getInplayCards()[i];
                c1.setAttackPower(c1.getAttackPower()+2);
            }
            if ((player2.getInplayCards()[i].getTypeofCard() == "Creature") && (player2.getInplayCards()[i].getColor() == "Black")){
                c2 = player2.getInplayCards()[i];
                c2.setAttackPower(c2.getAttackPower()+2);
            }
        }
    }
    UnholyWar(): EnchanmentCard("UnholyWar", 2, "Black") {}
};


class Restrain : public EnchanmentCard {  //All green creatures lose Trample
public:
    Restrain(Player &player1, Player &player2) : EnchanmentCard("Restrain", 3, "Red") {
        creatureCard c1, c2;
        for (int i = 0; i < player1.getInplayCards().size(); i++) {
            if ((player1.getInplayCards()[i].getTypeofCard() == "Creature") && (player1.getInplayCards()[i].getColor() == "Green")){
                c1.setHasTrmb(false);
            }
            if ((player2.getInplayCards()[i].getTypeofCard() == "Creature") && (player2.getInplayCards()[i].getColor() == "Green")){
                c2.setHasTrmb(false);
            }
        }
    }
    Restrain(): EnchanmentCard("Restrain", 3, "Red") {}
};

class Slow : public EnchanmentCard {   //1 Target creature loses First Strike
public:
    Slow(Player &p, std::string name) : EnchanmentCard("Slow", 1, "Black") {
        creatureCard crt;
        for (auto & i : p.getInplayCards()) {
            if ((i.getTypeofCard() == "Creature") && (i.getName() == name)){
                crt = i;
                crt.setHasFirstStr(false);
                break;
            }
        }
    }
    Slow(): EnchanmentCard("Slow", 1, "Black") {}
};

std::vector<EnchanmentCard> enchanmentCards{Rage(), HolyWar(), HolyLight(), UnholyWar(), Restrain(), Slow()};


class Game {
protected:
    Player player1;
    Player player2;
    int roundNumber;
    int turnNumber;
    bool playingCreatureAttack;
    creatureCard creatureCardToAttack;
    std::vector<creatureCard> attackableCardsForPlayer1;
    std::vector<creatureCard> attackableCardsForPlayer2;

public:
    Game(Player &p1, Player &p2) : player1(p1), player2(p2) {
        roundNumber = 1;
        turnNumber = 1;
        playingCreatureAttack = false; // new
        creatureCardToAttack = creatureCard();
    }

    Game(const Game &g) {
        player1 = g.player1;
        player2 = g.player2;
        roundNumber = g.roundNumber;
        turnNumber = g.turnNumber;
        playingCreatureAttack = false; // new
        creatureCardToAttack = g.creatureCardToAttack;
        attackableCardsForPlayer1 = g.attackableCardsForPlayer1;
        attackableCardsForPlayer2 = g.attackableCardsForPlayer2;
    }

    void attackLogicFunction(){




    }

    void playGame() {
        while (player1.getIsAlive() && player2.getIsAlive()) { // represents a round

            if (turnNumber > 2) {
                turnNumber -= 2;
                roundNumber++;
            }
            if (roundNumber > 1) {
                player1.addCardIntoHandPile();
                player2.addCardIntoHandPile();
            }
            std::cout << "\n****-----****   ROUND " << roundNumber << "   ****-----****\n" << std::endl;
            if (turnNumber == 1) {

                std::vector<std::string> cardnamesPlayedThisTurn;
                std::vector<std::string> creatureCardnamesPlayedThisTurn;

                player1.setIsLandCardPlayed(false);
                player1.setLandPowerDecreaser();
                std::cout << "Player 1's turn!" << std::endl;
                std::cout << "Player 1 Hand:" << std::endl;
                player1.printHandPile();

                std::cout << "\nWould you like to play a card from your hand pile?(yes/no)" << std::endl;
                std::string answer;
                std::cin >> answer;

                while (answer == "yes") {

                    std::cout << "\nNumber of land cards you have: " << player1.getLandPower() << std::endl;
                    std::cout << "Number of land cards untapped: " << player1.getLandPowerDecreaser() << "\n"<< std::endl;

                    player1.printIfPlayableCards();

                    if (!player1.getPlayableCards().empty()) {

                        std::cout << "Enter the card name to play..." << std::endl;
                        std::string playingCard;
                        std::cin >> playingCard;

                        bool isPlayable = false;
                        int indexAtPlayableVector;
                        std::string cardType;

                        for (int i = 0; i < player1.getPlayableCards().size(); i++) {
                            if (playingCard == player1.getPlayableCards()[i].getName() && (player1.getPlayableCards()[i].getTypeofCard() == "Land")) {
                                player1.landCardPlayed();
                                isPlayable = true;
                            }
                            else if (playingCard == player1.getPlayableCards()[i].getName()) {
                                auto manaCost = player1.getPlayableCards()[i].getManaCost();
                                if (player1.getLandPowerDecreaser() >= manaCost) {
                                    player1.decreaseLandPowerDecreaser(manaCost);
                                    isPlayable = true;
                                }
                            }
                            if (isPlayable) {
                                indexAtPlayableVector = i;
                                break;
                            }
                        }
                        if (isPlayable){
                            player1.addToInplayCardsAndRemoveFromHand(indexAtPlayableVector);
                            std::cout << playingCard << " is sent to inplay card list." << std::endl;
                            cardnamesPlayedThisTurn.push_back(playingCard);
                            for (int i = 0; i < creatureCards.size(); i++) {
                                if (playingCard == creatureCards[i].getName()){
                                    creatureCardnamesPlayedThisTurn.push_back(playingCard);
                                }
                            }
                        }
                        else{
                            std::cout << "Wrong playable card name!!!" << std::endl;
                        }
                    }
                    std::cout << "\nWould you like to play a card from your hand pile?(yes/no)" << std::endl;
                    std::cin >> answer;
                }

                if (answer == "no") {
                    bool canAttack = false;
                    bool isAttacking = false;
                    std::string attack;
                    std::string attackCard;
                    std::vector<Card> attackingCards;
                    std::vector<Card> tempInplayCards;
                    int numOfCreatureCardChosen = 0;

                    attackingCards.clear();

                    for (int i = 0; i < creatureCardnamesPlayedThisTurn.size(); i++) {
                        for (int j = 0; j < creatureCards.size(); j++) {
                            if (creatureCardnamesPlayedThisTurn[i] == creatureCards[j].getName()){
                                attackableCardsForPlayer1.push_back(creatureCards[j]);
                            }
                        }
                    }

// Mistake code below because of the adding the same card once in every round !!! Correct one is upon.
//
//                    for (int i = 0; i < player1.getInplayCards().size(); i++) {
//                        if ((player1.getInplayCards()[i].getTypeofCard() == "Creature")) {
//                            for (int j = 0; j < creatureCards.size(); j++) {
//                                if(player1.getInplayCards()[i].getName() == creatureCards[j].getName()){
//                                    attackableCardsForPlayer1.push_back(creatureCards[j]);
//                                }
//                            }
//                        }
//                    }

                    for (int i = 0; i < attackableCardsForPlayer1.size(); i++) {
                        if (!attackableCardsForPlayer1[i].getIisTapped1()){
                            canAttack = true;
                            break;
                        }
                    }

                    if (canAttack) {

                        std::cout << "\nWould you like to attack? (yes/no)" << std::endl;
                        std::cin >> attack;

                        if (attack == "yes") {
                            isAttacking = true;

                            for (int j = 0; j < attackableCardsForPlayer1.size(); j++) {
                                if (!attackableCardsForPlayer1[j].getIisTapped1()){
                                    tempInplayCards.push_back(attackableCardsForPlayer1[j]);
                                }
                            }
                        }

                        if (isAttacking) {

                            while (true) {
                                if (attack == "no")
                                    break;

                                std::cout << "\nSelect one creature card from the list below!" << std::endl;
                                player1.printCreatureCardsInplay(tempInplayCards);
                                std::cin >> attackCard;



                                for (int i = 0; i < tempInplayCards.size(); i++) {
                                    if (attackCard == tempInplayCards[i].getName()) {
                                        attackingCards.push_back(tempInplayCards[i]);
                                        numOfCreatureCardChosen++;
                                        tempInplayCards.erase(tempInplayCards.begin() + i);
                                    }
                                }
                                if (numOfCreatureCardChosen < (tempInplayCards.size())){
                                    std::cout << "\nWould you like to use one more creature card? (yes/no)" << std::endl;
                                    std::cin >> attack;
                                }
                                else
                                    attack = "no";
                            }

                            bool canBeDefended = false;

                            for (int i = 0; i < player2.getInplayCards().size(); i++) {
                                if (!attackableCardsForPlayer2.empty()){
                                    canBeDefended = true;
                                    break;
                                }
                            }

                            std::string askDeffance;

                            if (canBeDefended){
                                std::cout << "\n-Player 2-" << std::endl;
                                std::cout << "\nWould you like to defend with inplay cards? (yes/no)" << std::endl;
                                std::cin >> askDeffance;

                                std::string deffanceCardName;
                                std::string defendedAttackingCardName;

                                if (askDeffance == "yes"){

                                std::cout << "\nSelect one creature card from the list below!" << std::endl;
                                player2.printCreatureCardsInplay(player2.getInplayCards());
                                std::cin >> deffanceCardName;

                                std::cout << "Player 1's attacking cards:" << std::endl;

                                for (auto & attackingCard : attackingCards) {
                                    std::cout << attackingCard.getName() << " ";
                                }
                                std::cout << std::endl;

                                std::cout << "\nSelect the card you want to defend with: " ;
                                std::cin >> defendedAttackingCardName;


                                    for (int i = 0; i < player2.getInplayCards().size(); i++) {
                                        if (deffanceCardName == player2.getInplayCards()[i].getName()) {
                                            for (int j = 0; j < attackingCards.size(); j++) {
                                                if (attackingCards[j].getName() == defendedAttackingCardName){

                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else if ((askDeffance == "no") || !canBeDefended){

                                for (int i = 0; i < attackingCards.size(); i++) {
                                    for (int j = 0; j < creatureCards.size(); j++) {
                                        if (attackingCards[i].getName() == creatureCards[j].getName()){
                                            player2.dealDamageToThisPlayer(creatureCards[j].getAttackPower());
                                            std::cout << "Player 2 got " << creatureCards[j].getAttackPower() << " damage!" << std::endl;
                                            std::cout << "Player 2's current HP: " << player2.getCurrentHp() << std::endl;
                                            std::cout << std::endl;

                                            if (player2.getCurrentHp()<=0){
                                                player2.setIsAlive(false);
                                            }

                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if ((player1.getIsAlive()) && player2.getIsAlive()) {
                turnNumber++;
            }

            if (!attackableCardsForPlayer2.empty()){
                for (int i = 0; i < player2.getInplayCards().size(); i++) {
                    attackableCardsForPlayer2[i].setIsTapped(false);
                }
            }

            if (turnNumber == 2) {

                std::vector<std::string> cardnamesPlayedThisTurn;
                std::vector<std::string> creatureCardnamesPlayedThisTurn;

                player2.setIsLandCardPlayed(false);
                player2.setLandPowerDecreaser();
                std::cout << "Player 2's turn!" << std::endl;
                std::cout << "Player 2 Hand:" << std::endl;
                player2.printHandPile();

                std::cout << "\nWould you like to play a card from your hand pile?(yes/no)" << std::endl;
                std::string answer;
                std::cin >> answer;

                while (answer == "yes") {

                    std::cout << "\nNumber of land cards you have: " << player2.getLandPower() << std::endl;
                    std::cout << "Number of land cards untapped: " << player2.getLandPowerDecreaser() << "\n"<< std::endl;

                    player2.printIfPlayableCards();

                    if (!player2.getPlayableCards().empty()) {

                        std::cout << "Enter the card name to play..." << std::endl;
                        std::string playingCard;
                        std::cin >> playingCard;

                        bool isPlayable = false;
                        int indexAtPlayableVector;
                        std::string cardType;

                        for (int i = 0; i < player2.getPlayableCards().size(); i++) {
                            if (playingCard == player2.getPlayableCards()[i].getName() && (player2.getPlayableCards()[i].getTypeofCard() == "Land")) {
                                player2.landCardPlayed();
                                isPlayable = true;
                            }
                            else if (playingCard == player2.getPlayableCards()[i].getName()) {
                                auto manaCost = player2.getPlayableCards()[i].getManaCost();
                                if (player2.getLandPowerDecreaser() >= manaCost) {
                                    player2.decreaseLandPowerDecreaser(manaCost);
                                    isPlayable = true;
                                }
                            }
                            if (isPlayable) {
                                indexAtPlayableVector = i;
                                break;
                            }
                        }
                        if (isPlayable){
                            player2.addToInplayCardsAndRemoveFromHand(indexAtPlayableVector);
                            std::cout << playingCard << " is sent to inplay card list." << std::endl;
                            cardnamesPlayedThisTurn.push_back(playingCard);
                            for (int i = 0; i < creatureCards.size(); i++) {
                                if (playingCard == creatureCards[i].getName()){
                                    creatureCardnamesPlayedThisTurn.push_back(playingCard);
                                }
                            }
                        }
                        else{
                            std::cout << "Wrong playable card name!!!" << std::endl;
                        }
                    }
                    std::cout << "\nWould you like to play a card from your hand pile?(yes/no)" << std::endl;
                    std::cin >> answer;
                }

                if (answer == "no") {
                    bool canAttack = false;
                    bool isAttacking = false;
                    std::string attack;
                    std::string attackCard;
                    std::vector<Card> attackingCards;
                    std::vector<Card> tempInplayCards;
                    int numOfCreatureCardChosen = 0;

                    attackingCards.clear();

                    for (int i = 0; i < creatureCardnamesPlayedThisTurn.size(); i++) {
                        for (int j = 0; j < creatureCards.size(); j++) {
                            if (creatureCardnamesPlayedThisTurn[i] == creatureCards[j].getName()){
                                attackableCardsForPlayer2.push_back(creatureCards[j]);
                            }
                        }
                    }


                    for (int i = 0; i < attackableCardsForPlayer2.size(); i++) {
                        if (!attackableCardsForPlayer2[i].getIisTapped1()){
                            canAttack = true;
                        }
                    }


                    if (canAttack) {

                        std::cout << "\nWould you like to attack? (yes/no)" << std::endl;
                        std::cin >> attack;

                        if (attack == "yes") {
                            isAttacking = true;

                            for (int j = 0; j < attackableCardsForPlayer2.size(); j++) {
                                if (!attackableCardsForPlayer2[j].getIisTapped1()){
                                    tempInplayCards.push_back(attackableCardsForPlayer2[j]);
                                }
                            }
                        }

                        if (isAttacking) {

                            while (true) {
                                if (attack == "no")
                                    break;

                                std::cout << "\nSelect one creature card from the list below!" << std::endl;
                                player2.printCreatureCardsInplay(tempInplayCards);
                                std::cin >> attackCard;



                                for (int i = 0; i < tempInplayCards.size(); i++) {
                                    if (attackCard == tempInplayCards[i].getName()) {
                                        attackingCards.push_back(tempInplayCards[i]);
                                        numOfCreatureCardChosen++;
                                        tempInplayCards.erase(tempInplayCards.begin() + i);
                                    }
                                }
                                if (numOfCreatureCardChosen < (tempInplayCards.size())){
                                    std::cout << "\nWould you like to use one more creature card? (yes/no)" << std::endl;
                                    std::cin >> attack;
                                }
                                else
                                    attack = "no";
                            }

                            bool canBeDefended = false;

                            for (int i = 0; i < player1.getInplayCards().size(); i++) {
                                if (!attackableCardsForPlayer1.empty()){
                                    canBeDefended = true;
                                    break;
                                }
                            }

                            std::string askDeffance;

                            if (canBeDefended){
                                std::cout << "\n-Player 1-" << std::endl;
                                std::cout << "\nWould you like to defend with inplay cards? (yes/no)" << std::endl;
                                std::cin >> askDeffance;

                                std::string deffanceCardName;
                                std::string defendedAttackingCardName;

                                if (askDeffance == "yes"){

                                    std::cout << "\nSelect one creature card from the list below!" << std::endl;
                                    player1.printCreatureCardsInplay(player1.getInplayCards());
                                    std::cin >> deffanceCardName;

                                    std::cout << "Player 2's attacking cards:" << std::endl;

                                    for (auto & attackingCard : attackingCards) {
                                        std::cout << attackingCard.getName() << " ";
                                    }
                                    std::cout << std::endl;

                                    std::cout << "\nSelect the card you want to defend with: " ;
                                    std::cin >> defendedAttackingCardName;


                                    for (int i = 0; i < player1.getInplayCards().size(); i++) {
                                        if (deffanceCardName == player1.getInplayCards()[i].getName()) {
                                            for (int j = 0; j < attackingCards.size(); j++) {
                                                if (attackingCards[j].getName() == defendedAttackingCardName){

                                                    // attackingLogicFunction was planned to use HERE

                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else if ((askDeffance == "no") || !canBeDefended){

                                for (int i = 0; i < attackingCards.size(); i++) {
                                    for (int j = 0; j < creatureCards.size(); j++) {
                                        if (attackingCards[i].getName() == creatureCards[j].getName()){
                                            player1.dealDamageToThisPlayer(creatureCards[j].getAttackPower());
                                            std::cout << "Player 1 got " << creatureCards[j].getAttackPower() << " damage!" << std::endl;
                                            std::cout << "Player 1's current HP: " << player1.getCurrentHp() << std::endl;
                                            std::cout << std::endl;

                                            if (player1.getCurrentHp()<=0){
                                                player1.setIsAlive(false);
                                            }

                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if ((player1.getIsAlive()) && player2.getIsAlive()) {
                turnNumber++;
            }

            for (int i = 0; i < attackableCardsForPlayer1.size(); i++) {
                attackableCardsForPlayer1[i].setIsTapped(false);
            }
        }
    }
};




std::vector<Card> player1Deck{Plain(), Plain(), Plain(), Plain(), Plain(),Forest(), Forest(), Forest(),Island(),Soldier(), Soldier(), Soldier(),ArmoredPegasus(), ArmoredPegasus(),WhiteKnight(), WhiteKnight(),AngryBear(),Guard(),Werewolf(),Disenchant(),LightningBolt(),Flood(), Flood(),Rage(),HolyWar(),HolyLight()};

std::vector<Card> player2Deck{Swamp(), Swamp(), Swamp(), Swamp(), Swamp(),Mountain(), Mountain(), Mountain(),Island(),Skeleton(), Skeleton(), Skeleton(),Ghost(), Ghost(),BlackKnight(), BlackKnight(),OrcManiac(),Hobgoblin(),Vampire(),Reanimate(),Plague(),Terror(),Terror(),UnholyWar(),Restrain(),Slow()};












/*
 * ALL POSSIBILITIES OF ATTACKING AND DEFENSE ARE PROPERLY INCLUDED
 * IN THE FUNCTION BELOW. BUT I HAD TROUBLE IN COMBINING THE GAME CLASS
 * WITH THE ATTACK FUNCTION I WROTE. BECAUSE THE PROGRESS OF THE TWO
 * STRUCTURES WAS DIFFERENT, I DID NOT HAVE ENOUGH TIME TO CONNECT. RESPECTS.
 */


/*
// first parameter attacker, second one is defender
void attackLogicFunction(Player& player1,Player& player2, creatureCard& in_creature) {
    bool answerForDefence = player2.askForDefence();
    if (answerForDefence == false) {
        player2.dealDamageToThisPlayer(in_creature.getAttackPower());
    } else { // true
        std::string defenderCreatureCardName = player2.getDefenderCreatureCardName();
        creatureCard defenderCreatureCard = player2.getCreatureCardFromInInPlay(defenderCreatureCardName);
        int defenderCreatureAttack = defenderCreatureCard.getAttackPower();
        int defenderCreatureDefence = defenderCreatureCard.getHp();
        bool hasTramble = defenderCreatureCard.getIsHasTrmb();
        bool firstStrike = defenderCreatureCard.getIsHasFirstStr();
        if (((firstStrike && in_creature.getIsHasFirstStr()) && ((!hasTramble) && (!(in_creature.getIsHasTrmb())))) ||
            ((!firstStrike) && (!(in_creature.getIsHasFirstStr())))  && ((!hasTramble) && (!(in_creature.getIsHasTrmb()))) ) {
            if (in_creature.getAttackPower() >= defenderCreatureDefence && defenderCreatureAttack <= in_creature.getHp()) {
                player2.destroyCardFromInplay(defenderCreatureCardName);
            } else if (in_creature.getAttackPower() >= defenderCreatureDefence && defenderCreatureAttack <= in_creature.getHp()) {
                player1.destroyCardFromInplay(in_creature.getName());
                player2.destroyCardFromInplay(defenderCreatureCardName);
            } else if (in_creature.getAttackPower() < defenderCreatureDefence && defenderCreatureAttack <= in_creature.getHp()) {
                player1.destroyCardFromInplay(in_creature.getName());
            } // else {}  // 4th case
        }
        else if (in_creature.getIsHasTrmb()) {
//            AC -> atak>=defans  ( X = Attack power- defance power  , baseHp=basehp-X)
//            DC -> atak<defans
            if (in_creature.getAttackPower() >= defenderCreatureDefence && defenderCreatureAttack < in_creature.getHp()) {
                player2.destroyCardFromInplay(defenderCreatureCardName);
                int remainingAttackPower = in_creature.getAttackPower() - defenderCreatureDefence;
                player2.setCurrentHp(player2.getCurrentHp() - remainingAttackPower);
            }
            else if (in_creature.getAttackPower() >= defenderCreatureDefence && defenderCreatureAttack >= in_creature.getHp()) {
                player1.destroyCardFromInplay((in_creature.getName()));
                player2.destroyCardFromInplay(defenderCreatureCardName);
//                int remainingAttackPower = in_creature.getAttackPower() - defenderCreatureDefence;
//                player2.setCurrentHp(player2.getCurrentHp() - remainingAttackPower);
            }
            else if (in_creature.getAttackPower() < defenderCreatureDefence && defenderCreatureAttack > in_creature.getHp()) {
                player1.destroyCardFromInplay((in_creature.getName()));
            }
//            else if (in_creature.getAttackPower() < defenderCreatureDefence && defenderCreatureAttack < in_creature.getHp()) {
//            }
        }
        else if (in_creature.getIsHasFirstStr() && (!defenderCreatureCard.getIsHasFirstStr())) {
            if (in_creature.getAttackPower() >= defenderCreatureDefence) {
                player2.destroyCardFromInplay(defenderCreatureCardName); // DC oldu
            } else if  (in_creature.getAttackPower() < defenderCreatureDefence && defenderCreatureAttack >= in_creature.getHp()) {
                player1.destroyCardFromInplay((in_creature.getName()));
            }
//            else if  (in_creature.getAttackPower() < defenderCreatureDefence && defenderCreatureAttack < in_creature.getHp()) {
//            }
        }
        else if ((!(in_creature.getIsHasFirstStr())) && defenderCreatureCard.getIsHasFirstStr()) {
            if (defenderCreatureAttack >= in_creature.getHp()) {
                player1.destroyCardFromInplay((in_creature.getName()));
            } else if  (in_creature.getAttackPower() >= defenderCreatureDefence && defenderCreatureAttack < in_creature.getHp()) {
                player2.destroyCardFromInplay(defenderCreatureCardName); // DC oldu
            }
//            else if  (in_creature.getAttackPower() < defenderCreatureDefence && defenderCreatureAttack < in_creature.getHp()) {
//            }
        }
    }
}
void aboutAttack() {
    std::cout << "Would you like to attack?" << std::endl;
    std::string answer;
    std::cin >> answer;
    if (answer == "yes") {
        attackLogicFunction(player1, player2);
    }
}
*/


int main() {
    std::cout << "\nWelcome to the Magic the Gathering Lite card game!\n" << std::endl;

    randomizeCardInLibrary(player1Deck, player2Deck);

    Player player1(player1Deck);
    Player player2(player2Deck);

    Game mtgGame(player1, player2);
    mtgGame.playGame();


    return 0;}