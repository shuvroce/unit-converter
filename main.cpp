#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QClipboard>
#include <QMessageBox>
#include <QIcon>
#include <QDoubleValidator>
#include <memory>
#include <vector>

#include "calcs.h"

class ConverterApp : public QWidget {
    Q_OBJECT

public:
    ConverterApp(QWidget *parent = nullptr) : QWidget(parent) {
        auto *layout = new QVBoxLayout(this);

        // --- Category dropdown ---
        categoryCombo = new QComboBox(this);
        categoryCombo->addItems({"Length", "Temperature", "Velocity", "Force", "Moment", "Pressure", "Area", "Volume"});
        layout->addWidget(categoryCombo);

        // --- Input field ---
        input = new QLineEdit(this);
        input->setPlaceholderText("Enter number...");
        input->setValidator(new QDoubleValidator(-1e9, 1e9, 3, input));
        layout->addWidget(input);

        // --- From → To dropdowns ---
        auto *converterLayout = new QHBoxLayout();
        fromCombo = new QComboBox(this);
        toCombo   = new QComboBox(this);
        QLabel *arrowLabel = new QLabel("→", this);
        arrowLabel->setAlignment(Qt::AlignCenter);
        converterLayout->addWidget(fromCombo, 3);
        converterLayout->addWidget(arrowLabel, 1);
        converterLayout->addWidget(toCombo, 3);
        layout->addLayout(converterLayout);

        // --- Convert button ---
        auto *button = new QPushButton("Convert", this);
        button->setDefault(true);
        button->setAutoDefault(true);
        layout->addWidget(button);
        connect(input, &QLineEdit::returnPressed, this, &ConverterApp::doConvert);

        // --- Result layout ---
        auto *resultLayout = new QHBoxLayout();
        result = new QLabel("Result", this);
        result->setStyleSheet("QLabel { border: 1px solid #bebebe; border-radius: 2px; padding: 2px; background: #ffffff; }");
        resultLayout->addWidget(result);

        auto *copyButton = new QPushButton(this);
        copyButton->setIcon(QIcon(":/assets/copy.png"));
        copyButton->setToolTip("Copy");
        copyButton->setFixedSize(24, 24);
        copyButton->setIconSize(QSize(16, 16));
        copyButton->setFlat(true);
        resultLayout->addWidget(copyButton);
        layout->addLayout(resultLayout);

        layout->addStretch();

        // --- Bottom bar ---
        auto *bottomLayout = new QHBoxLayout();
        bottomLayout->setContentsMargins(0,0,0,0);
        bottomLayout->setSpacing(0);
        bottomLayout->addStretch();

        auto *aboutButton = new QPushButton(this);
        aboutButton->setIcon(QIcon(":/assets/info.png"));
        aboutButton->setToolTip("About");
        aboutButton->setFixedSize(24,24);
        aboutButton->setIconSize(QSize(16,16));
        aboutButton->setFlat(true);
        bottomLayout->addWidget(aboutButton);

        QWidget *bottomWidget = new QWidget(this);
        bottomWidget->setLayout(bottomLayout);
        bottomWidget->setFixedHeight(24);
        layout->addWidget(bottomWidget);

        // --- Load default converters ---
        loadConverters("Length");

        // --- Connections ---
        connect(button, &QPushButton::clicked, this, &ConverterApp::doConvert);
        connect(copyButton, &QPushButton::clicked, this, &ConverterApp::copyResult);
        connect(aboutButton, &QPushButton::clicked, this, &ConverterApp::showAbout);
        connect(categoryCombo, &QComboBox::currentTextChanged, this, &ConverterApp::loadConverters);

        // --- Window setup ---
        setWindowFlags(
            Qt::Window |
            Qt::WindowTitleHint |
            Qt::WindowSystemMenuHint |
            Qt::WindowMinimizeButtonHint |
            Qt::WindowCloseButtonHint |
            Qt::WindowStaysOnTopHint
        );
        setFixedSize(240, 200);
        setWindowTitle("Units");
        setWindowIcon(QIcon(":/assets/icon.png"));
    }

private slots:
    void doConvert() {
        bool ok;
        double value = input->text().toDouble(&ok);
        if (!ok) {
            result->setText("Invalid input");
            return;
        }

        int fromIdx = fromCombo->currentIndex();
        int toIdx   = toCombo->currentIndex();

        if (fromIdx >= 0 && fromIdx < static_cast<int>(converters.size()) &&
            toIdx   >= 0 && toIdx   < static_cast<int>(converters.size())) {

            double baseValue = converters[fromIdx]->toBase(value);
            result->setText(converters[toIdx]->fromBase(baseValue));
        }
    }

    void copyResult() {
        QApplication::clipboard()->setText(result->text());
    }

    void showAbout() {
        QString text =
            "<b>Unit Converter for Engineers</b><br><br>"
            "<u>Features:</u><br>"
            "- Supports multiple categories<br>"
            "- From → To conversion<br>"
            "- Copy result to clipboard<br>"
            "- Always on top<br>"
            "- Written with Qt and C++<br><br>"

            "Copyright (c) 2025 Md. Akram Hossain.<br>"
            "All rights reserved.<br><br>"

            // "<div align='center'>"
            "<a href='https://akramh.com'>Website</a> | "
            "<a href='https://facebook.com/ahshuvro'>Facebook</a> | "
            "<a href='https://linkedin.com/in/ahshuvro'>Linkedin</a> | "
            "<a href='https://github.com/shuvroce/unit-converter'>GitHub</a>";
            // "</div>";

        QMessageBox msgBox(this);  // parent = main window
        msgBox.setWindowTitle("About");
        msgBox.setTextFormat(Qt::RichText);
        msgBox.setTextInteractionFlags(Qt::TextBrowserInteraction);
        msgBox.setText(text);
        msgBox.setStandardButtons(QMessageBox::Ok);

        // make sure it stays on top like the main app
        msgBox.setWindowFlags(msgBox.windowFlags() | Qt::WindowStaysOnTopHint);

        msgBox.exec();
    }

    void loadConverters(const QString &category) {
        fromCombo->clear();
        toCombo->clear();
        converters.clear();

        if (category == "Length") {
            auto units = LengthConverter::allUnits();
            for (auto &u : units) registerConverter(std::move(u));
        }
        else if (category == "Temperature") {
            auto units = TemperatureConverter::allUnits();
            for (auto &u : units) registerConverter(std::move(u));
        }
        else if (category == "Velocity") {
            auto units = VelocityConverter::allUnits();
            for (auto &u : units) registerConverter(std::move(u));
        }
        else if (category == "Force") {
            auto units = ForceConverter::allUnits();
            for (auto &u : units) registerConverter(std::move(u));
        }
        else if (category == "Moment") {
            auto units = MomentConverter::allUnits();
            for (auto &u : units) registerConverter(std::move(u));
        }
        else if (category == "Pressure") {
            auto units = PressureConverter::allUnits();
            for (auto &u : units) registerConverter(std::move(u));
        }
        else if (category == "Area") {
            auto units = AreaConverter::allUnits();
            for (auto &u : units) registerConverter(std::move(u));
        }
        else if (category == "Volume") {
            auto units = VolumeConverter::allUnits();
            for (auto &u : units) registerConverter(std::move(u));
        }
    }

private:
    void registerConverter(std::unique_ptr<ConverterBase> conv) {
        fromCombo->addItem(conv->name());
        toCombo->addItem(conv->name());
        converters.push_back(std::move(conv));
    }

    QLineEdit *input;
    QComboBox *fromCombo;
    QComboBox *toCombo;
    QComboBox *categoryCombo;
    QLabel *result;
    std::vector<std::unique_ptr<ConverterBase>> converters;
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ConverterApp window;
    window.show();
    return app.exec();
}
