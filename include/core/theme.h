#ifndef THEME_H
#define THEME_H

#include <QObject>
#include <QColor>
#include <QMap>
#include <QString>
#include <QPalette>
#include <QStringList>

class Theme : public QObject
{
    Q_OBJECT

public:
    enum class Mode {
        Light,
        Dark,
        Custom
    };

    explicit Theme(QObject *parent = nullptr);
    ~Theme() override;

    void setMode(Mode mode);
    Mode currentMode() const { return m_mode; }

    void setCustomColors(const QMap<QString, QColor>& colors);
    QMap<QString, QColor> colors() const { return m_customColors; }

    static QString modeToString(Mode mode);
    static Mode stringToMode(const QString& mode);

    void applyTheme(QPalette& palette);
    void saveTheme(const QString& name = QString());
    void loadTheme(const QString& name = QString());
    void deleteTheme(const QString& name);
    QStringList savedThemes() const;
    bool loadSavedTheme(const QString& name);

signals:
    void themeChanged();

private:
    Mode m_mode;
    QMap<QString, QColor> m_customColors;
    
    void initializeDefaultColors();
    QString themeSettingsKey(const QString& name) const;
};

#endif // THEME_H 