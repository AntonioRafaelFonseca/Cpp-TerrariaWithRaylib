#pragma once

struct SlotIndex
{
private:
    int value = 0;

    // Função auxiliar para garantir que o valor fique SEMPRE entre 0 e 5
    void wrap()
    {
        // Se for positivo, o % 6 resolve (ex: 6 % 6 vira 0)
        // Se for negativo, somamos 6 para rodar o inverso (ex: -1 vira 5)
        value = (value % 5 + 5) % 5;
    }

public:
    // Construtor padrão
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

    // Sobrecarga do operador -- (ex: selected--)
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

    // Sobrecarga do operador = (permite atribuir um int direto)
    SlotIndex& operator=(int val) {
        value = val;
        wrap();
        return *this;
    }
};
