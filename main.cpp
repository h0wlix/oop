#include <iostream>
#include <vector>
#include "patterns.h"
using namespace std;

class BeerActionStrategy {
public:
    virtual ~BeerActionStrategy() {}
    virtual void Execute() = 0;
};

class DrinkStrategy:public BeerActionStrategy {
public:
    void Execute() override {cout << "Drinking the beer..\n";}
};

class PourOutStrategy:public BeerActionStrategy {
public:
    void Execute() override {cout << "Pouring out the beer...\n";}
};

class MakeSauceStrategy:public BeerActionStrategy {
public:
    void Execute() override {cout << "Making sauce with the beer...\n";}
};

enum class BeerActionType {
    Drink,
    PourOut,
    MakeSauce
};

BeerActionStrategy* CreateBeerStrategy(BeerActionType type) {
    switch(type){
        case BeerActionType::Drink: return new DrinkStrategy();
        case BeerActionType::PourOut: return new PourOutStrategy();
        case BeerActionType::MakeSauce: return new MakeSauceStrategy();
        default: return nullptr;
    }
}

class Beer {
private:
    BeerActionStrategy* BeerAction;
    int IBU; // International Bitterness Units - горечь
    double ABV; // Alcohol by Volume - крепость
    double OG; // Original Gravity - плотность
    int EBC; //European Brewing Convention - цвет
    void DoActionUsingStrategy() {
        if(BeerAction == nullptr) {
            cout << "No action!" ;
            return;
        }
        else {
            BeerAction->Execute();
        }
    }
protected:
    bool BeerWithAlcohol; // Алькогольное пиво или безалкогольное (1 - алкогольное, 0 безалкогольное)
    void PrepareForAction() {
        PrintType();
        cout << " : ";
        CheckAlcoholContent();
        cout << " : ";
        OpenBottle();
        cout << " : ";
    }

    virtual void PrintType() = 0;
    virtual void OpenBottle() = 0;

    void CheckAlcoholContent() {
        if(BeerWithAlcohol) cout << "ALCOHOLIC";
        else cout << "NON-ALCOHOLIC";
    }

public:
     Beer(int ibu, double abv, double og, int ebc)
        : IBU(ibu), ABV(abv), OG(og), EBC(ebc), BeerWithAlcohol(false), BeerAction(nullptr)
    {
        BeerWithAlcohol = (rand() % 10) < 7;
        cout << "Created a beer...\n";
    }

    virtual ~Beer() {
        if(BeerAction) delete BeerAction;
        cout << "Destroyed beer bottle...\n";
    }
    void PerformAction();
    void SetActionStrategy(BeerActionStrategy* action);
    bool IsAlcoholic() const { return BeerWithAlcohol; }
    int GetIBU() const { return IBU; }
    double GetABV() const { return ABV; }
    double GetOG() const { return OG; }
    int GetEBC() const { return EBC; }
    };
    void Beer::PerformAction() {
        PrepareForAction();
        DoActionUsingStrategy();
        cout << endl;
    }
    void Beer::SetActionStrategy(BeerActionStrategy* action) {
    if (BeerAction) delete BeerAction;
    BeerAction = action;
    };

class PaleLager : public Beer
{
public:
    PaleLager(int ibu, double abv, double og, int ebc);
    ~PaleLager();
protected:
    void PrintType() override { cout << "PALE LAGER"; }
    void OpenBottle() override { cout << "Twist-off cap opened"; }
};

PaleLager::PaleLager(int ibu, double abv, double og, int ebc):Beer(ibu, abv, og, ebc)
{
    BeerWithAlcohol = true;
    cout << "Taking a bottle of pale lager" << endl;
    SetActionStrategy(CreateBeerStrategy(BeerActionType::Drink));
}

PaleLager::~PaleLager() {
    cout << "Pale lager bottle removed\n" << endl;
}

class NoAlcoholLager : public Beer
{
public:
    NoAlcoholLager(int ibu, double abv, double og, int ebc);
    ~NoAlcoholLager();
protected:
    void PrintType() override { cout << "NON-ALCOHOLIC LAGER"; }
    void OpenBottle() override { cout << "Easy-open cap removed"; }
};

NoAlcoholLager::NoAlcoholLager(int ibu, double abv, double og, int ebc):Beer(ibu, abv, og, ebc)
{
    BeerWithAlcohol = false;
    cout << "Taking a bottle of no alcohol lager" << endl;
    SetActionStrategy(CreateBeerStrategy(BeerActionType::PourOut));
}

NoAlcoholLager::~NoAlcoholLager() {
    cout << "No alcohol lager bottle removed\n" << endl;
}

class Stout : public Beer
{
public:
    Stout(int ibu, double abv, double og, int ebc);
    ~Stout();
protected:
    void PrintType() override { cout << "STOUT"; }
    void OpenBottle() override { cout << "Pry-off cap opened"; }
};

Stout::Stout(int ibu, double abv, double og, int ebc):Beer(ibu, abv, og, ebc)
{
    BeerWithAlcohol = true;
    cout << "Taking a bottle of stout" << endl;
    SetActionStrategy(CreateBeerStrategy(BeerActionType::MakeSauce));
}

Stout::~Stout() {
    cout << "Stout bottle removed\n" << endl;
}


enum class TypeOfBeer : int {
    PaleLager = 1,
    NoAlcoholLager = 2,
    Stout = 3,
    Undefined = 0,
};
Beer *CreateBeer(TypeOfBeer type){
    Beer *newBeer = nullptr;
    if(type == TypeOfBeer::PaleLager){
        newBeer = new PaleLager(15, 4.5, 1.02, 8);
    }
    else if (type == TypeOfBeer::NoAlcoholLager){
        newBeer = new NoAlcoholLager(10, 0.5, 1.01, 6);
    }
    else if (type == TypeOfBeer::Stout){
        newBeer = new Stout(40, 7.5, 1.06, 35);
    }
    return newBeer;
}
void Action(Iterator<Beer*> *iterator)
{
    for (iterator->First();
        !iterator->IsDone();
         iterator->Next())
    {
        iterator->GetCurrent()->PerformAction();
    }
}

template<typename T>
class ChillIteratorDecorator : public IteratorDecorator<T> {
public:
    ChillIteratorDecorator(Iterator<T>* it) : IteratorDecorator<T>(it) {}

    void First() override {
        IteratorDecorator<T>::First();
        if (!this->IsDone()) {
            cout << "Chilling the beer before use...\n";
        }
    }

    void Next() override {
        IteratorDecorator<T>::Next();
        if (!this->IsDone()) {
            cout << "Chilling the beer before use...\n";
        }
    }
};
template<typename T>
class TasteRatingIteratorDecorator : public IteratorDecorator<T> {
private:
    int GetTasteRating(const Beer* beer) const {
        int rating = 0;
        if (beer->GetABV() > 5.0) rating += 3;
        if (beer->GetIBU() > 20) rating += 2;
        if (beer->GetEBC() > 20) rating += 1;
        return rating;
    }

public:
    TasteRatingIteratorDecorator(Iterator<T>* it) : IteratorDecorator<T>(it) {}

    void Next() override {
        IteratorDecorator<T>::Next();
        if (!this->IsDone()) {
            Beer* beer = this->GetCurrent();
            int rating = GetTasteRating(beer);
            cout << "Taste rating for this beer: " << rating << "/6\n";
        }
    }
};
template<typename T>
class PairWithFoodIteratorDecorator : public IteratorDecorator<T> {
private:
    string GetFoodPairing(const Beer* beer) const {
        if (beer->GetEBC() > 30) return "Dark chocolate";
        if (beer->GetIBU() < 15) return "Cheese platter";
        return "Pretzels";
    }

public:
    PairWithFoodIteratorDecorator(Iterator<T>* it) : IteratorDecorator<T>(it) {}

    void Next() override {
        IteratorDecorator<T>::Next();
        if (!this->IsDone()) {
            Beer* beer = this->GetCurrent();
            string foodPairing = GetFoodPairing(beer);
            cout << "This beer pairs well with: " << foodPairing << "\n";
        }
    }
};

int main()
{
    cout << "What beer do you want today (1 - pale lager, 2 - no alcohol lager, 3 - stout)?" << endl;
    TypeOfBeer type = TypeOfBeer::Undefined;
    int beertype;
    cin >> beertype;
    type = static_cast<TypeOfBeer>(beertype);

    cout << "\nUsing ConstIteratorAdapter with vector:\n";
    vector<Beer*> stlBeerVector;
    for (size_t i = 0; i < beertype; i++) {
        int numberBeer = rand() % 3 + 1;
        TypeOfBeer beer_type = static_cast<TypeOfBeer>(numberBeer);
        Beer* beer4 = CreateBeer(beer_type);
        stlBeerVector.push_back(beer4);
    }
    ConstIteratorAdapter<vector<Beer*>, Beer*> stlAdapter(&stlBeerVector);
    Action(&stlAdapter);
    for (auto beer : stlBeerVector) {
        delete beer;
    }
    cout << "\nStack with ChillIteratorDecorator:" << endl;
    Stack<Beer*> BeerStack;
    for (size_t i = 0; i < beertype; i++) {
        int numberBeer = rand() % 3 + 1;
        TypeOfBeer beer_type = static_cast<TypeOfBeer>(numberBeer);
        Beer* beer1 = CreateBeer(beer_type);
        BeerStack.Push(beer1);
    }
    StackIterator<Beer*> iteratorStack(&BeerStack);
    ChillIteratorDecorator<Beer*> chillDecorator(&iteratorStack);
    Action(&chillDecorator);
    while (!BeerStack.IsEmpty()) {
        delete BeerStack.Pop();
    }
    cout << "\nStack with PairWithFoodIteratorDecorator:" << endl;
    Stack<Beer*> BeerStackForFood;
    for (size_t i = 0; i < beertype; i++) {
        int numberBeer = rand() % 3 + 1;
        TypeOfBeer beer_type = static_cast<TypeOfBeer>(numberBeer);
        Beer* beer3 = CreateBeer(beer_type);
        BeerStackForFood.Push(beer3);
    }
    StackIterator<Beer*> iteratorStackForFood(&BeerStackForFood);
    PairWithFoodIteratorDecorator<Beer*> foodDecorator(&iteratorStackForFood);
    Action(&foodDecorator);
    while (!BeerStackForFood.IsEmpty()) {
        delete BeerStackForFood.Pop();
    }
    cout << "\nVector with TasteRatingIteratorDecorator:" << endl;
    Vector<Beer*> BeerVector;
    for (size_t i = 0; i < beertype; i++) {
        int numberBeer = rand() % 3 + 1;
        TypeOfBeer beer_type = static_cast<TypeOfBeer>(numberBeer);
        Beer* beer2 = CreateBeer(beer_type);
        BeerVector.Push(beer2);
    }
    VectorIterator<Beer*> iteratorVector(&BeerVector);
    TasteRatingIteratorDecorator<Beer*> tasteDecorator(&iteratorVector);
    Action(&tasteDecorator);
    while (!BeerVector.IsEmpty()) {
        delete BeerVector.Pop();
    }

    return 0;
}
