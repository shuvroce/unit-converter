#ifndef CALCS_H
#define CALCS_H

#include <QString>
#include <vector>
#include <memory>

// ===== Base Converter Interface =====
class ConverterBase {
public:
    virtual ~ConverterBase() {}
    virtual QString name() const = 0;
    virtual double toBase(double value) const = 0;
    virtual QString fromBase(double baseValue) const = 0;
};

// ===== Length Converter =====
class LengthConverter : public ConverterBase {
public:
    enum Unit { Meter, Kilometer, Centimeter, Millimeter, Inch, Foot, Yard, Mile };

    LengthConverter(Unit u) : unit(u) {}

    QString name() const override {
        switch(unit) {
            case Meter:      return "Meter";
            case Kilometer:  return "Kilometer";
            case Centimeter: return "Centimeter";
            case Millimeter: return "Millimeter";
            case Inch:       return "Inch";
            case Foot:       return "Foot";
            case Yard:       return "Yard";
            case Mile:       return "Mile";
        }
        return "Unknown";
    }

    double toBase(double value) const override {
        switch(unit) {
            case Meter:      return value;
            case Kilometer:  return value * 1000.0;
            case Centimeter: return value / 100.0;
            case Millimeter: return value / 1000.0;
            case Inch:       return value * 0.0254;
            case Foot:       return value * 0.3048;
            case Yard:       return value * 0.9144;
            case Mile:       return value * 1609.34;
        }
        return value;
    }

    QString fromBase(double baseValue) const override {
        double result = 0.0;
        switch(unit) {
            case Meter:      result = baseValue; break;
            case Kilometer:  result = baseValue / 1000.0; break;
            case Centimeter: result = baseValue * 100.0; break;
            case Millimeter: result = baseValue * 1000.0; break;
            case Inch:       result = baseValue / 0.0254; break;
            case Foot:       result = baseValue / 0.3048; break;
            case Yard:       result = baseValue / 0.9144; break;
            case Mile:       result = baseValue / 1609.34; break;
        }
        return QString::number(result, 'f', 6);
    }

    // --- Fixed for GCC 7.3 ---
    static std::vector<std::unique_ptr<ConverterBase>> allUnits() {
        std::vector<std::unique_ptr<ConverterBase>> v;
        v.push_back(std::make_unique<LengthConverter>(Meter));
        v.push_back(std::make_unique<LengthConverter>(Kilometer));
        v.push_back(std::make_unique<LengthConverter>(Centimeter));
        v.push_back(std::make_unique<LengthConverter>(Millimeter));
        v.push_back(std::make_unique<LengthConverter>(Inch));
        v.push_back(std::make_unique<LengthConverter>(Foot));
        v.push_back(std::make_unique<LengthConverter>(Yard));
        v.push_back(std::make_unique<LengthConverter>(Mile));
        return v;
    }

private:
    Unit unit;
};

// ===== Temperature Converter =====
class TemperatureConverter : public ConverterBase {
public:
    enum Unit { Celsius, Fahrenheit, Kelvin };

    TemperatureConverter(Unit u) : unit(u) {}

    QString name() const override {
        switch(unit) {
            case Celsius:    return "Celsius";
            case Fahrenheit: return "Fahrenheit";
            case Kelvin:     return "Kelvin";
        }
        return "Unknown";
    }

    double toBase(double value) const override {
        switch(unit) {
            case Celsius:    return value;
            case Fahrenheit: return (value - 32.0) * 5.0 / 9.0;
            case Kelvin:     return value - 273.15;
        }
        return value;
    }

    QString fromBase(double baseValue) const override {
        double result = 0.0;
        switch(unit) {
            case Celsius:    result = baseValue; break;
            case Fahrenheit: result = baseValue * 9.0 / 5.0 + 32.0; break;
            case Kelvin:     result = baseValue + 273.15; break;
        }
        return QString::number(result, 'f', 6);
    }

    // --- Fixed for GCC 7.3 ---
    static std::vector<std::unique_ptr<ConverterBase>> allUnits() {
        std::vector<std::unique_ptr<ConverterBase>> v;
        v.push_back(std::make_unique<TemperatureConverter>(Celsius));
        v.push_back(std::make_unique<TemperatureConverter>(Fahrenheit));
        v.push_back(std::make_unique<TemperatureConverter>(Kelvin));
        return v;
    }

private:
    Unit unit;
};

#endif // CALCS_H
