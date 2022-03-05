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
    void setSectionID(uint32_t newID) {sectionID = newID;};
    uint32_t getSectionID(){return sectionID;};

private:

    EditSectionEnum section;
    uint32_t sectionID;

};

#endif // SECTION_H

