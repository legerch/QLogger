# Changelog

All notable changes to this project will be documented in this file.  
The format is based on [Keep a Changelog] and this project adheres to [Semantic Versioning].

## [next] - 2.0.0
### Added
- Add support for multiples sinks, allowing to choose behaviour of the logger. Current sinks available are:
  - File rotating logs
- Allow to use custom formatter via `QLogger::LogFormatter`
- Add support for log level: user can now set a minimum level

## [1.0.2] - 2024-08-02
### Fixed
- Fix context informations not always properly detected

## [1.0.1] - 2024-01-22
### Fixed
- Fix log directory not properly created when using multiple subdirectories

## [1.0.0] - 2023-07-09
First release of `QLogger` which allow to:
- manage log file rotation
- set size file limit
- set maximum number of logs files

<!-- Links -->
[keep a changelog]: https://keepachangelog.com/en/1.0.0/
[semantic versioning]: https://semver.org/spec/v2.0.0.html

<!-- Versions -->
[next]: https://github.com/legerch/QLogger/compare/1.0.2...dev
[1.0.2]: https://github.com/legerch/QLogger/compare/1.0.1...1.0.2
[1.0.1]: https://github.com/legerch/QLogger/compare/1.0.0...1.0.1
[1.0.0]: https://github.com/legerch/QLogger/releases/tag/1.0.0
