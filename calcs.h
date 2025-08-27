#ifndef CALCS_H
#define CALCS_H

#include <QString>

// ===== Base Converter Interface =====
class ConverterBase {
public:
    virtual ~ConverterBase() {}
    virtual QString name() const = 0;        // Dropdown name
    virtual QString convert(double value) = 0; // Conversion logic
};

// ===== Length Converter =====
class LengthConverter : public ConverterBase {
public:
    QString name() const override;
    QString convert(double value) override;
};

// ===== Temperature Converter =====
class TemperatureConverter : public ConverterBase {
public:
    QString name() const override;
    QString convert(double value) override;
};

#endif // CALCS_H
