#include "ui/mainwindow.h"
#include <QMainWindow>
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QTimer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMenu>
#include <QColorDialog>
#include <QDoubleValidator>
#include <QUrl>
#include <QSettings>
#include <QApplication>
#include <QPalette>
#include <QStatusBar>
#include <QUrlQuery>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QDialog>
#include <QVector>
#include <QGroupBox>
#include <QComboBox>
#include <QInputDialog>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_apiManager(new CurrencyApiManager(this))
    , m_updateTimer(new QTimer(this))
    , m_amountInput(new QLineEdit(this))
    , m_fromCurrencyCombo(new QComboBox(this))
    , m_toCurrencyCombo(new QComboBox(this))
    , m_convertButton(new QPushButton(this))
    , m_swapButton(new QPushButton(this))
    , m_refreshButton(new QPushButton(this))
    , m_themeButton(new QPushButton(this))
    , m_languageButton(new QPushButton(this))
    , m_resultLabel(new QLabel(this))
    , m_exchangeRateLabel(new QLabel(this))
    , m_lastUpdate(new QLabel(this))
    , m_loadingLabel(new QLabel(this))
    , m_currentLanguage("en")
    , m_translator()
{
    setupUi();
    setupConnections();
    populateCurrencyComboBoxes();
    
    // Set default currencies
    m_fromCurrencyCombo->setCurrentText("USD");
    m_toCurrencyCombo->setCurrentText("SEK");
    
    // Set window title
    setWindowTitle(tr("Live Currency Exchange Calculator"));
    resize(600, 400);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Title
    QLabel* titleLabel = new QLabel(tr("Live Currency Exchange Calculator"));
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);
    
    // Input section
    QGroupBox* inputGroup = new QGroupBox(tr("Convert Currency"));
    QGridLayout* inputLayout = new QGridLayout(inputGroup);
    inputLayout->setSpacing(10);
    
    // Amount input
    inputLayout->addWidget(new QLabel(tr("Amount:")), 0, 0);
    
    // Add input validation - only allow positive numbers
    QDoubleValidator* validator = new QDoubleValidator(0.0, 999999999.99, 2, this);
    validator->setNotation(QDoubleValidator::StandardNotation);
    m_amountInput->setValidator(validator);
    m_amountInput->setPlaceholderText(tr("Enter amount..."));
    m_amountInput->setText("1.00");
    inputLayout->addWidget(m_amountInput, 0, 1, 1, 2);
    
    // From currency
    inputLayout->addWidget(new QLabel(tr("From:")), 1, 0);
    m_fromCurrencyCombo->setMinimumWidth(150);
    inputLayout->addWidget(m_fromCurrencyCombo, 1, 1);
    
    // Swap button
    m_swapButton->setText("⇄");
    m_swapButton->setToolTip(tr("Swap currencies"));
    m_swapButton->setMaximumWidth(40);
    inputLayout->addWidget(m_swapButton, 1, 2);
    
    // To currency
    inputLayout->addWidget(new QLabel(tr("To:")), 2, 0);
    m_toCurrencyCombo->setMinimumWidth(150);
    inputLayout->addWidget(m_toCurrencyCombo, 2, 1, 1, 2);
    
    mainLayout->addWidget(inputGroup);
    
    // Result section
    QGroupBox* resultGroup = new QGroupBox(tr("Conversion Result"));
    QVBoxLayout* resultLayout = new QVBoxLayout(resultGroup);
    
    // Result display
    m_resultLabel->setWordWrap(true);
    m_resultLabel->setAlignment(Qt::AlignCenter);
    m_resultLabel->setStyleSheet(
        "QLabel { "
        "padding: 15px; "
        "border: 2px solid #4CAF50; "
        "border-radius: 8px; "
        "background-color: #f9f9f9; "
        "font-size: 14pt; "
        "font-weight: bold; "
        "color: #2E7D32; "
        "}"
    );
    m_resultLabel->setText(tr("Enter amount and select currencies"));
    resultLayout->addWidget(m_resultLabel);
    
    // Exchange rate display
    m_exchangeRateLabel->setAlignment(Qt::AlignCenter);
    m_exchangeRateLabel->setStyleSheet("QLabel { color: #666; font-style: italic; }");
    resultLayout->addWidget(m_exchangeRateLabel);
    
    mainLayout->addWidget(resultGroup);
    
    // Loading indicator
    m_loadingLabel->setAlignment(Qt::AlignCenter);
    m_loadingLabel->setStyleSheet("QLabel { color: #FF9800; font-weight: bold; }");
    m_loadingLabel->hide();
    mainLayout->addWidget(m_loadingLabel);
    
    // Buttons section
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);
    
    m_convertButton->setText(tr("Convert"));
    m_convertButton->setStyleSheet(
        "QPushButton { "
        "background-color: #4CAF50; "
        "color: white; "
        "border: none; "
        "padding: 10px 20px; "
        "border-radius: 5px; "
        "font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #45a049; }"
        "QPushButton:pressed { background-color: #3d8b40; }"
    );
    
    m_refreshButton->setText(tr("Refresh Rates"));
    m_themeButton->setText(tr("Theme"));
    m_languageButton->setText(tr("Language"));
    
    buttonLayout->addWidget(m_convertButton);
    buttonLayout->addWidget(m_refreshButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_themeButton);
    buttonLayout->addWidget(m_languageButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Status bar
    statusBar()->addPermanentWidget(m_lastUpdate);
    
    // Set RTL support
    setLayoutDirection(Qt::LeftToRight);
    
    retranslateUi();
}

void MainWindow::setupConnections()
{
    // API Manager connections
    connect(m_apiManager, &CurrencyApiManager::ratesUpdated, this, &MainWindow::onRatesUpdated);
    connect(m_apiManager, &CurrencyApiManager::loadingChanged, this, &MainWindow::onLoadingChanged);
    connect(m_apiManager, &CurrencyApiManager::error, this, &MainWindow::onApiError);
    
    // Button connections
    connect(m_convertButton, &QPushButton::clicked, this, &MainWindow::convertCurrency);
    connect(m_swapButton, &QPushButton::clicked, this, &MainWindow::swapCurrencies);
    connect(m_refreshButton, &QPushButton::clicked, this, &MainWindow::refreshRates);
    connect(m_themeButton, &QPushButton::clicked, this, &MainWindow::createThemeMenu);
    connect(m_languageButton, &QPushButton::clicked, this, &MainWindow::createLanguageMenu);
    
    // Input connections
    connect(m_amountInput, &QLineEdit::returnPressed, this, &MainWindow::convertCurrency);
    connect(m_amountInput, &QLineEdit::textChanged, this, &MainWindow::onAmountChanged);
    connect(m_fromCurrencyCombo, &QComboBox::currentTextChanged, this, &MainWindow::onFromCurrencyChanged);
    connect(m_toCurrencyCombo, &QComboBox::currentTextChanged, this, &MainWindow::onToCurrencyChanged);
    
    // Theme and language connections
    connect(&m_language, &Language::languageChanged, this, &MainWindow::retranslateUi);
    connect(&m_theme, &Theme::themeChanged, this, &MainWindow::onThemeChanged);
}

void MainWindow::populateCurrencyComboBoxes()
{
    QStringList currencies = m_apiManager->getPopularCurrencies();
    
    // Add currency names for better user experience
    QMap<QString, QString> currencyNames;
    currencyNames["USD"] = "USD - US Dollar";
    currencyNames["EUR"] = "EUR - Euro";
    currencyNames["SEK"] = "SEK - Swedish Krona";
    currencyNames["IDR"] = "IDR - Indonesian Rupiah";
    currencyNames["GBP"] = "GBP - British Pound";
    currencyNames["JPY"] = "JPY - Japanese Yen";
    currencyNames["AUD"] = "AUD - Australian Dollar";
    currencyNames["CAD"] = "CAD - Canadian Dollar";
    currencyNames["CHF"] = "CHF - Swiss Franc";
    currencyNames["CNY"] = "CNY - Chinese Yuan";
    currencyNames["INR"] = "INR - Indian Rupee";
    currencyNames["KRW"] = "KRW - South Korean Won";
    currencyNames["SGD"] = "SGD - Singapore Dollar";
    currencyNames["THB"] = "THB - Thai Baht";
    currencyNames["MYR"] = "MYR - Malaysian Ringgit";
    currencyNames["PHP"] = "PHP - Philippine Peso";
    currencyNames["VND"] = "VND - Vietnamese Dong";
    currencyNames["NOK"] = "NOK - Norwegian Krone";
    currencyNames["DKK"] = "DKK - Danish Krone";
    currencyNames["PLN"] = "PLN - Polish Zloty";
    currencyNames["BRL"] = "BRL - Brazilian Real";
    currencyNames["MXN"] = "MXN - Mexican Peso";
    currencyNames["ZAR"] = "ZAR - South African Rand";
    currencyNames["TRY"] = "TRY - Turkish Lira";
    
    for (const QString& currency : currencies) {
        QString displayName = currencyNames.value(currency, currency);
        m_fromCurrencyCombo->addItem(displayName, currency);
        m_toCurrencyCombo->addItem(displayName, currency);
    }
}

void MainWindow::onRatesUpdated(const QMap<QString, CurrencyApiManager::CurrencyRate>& rates)
{
    m_currentRates = rates;
    lastUpdateTime = QDateTime::currentDateTime();
    updateLastUpdateLabel();
    updateConversionResult();
}

void MainWindow::onLoadingChanged(bool loading)
{
    if (loading) {
        m_loadingLabel->setText(tr("Fetching latest exchange rates..."));
        m_loadingLabel->show();
        m_convertButton->setEnabled(false);
        m_refreshButton->setEnabled(false);
    } else {
        m_loadingLabel->hide();
        m_convertButton->setEnabled(true);
        m_refreshButton->setEnabled(true);
    }
}

void MainWindow::onApiError(const QString& message)
{
    showError(message);
}

void MainWindow::convertCurrency()
{
    updateConversionResult();
}

void MainWindow::updateConversionResult()
{
    bool ok;
    double amount = m_amountInput->text().toDouble(&ok);
    if (!ok || amount <= 0) {
        m_resultLabel->setText(tr("Please enter a valid amount"));
        m_exchangeRateLabel->clear();
        return;
    }
    
    QString fromCurrency = m_fromCurrencyCombo->currentData().toString();
    QString toCurrency = m_toCurrencyCombo->currentData().toString();
    
    if (fromCurrency.isEmpty() || toCurrency.isEmpty()) {
        m_resultLabel->setText(tr("Please select currencies"));
        m_exchangeRateLabel->clear();
        return;
    }
    
    if (m_currentRates.isEmpty()) {
        m_resultLabel->setText(tr("Exchange rates not available. Please refresh."));
        m_exchangeRateLabel->clear();
        return;
    }
    
    // Calculate conversion
    double result = 0.0;
    double exchangeRate = 0.0;
    
    if (fromCurrency == toCurrency) {
        result = amount;
        exchangeRate = 1.0;
    } else {
        // Convert from base currency rate to target currency
        double fromRate = m_currentRates.value(fromCurrency).rate;
        double toRate = m_currentRates.value(toCurrency).rate;
        
        if (fromRate > 0 && toRate > 0) {
            // Convert: amount in fromCurrency -> USD -> toCurrency
            double usdAmount = amount / fromRate;
            result = usdAmount * toRate;
            exchangeRate = toRate / fromRate;
        } else {
            m_resultLabel->setText(tr("Exchange rate not available for selected currencies"));
            m_exchangeRateLabel->clear();
            return;
        }
    }
    
    // Format and display result
    QString formattedResult = formatCurrency(result, toCurrency);
    QString formattedAmount = formatCurrency(amount, fromCurrency);
    
    m_resultLabel->setText(tr("%1 = %2").arg(formattedAmount, formattedResult));
    
    // Show exchange rate
    m_exchangeRateLabel->setText(tr("1 %1 = %2 %3")
        .arg(fromCurrency)
        .arg(QString::number(exchangeRate, 'f', 6))
        .arg(toCurrency));
}

QString MainWindow::formatCurrency(double amount, const QString& currencyCode)
{
    // Format numbers based on currency
    int decimals = 2;
    if (currencyCode == "JPY" || currencyCode == "KRW" || currencyCode == "VND" || currencyCode == "IDR") {
        decimals = 0; // These currencies typically don't use decimal places
    }
    
    QString formattedAmount = QString::number(amount, 'f', decimals);
    
    // Add thousand separators
    QStringList parts = formattedAmount.split('.');
    QString integerPart = parts[0];
    QString decimalPart = parts.size() > 1 ? parts[1] : "";
    
    // Add commas for thousands
    for (int i = integerPart.length() - 3; i > 0; i -= 3) {
        integerPart.insert(i, ',');
    }
    
    if (!decimalPart.isEmpty()) {
        return QString("%1.%2 %3").arg(integerPart, decimalPart, currencyCode);
    } else {
        return QString("%1 %2").arg(integerPart, currencyCode);
    }
}

void MainWindow::swapCurrencies()
{
    QString fromCurrency = m_fromCurrencyCombo->currentData().toString();
    QString toCurrency = m_toCurrencyCombo->currentData().toString();
    
    // Find indices and swap
    for (int i = 0; i < m_fromCurrencyCombo->count(); ++i) {
        if (m_fromCurrencyCombo->itemData(i).toString() == toCurrency) {
            m_fromCurrencyCombo->setCurrentIndex(i);
            break;
        }
    }
    
    for (int i = 0; i < m_toCurrencyCombo->count(); ++i) {
        if (m_toCurrencyCombo->itemData(i).toString() == fromCurrency) {
            m_toCurrencyCombo->setCurrentIndex(i);
            break;
        }
    }
    
    updateConversionResult();
}

void MainWindow::onFromCurrencyChanged()
{
    updateConversionResult();
}

void MainWindow::onToCurrencyChanged()
{
    updateConversionResult();
}

void MainWindow::onAmountChanged()
{
    // Auto-convert as user types (with small delay to avoid too many calculations)
    static QTimer* delayTimer = new QTimer(this);
    delayTimer->setSingleShot(true);
    delayTimer->setInterval(500); // 500ms delay
    
    disconnect(delayTimer, &QTimer::timeout, this, &MainWindow::updateConversionResult);
    connect(delayTimer, &QTimer::timeout, this, &MainWindow::updateConversionResult);
    
    delayTimer->start();
}

void MainWindow::refreshRates()
{
    m_apiManager->fetchExchangeRates();
}

void MainWindow::updateLastUpdateLabel()
{
    if (!lastUpdateTime.isValid()) {
        m_lastUpdate->setText(tr("Last update: Never"));
    } else {
        m_lastUpdate->setText(tr("Last update: %1").arg(lastUpdateTime.toString("hh:mm:ss")));
    }
}

void MainWindow::showError(const QString& message)
{
    m_resultLabel->setText(message);
    m_resultLabel->setStyleSheet(
        "QLabel { "
        "padding: 15px; "
        "border: 2px solid #f44336; "
        "border-radius: 8px; "
        "background-color: #ffebee; "
        "color: #c62828; "
        "font-weight: bold; "
        "}"
    );
    
    // Reset style after a few seconds
    QTimer::singleShot(5000, this, [this]() {
        m_resultLabel->setStyleSheet(
            "QLabel { "
            "padding: 15px; "
            "border: 2px solid #4CAF50; "
            "border-radius: 8px; "
            "background-color: #f9f9f9; "
            "font-size: 14pt; "
            "font-weight: bold; "
            "color: #2E7D32; "
            "}"
        );
    });
    
    QMessageBox::warning(this, tr("Error"), message);
}

// Placeholder implementations for theme and language functionality
void MainWindow::onLanguageChanged()
{
    retranslateUi();
}

void MainWindow::createLanguageMenu()
{
    QMenu menu(this);
    
    QAction* englishAction = menu.addAction("English");
    QAction* swedishAction = menu.addAction("Svenska");
    QAction* indonesianAction = menu.addAction("Bahasa Indonesia");
    
    connect(englishAction, &QAction::triggered, [this]() { changeLanguage("en"); });
    connect(swedishAction, &QAction::triggered, [this]() { changeLanguage("sv"); });
    connect(indonesianAction, &QAction::triggered, [this]() { changeLanguage("id"); });
    
    menu.exec(m_languageButton->mapToGlobal(m_languageButton->rect().bottomLeft()));
}

void MainWindow::createThemeMenu()
{
    QMenu menu(this);
    
    QAction* lightAction = menu.addAction(tr("Light"));
    QAction* darkAction = menu.addAction(tr("Dark"));
    
    connect(lightAction, &QAction::triggered, this, [this]() {
        m_theme.setMode(Theme::Mode::Light);
    });
    
    connect(darkAction, &QAction::triggered, this, [this]() {
        m_theme.setMode(Theme::Mode::Dark);
    });
    
    menu.exec(m_themeButton->mapToGlobal(m_themeButton->rect().bottomLeft()));
}

void MainWindow::showThemeDialog()
{
    // Placeholder for theme customization dialog
}

void MainWindow::onThemeChanged()
{
    QPalette palette = QApplication::palette();
    m_theme.applyTheme(palette);
    QApplication::setPalette(palette);
}

void MainWindow::retranslateUi()
{
    setWindowTitle(tr("Live Currency Exchange Calculator"));
    m_convertButton->setText(tr("Convert"));
    m_refreshButton->setText(tr("Refresh Rates"));
    m_themeButton->setText(tr("Theme"));
    m_languageButton->setText(tr("Language"));
    
    updateLastUpdateLabel();
}

void MainWindow::changeLanguage(const QString& language)
{
    if (m_currentLanguage != language) {
        loadLanguage(language);
        m_currentLanguage = language;
        retranslateUi();
    }
}

void MainWindow::loadLanguage(const QString& language)
{
    // Remove existing translator
    qApp->removeTranslator(&m_translator);
    
    // Load new translation
    if (m_translator.load(QString("currencyexchange_%1").arg(language), ":/translations/")) {
        qApp->installTranslator(&m_translator);
    }
}

void MainWindow::updateDisplay()
{
    updateConversionResult();
}

void MainWindow::setupLanguageMenu()
{
    // Placeholder for language menu setup
}
