#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QObject>
#include <QString>
#include <QTranslator>
#include <QList>
#include <QCoreApplication>
#include <QFile>
#include <QLocale>
#include <QSettings>
#include <QDebug>
#include <QVector>

struct LanguageInfo {
    QString name;
    QString code;
    QString filename;
    
    LanguageInfo(const QString& n, const QString& c, const QString& f)
        : name(n), code(c), filename(f) {}
};

class Language : public QObject
{
    Q_OBJECT

public:
    explicit Language(QObject *parent = nullptr);
    ~Language() override;

    QList<LanguageInfo> availableLanguages();
    QString currentLanguage();
    void setCurrentLanguage(const QString& code);

    static QString getSystemLanguage();
    void saveLanguage() const;
    void loadLanguage();

signals:
    void languageChanged();

private:
    bool loadTranslationFile(const QString &languageCode);
    QTranslator m_translator;
    QString m_currentLanguage;
    QList<LanguageInfo> m_languages;
    QVector<LanguageInfo> m_languagesVector;
};

#endif // LANGUAGE_H 