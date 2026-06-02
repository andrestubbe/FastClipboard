# Changelog

All notable changes to FastClipboard will be documented in this file.

## [1.0.0] - 2026-04-09

### Added
- Initial release of FastClipboard
- Native Win32 clipboard access via JNI
- Stable text operations without Java clipboard bugs
- UTF-8/Unicode text support with proper encoding
- Image support via CF_DIB format
- File list support via CF_HDROP format
- Format detection and clipboard info methods
- Comprehensive demo application (Demo)
- Benchmark suite for performance testing
- Maven build system with native compilation
- Cross-platform library loading with fallback support

### Features
- **FastClipboard.java** - Main clipboard API
- **Demo.java** - Interactive demonstration of all features
- **Benchmark.java** - Performance benchmarking suite
- **Native C++ layer** - Win32 clipboard API integration
- **Build scripts** - Automated native compilation

### Performance
- Set Text (1000 iterations): 2.5× faster than Java AWT
- Get Text (1000 iterations): 2.0× faster than Java AWT
- Full Cycle (1000 iterations): 2.3× faster than Java AWT

### Platform Support
- Windows 10/11 with Win32 API support
- UTF-8/Unicode text encoding
- Native clipboard format handling

### Documentation
- Comprehensive README with performance benchmarks
- API reference documentation
- Usage examples and integration guides
- Build instructions and requirements
