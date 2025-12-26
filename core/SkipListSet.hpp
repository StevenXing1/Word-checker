// SkipListSet.hpp
//
// ICS 46 Winter 2022
// Project #4: Set the Controls for the Heart of the Sun
//
// A SkipListSet is an implementation of a Set that is a skip list, implemented
// as we discussed in lecture.  A skip list is a sequence of levels
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the keys and their
// values.  Instead, you'll need to implement your own dynamically-allocated
// nodes, with pointers connecting them.  You can, however, use other parts of
// the C++ Standard Library -- including <random>, notably.
//
// Each node should contain only two pointers: one to the node that follows it
// on the same level and another to the equivalent node on the level below it.
// Additional pointers use more memory but don't enable any techniques not
// enabled by the other two.
//
// A couple of utilities are included here: SkipListKind and SkipListKey.
// You can feel free to use these as-is and probably will not need to
// modify them, though you can make changes to them, if you'd like.

#ifndef SKIPLISTSET_HPP
#define SKIPLISTSET_HPP

#include <memory>
#include <optional>
#include <random>
#include "Set.hpp"




// SkipListKind indicates a kind of key: a normal one, the special key
// -INF, or the special key +INF.  It's necessary for us to implement
// the notion of -INF and +INF separately, since we're building a class
// template and not all types of keys would have a reasonable notion of
// -INF and +INF.

enum class SkipListKind
{
    Normal,
    NegInf,
    PosInf
};




// A SkipListKey represents a single key in a skip list.  It is possible
// to compare these keys using < or == operators (which are overloaded here)
// and those comparisons respect the notion of whether each key is normal,
// -INF, or +INF.

template <typename ElementType>
class SkipListKey
{
public:
    static SkipListKey normal(const ElementType& element);
    static SkipListKey negInf();
    static SkipListKey posInf();

    bool operator==(const SkipListKey& other) const;
    bool operator==(const ElementType& other) const;

    bool operator<(const SkipListKey& other) const;
    bool operator<(const ElementType& other) const;



private:
    SkipListKey(SkipListKind kind, std::optional<ElementType> element);

private:
    SkipListKind kind;
    std::optional<ElementType> element;
};


template <typename ElementType>
SkipListKey<ElementType> SkipListKey<ElementType>::normal(const ElementType& element)
{
    return SkipListKey{SkipListKind::Normal, std::make_optional(element)};
}


template <typename ElementType>
SkipListKey<ElementType> SkipListKey<ElementType>::negInf()
{
    return SkipListKey{SkipListKind::NegInf, std::nullopt};
}


template <typename ElementType>
SkipListKey<ElementType> SkipListKey<ElementType>::posInf()
{
    return SkipListKey{SkipListKind::PosInf, std::nullopt};
}


template <typename ElementType>
SkipListKey<ElementType>::SkipListKey(SkipListKind kind, std::optional<ElementType> element)
    : kind{kind}, element{element}
{
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator==(const SkipListKey& other) const
{
    return kind == other.kind
        && (kind != SkipListKind::Normal || *element == *other.element);
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator==(const ElementType& other) const
{
    return kind == SkipListKind::Normal && *element == other;
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator<(const SkipListKey& other) const
{
    switch (kind)
    {
    case SkipListKind::NegInf:
        return other.kind != SkipListKind::NegInf;

    case SkipListKind::PosInf:
        return false;

    default: // SkipListKind::Normal
        return other.kind == SkipListKind::PosInf
            || (other.kind == SkipListKind::Normal && *element < *other.element);
    }
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator<(const ElementType& other) const
{
    return kind == SkipListKind::NegInf
        || (kind == SkipListKind::Normal && *element < other);
}



// The SkipListLevelTester class represents the ability to decide whether
// a key placed on one level of the skip list should also occupy the next
// level.  This is the "coin flip," so to speak.  Note that this is an
// abstract base class with one implementation, RandomSkipListLevelTester,
// just below it.  RandomSkipListLevelTester is what it sounds like: It
// makes the decision at random (with a 50/50 chance of deciding whether
// a key should occupy the next level).  However, by setting things up
// this way, we have a way to control things more carefully in our
// testing (as you can, as well).
//
// DO NOT MAKE CHANGES TO THE SIGNATURES OF THE MEMBER FUNCTIONS OF
// THE "level tester" CLASSES.  You can add new member functions or even
// whole new level tester classes, but the ones declared below are part
// of how we test your implementation, so they need to maintain the
// parts of the interface that are declared already.

template <typename ElementType>
class SkipListLevelTester
{
public:
    virtual ~SkipListLevelTester() = default;

    virtual bool shouldOccupyNextLevel(const ElementType& element) = 0;
    virtual std::unique_ptr<SkipListLevelTester<ElementType>> clone() = 0;
};


template <typename ElementType>
class RandomSkipListLevelTester : public SkipListLevelTester<ElementType>
{
public:
    RandomSkipListLevelTester();

    bool shouldOccupyNextLevel(const ElementType& element) override;
    std::unique_ptr<SkipListLevelTester<ElementType>> clone() override;

private:
    std::default_random_engine engine;
    std::bernoulli_distribution distribution;
};


template <typename ElementType>
RandomSkipListLevelTester<ElementType>::RandomSkipListLevelTester()
    : engine{std::random_device{}()}, distribution{0.5}
{
}


template <typename ElementType>
bool RandomSkipListLevelTester<ElementType>::shouldOccupyNextLevel(const ElementType& element)
{
    return distribution(engine);
}


template <typename ElementType>
std::unique_ptr<SkipListLevelTester<ElementType>> RandomSkipListLevelTester<ElementType>::clone()
{
    return std::unique_ptr<SkipListLevelTester<ElementType>>{
        new RandomSkipListLevelTester<ElementType>};
}




template <typename ElementType>
class SkipListSet : public Set<ElementType>
{
public:
    // Initializes an SkipListSet to be empty, with or without a
    // "level tester" object that will decide, whenever a "coin flip"
    // is needed, whether a key should occupy the next level above.
    static constexpr unsigned int INITIAL_CAPACITY = 10;

    SkipListSet();
    explicit SkipListSet(std::unique_ptr<SkipListLevelTester<ElementType>> levelTester);

    // Cleans up the SkipListSet so that it leaks no memory.
    ~SkipListSet() noexcept override;

    // Initializes a new SkipListSet to be a copy of an existing one.
    SkipListSet(const SkipListSet& s);

    // Initializes a new SkipListSet whose contents are moved from an
    // expiring one.
    SkipListSet(SkipListSet&& s) noexcept;

    // Assigns an existing SkipListSet into another.
    SkipListSet& operator=(const SkipListSet& s);

    // Assigns an expiring SkipListSet into another.
    SkipListSet& operator=(SkipListSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a SkipListSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function runs in an expected time
    // of O(log n) (i.e., over the long run, we expect the average to be
    // O(log n)) with very high probability.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in an expected time of O(log n)
    // (i.e., over the long run, we expect the average to be O(log n))
    // with very high probability.
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // levelCount() returns the number of levels in the skip list.
    unsigned int levelCount() const noexcept;


    // elementsOnLevel() returns the number of elements that are stored
    // on the given level of the skip list.  Level 0 is the bottom level;
    // level 1 is the one above level 0; and so on.  If the given level
    // doesn't exist, this function returns 0.  (Note that the -INF
    // and +INF shouldn't be counted.)
    unsigned int elementsOnLevel(unsigned int level) const noexcept;


    // isElementOnLevel() returns true if the given element is on the
    // given level, false otherwise.  Level 0 is the bottom level; level 1
    // is the one above level 0; and so on.  If the given level doesn't
    // exist, this function returns false.
    bool isElementOnLevel(const ElementType& element, unsigned int level) const;


private:

    std::unique_ptr<SkipListLevelTester<ElementType>> levelTester;
    void clear();
    void copy(const SkipListSet& s);
    void resize();
    struct Node{
        Node* right;
        Node* below;
        SkipListKey<ElementType> key;

        Node():right{nullptr}, below{nullptr}, key{SkipListKey<ElementType>::negInf()}{};
    };

    Node** nodeList;
    int sz;
    int lv;
    int capacity;
    // You'll no doubt want to add member variables and "helper" member
    // functions here.
};



template <typename ElementType>
SkipListSet<ElementType>::SkipListSet()
    : SkipListSet{std::make_unique<RandomSkipListLevelTester<ElementType>>()}

{

}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(std::unique_ptr<SkipListLevelTester<ElementType>> levelTester)
    : levelTester{std::move(levelTester)}, nodeList{new Node*[INITIAL_CAPACITY]}, sz{0}, lv{1}, capacity{INITIAL_CAPACITY}
{
    
    for(int i=0;i<INITIAL_CAPACITY;i++){
        nodeList[i] = nullptr;
    }

}


template <typename ElementType>
void SkipListSet<ElementType>::clear(){
    for(int i =0; i<capacity;i++){
        Node * current = nodeList[i];
        while(current != nullptr){
            Node * temp=current->right;
            delete current;
            current = temp;
        }
    }

    delete [] nodeList;
}

template <typename ElementType>
void SkipListSet<ElementType>::copy(const SkipListSet& s){
    
    for (int i=0; i<s.capacity;i++){
        Node * current = s.nodeList[i];
        while(current != nullptr){
            Node * pNode = new Node();
            pNode->key= current->key;
            if(i!=0){
                
                Node* belowCurrent = nodeList[i-1];
                while(!(belowCurrent->key == pNode->key)){
                    belowCurrent = belowCurrent->right;
                }
                pNode->below = belowCurrent;
            }
            if (nodeList[i] == nullptr){
                this->nodeList[i] = pNode;
            }else{
                Node*nextNode = nodeList[i];
                while (nextNode->right != nullptr){
                    nextNode = nextNode->right;
                }
                nextNode->right = pNode;
            }
            current = current->right;
        }
    }
}

//This function resizes the skiplist when it's full;
template <typename ElementType>
void SkipListSet<ElementType>::resize(){
    Node**list = new Node*[capacity*2+1];
    for(int i=0; i<lv;i++){
        list[i] = nodeList[i];
    }
    delete[] nodeList;
    capacity=capacity*2+1;
    nodeList = list;
}

template <typename ElementType>
SkipListSet<ElementType>::~SkipListSet() noexcept
{
    clear();
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(const SkipListSet& s)
{
    
    SkipListSet<ElementType> tempSkip(std::make_unique<RandomSkipListLevelTester<ElementType>>());
    tempSkip.capacity = s.capacity;
    try{
        tempSkip.copy(s);
    }
    catch(...){
        throw;
    }

    //Initialize this skip list.
    levelTester = std::make_unique<RandomSkipListLevelTester<ElementType>>();
    nodeList=new Node*[s.capacity];
    sz = s.sz;
    lv = s.lv;
    capacity = s.capacity;

    for(int i=0; i<s.capacity;i++){
        nodeList[i] = nullptr;
    }
    copy(s);
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(SkipListSet&& s) noexcept
                        :levelTester{std::make_unique<RandomSkipListLevelTester<ElementType>>()}, nodeList{new Node*[INITIAL_CAPACITY]}, sz{s.sz}, lv{s.lv},capacity{INITIAL_CAPACITY}
{
    for(int i=0;i<INITIAL_CAPACITY;i++){
        nodeList[i] = nullptr;
    }
    Node** tempList = s.nodeList;
    int tempSz = s.sz;
    int tempLevel = s.lv;
    int tempCapacity = s.capacity;

    s.nodeList = nodeList;
    s.sz = sz;
    s.lv = lv;
    s.capacity = capacity;
    
    nodeList = tempList;
    sz = tempSz;
    lv = tempLevel;
    capacity = tempCapacity;
}


template <typename ElementType>
SkipListSet<ElementType>& SkipListSet<ElementType>::operator=(const SkipListSet& s)
{
    if(this != &s){
        SkipListSet<ElementType> tempSkip(std::make_unique<RandomSkipListLevelTester<ElementType>>());
        tempSkip.capacity = s.capacity;
        try{
            tempSkip.copy(s);
        }
        catch(...){
            throw;
        }
        clear();

        nodeList = new Node*[s.capacity];
        sz = s.sz;
        lv = s.lv;
        capacity = s.capacity;
        for(int i=0; i<s.capacity;i++){
            nodeList[i] = nullptr;
        }
        copy(s);

    }
    return *this;
}


template <typename ElementType>
SkipListSet<ElementType>& SkipListSet<ElementType>::operator=(SkipListSet&& s) noexcept                       
{

    Node** tempList = s.nodeList;
    int tempSz = s.sz;
    int tempLevel = s.lv;
    int tempCapacity = s.capacity;

    s.nodeList = nodeList;
    s.sz = sz;
    s.lv = lv;
    s.capacity = capacity;
    
    nodeList = tempList;
    sz = tempSz;
    lv = tempLevel;
    capacity = tempCapacity;
    return *this;
}


template <typename ElementType>
bool SkipListSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void SkipListSet<ElementType>::add(const ElementType& element)
{
    //check if the skip list doesn't have any element
    if(nodeList[0] == nullptr){
        nodeList[0] = new Node();

        nodeList[0]->right = new Node();
        nodeList[0]->right->key = SkipListKey<ElementType>::normal(element);
        nodeList[0]->right->right = new Node();
        nodeList[0]->right->right->key = SkipListKey<ElementType>::posInf();
        sz++;
    }
    else if(!contains(element)){
        //check if it contains this element

        Node* current = nodeList[lv-1];

        while(current!=nullptr){
            
            if(current->key<element &&  !(current->right->key< element)){
                //if we are in the right interval, then we move down. Otherwise, we keep moving right.
                if(current->below != nullptr){
                    current = current->below;
                }
                else{
                    //if we are in the correct interval and the current->below is nullptr, which means it's level 0, then we break out since we find the place we want.
                    break;
                }
            }
            else{
                current = current->right;
            }
        }
        
        //we assign the value for the level 0.
        Node* pNode = new Node();
        pNode->key = SkipListKey<ElementType>::normal(element);
        pNode->right = current->right;
        
        current->right = pNode;
        
        //check for the upperlevel
        int insertLevel = 1;
        while(levelTester->shouldOccupyNextLevel(element)){
            insertLevel++;
            if(insertLevel>lv){
                lv = insertLevel;
                if((double)lv/capacity >=0.8){
                    resize();
                }
                Node* negNode = new Node();
                negNode->below = nodeList[lv-2];
                Node* posNode = new Node();
                posNode->key = SkipListKey<ElementType>::posInf();
                Node*testNode = nodeList[lv-2];
                while(testNode->right!=nullptr){
                    testNode = testNode->right;
                }
                posNode->below = testNode;
                negNode->right = posNode;
                nodeList[lv-1] = negNode;
                
            }
            
            Node* insertCurrent = nodeList[insertLevel-1];
            while(insertCurrent!=nullptr){
                if(insertCurrent->key<element && !(insertCurrent->right->key< element)){
                    break;
                }
                insertCurrent = insertCurrent->right;
            }
            Node* levelNode = new Node();
            levelNode->key = SkipListKey<ElementType>::normal(element);
            levelNode->below = pNode;
            pNode = levelNode;
            levelNode->right = insertCurrent->right;
            insertCurrent->right = levelNode;
            
            

        }
        sz++;



    }


}


template <typename ElementType>
bool SkipListSet<ElementType>::contains(const ElementType& element) const
{
    Node* current = nodeList[lv-1];
    while(current!= nullptr){
        if(current->key == element){
            return true;
        }
        else if(current->key<element && SkipListKey<ElementType>::normal(element)<current->right->key ){
            current = current->below;
        }
        else{
            current = current->right;
        }
    }
    return false;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::size() const noexcept
{
    return sz;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::levelCount() const noexcept
{
    return lv;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::elementsOnLevel(unsigned int level) const noexcept
{
    int count = -2;
    if(level>=this->lv || level <0){
        return 0;
    }
    
    else{
        Node* current = nodeList[level];
        
        while(current!=nullptr){
            count++;
            current = current->right;
        }
        
    }
    return count;
}


template <typename ElementType>
bool SkipListSet<ElementType>::isElementOnLevel(const ElementType& element, unsigned int level) const
{
    if(level >= lv || level<0){
        return false;
    }
    Node* current = nodeList[level];
    while(current != nullptr){
        if(current->key == element){
            return true;
        }
     
        current = current->right;
    }
    return false;
    
}



#endif

