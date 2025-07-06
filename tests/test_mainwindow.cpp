#include <QtTest/QtTest>
#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include "ui/mainwindow.h"

class TestMainWindow : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testWindowCreation();
    void testUserInterface();
    void testCurrencySelection();
    void testInputValidation();

private:
    QApplication* app;
    MainWindow* window;
};

void TestMainWindow::initTestCase()
{
    // Create application instance if not already created
    if (!QApplication::instance()) {
        static int argc = 1;
        static char* argv[] = {"test"};
        app = new QApplication(argc, argv);
    }
    
    window = new MainWindow();
    window->show();
    QTest::qWaitForWindowExposed(window);
}

void TestMainWindow::cleanupTestCase()
{
    delete window;
    window = nullptr;
}

void TestMainWindow::testWindowCreation()
{
    QVERIFY(window != nullptr);
    QVERIFY(window->isVisible());
    QVERIFY(!window->windowTitle().isEmpty());
}

void TestMainWindow::testUserInterface()
{
    // Test that main UI elements are present
    QLineEdit* amountEdit = window->findChild<QLineEdit*>("amountEdit");
    QVERIFY(amountEdit != nullptr);
    
    QComboBox* fromCurrencyCombo = window->findChild<QComboBox*>("fromCurrencyCombo");
    QVERIFY(fromCurrencyCombo != nullptr);
    
    QComboBox* toCurrencyCombo = window->findChild<QComboBox*>("toCurrencyCombo");
    QVERIFY(toCurrencyCombo != nullptr);
    
    QLabel* resultLabel = window->findChild<QLabel*>("resultLabel");
    QVERIFY(resultLabel != nullptr);
}

void TestMainWindow::testCurrencySelection()
{
    QComboBox* fromCurrencyCombo = window->findChild<QComboBox*>("fromCurrencyCombo");
    QComboBox* toCurrencyCombo = window->findChild<QComboBox*>("toCurrencyCombo");
    
    if (fromCurrencyCombo && toCurrencyCombo) {
        QVERIFY(fromCurrencyCombo->count() > 0);
        QVERIFY(toCurrencyCombo->count() > 0);
        
        // Test that we have expected currencies
        QStringList items;
        for (int i = 0; i < fromCurrencyCombo->count(); ++i) {
            items << fromCurrencyCombo->itemText(i);
        }
        
        QVERIFY(items.contains("USD - US Dollar"));
        QVERIFY(items.contains("EUR - Euro"));
        QVERIFY(items.contains("SEK - Swedish Krona"));
        QVERIFY(items.contains("IDR - Indonesian Rupiah"));
    }
}

void TestMainWindow::testInputValidation()
{
    QLineEdit* amountEdit = window->findChild<QLineEdit*>("amountEdit");
    
    if (amountEdit) {
        // Test valid input
        amountEdit->setText("100.50");
        QCOMPARE(amountEdit->text(), QString("100.50"));
        
        // Test clearing
        amountEdit->clear();
        QVERIFY(amountEdit->text().isEmpty());
        
        // Test setting focus
        amountEdit->setFocus();
        QVERIFY(amountEdit->hasFocus());
    }
}

QTEST_MAIN(TestMainWindow)
#include "test_mainwindow.moc"
