#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <memory>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

#include "calcs.h"

class ConverterApp : public QWidget {
    Q_OBJECT

public:
    ConverterApp(QWidget *parent = nullptr) : QWidget(parent) {
        // --- Layout ---
        auto *layout = new QVBoxLayout(this);

        input = new QLineEdit(this);
        input->setPlaceholderText("Enter value...");
        layout->addWidget(input);

        combo = new QComboBox(this);
        layout->addWidget(combo);

        auto *button = new QPushButton("Convert", this);
        layout->addWidget(button);

        result = new QLabel("Result will appear here", this);
        layout->addWidget(result);

        // --- Register converters ---
        registerConverter(std::make_unique<LengthConverter>());
        registerConverter(std::make_unique<TemperatureConverter>());

        connect(button, &QPushButton::clicked, this, &ConverterApp::doConvert);

        // --- Window flags ---
        setWindowFlags(
            Qt::Window |                // Normal window
            Qt::WindowTitleHint |       // Show title bar
            Qt::WindowSystemMenuHint |  // Show system menu
            Qt::WindowMinimizeButtonHint | // Only minimize
            Qt::WindowCloseButtonHint   // Only close button
        );

        setFixedSize(350, 180);        // Non-resizable
        setWindowTitle("Unit Converter");
        setWindowFlag(Qt::WindowStaysOnTopHint, true); // Always on top
    }

private slots:
    void doConvert() {
        bool ok;
        double value = input->text().toDouble(&ok);
        if (!ok) {
            result->setText("Invalid number!");
            return;
        }

        int idx = combo->currentIndex();
        if (idx >= 0 && idx < converters.size()) {
            result->setText(converters[idx]->convert(value));
        }
    }

private:
    void registerConverter(std::unique_ptr<ConverterBase> conv) {
        combo->addItem(conv->name());
        converters.push_back(std::move(conv));
    }

    QLineEdit *input;
    QComboBox *combo;
    QLabel *result;
    std::vector<std::unique_ptr<ConverterBase>> converters;
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // --- Remove console window on Windows ---
    #ifdef _WIN32
    ::FreeConsole();
    #endif

    ConverterApp window;
    window.show();

    return app.exec();
}
