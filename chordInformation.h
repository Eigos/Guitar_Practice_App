#ifndef CHORDINFORMATION_H
#define CHORDINFORMATION_H

#include <string>

struct ChordInformation{
    std::string key;
    std::string suffix;
    int32_t chordPosition;

    void ResetInformation(){
        suffix.clear();
        key.clear();
        chordPosition = -1;
    }
};

#endif // CHORDINFORMATION_H
