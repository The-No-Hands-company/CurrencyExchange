#include "core/language.h"
#include <QTranslator>
#include <QApplication>
#include <QSettings>
#include <QDir>
#include <QDebug>

Language::Language(QObject *parent)
    : QObject(parent)
    , m_currentLanguage("en")
{
    // Initialize available languages
    m_languages.append(LanguageInfo(tr("English"), "en", ""));
    m_languages.append(LanguageInfo(tr("Spanish"), "es", "bitcoinsatoshicalculator_es.qm"));
    m_languages.append(LanguageInfo(tr("French"), "fr", "bitcoinsatoshicalculator_fr.qm"));
    m_languages.append(LanguageInfo(tr("German"), "de", "bitcoinsatoshicalculator_de.qm"));
    m_languages.append(LanguageInfo(tr("Italian"), "it", "bitcoinsatoshicalculator_it.qm"));
    m_languages.append(LanguageInfo(tr("Portuguese"), "pt", "bitcoinsatoshicalculator_pt.qm"));
    m_languages.append(LanguageInfo(tr("Russian"), "ru", "bitcoinsatoshicalculator_ru.qm"));
    m_languages.append(LanguageInfo(tr("Japanese"), "ja", "bitcoinsatoshicalculator_ja.qm"));
    m_languages.append(LanguageInfo(tr("Korean"), "ko", "bitcoinsatoshicalculator_ko.qm"));
    m_languages.append(LanguageInfo(tr("Chinese"), "zh", "bitcoinsatoshicalculator_zh.qm"));
    m_languages.append(LanguageInfo(tr("Arabic"), "ar", "bitcoinsatoshicalculator_ar.qm"));
    m_languages.append(LanguageInfo(tr("Hebrew"), "he", "bitcoinsatoshicalculator_he.qm"));
    
    loadLanguage();
}

Language::~Language()
{
}

bool Language::loadTranslationFile(const QString &languageCode) {
    // Remove existing translator if any
    if (!m_translator.isEmpty()) {
        qApp->removeTranslator(&m_translator);
    }

    // If English is selected, no translation needed
    if (languageCode == "en") {
        m_currentLanguage = languageCode;
        return true;
    }

    // Find the language info
    auto it = std::find_if(m_languages.begin(), m_languages.end(),
                          [languageCode](const LanguageInfo &lang) {
                              return lang.code == languageCode;
                          });

    if (it == m_languages.end() || it->filename.isEmpty()) {
        qDebug() << "Language not found or no translation file specified";
        return false;
    }

    // First try loading from resources
    if (m_translator.load(":/translations/" + it->filename)) {
        qApp->installTranslator(&m_translator);
        m_currentLanguage = languageCode;
        qDebug() << "Successfully loaded translation from resources:" << it->filename;
        return true;
    }

    // If not found in resources, search in file system
    QStringList searchPaths = {
        QCoreApplication::applicationDirPath() + "/translations",
        "translations",
        "../translations",
        "build/translations"
    };

    for (const QString &path : searchPaths) {
        QString filePath = path + "/" + it->filename;
        qDebug() << "Trying to load translation from:" << filePath;
        
        if (QFile::exists(filePath) && m_translator.load(filePath)) {
            qApp->installTranslator(&m_translator);
            m_currentLanguage = languageCode;
            qDebug() << "Successfully loaded translation from:" << filePath;
            return true;
        }
    }

    qDebug() << "Failed to load translation file for language:" << languageCode;
    return false;
}

QString Language::getSystemLanguage() {
    return QLocale::system().name().split('_').first();
}

void Language::saveLanguage() const {
    QSettings settings;
    settings.setValue("language/current", m_currentLanguage);
}

void Language::loadLanguage()
{
    QSettings settings;
    QString language = settings.value("language", "en").toString();
    m_currentLanguage = language;
    
    if (language == "en") {
        return; // No translation needed for English
    }
    
    // Remove existing translator
    if (!m_translator.isEmpty()) {
        QApplication::removeTranslator(&m_translator);
    }
    
    // Load translation file
    if (loadTranslationFile(language)) {
        emit languageChanged();
    }
}

QList<LanguageInfo> Language::availableLanguages()
{
    QList<LanguageInfo> languages;
    languages.append(LanguageInfo(tr("English"), "en", "bitcoinsatoshicalculator_en.qm"));
    languages.append(LanguageInfo(tr("Spanish"), "es", "bitcoinsatoshicalculator_es.qm"));
    languages.append(LanguageInfo(tr("French"), "fr", "bitcoinsatoshicalculator_fr.qm"));
    languages.append(LanguageInfo(tr("German"), "de", "bitcoinsatoshicalculator_de.qm"));
    languages.append(LanguageInfo(tr("Italian"), "it", "bitcoinsatoshicalculator_it.qm"));
    languages.append(LanguageInfo(tr("Portuguese"), "pt", "bitcoinsatoshicalculator_pt.qm"));
    languages.append(LanguageInfo(tr("Russian"), "ru", "bitcoinsatoshicalculator_ru.qm"));
    languages.append(LanguageInfo(tr("Japanese"), "ja", "bitcoinsatoshicalculator_ja.qm"));
    languages.append(LanguageInfo(tr("Korean"), "ko", "bitcoinsatoshicalculator_ko.qm"));
    languages.append(LanguageInfo(tr("Chinese"), "zh", "bitcoinsatoshicalculator_zh.qm"));
    languages.append(LanguageInfo(tr("Arabic"), "ar", "bitcoinsatoshicalculator_ar.qm"));
    languages.append(LanguageInfo(tr("Hebrew"), "he", "bitcoinsatoshicalculator_he.qm"));
    return languages;
}

QString Language::currentLanguage()
{
    QSettings settings;
    return settings.value("language", "en").toString();
}

void Language::setCurrentLanguage(const QString& code)
{
    if (m_currentLanguage != code) {
        m_currentLanguage = code;
        QSettings settings;
        settings.setValue("language", code);
        
        if (loadTranslationFile(code)) {
            emit languageChanged();
        }
    }
} 