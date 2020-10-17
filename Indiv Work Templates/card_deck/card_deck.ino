
class Card
{
    char suit;
    int value;
  public:
    Card();
    Card(int, char);
    char getSuit();
    int getValue();
};

Card::Card() {};

Card::Card(int val, char s)
{
  value = val;
  suit = s;
}

char Card::getSuit() {
  return suit;
}
int Card::getValue() {
  return value;
}

class Deck
{
    Card cards[52];
    int topCard;
  public:
    Deck();
    Card getTopCard();
    void reset();
    void shuffle();
};

Deck::Deck()
{
  topCard = 0;
  char suits[] = {'H', 'C', 'S', 'D'};
  int counter = 0;
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 13; j++)
    {
      cards[counter] = Card(j, (char)suits[i]);
      counter++;
    }
  }
}
Card Deck::getTopCard()
{
  Card c = cards[topCard];
  topCard++;
  return c;
}

void Deck::shuffle()
{
  for (int i = 0; i < 52; i++)
  {
    int r = random(52);
    Card buff = cards[i];
    cards[i] = cards[r];
    cards[r] = buff;
  }
  topCard = 0;
}

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  Deck d;
  d.shuffle();
  for (int i = 0; i < 52; i++) {
    Card topcard = d.getTopCard();
    Serial.print(topcard.getSuit()); Serial.println(topcard.getValue());
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
