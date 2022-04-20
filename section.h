#ifndef SECTION_H
#define SECTION_H

#include <stdint.h>
#include <QWidget>
#include <vector>


enum class EditSectionEnum{
    NoSelection,
    Tab,
    Lyric,
    Chord,
    Comment
};


class SectionInfo{

public:
    SectionInfo(EditSectionEnum sectionType) : section(sectionType){};

    EditSectionEnum getSectionType(){return section;};
    uint32_t getSectionID(){return sectionID;};
    void setSectionType(EditSectionEnum type){ section = type;};
    void setSectionID(uint32_t newID) {sectionID = newID;};

    SectionInfo& operator=(SectionInfo other){
        section = other.section;
        sectionID = other.sectionID;
        return *this;
    };

private:

    EditSectionEnum section;
    uint32_t sectionID;

};

#endif // SECTION_H

