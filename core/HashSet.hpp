// HashSet.hpp
//
// ICS 46 Winter 2022
// Project #4: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"



template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    ~HashSet() noexcept override;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8, in which case
    // the new capacity should be determined by this formula:
    //
    //     capacity * 2 + 1
    //
    // In the case where the array is resized, this function runs in linear
    // time (with respect to the number of elements, assuming a good hash
    // function); otherwise, it runs in constant time (again, assuming a good
    // hash function).  The amortized running time is also constant.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns false.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;


private:
    struct Node{
        ElementType value;
        Node* next;
    };
    void rehash();
    Node** hashTable;
    HashFunction hashFunction;
    int sz;
    int capacity;
    

    // You'll no doubt want to add member variables and "helper" member
    // functions here.
};



namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashTable{new Node*[DEFAULT_CAPACITY]}, hashFunction{hashFunction}, sz{0}, capacity{DEFAULT_CAPACITY}
{
    for(int j = 0;j<DEFAULT_CAPACITY;j++){
        hashTable[j] = nullptr;
    }
}


template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    for(int i =0; i<capacity; i++){
        Node* current = hashTable[i];
        while(current != nullptr){
            Node* tempNode = current;
            current = current->next;
            delete tempNode;
        }
    }
    delete []hashTable;
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    //: hashTable{new Node*[s.capacity]},hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}, sz{s.sz}, capacity{s.capacity}
{
    Node**tempTable;
    try{
        tempTable = new Node*[s.capacity];
        for(int k = 0; k<s.capacity;k++){
            tempTable[k] = nullptr;
        }
        for(int m=0; m<s.capacity;m++){
            Node * tempCurrent = s.hashTable[m];
            while(tempCurrent != nullptr){
                Node * pNode = new Node();
                pNode->value = tempCurrent->value;
                pNode->next = nullptr;
                if (tempTable[m] == nullptr){
                    tempTable[m] = pNode;
                }else{
                    Node*nextNode = tempTable[m];
                    while (nextNode->next != nullptr){
                        nextNode = nextNode->next;
                    }
                    nextNode->next = pNode;
                }
                tempCurrent = tempCurrent->next;
            }
        }
        for(int i =0; i<s.capacity; i++){
            Node* current = tempTable[i];
            while(current != nullptr){
                Node* tempNode = current;
                current = current->next;
                delete tempNode;
            }
        }
        delete []tempTable;
    }
    catch(...){
        throw;
    }

    hashTable = new Node*[s.capacity];
    hashFunction = s.hashFunction;
    sz = s.sz;
    capacity = s.capacity;
    for(int j = 0;j<s.capacity;j++){
        hashTable[j] = nullptr;
    }
    for (int i=0; i<s.capacity;i++){
        Node * current = s.hashTable[i];
        while(current != nullptr){
            Node * pNode = new Node();
            pNode->value = current->value;
            pNode->next = nullptr;
            if (hashTable[i] == nullptr){
                this->hashTable[i] = pNode;
            }else{
                Node*nextNode = hashTable[i];
                while (nextNode->next != nullptr){
                    nextNode = nextNode->next;
                }
                nextNode->next = pNode;
            }
            current = current->next;
        }
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashTable{new Node*[DEFAULT_CAPACITY]}, hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}, sz{0}, capacity{DEFAULT_CAPACITY}
{
    for(int j = 0;j<DEFAULT_CAPACITY;j++){
        hashTable[j] = nullptr;
    }
    Node** tempTable = s.hashTable;
    std::function tempFunction = s.hashFunction;
    int tempSz = s.sz;
    int tempCapacity = s.capacity;

    s.hashTable = hashTable;
    s.hashFunction = hashFunction;
    s.sz = sz;
    s.capacity = capacity;

    hashTable = tempTable;
    hashFunction = tempFunction;
    sz = tempSz;
    capacity = tempCapacity;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    if (this != &s){
        Node**tempTable;
        try{
            tempTable = new Node*[s.capacity];
            for(int k = 0; k<s.capacity;k++){
                tempTable[k] = nullptr;
            }
            for(int m=0; m<s.capacity;m++){
                Node * tempCurrent = s.hashTable[m];
                while(tempCurrent != nullptr){
                    Node * pNode = new Node();
                    pNode->value = tempCurrent->value;
                    pNode->next = nullptr;
                    if (tempTable[m] == nullptr){
                        tempTable[m] = pNode;
                    }else{
                        Node*nextNode = tempTable[m];
                        while (nextNode->next != nullptr){
                            nextNode = nextNode->next;
                        }
                        nextNode->next = pNode;
                    }
                    tempCurrent = tempCurrent->next;
                }
            }
            
            for(int i =0; i<capacity; i++){
                Node* current = tempTable[i];
                while(current != nullptr){
                    Node* tempNode = current;
                    current = current->next;
                    delete tempNode;
                }
            }
            delete []tempTable;
        }
        catch(...){
            throw;
        }
        //delete the original hashTable
        for(int q =0; q<capacity; q++){
            Node* current = hashTable[q];
            while(current != nullptr){
                Node* tempNode = current;
                current = current->next;
                delete tempNode;
            }
        }
        delete []hashTable;

        //Initialize this hash set again
        hashTable = new Node*[s.capacity];
        sz = s.sz;
        this->hashFunction = s.hashFunction;
        capacity = s.capacity;
        for(int a1=0; a1<s.capacity;a1++){
            hashTable[a1] = nullptr;
        }

        //Assign the value to our new hashTable
        for (int i=0; i<capacity;i++){
            Node * current = s.hashTable[i];
            while(current != nullptr){
                Node * pNode = new Node();
                pNode->value = current->value;
                pNode->next = nullptr;
                if (hashTable[i] == nullptr){
                    this->hashTable[i] = pNode;
                }else{
                    Node*nextNode = hashTable[i];
                    while (nextNode->next != nullptr){
                        nextNode = nextNode->next;
                    }
                    nextNode->next = pNode;
                }
                current = current->next;
            }
        }
    }
    return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    Node** tempTable = s.hashTable;
    std::function tempFunction = s.hashFunction;
    int tempSz = s.sz;
    int tempCapacity = s.capacity;

    s.hashTable = hashTable;
    s.hashFunction = hashFunction;
    s.sz = sz;
    s.capacity = capacity;

    hashTable = tempTable;
    hashFunction = tempFunction;
    sz = tempSz;
    capacity = tempCapacity;
    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}

template <typename ElementType>
void HashSet<ElementType>::rehash(){
    //rehash() reconstruct a new array of pointers that doubled the capacity of the previous one and rehash all the values within it to the new array.
    Node** nodeList = new Node*[capacity*2+1];
    for(int j = 0;j<capacity*2+1;j++){
        nodeList[j] = nullptr;
    }
    for(int i=0; i< capacity;i++){
        Node*current = hashTable[i];
        while(current != nullptr){
            int index = hashFunction(current->value)%(capacity*2+1);
            if(nodeList[index] == nullptr){
                nodeList[index] = new Node();
                nodeList[index]->value = current->value;
                nodeList[index]->next =nullptr;
            }
            else{
                Node* contemporaryNode = nodeList[index];
                while(contemporaryNode->next != nullptr){
                    contemporaryNode = contemporaryNode->next;
                }
                contemporaryNode->next = new Node();
                contemporaryNode->next->value = current->value;
                contemporaryNode->next->next = nullptr;
            }
            current = current->next;

        }
    }

    for(int k =0; k< capacity; k++){
        Node* now = hashTable[k];
        while(now != nullptr){
            Node* temp = now;
            now = now->next;
            delete temp;
        }
    }
    delete [] hashTable;
    hashTable = nodeList;
    capacity = capacity*2+1;
    
}

template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    
    if(!contains(element)){
        int index = hashFunction(element)%capacity;
        Node* pNode = new Node();
        pNode->value = element;
        pNode->next = nullptr;
        if(hashTable[index] == nullptr){
            hashTable[index] = pNode;
            
        }
        else{
            Node* current = hashTable[index];
            while(current->next != nullptr){
                current = current->next;
            }
            current->next = pNode;
            
        }
        sz++;
        if((double)sz/capacity >= 0.8){
            rehash();
        }
    }



}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    
    int currentIndex = hashFunction(element)%capacity;
    if(hashTable[currentIndex] == nullptr){
        return false;
    }
    Node*current = hashTable[currentIndex];
    while(current != nullptr){
        if(current->value == element){
            return true;
        }
        current = current->next;
    }
    
    return false;
    
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return sz;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    if(index >= capacity){
        return 0;
    }
    int count = 0;
    Node* current = hashTable[index];
    while(current != nullptr){
        count++;
        current = current->next;
    }
    return count;
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    if(index>=capacity){
        return false;
    }
    Node* current = hashTable[index];
    while(current != nullptr){
        if(current->value == element){
            return true;
        }
        current = current->next;
    }
    
    return false;
}



#endif

