# FastClipboard â€” Native Clipboard Access via JNI (2-3Ã— Faster than Java AWT) [ALPHA] - v0.1.0
**âš¡ Ultra-fast native clipboard access â€” stable copy/paste without Java clipboard bugs**

[![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![Java](https://img.shields.io/badge/Java-17+-blue.svg)](https://www.java.com)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010+-lightgrey.svg)]()
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![JitPack](https://jitpack.io/v/andrestubbe/FastClipboard.svg)](https://jitpack.io/#andrestubbe/FastClipboard)
[![GitHub stars](https://img.shields.io/github/stars/andrestubbe/fastclipboard.svg)](https://github.com/andrestubbe/fastclipboard/stargazers)

```java
// Quick Start â€” Stable clipboard operations
FastClipboard clipboard = new FastClipboard();

// Copy text to clipboard
clipboard.setClipboardText("Hello, World!");

// Get text from clipboard
String text = clipboard.getClipboardText();

// Copy image to clipboard
int[] pixels = new int[100 * 100];
clipboard.setClipboardImage(100, 100, pixels);

// Copy file list to clipboard
String[] files = {"C:\\file1.txt", "C:\\file2.txt"};
clipboard.setClipboardFiles(files);
```

FastClipboard is a **high-performance Java clipboard library** that replaces `java.awt.Toolkit.getDefaultToolkit().getSystemClipboard()` with a **native Windows backend** using Win32 API calls. Built for **stable text operations**, **image handling**, **file transfer**, and **format detection**. Supports **CF_UNICODETEXT**, **CF_DIB** (images), and **CF_HDROP** (file lists) formats.

**Keywords:** java clipboard alternative, native clipboard jni, win32 clipboard api, fast copy paste java, clipboard stability, java clipboard bug fix, windows clipboard automation, jni clipboard, clipboard image support, file list clipboard

If you need **reliable clipboard operations**, **image transfer**, or **file drag-and-drop** without Java's clipboard bugs, FastClipboard delivers native-level performance with Java simplicity.

---

## Table of Contents

- [Why FastClipboard?](#why-fastclipboard)
- [Key Features](#key-features)
- [Performance Benchmarks](#performance-benchmarks)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [API Reference](#api-reference)
- [Build from Source](#build-from-source)
- [Platform Support](#platform-support)
- [Use Cases](#use-cases)
- [License](#license)

---

## Why FastClipboard?

`java.awt.Toolkit.getDefaultToolkit().getSystemClipboard()` is convenient but buggy:
- Clipboard ownership issues
- Intermittent failures
- Thread-safety problems
- Slow performance

FastClipboard solves this with:
- **Direct Win32 API calls** â€” `OpenClipboard`, `GetClipboardData`, `SetClipboardData`
- **Stable operation** â€” no ownership issues
- **UTF-8/Unicode support** â€” proper text encoding
- **Minimal footprint** â€” 1 header + 1 JNI file + 1 Java class
- **1-evening implementation** â€” simple and maintainable

---

## Key Features

- **Direct Win32 API access** â€” stable clipboard operations
- **Text support** â€” UTF-8/Unicode text with proper encoding
- **Image support** â€” DIB format (CF_DIB) for bitmap images
- **File list support** â€” CF_HDROP format for file operations
- **Format detection** â€” check available clipboard formats
- **Minimal footprint** â€” small DLL, simple API
- **MIT licensed** â€” free for commercial use
- **Windows-only** â€” maximum performance on the most common platform

---

## Performance Benchmarks

| Operation | Java AWT Clipboard | FastClipboard | Speedup |
|-----------|-------------------|---------------|---------|
| Set Text (1000 iterations) | ~5000Î¼s | **~2000Î¼s** | **2.5Ã—** |
| Get Text (1000 iterations) | ~3000Î¼s | **~1500Î¼s** | **2.0Ã—** |
| Full Cycle (1000 iterations) | ~8000Î¼s | **~3500Î¼s** | **2.3Ã—** |

*Measured on Windows 11, Intel Core i7, Java 17*

**Run the benchmark:**
```bash
# Compile native DLL first [ALPHA] - v0.1.0
compile.bat

# Run benchmark [ALPHA] - v0.1.0
mvn compile exec:java -Dexec.mainClass="fastclipboard.Benchmark"
```

---

## Installation

### Option 1: Maven (Recommended)
Add the JitPack repository and the dependencies to your `pom.xml`:

```xml
<repositories>
    <repository>
        <id>jitpack.io</id>
        <url>https://jitpack.io</url>
    </repository>
</repositories>

<dependencies>
    <!-- FastClipboard Library -->
    <dependency>
        <groupId>com.github.andrestubbe</groupId>
        <artifactId>fastclipboard</artifactId>
        <version>v0.1.0</version>
    </dependency>
</dependencies>
```

### Option 2: Gradle (via JitPack)
```groovy
repositories {
    maven { url 'https://jitpack.io' }
}

dependencies {
    implementation 'com.github.andrestubbe:fastclipboard:v0.1.0'
}
```

### Option 3: Direct Download (No Build Tool)
Download the latest JARs directly to add them to your classpath:

1. 📦 **[fastclipboard-v0.1.0.jar](https://github.com/andrestubbe/FastClipboard/releases/download/v0.1.0/fastclipboard-v0.1.0.jar)** (The Core Library)


## Quick Start

### Basic Usage

```java
import fastclipboard.FastClipboard;

FastClipboard clipboard = new FastClipboard();

// Copy text to clipboard
boolean success = clipboard.setClipboardText("Hello, World!");

// Get text from clipboard
String text = clipboard.getClipboardText();
System.out.println("Clipboard: " + text);

// Check if clipboard has text
if (clipboard.hasClipboardText()) {
    System.out.println("Clipboard contains text");
}

// Clear clipboard
clipboard.clearClipboard();
```

### Optional Clipboard Watcher (for instant reads)

Enable the watcher for **570Ã— faster** repeated clipboard reads via automatic caching:

```java
FastClipboard clipboard = new FastClipboard();

// Enable watcher (opt-in, zero overhead when disabled)
clipboard.enableWatcher();

// First read fetches from clipboard (~500Î¼s)
String text = clipboard.getClipboardText();

// Subsequent reads use cache (~1Î¼s) - 570Ã— faster!
for (int i = 0; i < 1000; i++) {
    String cached = clipboard.getClipboardText(); // No JNI call!
}

// Cache auto-invalidates when clipboard changes externally
// Disable when done
clipboard.disableWatcher();
```

**When to use:**
- Bots/Agents with frequent clipboard polling
- Vision pipelines reading clipboard repeatedly
- Any scenario with >100 reads/second

**Zero overhead design:**
- Disabled by default - no threads, no events
- Opt-in only when needed
- Automatically invalidates cache on clipboard changes

### Format Detection

```java
// Check if specific format is available
if (clipboard.isFormatAvailable(FastClipboard.CF_UNICODETEXT)) {
    System.out.println("Unicode text available");
}

// Get number of available formats
int formatCount = clipboard.getFormatCount();
System.out.println("Available formats: " + formatCount);
```

### Image Operations

```java
// Copy image to clipboard (RGBA pixel data)
int width = 100;
int height = 100;
int[] pixels = new int[width * height];
// Fill with RGBA pixel data (0xAARRGGBB)
for (int i = 0; i < pixels.length; i++) {
    pixels[i] = 0xFFFF0000; // Red pixels
}
boolean success = clipboard.setClipboardImage(width, height, pixels);

// Get image from clipboard
int[] imageData = clipboard.getClipboardImage();
if (imageData != null) {
    int imgWidth = imageData[0];
    int imgHeight = imageData[1];
    int[] imgPixels = new int[imageData.length - 2];
    System.arraycopy(imageData, 2, imgPixels, 0, imgPixels.length);
}

// Check if clipboard has image
if (clipboard.hasClipboardImage()) {
    System.out.println("Clipboard contains image");
}
```

### File List Operations

```java
// Copy file list to clipboard
String[] files = {"C:\\path\\to\\file1.txt", "C:\\path\\to\\file2.txt"};
boolean success = clipboard.setClipboardFiles(files);

// Get file list from clipboard
String[] clipboardFiles = clipboard.getClipboardFiles();
if (clipboardFiles != null) {
    for (String file : clipboardFiles) {
        System.out.println("File: " + file);
    }
}

// Check if clipboard has file list
if (clipboard.hasClipboardFiles()) {
    System.out.println("Clipboard contains file list");
}
```

---

## API Reference

### Text Operations
- `setClipboardText(String text)` â€” Copy text to clipboard
- `getClipboardText()` â€” Get text from clipboard
- `hasClipboardText()` â€” Check if clipboard contains text
- `clearClipboard()` â€” Clear clipboard contents

### Image Operations
- `setClipboardImage(int width, int height, int[] pixels)` â€” Copy RGBA image to clipboard (DIB format)
- `getClipboardImage()` â€” Get image from clipboard (returns [width, height, pixelData...])
- `hasClipboardImage()` â€” Check if clipboard contains image

### File List Operations
- `setClipboardFiles(String[] filePaths)` â€” Copy file list to clipboard (CF_HDROP format)
- `getClipboardFiles()` â€” Get file list from clipboard
- `hasClipboardFiles()` â€” Check if clipboard contains file list

### Clipboard Info
- `isFormatAvailable(int format)` â€” Check if format is available
- `getFormatCount()` â€” Get number of available formats

### Format Constants
- `CF_TEXT = 1` â€” ANSI text
- `CF_UNICODETEXT = 13` â€” Unicode text (UTF-16)
- `CF_BITMAP = 2` â€” Bitmap image
- `CF_DIB = 8` â€” Device Independent Bitmap
- `CF_HDROP = 15` â€” File list

---

## Build from Source

See [COMPILE.md](COMPILE.md) for detailed build instructions.

---

## Platform Support

| Platform | Version | Status |
|----------|---------|--------|
| Windows 11 | v1.0 | âœ… Full support (Win32 API) |
| Windows 10 | v1.0 | âœ… Full support (Win32 API) |
| Linux | â€” | âŒ Not planned |
| macOS | â€” | âŒ Not planned |

**Windows-only by design** â€” we focus on maximum performance on the most common platform.

---

## Use Cases

- **Automation scripts** â€” reliable clipboard operations
- **Data processing pipelines** â€” fast text transfer
- **Bot development** â€” stable clipboard interaction
- **Test automation** â€” consistent clipboard behavior
- **Desktop applications** â€” bug-free clipboard access

---

## License

MIT License â€” free for commercial and private use. See [LICENSE](LICENSE) for details.

---

## Related Projects

- [FastHotkey](https://github.com/andrestubbe/FastHotkey) - Ultra-fast global hotkey library
- [FastRobot](https://github.com/andrestubbe/FastRobot) - High-performance screen capture
- [FastGraphics](https://github.com/andrestubbe/FastGraphics) - GPU-accelerated graphics
- [FastTheme](https://github.com/andrestubbe/FastTheme) - OS-aware Display/Theme monitor

---

## Support the Project

If FastClipboard helps you build something awesome:
- **Star** the repository â­
- **Share** with other developers
- **Report** issues and suggest features
- **Contribute** improvements via pull requests

---

**Small package. Maximum speed. Zero bloat.** ðŸš€ðŸ“‹

*Replace buggy Java clipboard with stable native performance!*


