#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QClipboard>
#include <QMessageBox>
#include <QIcon>
#include <QHBoxLayout>
#include <QMenuBar>
#include <memory>
#include <vector>
#include "calcs.h"

class ConverterApp : public QWidget {
    Q_OBJECT

public:
    ConverterApp(QWidget *parent = nullptr) : QWidget(parent) {
        auto *layout = new QVBoxLayout(this);
        layout->setMenuBar(createMenuBar());   // ✅ attach menu bar at top

        // --- Top category dropdown ---
        categoryCombo = new QComboBox(this);
        categoryCombo->addItems({"Length", "Temperature", "Forces", "Moments"});
        layout->addWidget(categoryCombo);
        
        // --- Converter options dropdown ---
        combo = new QComboBox(this);
        layout->addWidget(combo);

        // --- Input field (DoubleSpinBox) ---
        input = new QDoubleSpinBox(this);
        input->setRange(-1e9, 1e9);
        input->setDecimals(2);
        input->setValue(0.0);
        layout->addWidget(input);

        // --- Convert button ---
        auto *button = new QPushButton("Convert", this);
        layout->addWidget(button);

        // --- Result layout (label + copy button) ---
        auto *resultLayout = new QHBoxLayout();
        result = new QLabel("Result", this);
        result->setStyleSheet("QLabel { border: 1px solid gray; padding: 3px; background: #f9f9f9; }");
        resultLayout->addWidget(result);

        auto *copyButton = new QPushButton("Copy", this);
        resultLayout->addWidget(copyButton);
        layout->addLayout(resultLayout);

        // --- Register converters ---
        loadConverters("Length"); // default

        // --- Connections ---
        connect(button, &QPushButton::clicked, this, &ConverterApp::doConvert);
        connect(copyButton, &QPushButton::clicked, this, &ConverterApp::copyResult);
        connect(categoryCombo, &QComboBox::currentTextChanged, this, &ConverterApp::loadConverters);

        // --- Window setup ---
        setWindowFlags(
            Qt::Window |
            Qt::WindowTitleHint |
            Qt::WindowSystemMenuHint |
            Qt::WindowMinimizeButtonHint |
            Qt::WindowCloseButtonHint
        );

        setFixedSize(200, 250);
        setWindowTitle("eUnits");

        // --- App Icon ---
        QIcon appIcon(":/assets/icon.png"); // use resource file
        setWindowIcon(appIcon);
    }

private slots:
    void doConvert() {
        double value = input->value();

        int idx = combo->currentIndex();
        if (idx >= 0 && idx < static_cast<int>(converters.size())) {
            result->setText(converters[idx]->convert(value));
        }
    }

    void copyResult() {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(result->text());
    }

    void showAbout() {
        QMessageBox::about(this, "About Unit Converter",
            "Unit Converter App\n\n"
            "Features:\n"
            "- Supports multiple categories\n"
            "- Copy result to clipboard\n"
            "- Always on top\n"
            "- Written with Qt and C++");
    }

    void loadConverters(const QString &category) {
        combo->clear();
        converters.clear();

        if (category == "Length") {
            registerConverter(std::make_unique<LengthConverter>());
        } else if (category == "Temperature") {
            registerConverter(std::make_unique<TemperatureConverter>());
        } else if (category == "Forces") {
            // TODO: add ForceConverter class
        } else if (category == "Moments") {
            // TODO: add MomentConverter class
        }
    }

private:
    QMenuBar* createMenuBar() {
        auto *menuBar = new QMenuBar(this);
        auto *helpMenu = menuBar->addMenu("&Help");

        QAction *aboutAction = helpMenu->addAction("About");
        connect(aboutAction, &QAction::triggered, this, &ConverterApp::showAbout);

        return menuBar;
    }

    void registerConverter(std::unique_ptr<ConverterBase> conv) {
        combo->addItem(conv->name());
        converters.push_back(std::move(conv));
    }

    QDoubleSpinBox *input;
    QComboBox *combo;
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
