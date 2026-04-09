# FastClipboard — Native Clipboard Access via JNI (Stable & Fast)

**⚡ Ultra-fast native clipboard access — stable copy/paste without Java clipboard bugs**

[![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![Java](https://img.shields.io/badge/Java-17+-blue.svg)](https://www.java.com)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010+-lightgrey.svg)]()
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

```java
// Quick Start — Stable clipboard operations
FastClipboard clipboard = new FastClipboard();

// Copy text to clipboard
clipboard.setClipboardText("Hello, World!");

// Get text from clipboard
String text = clipboard.getClipboardText();

// Check if clipboard has text
if (clipboard.hasClipboardText()) {
    System.out.println("Clipboard contains: " + text);
}
```

FastClipboard is a **minimal JNI module** that provides direct Win32 clipboard access for stable, fast copy/paste operations. Built to replace the buggy `java.awt.Toolkit.getDefaultToolkit().getSystemClipboard()` with a native implementation using `OpenClipboard`, `GetClipboardData`, and `SetClipboardData`.

**Keywords:** java clipboard alternative, native clipboard jni, win32 clipboard api, fast copy paste java, clipboard stability, java clipboard bug fix, windows clipboard automation, jni clipboard

If you need **reliable clipboard operations** without Java's clipboard bugs, FastClipboard delivers native-level performance with Java simplicity.

---

## Table of Contents

- [Why FastClipboard?](#why-fastclipboard)
- [Key Features](#key-features)
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
- **UTF-8/Unicode text support** — proper encoding
- **Format detection** — check available clipboard formats
- **Minimal footprint** — small DLL, simple API
- **MIT licensed** — free for commercial use
- **Windows-only** — maximum performance on the most common platform

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

---

## API Reference

### Text Operations
- `setClipboardText(String text)` — Copy text to clipboard
- `getClipboardText()` — Get text from clipboard
- `hasClipboardText()` — Check if clipboard contains text
- `clearClipboard()` — Clear clipboard contents

### Clipboard Info
- `isFormatAvailable(int format)` — Check if format is available
- `getFormatCount()` — Get number of available formats

### Format Constants
- `CF_TEXT = 1` — ANSI text
- `CF_UNICODETEXT = 13` — Unicode text (UTF-16)
- `CF_BITMAP = 2` — Bitmap image
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
