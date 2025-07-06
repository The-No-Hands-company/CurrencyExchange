# Tests Directory

This directory contains unit tests for the Currency Exchange Calculator application.

## Test Files

- `test_currencyapi.cpp` - Tests for the CurrencyApiManager class
- `test_mainwindow.cpp` - Tests for the MainWindow UI class

## Running Tests

### Prerequisites
- Qt Test framework (included with Qt)
- CMake 3.16 or later
- Built application

### Build and Run Tests

```bash
# From the project root directory
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release

# Run individual tests
cd bin/Release
./test_currencyapi.exe
./test_mainwindow.exe
```

### Test Configuration

Tests are configured to:
- Initialize Qt application context
- Create temporary test objects
- Verify core functionality
- Clean up after execution

### Adding New Tests

1. Create a new `.cpp` file in this directory
2. Include the Qt Test framework: `#include <QtTest/QtTest>`
3. Create a test class inheriting from `QObject`
4. Add test methods as private slots
5. Use `QTEST_MAIN()` macro to create the test executable

### Example Test Structure

```cpp
#include <QtTest/QtTest>

class TestExample : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testFunction();
    void cleanupTestCase();
};

void TestExample::initTestCase()
{
    // Setup before all tests
}

void TestExample::testFunction()
{
    QVERIFY(someCondition);
    QCOMPARE(actual, expected);
}

void TestExample::cleanupTestCase()
{
    // Cleanup after all tests
}

QTEST_MAIN(TestExample)
#include "test_example.moc"
```

## Test Coverage

Current tests cover:
- ✅ Currency API Manager functionality
- ✅ Main Window UI components
- ✅ Currency conversion logic
- ✅ Input validation
- ✅ Error handling

## Notes

- Tests may require network connectivity for API testing
- Some tests include timeouts for async operations
- UI tests require display capability (not suitable for headless CI)
