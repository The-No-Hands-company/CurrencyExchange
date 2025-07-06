#include "core/theme.h"
#include <QApplication>
#include <QSettings>
#include <QStyle>
#include <QPalette>
#include <QColor>
#include <QString>
#include <QMap>
#include <QStringList>

Theme::Theme(QObject *parent)
    : QObject(parent)
    , m_mode(Mode::Light)
{
    initializeDefaultColors();
    
    // Load saved theme mode
    QSettings settings;
    QString savedMode = settings.value("theme/mode", "light").toString();
    m_mode = stringToMode(savedMode);
    
    // If it's a custom theme, load the colors
    if (m_mode == Mode::Custom) {
        loadTheme();
    }
}

Theme::~Theme()
{
}

void Theme::setMode(Mode mode)
{
    if (m_mode != mode) {
        m_mode = mode;
        emit themeChanged();
    }
}

void Theme::setCustomColors(const QMap<QString, QColor>& colors)
{
    m_customColors = colors;
    m_mode = Mode::Custom;
    emit themeChanged();
}

QString Theme::modeToString(Mode mode)
{
    switch (mode) {
        case Mode::Dark:
            return "dark";
        case Mode::Custom:
            return "custom";
        default:
            return "light";
    }
}

Theme::Mode Theme::stringToMode(const QString& mode)
{
    if (mode == "dark") {
        return Mode::Dark;
    } else if (mode == "custom") {
        return Mode::Custom;
    }
    return Mode::Light;
}

void Theme::applyTheme(QPalette& palette)
{
    switch (m_mode) {
        case Mode::Dark:
            palette.setColor(QPalette::Window, QColor(53, 53, 53));
            palette.setColor(QPalette::WindowText, Qt::white);
            palette.setColor(QPalette::Base, QColor(25, 25, 25));
            palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
            palette.setColor(QPalette::ToolTipBase, Qt::white);
            palette.setColor(QPalette::ToolTipText, Qt::white);
            palette.setColor(QPalette::Text, Qt::white);
            palette.setColor(QPalette::Button, QColor(53, 53, 53));
            palette.setColor(QPalette::ButtonText, Qt::white);
            palette.setColor(QPalette::BrightText, Qt::red);
            palette.setColor(QPalette::Link, QColor(42, 130, 218));
            palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
            palette.setColor(QPalette::HighlightedText, Qt::black);
            palette.setColor(QPalette::PlaceholderText, Qt::white);
            palette.setColor(QPalette::Light, QColor(53, 53, 53));
            palette.setColor(QPalette::Midlight, QColor(53, 53, 53));
            palette.setColor(QPalette::Dark, QColor(35, 35, 35));
            palette.setColor(QPalette::Mid, QColor(35, 35, 35));
            palette.setColor(QPalette::Shadow, QColor(20, 20, 20));
            qApp->setStyleSheet(
                "QMenu { background-color: rgb(53, 53, 53); color: white; border: 1px solid rgb(80, 80, 80); } "
                "QMenu::item:selected { background-color: rgb(42, 130, 218); color: black; }"
            );
            break;
            
        case Mode::Light:
            palette = QApplication::style()->standardPalette();
            qApp->setStyleSheet("");
            break;
            
        case Mode::Custom:
            if (m_customColors.contains("window"))
                palette.setColor(QPalette::Window, m_customColors["window"]);
            if (m_customColors.contains("windowText"))
                palette.setColor(QPalette::WindowText, m_customColors["windowText"]);
            if (m_customColors.contains("base"))
                palette.setColor(QPalette::Base, m_customColors["base"]);
            if (m_customColors.contains("alternateBase"))
                palette.setColor(QPalette::AlternateBase, m_customColors["alternateBase"]);
            if (m_customColors.contains("toolTipBase"))
                palette.setColor(QPalette::ToolTipBase, m_customColors["toolTipBase"]);
            if (m_customColors.contains("toolTipText"))
                palette.setColor(QPalette::ToolTipText, m_customColors["toolTipText"]);
            if (m_customColors.contains("text"))
                palette.setColor(QPalette::Text, m_customColors["text"]);
            if (m_customColors.contains("button"))
                palette.setColor(QPalette::Button, m_customColors["button"]);
            if (m_customColors.contains("buttonText"))
                palette.setColor(QPalette::ButtonText, m_customColors["buttonText"]);
            if (m_customColors.contains("brightText"))
                palette.setColor(QPalette::BrightText, m_customColors["brightText"]);
            if (m_customColors.contains("link"))
                palette.setColor(QPalette::Link, m_customColors["link"]);
            if (m_customColors.contains("highlight"))
                palette.setColor(QPalette::Highlight, m_customColors["highlight"]);
            if (m_customColors.contains("highlightedText"))
                palette.setColor(QPalette::HighlightedText, m_customColors["highlightedText"]);

            // Add menu styling using custom colors
            QString menuStyle = QString(
                "QMenu { background-color: %1; color: %2; border: 1px solid %3; } "
                "QMenu::item:selected { background-color: %4; color: %5; }"
            ).arg(
                m_customColors.value("window", QColor(53, 53, 53)).name(),
                m_customColors.value("windowText", Qt::white).name(),
                m_customColors.value("alternateBase", QColor(80, 80, 80)).name(),
                m_customColors.value("highlight", QColor(42, 130, 218)).name(),
                m_customColors.value("highlightedText", Qt::black).name()
            );
            qApp->setStyleSheet(menuStyle);
            break;
    }
}

void Theme::saveTheme(const QString& name)
{
    QSettings settings;
    
    // Save current theme mode
    settings.setValue("theme/mode", modeToString(m_mode));
    
    if (m_mode == Mode::Custom) {
        QString key = themeSettingsKey(name);
        settings.setValue(key + "/name", name);
        settings.setValue(key + "/window", m_customColors["window"].name());
        settings.setValue(key + "/windowText", m_customColors["windowText"].name());
        settings.setValue(key + "/base", m_customColors["base"].name());
        settings.setValue(key + "/alternateBase", m_customColors["alternateBase"].name());
        settings.setValue(key + "/toolTipBase", m_customColors["toolTipBase"].name());
        settings.setValue(key + "/toolTipText", m_customColors["toolTipText"].name());
        settings.setValue(key + "/text", m_customColors["text"].name());
        settings.setValue(key + "/button", m_customColors["button"].name());
        settings.setValue(key + "/buttonText", m_customColors["buttonText"].name());
        settings.setValue(key + "/link", m_customColors["link"].name());
        settings.setValue(key + "/highlight", m_customColors["highlight"].name());
        settings.setValue(key + "/highlightedText", m_customColors["highlightedText"].name());
        
        // Add to saved themes list if it has a name
        if (!name.isEmpty()) {
            QStringList themes = settings.value("theme/saved_themes").toStringList();
            if (!themes.contains(name)) {
                themes.append(name);
                settings.setValue("theme/saved_themes", themes);
            }
        }
    }
}

void Theme::loadTheme(const QString& name)
{
    QSettings settings;
    Mode mode = stringToMode(settings.value("theme/mode", "light").toString());
    
    if (mode == Mode::Custom) {
        QString key = themeSettingsKey(name);
        QMap<QString, QColor> colors;
        colors["window"] = QColor(settings.value(key + "/window").toString());
        colors["windowText"] = QColor(settings.value(key + "/windowText").toString());
        colors["base"] = QColor(settings.value(key + "/base").toString());
        colors["alternateBase"] = QColor(settings.value(key + "/alternateBase").toString());
        colors["toolTipBase"] = QColor(settings.value(key + "/toolTipBase").toString());
        colors["toolTipText"] = QColor(settings.value(key + "/toolTipText").toString());
        colors["text"] = QColor(settings.value(key + "/text").toString());
        colors["button"] = QColor(settings.value(key + "/button").toString());
        colors["buttonText"] = QColor(settings.value(key + "/buttonText").toString());
        colors["link"] = QColor(settings.value(key + "/link").toString());
        colors["highlight"] = QColor(settings.value(key + "/highlight").toString());
        colors["highlightedText"] = QColor(settings.value(key + "/highlightedText").toString());
        setCustomColors(colors);
    } else {
        setMode(mode);
    }
}

void Theme::deleteTheme(const QString& name)
{
    if (name.isEmpty()) return;
    
    QSettings settings;
    
    // Remove theme from saved themes list
    QStringList themes = settings.value("theme/saved_themes").toStringList();
    themes.removeOne(name);
    settings.setValue("theme/saved_themes", themes);
    
    // Remove theme settings
    QString key = themeSettingsKey(name);
    settings.remove(key);
}

QStringList Theme::savedThemes() const
{
    QSettings settings;
    return settings.value("theme/saved_themes").toStringList();
}

bool Theme::loadSavedTheme(const QString& name)
{
    QSettings settings;
    QString key = themeSettingsKey(name);
    
    if (settings.contains(key + "/name")) {
        loadTheme(name);
        return true;
    }
    return false;
}

QString Theme::themeSettingsKey(const QString& name) const
{
    return name.isEmpty() ? "theme/colors" : QString("theme/saved/%1/colors").arg(name);
}

void Theme::initializeDefaultColors()
{
    m_customColors["window"] = QColor(53, 53, 53);
    m_customColors["windowText"] = Qt::white;
    m_customColors["base"] = QColor(25, 25, 25);
    m_customColors["alternateBase"] = QColor(53, 53, 53);
    m_customColors["toolTipBase"] = Qt::white;
    m_customColors["toolTipText"] = Qt::white;
    m_customColors["text"] = Qt::white;
    m_customColors["button"] = QColor(53, 53, 53);
    m_customColors["buttonText"] = Qt::white;
    m_customColors["brightText"] = Qt::red;
    m_customColors["link"] = QColor(42, 130, 218);
    m_customColors["highlight"] = QColor(42, 130, 218);
    m_customColors["highlightedText"] = Qt::black;
} 