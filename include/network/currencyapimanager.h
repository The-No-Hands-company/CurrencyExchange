#ifndef CURRENCYAPIMANAGER_H
#define CURRENCYAPIMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrl>
#include <QUrlQuery>
#include <QMap>
#include <QDateTime>

class CurrencyApiManager : public QObject
{
    Q_OBJECT

public:
    struct CurrencyRate {
        QString currencyCode;
        QString currencyName;
        QString symbol;
        double rate = 0.0;
        QDateTime lastUpdated;
    };

    explicit CurrencyApiManager(QObject *parent = nullptr);
    ~CurrencyApiManager();

    void fetchExchangeRates();
    void fetchSpecificRates(const QStringList& currencies);
    
    QMap<QString, CurrencyRate> currentRates() const { return m_rates; }
    bool isLoading() const { return m_isLoading; }
    
    // Get available currencies
    QStringList getAvailableCurrencies() const;
    QStringList getPopularCurrencies() const;
    
    // Settings
    void setUpdateInterval(int minutes);
    void setRequestTimeout(int seconds);
    void setBaseCurrency(const QString& currency);
    QString baseCurrency() const { return m_baseCurrency; }

signals:
    void ratesUpdated(const QMap<QString, CurrencyRate>& rates);
    void loadingChanged(bool loading);
    void error(const QString& message);

private slots:
    void handleExchangeRateResponse();
    void handleNetworkError(QNetworkReply::NetworkError error);
    void handleTimeout();

private:
    void makeRequest(const QUrl& url);
    void processResponse(QNetworkReply* reply);
    void setLoading(bool loading);
    void scheduleRetry();
    void initializeCurrencyData();

    QNetworkAccessManager* m_networkManager;
    QTimer* m_updateTimer;
    QTimer* m_timeoutTimer;
    
    QMap<QString, CurrencyRate> m_rates;
    QMap<QString, QString> m_currencyNames;
    QMap<QString, QString> m_currencySymbols;
    
    bool m_isLoading;
    int m_requestTimeout;
    int m_maxRetries;
    int m_currentRetries;
    QString m_baseCurrency;
    
    QNetworkReply* m_currentReply;
};

#endif // CURRENCYAPIMANAGER_H
