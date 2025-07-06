#include "network/currencyapimanager.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

CurrencyApiManager::CurrencyApiManager(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_updateTimer(new QTimer(this))
    , m_timeoutTimer(new QTimer(this))
    , m_isLoading(false)
    , m_requestTimeout(10000)
    , m_maxRetries(3)
    , m_currentRetries(0)
    , m_baseCurrency("USD")
    , m_currentReply(nullptr)
{
    // Initialize currency data
    initializeCurrencyData();
    
    // Set up update timer (every 5 minutes)
    m_updateTimer->setInterval(300000);
    connect(m_updateTimer, &QTimer::timeout, this, &CurrencyApiManager::fetchExchangeRates);
    
    // Set up timeout timer
    m_timeoutTimer->setSingleShot(true);
    connect(m_timeoutTimer, &QTimer::timeout, this, &CurrencyApiManager::handleTimeout);
    
    // Start initial fetch
    fetchExchangeRates();
    m_updateTimer->start();
}

CurrencyApiManager::~CurrencyApiManager()
{
    if (m_currentReply) {
        m_currentReply->abort();
    }
}

void CurrencyApiManager::initializeCurrencyData()
{
    // Popular world currencies with their names and symbols
    m_currencyNames["USD"] = "US Dollar";
    m_currencyNames["EUR"] = "Euro";
    m_currencyNames["SEK"] = "Swedish Krona";
    m_currencyNames["IDR"] = "Indonesian Rupiah";
    m_currencyNames["GBP"] = "British Pound";
    m_currencyNames["JPY"] = "Japanese Yen";
    m_currencyNames["AUD"] = "Australian Dollar";
    m_currencyNames["CAD"] = "Canadian Dollar";
    m_currencyNames["CHF"] = "Swiss Franc";
    m_currencyNames["CNY"] = "Chinese Yuan";
    m_currencyNames["INR"] = "Indian Rupee";
    m_currencyNames["KRW"] = "South Korean Won";
    m_currencyNames["SGD"] = "Singapore Dollar";
    m_currencyNames["THB"] = "Thai Baht";
    m_currencyNames["MYR"] = "Malaysian Ringgit";
    m_currencyNames["PHP"] = "Philippine Peso";
    m_currencyNames["VND"] = "Vietnamese Dong";
    m_currencyNames["NOK"] = "Norwegian Krone";
    m_currencyNames["DKK"] = "Danish Krone";
    m_currencyNames["PLN"] = "Polish Zloty";
    m_currencyNames["CZK"] = "Czech Koruna";
    m_currencyNames["HUF"] = "Hungarian Forint";
    m_currencyNames["RUB"] = "Russian Ruble";
    m_currencyNames["BRL"] = "Brazilian Real";
    m_currencyNames["MXN"] = "Mexican Peso";
    m_currencyNames["ARS"] = "Argentine Peso";
    m_currencyNames["CLP"] = "Chilean Peso";
    m_currencyNames["ZAR"] = "South African Rand";
    m_currencyNames["TRY"] = "Turkish Lira";
    m_currencyNames["ILS"] = "Israeli Shekel";
    m_currencyNames["AED"] = "UAE Dirham";
    m_currencyNames["SAR"] = "Saudi Riyal";
    
    // Currency symbols
    m_currencySymbols["USD"] = "$";
    m_currencySymbols["EUR"] = "€";
    m_currencySymbols["SEK"] = "kr";
    m_currencySymbols["IDR"] = "Rp";
    m_currencySymbols["GBP"] = "£";
    m_currencySymbols["JPY"] = "¥";
    m_currencySymbols["AUD"] = "A$";
    m_currencySymbols["CAD"] = "C$";
    m_currencySymbols["CHF"] = "CHF";
    m_currencySymbols["CNY"] = "¥";
    m_currencySymbols["INR"] = "₹";
    m_currencySymbols["KRW"] = "₩";
    m_currencySymbols["SGD"] = "S$";
    m_currencySymbols["THB"] = "฿";
    m_currencySymbols["MYR"] = "RM";
    m_currencySymbols["PHP"] = "₱";
    m_currencySymbols["VND"] = "₫";
    m_currencySymbols["NOK"] = "kr";
    m_currencySymbols["DKK"] = "kr";
    m_currencySymbols["PLN"] = "zł";
    m_currencySymbols["CZK"] = "Kč";
    m_currencySymbols["HUF"] = "Ft";
    m_currencySymbols["RUB"] = "₽";
    m_currencySymbols["BRL"] = "R$";
    m_currencySymbols["MXN"] = "$";
    m_currencySymbols["ARS"] = "$";
    m_currencySymbols["CLP"] = "$";
    m_currencySymbols["ZAR"] = "R";
    m_currencySymbols["TRY"] = "₺";
    m_currencySymbols["ILS"] = "₪";
    m_currencySymbols["AED"] = "د.إ";
    m_currencySymbols["SAR"] = "﷼";
}

void CurrencyApiManager::fetchExchangeRates()
{
    if (m_isLoading) {
        return; // Already loading
    }
    
    setLoading(true);
    m_currentRetries = 0;
    
    // Get popular currencies
    QStringList currencies = getPopularCurrencies();
    fetchSpecificRates(currencies);
}

void CurrencyApiManager::fetchSpecificRates(const QStringList& currencies)
{
    // Use exchangerate-api.com (free tier, no API key required for basic usage)
    // Alternative: fixer.io, currencylayer.com, or exchangeratesapi.io
    QString currencyList = currencies.join(",");
    
    QUrl url(QString("https://api.exchangerate-api.com/v4/latest/%1").arg(m_baseCurrency));
    
    makeRequest(url);
}

void CurrencyApiManager::makeRequest(const QUrl& url)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Currency Exchange Calculator/1.0");
    request.setRawHeader("Accept", "application/json");
    request.setTransferTimeout(m_requestTimeout);
    
    m_currentReply = m_networkManager->get(request);
    
    connect(m_currentReply, &QNetworkReply::finished, this, &CurrencyApiManager::handleExchangeRateResponse);
    connect(m_currentReply, &QNetworkReply::errorOccurred, this, &CurrencyApiManager::handleNetworkError);
    
    // Start timeout timer
    m_timeoutTimer->start(m_requestTimeout + 5000);
}

void CurrencyApiManager::handleExchangeRateResponse()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;
    
    processResponse(reply);
    m_currentReply = nullptr;
    reply->deleteLater();
}

void CurrencyApiManager::processResponse(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject obj = doc.object();
        
        if (obj.contains("rates")) {
            QJsonObject rates = obj["rates"].toObject();
            QDateTime updateTime = QDateTime::currentDateTime();
            
            // Clear old rates
            m_rates.clear();
            
            // Add base currency (always 1.0)
            CurrencyRate baseRate;
            baseRate.currencyCode = m_baseCurrency;
            baseRate.currencyName = m_currencyNames.value(m_baseCurrency, m_baseCurrency);
            baseRate.symbol = m_currencySymbols.value(m_baseCurrency, m_baseCurrency);
            baseRate.rate = 1.0;
            baseRate.lastUpdated = updateTime;
            m_rates[m_baseCurrency] = baseRate;
            
            // Process all available rates
            for (auto it = rates.begin(); it != rates.end(); ++it) {
                QString currency = it.key();
                double rate = it.value().toDouble();
                
                if (rate > 0) {
                    CurrencyRate currencyRate;
                    currencyRate.currencyCode = currency;
                    currencyRate.currencyName = m_currencyNames.value(currency, currency);
                    currencyRate.symbol = m_currencySymbols.value(currency, currency);
                    currencyRate.rate = rate;
                    currencyRate.lastUpdated = updateTime;
                    
                    m_rates[currency] = currencyRate;
                }
            }
            
            setLoading(false);
            emit ratesUpdated(m_rates);
            
            qDebug() << "Exchange rates updated for" << m_rates.size() << "currencies";
        } else {
            scheduleRetry();
        }
    } else {
        scheduleRetry();
    }
}

void CurrencyApiManager::handleNetworkError(QNetworkReply::NetworkError error)
{
    Q_UNUSED(error)
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (reply) {
        qDebug() << "Network error:" << reply->errorString();
        emit this->error(reply->errorString());
    }
    scheduleRetry();
}

void CurrencyApiManager::handleTimeout()
{
    if (m_currentReply && m_currentReply->isRunning()) {
        m_currentReply->abort();
    }
    
    emit error("Request timeout");
    scheduleRetry();
}

void CurrencyApiManager::scheduleRetry()
{
    if (m_currentRetries < m_maxRetries) {
        m_currentRetries++;
        
        // Retry after delay (exponential backoff)
        int delay = 2000 * m_currentRetries;
        QTimer::singleShot(delay, this, [this]() {
            fetchExchangeRates();
        });
    } else {
        setLoading(false);
        emit error("Failed to fetch exchange rates after " + QString::number(m_maxRetries) + " retries");
    }
}

void CurrencyApiManager::setLoading(bool loading)
{
    if (m_isLoading != loading) {
        m_isLoading = loading;
        emit loadingChanged(loading);
    }
}

QStringList CurrencyApiManager::getAvailableCurrencies() const
{
    return m_currencyNames.keys();
}

QStringList CurrencyApiManager::getPopularCurrencies() const
{
    // Return most commonly used currencies for international exchange
    return {
        "USD", "EUR", "SEK", "IDR", "GBP", "JPY", "AUD", "CAD", 
        "CHF", "CNY", "INR", "KRW", "SGD", "THB", "MYR", "PHP", 
        "VND", "NOK", "DKK", "PLN", "BRL", "MXN", "ZAR", "TRY"
    };
}

void CurrencyApiManager::setUpdateInterval(int minutes)
{
    m_updateTimer->setInterval(minutes * 60000);
}

void CurrencyApiManager::setRequestTimeout(int seconds)
{
    m_requestTimeout = seconds * 1000;
}

void CurrencyApiManager::setBaseCurrency(const QString& currency)
{
    if (m_baseCurrency != currency) {
        m_baseCurrency = currency;
        // Refresh rates with new base currency
        if (!m_isLoading) {
            fetchExchangeRates();
        }
    }
}
