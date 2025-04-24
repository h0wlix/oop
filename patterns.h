#ifndef PATTERNS_H_INCLUDED
#define PATTERNS_H_INCLUDED
#include <vector>
using namespace std;
const size_t MaxSize = 100;

template<typename T>
class Iterator {
protected:
    Iterator() {}
public:
    virtual ~Iterator() {}
    virtual void First() = 0;
    virtual void Next() = 0;
    virtual bool IsDone() const = 0;
    virtual T GetCurrent() const = 0;
};

template <typename T>
class Containers {
public:
    virtual ~Containers() {}
    virtual bool IsEmpty() const = 0;
    virtual size_t Size() const = 0;
    virtual T GetElementByIndex(size_t index) const = 0;
    virtual Iterator<T>* GetIterator() = 0;
};

template<typename T>
class StackIterator : public Iterator<T> {
private:
    const Containers<T>* StackContainer;
    size_t Pos;
public:
    StackIterator(const Containers<T>* container)
        : StackContainer(container), Pos(0) {}
    void First() override { Pos = 0; }
    void Next() override { Pos++; }
    bool IsDone() const override { return (Pos >= StackContainer->Size()); }
    T GetCurrent() const override { return StackContainer->GetElementByIndex(Pos); }
};

template<typename T>
class Stack : public Containers<T> {
private:
    T Bottles[MaxSize];
    size_t Border;
public:
    bool IsEmpty() const override {
        return (Border == 0);
    }
    size_t Size() const override {
        return Border;
    }
    void Push(T newBottle) {
        Bottles[Border++] = newBottle;
    }
    T Pop() {
        return Bottles[--Border];
    }
    T GetElementByIndex(size_t index) const override {
        return Bottles[index];
    }
    Iterator<T>* GetIterator() override {
        return new StackIterator<T>(this);
    }
    Stack() : Border(0) {}
};

template<typename T>
class VectorIterator : public Iterator<T> {
private:
    const Containers<T>* VectorContainer;
    size_t Pos;
public:
    VectorIterator(const Containers<T>* container)
        : VectorContainer(container), Pos(0) {}
    void First() override { Pos = 0; }
    void Next() override { Pos++; }
    bool IsDone() const override { return (Pos >= VectorContainer->Size()); }
    T GetCurrent() const override { return VectorContainer->GetElementByIndex(Pos); }
};

template<typename T>
class Vector : public Containers<T> {
private:
    vector<T> elements;
public:
    Vector() = default;
    bool IsEmpty() const override {
        return elements.empty();
    }
    size_t Size() const override {
        return elements.size();
    }
    void Push(const T& newElement) {
        elements.push_back(newElement);
    }
    T Top() const {
        return elements.back();
    }
    T Pop() {
        T last = elements.back();
        elements.pop_back();
        return last;
    }
    T GetElementByIndex(size_t index) const override {
        return elements[index];
    }
    Iterator<T>* GetIterator() override {
        return new VectorIterator<T>(this);
    }
};

template<typename T>
class IteratorDecorator : public Iterator<T>
{
protected:
    Iterator<T> *It;
public:
    IteratorDecorator(Iterator<T> *it) : It(it) {}
    virtual ~IteratorDecorator() { delete It; }
    virtual void First() { It->First(); }
    virtual void Next() { It->Next(); }
    virtual bool IsDone() const { return It->IsDone(); }
    virtual T GetCurrent() const { return It->GetCurrent(); }
};
 template<typename ContainerType, typename ItemType>
class ConstIteratorAdapter : public Iterator<ItemType>
{
protected:
    ContainerType *Container;
    typename ContainerType::const_iterator It;

public:
    ConstIteratorAdapter(ContainerType *container)
    : Container(container)
    {
        It = Container->begin();
    }

    virtual ~ConstIteratorAdapter() {}
    virtual void First() { It = Container->begin(); }
    virtual void Next() { It++; }
    virtual bool IsDone() const { return (It == Container->end()); }
    virtual ItemType GetCurrent() const { return *It; }
};
#endif // PATTERNS_H_INCLUDED
