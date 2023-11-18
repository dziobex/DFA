# Automat skonczony
Projekt zrobiony w ramach pracy domowej ze studiów.

# Preview
![](https://github.com/dziobex/DFA/blob/main/preview.mp4)

# Jak uruchomić?
Działa tylko na systemach MS Windows.

1. Pobrać już skompilowany plik.

plik: [automat.rar](https://github.com/dziobex/DFA/blob/main/automat.rar)

2. Kompilacja samemu.

Upewnij się, że masz zainstalowane oprogramowania: CMake, mingw64, Make, Git Bash

Następnie wpisz w CMD:

```
git clone https://github.com/dziobex/DFA.git
cd CFA
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

Dalej: build > bin > Release > automat.exe (uruchomić)