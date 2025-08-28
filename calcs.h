#ifndef CALCS_H
#define CALCS_H

#include <QString>
#include <vector>
#include <memory>

// ===== Base Converter Interface =====
class ConverterBase {
public:
    virtual ~ConverterBase() {}
    virtual QString name() const = 0;           // Dropdown / abbreviation
    virtual double toBase(double value) const = 0; 
    virtual QString fromBase(double baseValue) const = 0;
};

// ===== Length =====
class LengthConverter : public ConverterBase {
public:
    enum Unit { MM, CM, M, KM, IN, FT, MILE, YARD };
    LengthConverter(Unit u) : unit(u) {}

    QString name() const override {
        switch(unit) {
            case MM:   return "mm";
            case CM:   return "cm";
            case M:    return "m";
            case KM:   return "km";
            case IN:   return "in";
            case FT:   return "ft";
            case MILE: return "mile";
            case YARD: return "yard";
        }
        return "Unknown";
    }

    double toBase(double value) const override { // base = meters
        double result = 0.0;
        switch(unit) {
            case MM:   result = value / 1000.0; break;
            case CM:   result = value / 100.0; break;
            case M:    result = value; break;
            case KM:   result = value * 1000.0; break;
            case IN:   result = value * 0.0254; break;
            case FT:   result = value * 0.3048; break;
            case YARD: result = value * 0.9144; break;
            case MILE: result = value * 1609.34; break;
        }
        return result;
    }

    QString fromBase(double baseValue) const override {
        double result = 0.0;
        switch(unit) {
            case MM:   result = baseValue * 1000.0; break;
            case CM:   result = baseValue * 100.0; break;
            case M:    result = baseValue; break;
            case KM:   result = baseValue / 1000.0; break;
            case IN:   result = baseValue / 0.0254; break;
            case FT:   result = baseValue / 0.3048; break;
            case YARD: result = baseValue / 0.9144; break;
            case MILE: result = baseValue / 1609.34; break;
        }
        return QString::number(result, 'f', 3);
    }

    static std::vector<std::unique_ptr<ConverterBase>> allUnits() {
        std::vector<std::unique_ptr<ConverterBase>> v;
        v.push_back(std::make_unique<LengthConverter>(MM));
        v.push_back(std::make_unique<LengthConverter>(CM));
        v.push_back(std::make_unique<LengthConverter>(M));
        v.push_back(std::make_unique<LengthConverter>(KM));
        v.push_back(std::make_unique<LengthConverter>(IN));
        v.push_back(std::make_unique<LengthConverter>(FT));
        v.push_back(std::make_unique<LengthConverter>(MILE));
        v.push_back(std::make_unique<LengthConverter>(YARD));
        return v;
    }

private:
    Unit unit;
};

// ===== Temperature =====
class TemperatureConverter : public ConverterBase {
public:
    enum Unit { C, F, K };
    TemperatureConverter(Unit u) : unit(u) {}

    QString name() const override {
        switch(unit) {
            case C: return "°C";
            case F: return "°F";
            case K: return "K";
        }
        return "Unknown";
    }

    double toBase(double value) const override { // base = Celsius
        double result = 0.0;
        switch(unit) {
            case C: result = value; break;
            case F: result = (value - 32.0) * 5.0 / 9.0; break;
            case K: result = value - 273.15; break;
        }
        return result;
    }

    QString fromBase(double baseValue) const override {
        double result = 0.0;
        switch(unit) {
            case C: result = baseValue; break;
            case F: result = baseValue * 9.0 / 5.0 + 32.0; break;
            case K: result = baseValue + 273.15; break;
        }
        return QString::number(result, 'f', 3);
    }

    static std::vector<std::unique_ptr<ConverterBase>> allUnits() {
        std::vector<std::unique_ptr<ConverterBase>> v;
        v.push_back(std::make_unique<TemperatureConverter>(C));
        v.push_back(std::make_unique<TemperatureConverter>(F));
        v.push_back(std::make_unique<TemperatureConverter>(K));
        return v;
    }

private:
    Unit unit;
};

// ===== Velocity =====
class VelocityConverter : public ConverterBase {
public:
    enum Unit { MPH, KMPH, MS, FTS };
    VelocityConverter(Unit u) : unit(u) {}

    QString name() const override {
        switch(unit) {
            case MPH:  return "mph";
            case KMPH: return "km/h";
            case MS:   return "m/s";
            case FTS:  return "ft/s";
        }
        return "Unknown";
    }

    double toBase(double value) const override { // base = m/s
        double result = 0.0;
        switch(unit) {
            case MPH:  result = value * 0.44704; break;
            case KMPH: result = value / 3.6; break;
            case MS:   result = value; break;
            case FTS:  result = value * 0.3048; break;
        }
        return result;
    }

    QString fromBase(double baseValue) const override {
        double result = 0.0;
        switch(unit) {
            case MPH:  result = baseValue / 0.44704; break;
            case KMPH: result = baseValue * 3.6; break;
            case MS:   result = baseValue; break;
            case FTS:  result = baseValue / 0.3048; break;
        }
        return QString::number(result, 'f', 3);
    }

    static std::vector<std::unique_ptr<ConverterBase>> allUnits() {
        std::vector<std::unique_ptr<ConverterBase>> v;
        v.push_back(std::make_unique<VelocityConverter>(MPH));
        v.push_back(std::make_unique<VelocityConverter>(KMPH));
        v.push_back(std::make_unique<VelocityConverter>(MS));
        v.push_back(std::make_unique<VelocityConverter>(FTS));
        return v;
    }

private:
    Unit unit;
};

// ===== Force =====
class ForceConverter : public ConverterBase {
public:
    enum Unit { N, KN, KGF, TONF, LB, KIP };
    ForceConverter(Unit u) : unit(u) {}

    QString name() const override {
        switch(unit) {
            case N:    return "N";
            case KN:   return "kN";
            case KGF:  return "kgf";
            case TONF: return "tonf";
            case LB:   return "lb";
            case KIP:  return "kip";
        }
        return "Unknown";
    }

    double toBase(double value) const override { // base = N
        double result = 0.0;
        switch(unit) {
            case N:    result = value; break;
            case KN:   result = value * 1000.0; break;
            case KGF:  result = value * 9.80665; break;
            case TONF: result = value * 9806.65; break;
            case LB:   result = value * 4.44822; break;
            case KIP:  result = value * 4448.22; break;
        }
        return result;
    }

    QString fromBase(double baseValue) const override {
        double result = 0.0;
        switch(unit) {
            case N:    result = baseValue; break;
            case KN:   result = baseValue / 1000.0; break;
            case KGF:  result = baseValue / 9.80665; break;
            case TONF: result = baseValue / 9806.65; break;
            case LB:   result = baseValue / 4.44822; break;
            case KIP:  result = baseValue / 4448.22; break;
        }
        return QString::number(result, 'f', 3);
    }

    static std::vector<std::unique_ptr<ConverterBase>> allUnits() {
        std::vector<std::unique_ptr<ConverterBase>> v;
        v.push_back(std::make_unique<ForceConverter>(N));
        v.push_back(std::make_unique<ForceConverter>(KN));
        v.push_back(std::make_unique<ForceConverter>(KGF));
        v.push_back(std::make_unique<ForceConverter>(TONF));
        v.push_back(std::make_unique<ForceConverter>(LB));
        v.push_back(std::make_unique<ForceConverter>(KIP));
        return v;
    }

private:
    Unit unit;
};

// ===== Moment =====
class MomentConverter : public ConverterBase {
public:
    enum Unit { N_M, N_MM, KN_M, KN_MM, LB_IN, LB_FT, KIP_IN, KIP_FT, KGF_M, KGF_MM, KGF_IN, KGF_FT };
    MomentConverter(Unit u) : unit(u) {}

    QString name() const override {
        switch(unit) {
            case N_M:    return "N-m";    case N_MM:    return "N-mm";
            case KN_M:   return "kN-m";   case KN_MM:   return "kN-mm";
            case LB_IN:  return "lb-in";  case LB_FT:   return "lb-ft";
            case KIP_IN: return "kip-in"; case KIP_FT:  return "kip-ft";
            case KGF_M:  return "kgf-m";  case KGF_MM:  return "kgf-mm";
            case KGF_IN: return "kgf-in"; case KGF_FT:  return "kgf-ft";
        }
        return "Unknown";
    }

    double toBase(double value) const override { // base = N-m
        double result = 0.0;
        switch(unit) {
            case N_M:    result = value; break;
            case N_MM:   result = value / 1000.0; break;
            case KN_M:   result = value * 1000.0; break;
            case KN_MM:  result = value; break;
            case LB_IN:  result = value * 0.113; break;
            case LB_FT:  result = value * 1.356; break;
            case KIP_IN: result = value * 113.0; break;
            case KIP_FT: result = value * 1356.0; break;
            case KGF_M:  result = value * 9.80665; break;
            case KGF_MM: result = value * 0.00980665; break;
            case KGF_IN: result = value * 0.8139; break;
            case KGF_FT: result = value * 9.766; break;
        }
        return result;
    }

    QString fromBase(double baseValue) const override {
        double result = 0.0;
        switch(unit) {
            case N_M:    result = baseValue; break;
            case N_MM:   result = baseValue * 1000.0; break;
            case KN_M:   result = baseValue / 1000.0; break;
            case KN_MM:  result = baseValue; break;
            case LB_IN:  result = baseValue / 0.113; break;
            case LB_FT:  result = baseValue / 1.356; break;
            case KIP_IN: result = baseValue / 113.0; break;
            case KIP_FT: result = baseValue / 1356.0; break;
            case KGF_M:  result = baseValue / 9.80665; break;
            case KGF_MM: result = baseValue / 0.00980665; break;
            case KGF_IN: result = baseValue / 0.8139; break;
            case KGF_FT: result = baseValue / 9.766; break;
        }
        return QString::number(result, 'f', 3);
    }

    static std::vector<std::unique_ptr<ConverterBase>> allUnits() {
        std::vector<std::unique_ptr<ConverterBase>> v;
        v.push_back(std::make_unique<MomentConverter>(N_M));
        v.push_back(std::make_unique<MomentConverter>(N_MM));
        v.push_back(std::make_unique<MomentConverter>(KN_M));
        v.push_back(std::make_unique<MomentConverter>(KN_MM));
        v.push_back(std::make_unique<MomentConverter>(LB_IN));
        v.push_back(std::make_unique<MomentConverter>(LB_FT));
        v.push_back(std::make_unique<MomentConverter>(KIP_IN));
        v.push_back(std::make_unique<MomentConverter>(KIP_FT));
        v.push_back(std::make_unique<MomentConverter>(KGF_M));
        v.push_back(std::make_unique<MomentConverter>(KGF_MM));
        v.push_back(std::make_unique<MomentConverter>(KGF_IN));
        v.push_back(std::make_unique<MomentConverter>(KGF_FT));
        return v;
    }

private:
    Unit unit;
};

// ===== Pressure =====
class PressureConverter : public ConverterBase {
public:
    enum Unit { PA, KPA, MPA, PSI, KSI, PSF, KSF };
    PressureConverter(Unit u) : unit(u) {}

    QString name() const override {
        switch(unit) {
            case PA:  return "Pa (N/m²)";
            case KPA: return "kPa (kN/m²)";
            case MPA: return "MPa (N/mm²)";
            case PSI: return "psi (lb/in²)";
            case KSI: return "ksi (kip/in²)";
            case PSF: return "psf (lb/ft²)";
            case KSF: return "ksf (kip/ft²)";
        }
        return "Unknown";
    }

    double toBase(double value) const override { // base = Pa
        double result = 0.0;
        switch(unit) {
            case PA:  result = value; break;
            case KPA: result = value * 1000.0; break;
            case MPA: result = value * 1e6; break;
            case PSI: result = value * 6894.76; break;
            case KSI: result = value * 6.89476e6; break;
            case PSF: result = value * 47.8803; break;
            case KSF: result = value * 47880.3; break;
        }
        return result;
    }

    QString fromBase(double baseValue) const override {
        double result = 0.0;
        switch(unit) {
            case PA:  result = baseValue; break;
            case KPA: result = baseValue / 1000.0; break;
            case MPA: result = baseValue / 1e6; break;
            case PSI: result = baseValue / 6894.76; break;
            case KSI: result = baseValue / 6.89476e6; break;
            case PSF: result = baseValue / 47.8803; break;
            case KSF: result = baseValue / 47880.3; break;
        }
        return QString::number(result, 'f', 3);
    }

    static std::vector<std::unique_ptr<ConverterBase>> allUnits() {
        std::vector<std::unique_ptr<ConverterBase>> v;
        v.push_back(std::make_unique<PressureConverter>(PA));
        v.push_back(std::make_unique<PressureConverter>(KPA));
        v.push_back(std::make_unique<PressureConverter>(MPA));
        v.push_back(std::make_unique<PressureConverter>(PSI));
        v.push_back(std::make_unique<PressureConverter>(KSI));
        v.push_back(std::make_unique<PressureConverter>(PSF));
        v.push_back(std::make_unique<PressureConverter>(KSF));
        return v;
    }

private:
    Unit unit;
};

// ===== Area =====
class AreaConverter : public ConverterBase {
public:
    enum Unit { MM2, CM2, M2, KM2, IN2, FT2 };
    AreaConverter(Unit u) : unit(u) {}

    QString name() const override {
        switch(unit) {
            case MM2: return "mm²";
            case CM2: return "cm²";
            case M2:  return "m²";
            case KM2: return "km²";
            case IN2: return "in²";
            case FT2: return "ft²";
        }
        return "Unknown";
    }

    double toBase(double value) const override { // base = m2
        double result = 0.0;
        switch(unit) {
            case MM2: result = value / 1e6; break;
            case CM2: result = value / 1e4; break;
            case M2:  result = value; break;
            case KM2: result = value * 1e6; break;
            case IN2: result = value * 0.00064516; break;
            case FT2: result = value * 0.092903; break;
        }
        return result;
    }

    QString fromBase(double baseValue) const override {
        double result = 0.0;
        switch(unit) {
            case MM2: result = baseValue * 1e6; break;
            case CM2: result = baseValue * 1e4; break;
            case M2:  result = baseValue; break;
            case KM2: result = baseValue / 1e6; break;
            case IN2: result = baseValue / 0.00064516; break;
            case FT2: result = baseValue / 0.092903; break;
        }
        return QString::number(result, 'f', 3);
    }

    static std::vector<std::unique_ptr<ConverterBase>> allUnits() {
        std::vector<std::unique_ptr<ConverterBase>> v;
        v.push_back(std::make_unique<AreaConverter>(MM2));
        v.push_back(std::make_unique<AreaConverter>(CM2));
        v.push_back(std::make_unique<AreaConverter>(M2));
        v.push_back(std::make_unique<AreaConverter>(KM2));
        v.push_back(std::make_unique<AreaConverter>(IN2));
        v.push_back(std::make_unique<AreaConverter>(FT2));
        return v;
    }

private:
    Unit unit;
};

// ===== Volume =====
class VolumeConverter : public ConverterBase {
public:
    enum Unit { MM3, CM3, M3, KM3, IN3, FT3 };
    VolumeConverter(Unit u) : unit(u) {}

    QString name() const override {
        switch(unit) {
            case MM3: return "mm³";
            case CM3: return "cm³";
            case M3:  return "m³";
            case KM3: return "km³";
            case IN3: return "in³";
            case FT3: return "ft³";
        }
        return "Unknown";
    }

    double toBase(double value) const override { // base = m3
        double result = 0.0;
        switch(unit) {
            case MM3: result = value / 1e9; break;
            case CM3: result = value / 1e6; break;
            case M3:  result = value; break;
            case KM3: result = value * 1e9; break;
            case IN3: result = value * 1.6387e-5; break;
            case FT3: result = value * 0.0283168; break;
        }
        return result;
    }

    QString fromBase(double baseValue) const override {
        double result = 0.0;
        switch(unit) {
            case MM3: result = baseValue * 1e9; break;
            case CM3: result = baseValue * 1e6; break;
            case M3:  result = baseValue; break;
            case KM3: result = baseValue / 1e9; break;
            case IN3: result = baseValue / 1.6387e-5; break;
            case FT3: result = baseValue / 0.0283168; break;
        }
        return QString::number(result, 'f', 3);
    }

    static std::vector<std::unique_ptr<ConverterBase>> allUnits() {
        std::vector<std::unique_ptr<ConverterBase>> v;
        v.push_back(std::make_unique<VolumeConverter>(MM3));
        v.push_back(std::make_unique<VolumeConverter>(CM3));
        v.push_back(std::make_unique<VolumeConverter>(M3));
        v.push_back(std::make_unique<VolumeConverter>(KM3));
        v.push_back(std::make_unique<VolumeConverter>(IN3));
        v.push_back(std::make_unique<VolumeConverter>(FT3));
        return v;
    }

private:
    Unit unit;
};

#endif // CALCS_H
