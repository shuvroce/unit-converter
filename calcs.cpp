#include "calcs.h"

// ===== Length Converter =====
QString LengthConverter::name() const {
    return "Meters → Feet";
}
QString LengthConverter::convert(double value) {
    double feet = value * 3.28084;
    return QString::number(feet) + " ft";
}

// ===== Temperature Converter =====
QString TemperatureConverter::name() const {
    return "Celsius → Fahrenheit";
}
QString TemperatureConverter::convert(double value) {
    double fahrenheit = (value * 9/5) + 32;
    return QString::number(fahrenheit) + " °F";
}
