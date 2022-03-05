#ifndef SECTIONMANAGER_H
#define SECTIONMANAGER_H

#include "lyricobject.h"
#include "chordlayout.h"

class SectionManager{

public:
    SectionManager(){ dataCount = 0; lastAddedData = nullptr; lastAddedSecInfo = EditSectionEnum::NoSelection;};
    void AddSection(ChordLayout* data);
    void AddSection(LyricObject* data);

    void RemoveSection(EditSectionEnum sectionType, void* data);
    void RemoveSection(EditSectionEnum sectionType, uint32_t index);

    void* getData(EditSectionEnum sectionType, uint32_t index);

    EditSectionEnum getLastAddedSecitonInfo();
    void* getLastAdded();

    const std::vector<ChordLayout*> getChordList();
    const std::vector<LyricObject*> getLyricList();


    uint32_t getCount(EditSectionEnum sectionType);

private:

    std::vector<ChordLayout*> chordList;
    std::vector<LyricObject*> lyricList;

    uint32_t dataCount; //HATA OLABİLİR DOĞRU AZALTMADI
    void* lastAddedData; //HATA OLABİLİR DOĞRU DEĞİŞİKLİK YAPILMADI
    EditSectionEnum lastAddedSecInfo; //HATA OLABİLİR DOĞRU DEĞİŞİKLİK YAPILMADI


};


#endif
