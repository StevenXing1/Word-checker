// WordChecker.cpp
//
// ICS 46 Winter 2022
// Project #4: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"



WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
    if(words.contains(word)){
        return true;
    }
    else{
        return false;
    }
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
    // Nonsensical code because the compiler requires the member variables
    // 'words' to be used somewhere, or else it becomes a warning (which
    // turns into an error).
    std::vector<std::string> result;
    std::set<std::string> tempSet;
    
    for(int i=0; i<word.size()-1; i++){
        //First technique
        std::string newWord = word.substr(0, i)+word[i+1]+word[i]+word.substr(i+2);
        if(wordExists(newWord)){
            tempSet.insert(newWord);
        }



    }
    
    //Second technique
    for(int k=0; k<=word.size();k++){
        for(char j = 'A'; j<='Z';j++){
            std::string myWord = word;
            
            std::string testWord = myWord.insert(k, 1, j);
            if(wordExists(testWord)){
                tempSet.insert(testWord);
            }
        }
    }

    //Third technique
    for(int m = 0; m<word.size(); m++){
        std::string deleteWord = word;
        deleteWord.erase(m,1);
        if(wordExists(deleteWord)){
            tempSet.insert(deleteWord);
        }
    }
    
    //Fourth technique
    for(int n=0; n<=word.size();n++){
        for(char  l= 'A'; l<='Z';l++){
            std::string word4 = word;
            std::string testWord4 = word4.replace(n, 1, 1, l);
            if(wordExists(testWord4)){
                tempSet.insert(word4);
            }
        }
    }
    
    //Fifth technique
    for(int a= 1; a<word.size();a++){
        std::string word5 = word;
        std::string testWord5 = word5.insert(a, 1, ' ');
        if(wordExists(testWord5.substr(0, a)) && wordExists(testWord5.substr(a+1))){
            tempSet.insert(testWord5);
        }
    }
    for(auto iterator = tempSet.begin(); iterator != tempSet.end(); iterator++){
        result.push_back(*iterator);
    }
    return result;
}

