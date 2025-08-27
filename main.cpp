#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QClipboard>
#include <QMessageBox>
#include <QIcon>
#include <QHBoxLayout>
#include <QFrame>
#include <QDoubleValidator>
#include <memory>
#include <vector>
#include "calcs.h"

class ConverterApp : public QWidget {
    Q_OBJECT

public:
    ConverterApp(QWidget *parent = nullptr) : QWidget(parent) {
        auto *layout = new QVBoxLayout(this);

        // --- Top category dropdown ---
        categoryCombo = new QComboBox(this);
        categoryCombo->addItems({"Length", "Temperature", "Forces", "Moments"});
        layout->addWidget(categoryCombo);

        // --- Converter options dropdown ---
        combo = new QComboBox(this);
        layout->addWidget(combo);

        // --- Input field (QLineEdit with numeric validator) ---
        input = new QLineEdit(this);
        input->setPlaceholderText("Enter number...");
        input->setValidator(new QDoubleValidator(-1e9, 1e9, 6, input));  // only numbers allowed
        layout->addWidget(input);

        // --- Convert button ---
        auto *button = new QPushButton("Convert", this);
        button->setDefault(true); // Enter triggers this button
        layout->addWidget(button);

        // --- Result layout (label + copy icon button) ---
        auto *resultLayout = new QHBoxLayout();
        result = new QLabel("Result", this);
        result->setStyleSheet("QLabel { border: 1px solid gray; padding: 3px; background: #f9f9f9; }");
        resultLayout->addWidget(result);

        auto *copyButton = new QPushButton(this);
        copyButton->setIcon(QIcon(":/assets/copy.png"));
        resultLayout->addWidget(copyButton);
        layout->addLayout(resultLayout);

        // --- Spacer pushes bottom bar to the bottom ---
        layout->addStretch();

        /// --- Separator line and bottom bar ---
        QFrame *line = new QFrame(this);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        layout->addWidget(line);

        // --- Compact bottom bar ---
        auto *bottomLayout = new QHBoxLayout();
        bottomLayout->setContentsMargins(0, 0, 0, 0);  // reduce padding
        bottomLayout->setSpacing(0);
        bottomLayout->addStretch(); // push About button to right

        auto *aboutButton = new QPushButton(this);
        aboutButton->setIcon(QIcon(":/assets/info.png"));
        aboutButton->setToolTip("About");
        aboutButton->setFixedSize(24, 24); // smaller button
        aboutButton->setIconSize(QSize(16, 16)); // smaller icon
        aboutButton->setFlat(true); // remove button border to make it look like an icon
        bottomLayout->addWidget(aboutButton);

        QWidget *bottomWidget = new QWidget(this);
        bottomWidget->setLayout(bottomLayout);
        bottomWidget->setFixedHeight(16); // compact bottom bar
        layout->addWidget(bottomWidget);

        // --- Register converters ---
        loadConverters("Length"); // default

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
            Qt::WindowCloseButtonHint
        );

        setFixedSize(200, 220);
        setWindowTitle("eUnits");

        // --- App Icon ---
        QIcon appIcon(":/assets/icon.png");
        setWindowIcon(appIcon);
    }

private slots:
    void doConvert() {
        bool ok;
        double value = input->text().toDouble(&ok);
        if (!ok) {
            result->setText("Invalid input");
            return;
        }

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
            "Unit Converter\n\n"
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
    void registerConverter(std::unique_ptr<ConverterBase> conv) {
        combo->addItem(conv->name());
        converters.push_back(std::move(conv));
    }

    QLineEdit *input;
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
