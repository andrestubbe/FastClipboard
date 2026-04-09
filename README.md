# FastClipboard — Native Clipboard Access via JNI (2-3× Faster than Java AWT)

**⚡ Ultra-fast native clipboard access — stable copy/paste without Java clipboard bugs**

[![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![Java](https://img.shields.io/badge/Java-17+-blue.svg)](https://www.java.com)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010+-lightgrey.svg)]()
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![JitPack](https://jitpack.io/v/andrestubbe/FastClipboard.svg)](https://jitpack.io/#andrestubbe/FastClipboard)
[![GitHub stars](https://img.shields.io/github/stars/andrestubbe/fastclipboard.svg)](https://github.com/andrestubbe/fastclipboard/stargazers)

```java
// Quick Start — Stable clipboard operations
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
- **Direct Win32 API calls** — `OpenClipboard`, `GetClipboardData`, `SetClipboardData`
- **Stable operation** — no ownership issues
- **UTF-8/Unicode support** — proper text encoding
- **Minimal footprint** — 1 header + 1 JNI file + 1 Java class
- **1-evening implementation** — simple and maintainable

---

## Key Features

- **Direct Win32 API access** — stable clipboard operations
- **Text support** — UTF-8/Unicode text with proper encoding
- **Image support** — DIB format (CF_DIB) for bitmap images
- **File list support** — CF_HDROP format for file operations
- **Format detection** — check available clipboard formats
- **Minimal footprint** — small DLL, simple API
- **MIT licensed** — free for commercial use
- **Windows-only** — maximum performance on the most common platform

---

## Performance Benchmarks

| Operation | Java AWT Clipboard | FastClipboard | Speedup |
|-----------|-------------------|---------------|---------|
| Set Text (1000 iterations) | ~5000μs | **~2000μs** | **2.5×** |
| Get Text (1000 iterations) | ~3000μs | **~1500μs** | **2.0×** |
| Full Cycle (1000 iterations) | ~8000μs | **~3500μs** | **2.3×** |

*Measured on Windows 11, Intel Core i7, Java 17*

**Run the benchmark:**
```bash
# Compile native DLL first
compile.bat

# Run benchmark
mvn compile exec:java -Dexec.mainClass="fastclipboard.Benchmark"
```

---

## Installation

### Maven

```xml
<repositories>
    <repository>
        <id>jitpack.io</id>
        <url>https://jitpack.io</url>
    </repository>
</repositories>

<dependency>
    <groupId>com.github.andrestubbe</groupId>
    <artifactId>fastclipboard</artifactId>
    <version>1.0.0</version>
</dependency>
```

### Gradle

```groovy
repositories {
    maven { url 'https://jitpack.io' }
}

dependencies {
    implementation 'com.github.andrestubbe:fastclipboard:1.0.0'
}
```

### Direct Download

Download the pre-built JAR with embedded native library:

**[📥 Download fastclipboard-1.0.0.jar](https://github.com/andrestubbe/FastClipboard/releases/download/v1.0/fastclipboard-1.0.0.jar)** (64 KB)

```bash
# Run directly
java -cp fastclipboard-1.0.0.jar fastclipboard.Demo
```

---

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
- `setClipboardText(String text)` — Copy text to clipboard
- `getClipboardText()` — Get text from clipboard
- `hasClipboardText()` — Check if clipboard contains text
- `clearClipboard()` — Clear clipboard contents

### Image Operations
- `setClipboardImage(int width, int height, int[] pixels)` — Copy RGBA image to clipboard (DIB format)
- `getClipboardImage()` — Get image from clipboard (returns [width, height, pixelData...])
- `hasClipboardImage()` — Check if clipboard contains image

### File List Operations
- `setClipboardFiles(String[] filePaths)` — Copy file list to clipboard (CF_HDROP format)
- `getClipboardFiles()` — Get file list from clipboard
- `hasClipboardFiles()` — Check if clipboard contains file list

### Clipboard Info
- `isFormatAvailable(int format)` — Check if format is available
- `getFormatCount()` — Get number of available formats

### Format Constants
- `CF_TEXT = 1` — ANSI text
- `CF_UNICODETEXT = 13` — Unicode text (UTF-16)
- `CF_BITMAP = 2` — Bitmap image
- `CF_DIB = 8` — Device Independent Bitmap
- `CF_HDROP = 15` — File list

---

## Build from Source

### Prerequisites

#### 1. Java Development Kit (JDK) 17 or higher
**Download:** https://www.oracle.com/java/technologies/downloads/ or https://adoptium.net/

**Verify installation:**
```bash
java -version
javac -version
```

#### 2. Visual Studio 2019 or 2022 (Community Edition is free)
**Download:** https://visualstudio.microsoft.com/downloads/

**Required components:**
- ✅ **Desktop development with C++** workload
- ✅ **Windows 10/11 SDK**
- ✅ **MSVC v142/v143 - VS 2019/2022 C++ x64/x86 build tools**

#### 3. Apache Maven 3.8+ (optional, for packaging)
**Download:** https://maven.apache.org/download.cgi

### Build

```bash
git clone https://github.com/andrestubbe/fastclipboard.git
cd fastclipboard

# Compile native DLL
compile.bat

# Build Java + package
mvn clean package
```

---

## Platform Support

| Platform | Version | Status |
|----------|---------|--------|
| Windows 11 | v1.0 | ✅ Full support (Win32 API) |
| Windows 10 | v1.0 | ✅ Full support (Win32 API) |
| Linux | — | ❌ Not planned |
| macOS | — | ❌ Not planned |

**Windows-only by design** — we focus on maximum performance on the most common platform.

---

## Use Cases

- **Automation scripts** — reliable clipboard operations
- **Data processing pipelines** — fast text transfer
- **Bot development** — stable clipboard interaction
- **Test automation** — consistent clipboard behavior
- **Desktop applications** — bug-free clipboard access

---

## License

MIT License — free for commercial and private use. See [LICENSE](LICENSE) for details.

---

## Support the Project

If FastClipboard helps you build something awesome:
- **Star** the repository ⭐
- **Share** with other developers
- **Report** issues and suggest features
- **Contribute** improvements via pull requests

---

**Small package. Maximum speed. Zero bloat.** 🚀📋

*Replace buggy Java clipboard with stable native performance!*
