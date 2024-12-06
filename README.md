[**QLogger**][repo-gh] is made to be compatible with [_Qt framework logs_][doc-qt-log-module] management, this library provide an easy (and _thread-safe_) way to use multiple sinks behaviour.  

> [!TIP]
> Latest development/pull requests will be committed into `main` branch.  
> Each stable release have their dedicated branch:
> - `1.0.x`: branch `dev/1.0`
> - `1.1.x`: branch `dev/1.1`
> - etc...

**Table of contents:**
- [1. Requirements](#1-requirements)
  - [1.1. C++ Standards](#11-c-standards)
  - [1.2. Dependencies](#12-dependencies)
- [2. How to build](#2-how-to-build)
- [3. How to use](#3-how-to-use)
  - [3.1. Initialization](#31-initialization)
  - [3.2. Desinitialization](#32-desinitialization)
  - [3.3. Customization](#33-customization)
    - [3.3.1. Log format](#331-log-format)
    - [3.3.2. Log level](#332-log-level)
  - [3.4. Library version](#34-library-version)
    - [3.4.1. Compilation time](#341-compilation-time)
    - [3.4.2. Runtime](#342-runtime)
- [4. Documentation](#4-documentation)
- [5. Library details](#5-library-details)
  - [5.1. Why another log library ?](#51-why-another-log-library-)
  - [5.2. Implementation](#52-implementation)
- [6. License](#6-license)
- [7. Ressources](#7-ressources)

# 1. Requirements
## 1.1. C++ Standards

This library requires at least **C++ 11** standard (see [implementation section][anchor-implementation] for more details)

## 1.2. Dependencies

Below, list of required dependencies:

| Dependencies | Comments |
|:-:|:-:|
| [Qt][qt-official] | Library built with **Qt framework** and compatible with series `5.15.x` and `6.x` |

# 2. How to build

This library can be use as an embedded library in a subdirectory of your project (like a git submodule for example):
1. In the **root** CMakeLists, add instructions:
```cmake
add_subdirectory(qlogger) # Or if library is put in a folder "dependencies" : add_subdirectory(dependencies/qlogger)
```

1. In the **application** CMakeLists, add instructions :
```cmake
# Link QLogger library
target_link_libraries(${PROJECT_NAME} PRIVATE qlogger)
```

# 3. How to use
## 3.1. Initialization

This library only have to be initialized in the `main` method of the application, then all calls to Qt logs methods (`qDebug()`, `qInfo()`, `qWarning()`, etc...) of the application and used libraries will be redirected to _QLogger_. Multiple sinks behaviour are available:
- **File rotating sink** via `QLogger::QLoggerFactory::initLoggerRotating()`
- **Daily sink** via `QLogger::QLoggerFactory::initLoggerDaily()`

_Example:_
```cpp
#include "qlogger/qloggerfactory.h"

int main(int argc, char *argv[])
{
    /* Set logs */
    QLogger::QLoggerFactory::instance().initLoggerRotating(QFileInfo("logs/log.txt"), 3, 1024 * 1024 * 5, true);

    /* Manage application properties */
    QApplication app(argc, argv);
    ...

    return app.exec();
}
```
This will configure **QLogger** to:
- Use `logs/log.txt` as main file (once rotated, generated files will be: `logs/log1.txt` and `logs/log2.txt`)
- Number of logs files limited to **3**
- Log file max size limited to **5 megabytes (5Mb)** (_1 Kb = 1024 bytes_, _1Mb = 1024 * 1024 bytes_)
- All logs messages will also be redirected to console (useful during development phase)

## 3.2. Desinitialization

**QLogger** library will properly destruct (and close) all used ressources automatically when application is closed. But if user need to quit **QLogger** manually, just use:
```cpp
QLogger::QLoggerFactory::instance().desinit();
```

In this case, default Qt logger will be restablished. User can still use his own log behaviour with [`qInstallMessageHandler()`][doc-qt-log-install-custom] method (**QLogger** must have been desinitialized !)

> [!TIP]
> If log sink must be changed while one has already been initialized, caller don't need to manually call `desinit()` method, each sink init method will properly take care of this.

## 3.3. Customization
### 3.3.1. Log format

By default, log message will be formatted as below, depending of type of build:
- **Release:** `[utc-date][type-log] log-msg`
- **Debug:** `[utc-date][type-log] log-msg (cpp-file:cpp-line, cpp-function)`

If source file informations must appears on logs even on **release** mode, please use the associated macro in your main _CMakefile_: [`QT_MESSAGELOGCONTEXT`][doc-qt-log-context].

So for example, when using:
```cpp
const QString strValue = "nine";
const int value = 9;

qDebug() << "My value is:" << myValue;
qDebug("String [%s] converted as number is [%d]", qUtf8Printable(strValue), value);
```

This will log:
```text
[2023-07-09T13:36:09.245Z][debug] My value is: 9 (mainwindow.cpp:14, onMyCustomBtnClicked)
[2023-07-09T13:36:09.246Z][debug] String [nine] converted as number is [9] (mainwindow.cpp:15, onMyCustomBtnClicked)
```

Format log message can also be customized by using a custom `QLogger::LogFormatter` method, here an example:
```cpp
#include "qlogger/qloggerfactory.h"

/*****************************/
/* Macro definitions         */
/*****************************/
#define APP_LOG_FILE            "logs/log.txt"
#define APP_LOG_NB_FILES        3
#define APP_LOG_SIZE            (1024 * 1024 * 5) // Equals 5 megabytes (Mb)
#define APP_LOG_ENABLE_CONSOLE  true

/*****************************/
/* Custom log formatter      */
/*****************************/
QString logFormatter(const QLogger::LogEntry &log)
{
    QString fmt = QString("[%1][%2] %3").arg(
        QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs),
        log.getTypeString(),
        log.getMsg()
    );

    /* Can we add context informations ? */
    if(log.contextIsAvailable()){
        fmt += QString(" (%1:%2, %3)")
                   .arg(log.getCtxFile().fileName())
                   .arg(log.getCtxLine())
                   .arg(log.getCtxFctName());
    }

    /* Terminate log line */
    fmt += '\n';

    return fmt;
}

/*****************************/
/* Main method               */
/*****************************/
int main(int argc, char *argv[])
{
    /* Set logs */
    QLogger::LogFormatter::setCustomFormat(logFormatter);
    QLogger::QLoggerFactory::instance().initLoggerRotating(QFileInfo(APP_LOG_FILE), APP_LOG_NB_FILES, APP_LOG_SIZE, APP_LOG_ENABLE_CONSOLE);

    /* Manage application properties */
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
```

### 3.3.2. Log level

User can also configure whether or not all messages should be logged. By default, all messages are logged but that can be changed by choosing minimum level to use (see [Qt log level enum doc][doc-qt-log-level]):
```cpp
QLogger::QLoggerFactory::instance().setLevel(QtWarningMsg);
```
> [!NOTE]
> This example will set logger to only manage message of level: `QtWarningMsg`, `QtCriticalMsg` and `QtFatalMsg`

## 3.4. Library version
### 3.4.1. Compilation time

In order to easily check at compilation time library version (to manage compatibility between multiple versions for example), macro `QLOGGER_VERSION_ENCODE` (defined inside _qloggerglobal.h_ file) can be used:
```cpp
#if QLOGGER_VERSION >= QLOGGER_VERSION_ENCODE(2,0,0)
    // Do stuff for version 2.0.0 or higher
#else
    // Do stuff for earlier versions
#endif
```

### 3.4.2. Runtime

At runtime, it is recommended to use the static method:
```cpp
#include "qlogger/qloggerfactory.h"

const QVersionNumber &qloggerSemver = QLogger::QLoggerFactory::getLibraryVersion();
```

# 4. Documentation

All classes/methods has been documented with [Doxygen][doc-doxygen] utility and automatically generated at [online website documentation][repo-doc-web].

> [!NOTE]
> This repository contains two kinds of documentation:
> - **Public API:** Available via [online website documentation][repo-doc-web] or locally via Doxyfile `docs/fragments/Doxyfile-public-api.in`
> - **Internal:** Available locally only via `docs/fragments/Doxyfile-internal.in`

To generate documentation locally, we can use:
```shell
# Run documentation generation
doxygen ./Doxyfile-name

# Under Windows OS, maybe doxygen is not added to the $PATH
"C:\Program Files\doxygen\bin\doxygen.exe" ./Doxyfile-name
```
> [!TIP]
> You can also load the _Doxyfile_ into _Doxywizard_ (Doxygen GUI) and run generation.

# 5. Library details
## 5.1. Why another log library ?

We already have many good C++ logs libraries outside, so why to create a new one ? Since many of my custom application are built with Qt framework, I thought that having Qt framework as a dependency was already enough and didnt' want to use another just for logging. Besides, alternatives C++ logs libraries doesn't behave properly when used inside a library (`.dll` under **Windows**, `.so` under **Linux**) but Qt log framework allow such usage. 

Qt log module allow to easily add our custom log behaviour throught [`qInstallMessageHandler()`][doc-qt-log-install-custom]. This library is just build around that extension feature.

## 5.2. Implementation

This library use the [singleton pattern][doc-pattern-singleton] to manage **QLoggerFactory** instance. This singleton was implemented using [Meyer's Singleton][doc-singleton-meyer-1] pattern, this implementation use static variable initialization to ensure thread-safety at initialization. This doesn't ensure your singleton to be thread-safe... only his _initialization_ and _desininitialization_ are !  
Note that (at least !) a **C++11** compiler is required (**C++11** added requirement for static variable initialization to be thread-safe).
> For more informations about **Meyer's Singleton**, see below:
> - [Stackexchange - Singleton class and correct way to access it in C++][doc-singleton-meyer-2]
> - [Stackoverflow - How is Meyers' implementation of a Singleton actually a Singleton][doc-singleton-meyer-3]

Then, library thread-safety is ensured by using a mutex whenever a log message is received (to prevent from thread-race issues when trying to write to log to file for example).

# 6. License

This library is licensed under [MIT license][repo-license].

# 7. Ressources

- Qt ressources
  - [Qt logs module][doc-qt-log-module]
  - [Qt log custom handler][doc-qt-log-install-custom]
- Singleton pattern
  - [RefactoringGuru - Singleton pattern][doc-pattern-singleton]
  - [Meyer’s Singleton][doc-singleton-meyer-1]
  - [Stackexchange - Singleton class and correct way to access it in C++][doc-singleton-meyer-2]
  - [Stackoverflow - How is Meyers' implementation of a Singleton actually a Singleton][doc-singleton-meyer-3]
- Documentation utility
  - [Doxygen][doc-doxygen]
- C++ logs libraries:
    - [sdplog][log-lib-sdplog]
    - [plog][log-lib-plog]
    - [glog][log-lib-glog]
    - [easyloggingcpp][log-lib-easyloggingcpp]
    - [log4cplus][log-lib-log4cplus]
    - [loguru][log-lib-loguru]

<!-- Anchors of this file -->
[anchor-implementation]: #52-implementation

<!-- Repository links -->
[repo-gh]: https://github.com/legerch/QLogger
[repo-doc-web]: https://legerch.github.io/QLogger/
[repo-license]: LICENSE

<!-- External links -->
[doc-doxygen]: https://www.doxygen.nl/

[doc-qt-log-module]: https://doc.qt.io/qt-6/qtlogging.html
[doc-qt-log-level]: https://doc.qt.io/qt-6/qtlogging.html#QtMsgType-enum
[doc-qt-log-install-custom]: https://doc.qt.io/qt-5/qtglobal.html#qInstallMessageHandler
[doc-qt-log-context]: https://doc.qt.io/qt-6/qmessagelogcontext.html

[doc-pattern-singleton]: https://refactoring.guru/design-patterns/singleton

[doc-singleton-meyer-1]: https://laristra.github.io/flecsi/src/developer-guide/patterns/meyers_singleton.html
[doc-singleton-meyer-2]: https://codereview.stackexchange.com/questions/197486/singleton-class-and-correct-way-to-access-it-in-c
[doc-singleton-meyer-3]: https://stackoverflow.com/questions/17712001/how-is-meyers-implementation-of-a-singleton-actually-a-singleton

[log-lib-sdplog]: https://github.com/gabime/spdlog
[log-lib-plog]: https://github.com/SergiusTheBest/plog
[log-lib-glog]: https://github.com/google/glog
[log-lib-easyloggingcpp]: https://github.com/abumq/easyloggingpp
[log-lib-log4cplus]: https://github.com/log4cplus/log4cplus
[log-lib-loguru]: https://github.com/emilk/loguru

[qt-official]: https://www.qt.io/