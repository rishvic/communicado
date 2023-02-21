# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- RHEL-recommended compiler options to vendor/libtask/CMakeLists.txt.
- _TFTPack_, a library containing functions relating to properly writing/reading
  TFTP packets.

### Changed

- Moved tftpd business logic code from tftpd/ to lib/tftpd/. tftpd now depends
  on the static libtftpd library.
- Moved default tftpd options from args.h to config.h.in.
- Changed `libtftpd` API to use libpcap-like `errbuf`, instead of using fprintf
  from inside the library.
- Changed `StopServer` method from forcefully closing `sockfd`, to using
  channels to signal stopping of server.

## [0.0.1] - 2023-01-27

### Added

- Added `tftpd`: a TFTP server executable.
- Placed libtask library, inside the vendor/ directory.
- Basic `README.md` file with basic instruction regarding compatibility, build
  instructions, etc.
- This `CHANGELOG.md` file, noting down notable changes.

[unreleased]: https://github.com/rishvic/communicado/compare/v0.0.1...HEAD
[0.0.1]: https://github.com/rishvic/communicado/releases/tag/v0.0.1
