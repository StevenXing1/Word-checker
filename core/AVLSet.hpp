// AVLSet.hpp
//
// ICS 46 Winter 2022
// Project #4: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.hpp"






template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    ~AVLSet() noexcept override;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const noexcept;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;







private:
    // You'll no doubt want to add member variables and "helper" member
    // functions here.
    struct Node
    {
        ElementType value;
        int height;
        Node* left;
        Node* right;
    };
    
    void clear(Node* n);

    void copy(Node* n);

    void assignValue(const ElementType& element, Node* n);
    
    bool containValue(const ElementType& element, Node* n) const;

    int checkHeight(Node* n) const noexcept;

    void preorderVisit(VisitFunction visit, Node* n) const;

    void inorderVisit(VisitFunction visit, Node* n) const;

    void postorderVisit(VisitFunction visit, Node* n) const;

    void balanceTree(Node* n) noexcept;




    Node* start;
    Node* prev;
    
    bool balance;
    int sz;

};

template <typename ElementType>
void AVLSet<ElementType>::copy(Node* n)
{
    if (n != nullptr){
        add(n->value);
        if(n->left != nullptr){
            copy(n->left);
        }
        if(n->right != nullptr){
            copy(n->right);
        }
    }
}

template <typename ElementType>
void AVLSet<ElementType>::clear(Node* n)
{
    if(n != nullptr){
        if(n->left != nullptr){
            clear(n->left);
        }
        if(n->right != nullptr){
            clear(n->right);
        }
        delete n;
    }
    
    
}

template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
                    :start{nullptr}, prev{nullptr},balance{shouldBalance},sz{0}
{
}



template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
    clear(start);
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
                    //: start{nullptr}, prev{nullptr},balance{s.balance}, sz{0}
{
    //strong guarantee
    AVLSet<ElementType> mySet(s.balance);
    try{
        mySet.copy(s.start);
    }
    catch(...){
        throw;
    }

    start = nullptr;
    prev = nullptr;
    balance = s.balance;
    sz = 0;
    copy(s.start);
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
                    :start{nullptr}, prev{nullptr}, balance{s.balance}, sz{0}
{
    Node* tempStart = s.start;
    Node* tempPrev = s.prev;
    bool tempBalance = s.balance;
    int tempSize = s.sz;



    s.start = start;
    s.prev = prev;
    s.balance = balance;
    s.sz = sz;

    prev = tempPrev;
    start = tempStart;
    balance = tempBalance;
    sz = tempSize;

}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    if(this != &s){
        AVLSet<ElementType> mySet(s.balance);
        try{
            mySet.copy(s.start);
        }
        catch(...){
            throw;
        }
        clear(start);
        start = nullptr;
        prev = nullptr;
        balance = s.balance;
        sz = 0;
        copy(s.start);
    }
    return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    Node* tempStart = s.start;
    Node* tempPrev = s.prev;
    bool tempBalance = s.balance;
    int tempSize = s.sz;



    s.start = start;
    s.prev = prev;
    s.balance = balance;
    s.sz = sz;

    prev = tempPrev;
    start = tempStart;
    balance = tempBalance;
    sz = tempSize;

    return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
    return true;
}

template <typename ElementType>
void AVLSet<ElementType>::balanceTree(Node* n) noexcept
{
    if(n != nullptr){
        if(n->left != nullptr && (n->right == nullptr || n->left->height-n->right->height > 1)&& n->left->height>1){
            
            if(n->left->left != nullptr && (n->left->right == nullptr || n->left->left->height> n->left->right->height)){      //LL rotation
                
                if(n == start){
                    start = n->left;
                    n->left = n->left->right;
                    start->right = n;

                    int leftValue = start->right->left==nullptr?0:start->right->left->height;
                    int rightValue = start->right->right ==nullptr?0:start->right->right->height;
                    start->right->height = (leftValue>rightValue?leftValue:rightValue)+1;
                    start->height = start->right->height+1;
                    
                }
                else if(prev != nullptr && prev->left == n){
                    prev->left = n->left;
                    n->left = n->left->right;
                    prev->left->right = n;

                    int leftValue = prev->left->right->left==nullptr?0:prev->left->right->left->height;
                    int rightValue = prev->left->right->right ==nullptr?0:prev->left->right->right->height;
                    prev->left->right->height = (leftValue>rightValue?leftValue:rightValue)+1;
                    prev->left->height = prev->left->right->height+1;

                }
                else{
                    prev->right = n->left;
                    n->left = n->left->right;
                    prev->right->right = n;

                    int leftValue = prev->right->right->left==nullptr?0:prev->right->right->left->height;
                    int rightValue = prev->right->right->right ==nullptr?0:prev->right->right->right->height;
                    prev->right->right->height = (leftValue>rightValue?leftValue:rightValue)+1;
                    prev->right->height = prev->right->right->height+1;
                }
            }
            else{                            //LR rotation

                if(n == start){ 
                    start = n->left->right;
                    n->left->right = start->left;
                    start->left = n->left;
                    n->left = start->right;
                    start->right = n;

                    int leftValue = start->left->left == nullptr?0:start->left->left->height;
                    int rightValue = start->left->right == nullptr?0:start->left->right->height;
                    start->left->height = (leftValue>rightValue?leftValue:rightValue)+1;

                    int lValue = start->right->left == nullptr?0:start->right->left->height;
                    int rValue = start->right->right == nullptr?0:start->right->right->height;
                    start->right->height = (lValue>rValue?lValue:rValue)+1;

                    start->height = (start->right->height>start->left->height?start->right->height:start->left->height)+1;

                }
                else if(prev != nullptr && prev->left == n){         //Previous node's left substree needs to be rotated
                    prev->left = n->left->right;
                    n->left->right = prev->left->left;
                    prev->left->left = n->left;
                    n->left = prev->left->right;
                    prev->left->right = n;

                    int leftValue = prev->left->left->left == nullptr?0:prev->left->left->left->height;
                    int rightValue = prev->left->left->right == nullptr?0:prev->left->left->right->height;
                    prev->left->left->height = (leftValue>rightValue?leftValue:rightValue)+1;

                    int lValue = prev->left->right->left == nullptr?0:prev->left->right->left->height;
                    int rValue = prev->left->right->right == nullptr?0:prev->left->right->right->height;
                    prev->left->right->height = (lValue>rValue?lValue:rValue)+1;
                    prev->left->height = (prev->left->right->height>prev->left->left->height?prev->left->right->height:prev->left->left->height)+1;
                }
                else{                                          //Previous node's right substree needs to be rotated
                    prev->right = n->left->right;
                    n->left->right = prev->right->left;
                    
                    prev->right->left = n->left;
                    
                    n->left = prev->right->right;
                    
                    prev->right->right = n;

                    int leftValue = prev->right->left->left == nullptr?0:prev->right->left->left->height;
                    int rightValue = prev->right->left->right == nullptr?0:prev->right->left->right->height;
                    prev->right->left->height = (leftValue>rightValue?leftValue:rightValue)+1;

                    int lValue = prev->right->right->left == nullptr?0:prev->right->right->left->height;
                    int rValue = prev->right->right->right == nullptr?0:prev->right->right->right->height;
                    prev->right->right->height = (lValue>rValue?lValue:rValue)+1;
                    
 
                    prev->right->height = (prev->right->right->height>prev->right->left->height?prev->right->right->height:prev->right->left->height)+1;

                }
            }
            
        }
        else if(n->right != nullptr && (n->left == nullptr||n->right->height-n->left->height>1) && n->right->height>1){
            
            if(n->right->right != nullptr && (n->right->left == nullptr || n->right->right->height > n->right->left->height)){      //RR rotation
                
                if(n==start){
                    start = n->right;
                    n->right = n->right->left;
                    start->left = n;

                    int leftValue = start->left->left==nullptr?0:start->left->left->height;
                    int rightValue = start->left->right ==nullptr?0:start->left->right->height;
                    start->left->height = (leftValue>rightValue?leftValue:rightValue)+1;
                    start->height = start->left->height+1;

                }
                else if(prev != nullptr && prev->left == n){ //Previous node's left substree needs to be rotated

                    prev->left = n->right;
                    n->right = n->right->left;
                    prev->left->left = n;

                    int leftValue = prev->left->left->left==nullptr?0:prev->left->left->left->height;
                    int rightValue = prev->left->left->right ==nullptr?0:prev->left->left->right->height;
                    prev->left->left->height = (leftValue>rightValue?leftValue:rightValue)+1;
                    prev->left->height = prev->left->left->height+1;

                }
                else{
                    prev->right = n->right;
                    n->right = n->right->left;
                    prev->right->left = n;

                    int leftValue = prev->right->left->left==nullptr?0:prev->right->left->left->height;
                    int rightValue = prev->right->left->right ==nullptr?0:prev->right->left->right->height;
                    prev->right->left->height = (leftValue>rightValue?leftValue:rightValue)+1;
                    prev->right->height = prev->right->left->height+1;

                }
            }else{                            //RL rotation
                if(n==start){
                    
                    start = n->right->left;
                    n->right->left = start->right;
                    start->right = n->right;
                    n->right = start->left;
                    start->left = n;

                    int leftValue = start->right->left==nullptr?0:start->right->left->height;
                    int rightValue = start->right->right==nullptr?0:start->right->right->height;
                    start->right->height = (leftValue>rightValue?leftValue:rightValue)+1;

                    int lValue = start->left->left == nullptr?0:start->left->left->height;
                    int rValue = start->left->right == nullptr?0:start->left->right->height;
                    start->left->height = (lValue>rValue?lValue:rValue)+1;
                    start->height = (start->right->height>start->left->height?start->right->height:start->left->height)+1;


                }
                else if(prev != nullptr && prev->left == n){
                    
                    prev->left = n->right->left;
                    n->right->left = prev->left->right;
                    prev->left->right = n->right;
                    n->right = prev->left->left;
                    prev->left->left = n;

                    int leftValue = prev->left->right->left==nullptr?0:prev->left->right->left->height;
                    int rightValue = prev->left->right->right==nullptr?0:prev->left->right->right->height;
                    prev->left->right->height = (leftValue>rightValue?leftValue:rightValue)+1;

                    int lValue = prev->left->left->left == nullptr?0:prev->left->left->left->height;
                    int rValue = prev->left->left->right == nullptr?0:prev->left->left->right->height;
                    prev->left->left->height = (lValue>rValue?lValue:rValue)+1;
                    prev->left->height = (prev->left->right->height>prev->left->left->height?prev->left->right->height:prev->left->left->height)+1;
                }
                else{
                     
                    prev->right = n->right->left;
                    
                    n->right->left = prev->right->right;
                    prev->right->right = n->right;
                    n->right = prev->right->left;
                    prev->right->left = n;
                    
                    int leftValue = prev->right->right->left==nullptr?0:prev->right->right->left->height;
                    int rightValue = prev->right->right->right==nullptr?0:prev->right->right->right->height;
                    prev->right->right->height = (leftValue>rightValue?leftValue:rightValue)+1;

                    int lValue = prev->right->left->left == nullptr?0:prev->right->left->left->height;
                    int rValue = prev->right->left->right == nullptr?0:prev->right->left->right->height;
                    prev->right->left->height = (lValue>rValue?lValue:rValue)+1;
                    prev->right->height = (prev->right->right->height>prev->right->left->height?prev->right->right->height:prev->right->left->height)+1;
                }
            }
        }

    }
    
 

}

template <typename ElementType>
void AVLSet<ElementType>::assignValue(const ElementType& element, Node* n){
    bool flag = false;
    if(element > n->value){
        flag = true;
        if(n->right == nullptr){
            n->right = new Node;
            n->right->value = element;
            n->right->right = nullptr;
            n->right->left = nullptr;
            n->right->height = 1;
            sz++;
        }
        else{
            prev = n;
            assignValue(element, n->right);
        }
        
    }
    else if(element < n->value){
        if(n->left == nullptr){
            n->left = new Node;
            n->left->value = element;
            n->left->right = nullptr;
            n->left->left = nullptr;
            n->left->height = 1;
            sz++;
        }
        else{
            prev = n;
            assignValue(element, n->left);
        }
    }
    n->height = checkHeight(n);

    if(balance){
        prev = n;
        if(flag){
            balanceTree(n->right);
        }
        else{
            balanceTree(n->left);
        }
        balanceTree(start);
    }
    
    

}

template <typename ElementType>
bool AVLSet<ElementType>::containValue(const ElementType& element, Node* n) const{
    if(n == nullptr){
        return false;
    }
    if(element > n->value){
        return containValue(element, n->right);
    }
    else if(element < n->value){
        return containValue(element, n->left);
    }
    else{
        return true;
    }
}

template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
    if(start == nullptr){
        start = new Node;
        start->value = element;
        start->left = nullptr;
        start->right = nullptr;
        start->height = 0;
        sz++;
        
    }
    else if(!contains(element)){
        
        assignValue(element, start);

    }



}


template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
    return containValue(element, start);
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return sz;
}

template <typename ElementType>
int AVLSet<ElementType>::checkHeight(Node* n) const noexcept{
    if(n == nullptr){
        return 0;
    }
    else{
        //return (checkHeight(n->left)>checkHeight(n->right)?checkHeight(n->left):checkHeight(n->right))+1;
        if(n->left == nullptr && n->right == nullptr){
            return 1;
        }
        else if(n->left == nullptr){
            return n->right->height+1;
        }
        else if(n->right == nullptr){
            return n->left->height+1;
        }
        else{
            return (n->left->height>n->right->height?n->left->height:n->right->height)+1;
        }
        
    }
    
}

template <typename ElementType>
int AVLSet<ElementType>::height() const noexcept
{

    return checkHeight(start)-1;
}

template <typename ElementType>
void AVLSet<ElementType>::preorderVisit(VisitFunction visit, Node* n) const
{
    if( n!= nullptr){
        visit(n->value);
        preorderVisit(visit, n->left);
        preorderVisit(visit, n->right);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
    preorderVisit(visit, start);
}

template <typename ElementType>
void AVLSet<ElementType>::inorderVisit(VisitFunction visit, Node* n) const{
    if(n!= nullptr){
        inorderVisit(visit, n->left);
        visit(n->value);
        inorderVisit(visit, n->right);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
    inorderVisit(visit, start);
}

template <typename ElementType>
void AVLSet<ElementType>::postorderVisit(VisitFunction visit, Node* n) const{
    if(n!= nullptr){
        postorderVisit(visit, n->left);
        postorderVisit(visit, n->right);
        visit(n->value);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    postorderVisit(visit, start);
}



#endif

