#include "number.h"

void GetBin(uint8_t value, uint8_t* result) {
    int i = 7;
    for (int i = 0; i < 8; i++) {
        result[i] = 0;
    }

    while (value) {
        result[i] = value % 2;
        value /= 2;
        i--;
    }

    return;
}

void BitwiseShiftLeft(uint239_t& value, uint64_t shift) {
    for (int j = 0; j < shift; j++) {
        for (int i = 34; i > -1; i--) {
            value.data[i] = (value.data[i] << 1) & 255; 
            if (i != 34) {
                value.data[i] += ((value.data[i + 1] & 128) >> 7);
                value.data[i + 1] &= 127;
            }
        }
    }

    for (int i = 0; i < 35; i++) {
        value.data[i] &= 127;
    }
    
    return;
}

void BitwiseShiftRight(uint239_t& value, uint64_t shift) {
    uint8_t saved_bit = 0;

    for (int i = 0; i < 35; i++) {
        value.data[i] &= 127;
    }

    for (int j = 0; j < shift; j++) {
        for (int i = 0; i < 35; i++) {
            uint8_t tmp_bit = value.data[i] & 1;
            value.data[i] = (value.data[i] >> 1) + saved_bit * 64; 
            saved_bit = tmp_bit;
        }
    }
    
    return;
}

void ShiftData(uint239_t& value, uint64_t shift_value) {
    uint64_t shift_value_reminder = shift_value % 245;

    for (int i = 0; i < shift_value_reminder; i++) {
        uint8_t saved_bit = value.data[0] & 64;
        BitwiseShiftLeft(value, 1);
        value.data[34] += (saved_bit >> 6);
    }

    int i = 34;
    while (shift_value and i > -1) {
            value.data[i] += (shift_value % 2) << 7;
            shift_value /= 2;
            i--; 
    }

    return;
}

uint239_t GetShiftedToNull (const uint239_t& value) {
    uint239_t result;
    for (int i = 0; i < 35; i++) {
        result.data[i] = value.data[i];
    }
    uint64_t shift = (GetShift(value) % 245);
    
    ShiftData(result, 245 - shift);

    for (int i = 0; i < 35; i++) {
        result.data[i] = result.data[i] & 127;
    }

    return result;
}

void DivideBy2(int* value, int len, int& rem) {
        int reminder = 0;
        int res[len];

        for (int i = 0; i < len; i++) {
            int Carry = reminder * 10 + value[i];
            res[i] = Carry / 2;
            reminder = Carry % 2;
        }

        for (int i = 0; i < len; i++) {
            value[i] = res[i];
        }

        rem = reminder;
}

uint239_t FromInt(uint32_t value, uint32_t shift) {
    uint239_t result;

    for (int i = 0; i < 35; i++) {
        result.data[i] = 0;
    }

    int i = 34;
    while (value) {
        int value_bits = 0;
        if (value) {
            value_bits = value & 127;
            value = value >> 7;
        }

        result.data[i] = (uint8_t)(value_bits);
        i--;
    }

    ShiftData(result, (uint64_t)shift);

    return result;
}

uint239_t FromString(const char* str, uint32_t shift) {
    uint239_t result = FromInt(0, 0);

    if (str == nullptr) {
        return result;
    }

    int len = 0;
    while (str[len] != '\0') {
        len++;
    }

    int big_int[len];
    for (int i = 0; i < len; i++) {
        big_int[i] = static_cast<int>(str[i]) - 48;
    }

    for (int i = 34; i > -1; i--) {
        uint8_t power_of_two = 1;
        int reminder;        
        for (int j = 0; j < 7; j++) {
            DivideBy2(big_int, len, reminder);
            result.data[i] += reminder * power_of_two;
            power_of_two *= 2;
        }
    }

    ShiftData(result, (uint64_t)shift);

    return result;
}

uint239_t operator+(const uint239_t& lhs, const uint239_t& rhs) {
    uint239_t result;
    uint64_t result_shift;
    uint239_t lhs_copy, rhs_copy;
    result_shift = GetShift(lhs) + GetShift(rhs);
    lhs_copy = GetShiftedToNull(lhs);
    rhs_copy = GetShiftedToNull(rhs);

    uint8_t CarryBit = 0;
    for (int i = 34; i > -1; i--) {
        result.data[i] = (lhs_copy.data[i] + rhs_copy.data[i] + CarryBit) & 127;
        CarryBit = ((lhs_copy.data[i] + rhs_copy.data[i] + CarryBit) & 128) >> 7;
    }

    ShiftData(result, result_shift);

    return result;
}

uint239_t operator-(const uint239_t& lhs, const uint239_t& rhs) {
    uint239_t result;
    uint64_t result_shift;
    uint239_t lhs_copy, rhs_copy;
    lhs_copy = GetShiftedToNull(lhs);
    rhs_copy = GetShiftedToNull(rhs);

    if (rhs == FromInt(0, 0)) {
        return lhs;
    }

    result_shift = GetShift(lhs) - GetShift(rhs);
    if (GetShift(rhs) > GetShift(lhs)) {
        result_shift = 34359738368 + GetShift(lhs) - GetShift(rhs); // 2^35 = 34359738368
    }
    else {
        result_shift = GetShift(lhs) - GetShift(rhs);
    }

    rhs_copy.data[0] = 1;
    for (int i = 1; i < 35; i++) {
        rhs_copy.data[i] ^= 127;
    }
    rhs_copy = rhs_copy + FromInt(1, 0);
    
    result = lhs_copy + rhs_copy;
    result.data[0] &= 1;
    ShiftData(result, result_shift);

    return result;
}

uint239_t operator*(const uint239_t& lhs, const uint239_t& rhs) {
    uint239_t result;
    for (int i = 0; i < 35; i++) {
        result.data[i] = 0;
    }
    uint64_t result_shift;
    uint239_t lhs_copy, rhs_copy;

    lhs_copy = GetShiftedToNull(lhs);
    rhs_copy = GetShiftedToNull(rhs);
    result_shift = GetShift(lhs) + GetShift(rhs);

    for (int i = 34; i > -1; i--) {
        uint8_t power_of_two = 1;
        for (int j = 0; j < 7; j++) {
            if ((lhs_copy.data[i] & power_of_two) >> j) {
                result = result + rhs_copy;
            }
            BitwiseShiftLeft(rhs_copy, 1);
            power_of_two *= 2;
        }
    }

    ShiftData(result, result_shift);

    return result;
}

uint239_t operator/(const uint239_t& lhs, const uint239_t& rhs) {
    uint239_t result = FromInt(0, 0);
    uint64_t result_shift;
    uint239_t lhs_copy = GetShiftedToNull(lhs);
    uint239_t rhs_copy = GetShiftedToNull(rhs);

    if (GetShift(rhs) > GetShift(lhs)) {
        result_shift = 34359738368 + GetShift(lhs) - GetShift(rhs); // 2^35 = 34359738368
    }
    else {
        result_shift = GetShift(lhs) - GetShift(rhs);
    }

    if (rhs_copy == FromInt(0, 0)) {
        std::cerr << "division by zero" << std::endl;
        exit(1);
    }
    

    if (rhs_copy > lhs_copy) {
        return FromInt(0, result_shift);
    }

    int shift_count = 1;
    while (lhs_copy > rhs_copy) {
        BitwiseShiftLeft(rhs_copy, 1);
        shift_count++;
    }


    for (int i = 0; i < shift_count; i++) {
        if ((lhs_copy > rhs_copy) or (lhs_copy == rhs_copy)) {
            lhs_copy = lhs_copy - rhs_copy;
            result.data[34] += 1;
        }
        BitwiseShiftRight(rhs_copy, 1);
        BitwiseShiftLeft(result, 1);
    }

    BitwiseShiftRight(result, 1);
    ShiftData(result, result_shift);

    return result;
}

bool operator>(const uint239_t& lhs, const uint239_t& rhs) {
    uint239_t lhs_copy = GetShiftedToNull(lhs);
    uint239_t rhs_copy = GetShiftedToNull(rhs);

    for (int i = 0; i < 35; i++) {
        if (lhs_copy.data[i] > rhs_copy.data[i]) {
            return 1;
        }
        else if (lhs_copy.data[i] != rhs_copy.data[i]) {
            return 0;
        }
    }

    return 0;
}

bool operator==(const uint239_t& lhs, const uint239_t& rhs) {
    uint239_t lhs_copy = GetShiftedToNull(lhs);
    uint239_t rhs_copy = GetShiftedToNull(rhs); 

    for (int i = 34; i > -1; i--) {
        if (lhs_copy.data[i] != rhs_copy.data[i]) {
            return 0;
        }
    }

    return 1;
}

bool operator!=(const uint239_t& lhs, const uint239_t& rhs) {
    if (lhs == rhs) {
        return 0;
    }

    return 1;
}

std::ostream& operator<<(std::ostream& stream, const uint239_t& value) {
    uint8_t byte[8];

    for (int i = 0; i < 35; i++) {
        GetBin(value.data[i], byte);
        for (int j = 0; j < 8; j++) {
            stream << (int)byte[j];
        }
    }
    stream << std::endl;

    return stream;
}

uint64_t GetShift(const uint239_t& value) {
    uint64_t shift = 0;
    uint64_t power_of_two = 1;

    for (int i = 34; i > -1; i--) {
        shift += (uint64_t)(((value.data[i] & 128) >> 7) * power_of_two);
        power_of_two *= 2;
    }

    return shift;
}
