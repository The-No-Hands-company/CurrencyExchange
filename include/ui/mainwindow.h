#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QDateTime>
#include <QMenu>
#include <QMenuBar>
#include <QJsonObject>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QColorDialog>
#include <QPalette>
#include <QInputDialog>
#include <QVector>
#include <QTranslator>
#include <QApplication>
#include <QSwipeGesture>
#include "core/language.h"
#include "core/theme.h"
#include "network/currencyapimanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRatesUpdated(const QMap<QString, CurrencyApiManager::CurrencyRate>& rates);
    void onLoadingChanged(bool loading);
    void onApiError(const QString& message);
    void updateDisplay();
    void convertCurrency();
    void swapCurrencies();
    void onLanguageChanged();
    void createLanguageMenu();
    void createThemeMenu();
    void showThemeDialog();
    void onThemeChanged();
    void retranslateUi();
    void refreshRates();
    void showError(const QString& message);
    void changeLanguage(const QString& language);
    void onFromCurrencyChanged();
    void onToCurrencyChanged();
    void onAmountChanged();

private:
    void setupUi();
    void setupConnections();
    void updateLastUpdateLabel();
    void loadLanguage(const QString& language);
    void setupLanguageMenu();
    void populateCurrencyComboBoxes();
    void updateConversionResult();
    QString formatCurrency(double amount, const QString& currencyCode);

    CurrencyApiManager* m_apiManager;
    QTimer* m_updateTimer;
    
    // UI Components
    QLineEdit* m_amountInput;
    QComboBox* m_fromCurrencyCombo;
    QComboBox* m_toCurrencyCombo;
    QPushButton* m_convertButton;
    QPushButton* m_swapButton;
    QPushButton* m_refreshButton;
    QPushButton* m_themeButton;
    QPushButton* m_languageButton;
    QLabel* m_resultLabel;
    QLabel* m_exchangeRateLabel;
    QLabel* m_lastUpdate;
    QLabel* m_loadingLabel;
    
    // Data
    QMap<QString, CurrencyApiManager::CurrencyRate> m_currentRates;
    QDateTime lastUpdateTime;
    
    // Settings
    Language m_language;
    Theme m_theme;
    QTranslator m_translator;
    QString m_currentLanguage;
};

#endif // MAINWINDOW_H 