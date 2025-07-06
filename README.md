# Live Currency Exchange Calculator

A modern and efficient real-time currency converter supporting major world currencies including Swedish Krona (SEK), Indonesian Rupiah (IDR), Euro (EUR), US Dollar (USD), and many more.

## ✨ Features

- **Real-time Exchange Rates**: Live currency conversion with up-to-date exchange rates
- **Extensive Currency Support**: 25+ major world currencies including:
  - 🇺🇸 USD (US Dollar)
  - 🇪🇺 EUR (Euro) 
  - 🇸🇪 SEK (Swedish Krona)
  - 🇮🇩 IDR (Indonesian Rupiah)
  - 🇬🇧 GBP (British Pound)
  - 🇯🇵 JPY (Japanese Yen)
  - 🇦🇺 AUD (Australian Dollar)
  - 🇨🇦 CAD (Canadian Dollar)
  - And many more...

- **Smart Features**:
  - Auto-conversion as you type
  - Quick currency swap button
  - Formatted currency display with proper symbols
  - Real-time exchange rate display
  - Automatic rate updates every 5 minutes

- **User-Friendly Interface**:
  - Clean, modern design
  - Responsive layout
  - Input validation
  - Loading indicators
  - Error handling with recovery

- **Customization**:
  - Light and dark themes
  - Multiple language support
  - Persistent settings

## 🚀 Perfect For

- **Travelers**: Converting between currencies when planning trips
- **Business**: International commerce and pricing
- **Students**: Learning about global currencies and exchange rates
- **Daily Use**: Quick currency conversions for online shopping, remittances, etc.

## 📦 Installation

### For Users

1. Download the latest release from the releases page
2. Extract the files to a folder of your choice
3. Run `CurrencyExchange.exe`

### Requirements

- Windows 10 or later
- Internet connection for real-time exchange rates

## 🎯 Usage

1. **Enter Amount**: Type the amount you want to convert
2. **Select From Currency**: Choose the source currency (e.g., USD)
3. **Select To Currency**: Choose the target currency (e.g., SEK)
4. **Instant Conversion**: See results immediately with live exchange rates
5. **Swap Currencies**: Use the ⇄ button to quickly swap from/to currencies
6. **Refresh Rates**: Manual refresh for latest exchange rates

### Example Conversions
- Convert USD to Swedish Krona for travel planning
- Check Indonesian Rupiah exchange rates for business
- Compare Euro to Dollar rates for international purchases

## 🌐 API Sources

The application uses reliable financial APIs to ensure accurate exchange rates:

1. **Primary**: ExchangeRate-API (free tier, reliable data)
2. **Fallback**: Additional API sources for redundancy
3. **Update Frequency**: Every 5 minutes for live rates

## 🛠️ For Developers

### Prerequisites
- Qt 6.8.1 or later
- CMake 3.16 or later
- Visual Studio 2022 or MinGW compiler

### Building
```bash
# Clone the repository
git clone https://github.com/The-No-Hands-company/CurrencyExchange.git
cd CurrencyExchange

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64

# Build the project
cmake --build . --config Release
```

### Project Structure
```
CurrencyExchange/
├── src/
│   ├── main.cpp                     # Application entry point
│   ├── ui/mainwindow.cpp            # Main UI window
│   ├── network/currencyapimanager.cpp # Exchange rate API management
│   ├── core/theme.cpp               # Theme system
│   └── core/language.cpp            # Language support
├── include/                         # Header files
├── resources/                       # Application resources
│   ├── translations/                # Translation files
│   ├── icons/                       # Application icons
│   └── themes/                      # Theme files
├── docs/                           # Documentation
├── scripts/                        # Build and deployment scripts
└── tests/                          # Unit tests
```

## 🔧 Configuration

The application automatically saves your preferences:
- Last selected currencies
- Preferred theme
- Language setting
- Window position and size

## 📱 Supported Currencies

### Major Currencies
- USD 🇺🇸 - US Dollar
- EUR 🇪🇺 - Euro
- GBP 🇬🇧 - British Pound
- JPY 🇯🇵 - Japanese Yen

### Nordic Currencies
- SEK 🇸🇪 - Swedish Krona
- NOK 🇳🇴 - Norwegian Krone
- DKK 🇩🇰 - Danish Krone

### Asian Currencies
- IDR 🇮🇩 - Indonesian Rupiah
- CNY 🇨🇳 - Chinese Yuan
- INR 🇮🇳 - Indian Rupee
- KRW 🇰🇷 - South Korean Won
- SGD 🇸🇬 - Singapore Dollar
- THB 🇹🇭 - Thai Baht
- MYR 🇲🇾 - Malaysian Ringgit
- PHP 🇵🇭 - Philippine Peso
- VND 🇻🇳 - Vietnamese Dong

### Other Popular Currencies
- AUD 🇦🇺 - Australian Dollar
- CAD 🇨🇦 - Canadian Dollar
- CHF 🇨🇭 - Swiss Franc
- BRL 🇧🇷 - Brazilian Real
- MXN 🇲🇽 - Mexican Peso
- ZAR 🇿🇦 - South African Rand
- TRY 🇹🇷 - Turkish Lira

## 🐛 Known Issues

- None currently reported. If you encounter any issues, please report them.

## 🚧 Upcoming Features

- Historical exchange rate charts
- Rate alerts and notifications
- Cryptocurrency support
- Mobile companion app
- Offline mode with cached rates
- Rate comparison across multiple sources

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests or report issues.

## 📞 Support

For support, feature requests, or bug reports, please open an issue on the GitHub repository.

## 🎉 Evolution

This application evolved from a Bitcoin Satoshi calculator to a comprehensive currency exchange tool, reflecting the practical needs of real-world currency conversion for travelers, businesses, and anyone dealing with international currencies.

## Acknowledgments

- Exchange rates provided by [CoinGecko](https://www.coingecko.com/)
- Built with Qt 6.8.1 