#pragma once

struct SlotIndex
{
private:
    int value = 0;

    void wrap()
    {
        value = (value % 5 + 5) % 5;
    }

public:
    SlotIndex() : value(0) {}
    SlotIndex(int val) : value(val) { wrap(); }

    operator int() const { return value; }

    SlotIndex& operator++() { // ++var
        value++;
        wrap();
        return *this;
    }
    SlotIndex operator++(int) { // var++
        SlotIndex temp = *this;
        value++;
        wrap();
        return temp;
    }

    SlotIndex& operator--() { // --var
        value--;
        wrap();
        return *this;
    }
    SlotIndex operator--(int) { // var--
        SlotIndex temp = *this;
        value--;
        wrap();
        return temp;
    }

    SlotIndex& operator=(int val) {
        value = val;
        wrap();
        return *this;
    }
};


struct LayerIndex
{
public:
        unsigned char data = 0b00000001; // 1=front layer 0 = backlayer
        void wrap()
        {
            data = data & 1;
        }

    LayerIndex& operator+=(int v) {
        data+=v;
        wrap();
        return *this;
    }
    LayerIndex& operator=(unsigned char v) {
        data=v;
        wrap();
        return *this;
    }
    bool operator==(char b) {
        if(b == 'f' && data == 0b00000001)
        {
            return true;
        }
        else if (b == 'b' && data == 0b00000000)
        {
            return true;
        }
        return false;
    }
    
};