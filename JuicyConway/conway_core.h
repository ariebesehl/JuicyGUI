#ifndef CONWAY_CORE_H_INCLUDED
#define CONWAY_CORE_H_INCLUDED

#define CONWAYGOL_VALUE_EMPTY 0x0
#define CONWAYGOL_VALUE_ALIVE 0xffffffff

class ConwayGOL {
    public:
        ConwayGOL(unsigned int size_x, unsigned int size_y);
        ~ConwayGOL();
        unsigned int Iterate();
        unsigned int** GetDataPtr() {return &dataPtr;};
        unsigned int FlipCell(unsigned int iX, unsigned int iY);
        unsigned int SetCellValue(unsigned int iX, unsigned int iY, unsigned int iValue);
        unsigned int GetCellValue(unsigned int iX, unsigned int iY);
        unsigned int SetCellAlive(unsigned int iX, unsigned int iY) {return SetCellValue(iX, iY, valAlive);};
        unsigned int SetCellEmpty(unsigned int iX, unsigned int iY) {return SetCellValue(iX, iY, valEmpty);};
        unsigned int GetValueEmpty(void) {return valEmpty;};
        unsigned int GetValueAlive(void) {return valAlive;};
        unsigned int SetValueEmpty(unsigned int iValue) {valEmpty = iValue; return valEmpty;};
        unsigned int SetValueAlive(unsigned int iValue) {valAlive = iValue; return valAlive;};
        unsigned int CountAlive();
    private:
        bool checkInput(unsigned int iX, unsigned int iY);
        unsigned int valEmpty;
        unsigned int valAlive;
        unsigned int* dataPtr;
        unsigned int size_x;
        unsigned int size_y;
        unsigned int* buf0;
        unsigned int* buf1;
};


#endif // CONWAY_CORE_H_INCLUDED
