# FastClipboard 0.1.0 [ALPHA] — Native Clipboard Access via JNI for Java

[![Status](https://img.shields.io/badge/status-0.1.0-brightgreen.svg)](https://github.com/andrestubbe/FastClipboard/releases/tag/v0.1.0)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Java](https://img.shields.io/badge/Java-17+-blue.svg)](https://www.java.com)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010+-lightgrey.svg)]()
[![JitPack](https://img.shields.io/badge/JitPack-ready-green.svg)](https://jitpack.io/#andrestubbe/FastClipboard)

---

**⚡ Ultra-fast Win32 native clipboard access — stable copy/paste without Java AWT clipboard bugs and ownership stalls.**

**FastClipboard** is a high-performance Java clipboard library that replaces `java.awt.Toolkit.getDefaultToolkit().getSystemClipboard()` with a direct native Windows backend using Win32 API calls (`OpenClipboard`, `GetClipboardData`, `SetClipboardData`). It supports Unicode text (`CF_UNICODETEXT`), bitmap images (`CF_DIB`), shell file lists (`CF_HDROP`), and an optional ultra-fast caching watcher delivering ~1 µs read speeds.

---

## Quick Start

```java
import fastclipboard.FastClipboard;

public class Demo {
    public static void main(String[] args) {
        FastClipboard clipboard = new FastClipboard();

        // 1. Copy text to clipboard
        clipboard.setClipboardText("Hello, World from FastClipboard!");

        // 2. Read text from clipboard
        String text = clipboard.getClipboardText();
        System.out.println("Clipboard Content: " + text);

        // 3. Optional Watcher for instant repeated reads (~1 µs cached)
        clipboard.enableWatcher();
        String cached = clipboard.getClipboardText(); // Instant!
        clipboard.disableWatcher();
    }
}
```

---

## Table of Contents

- [Quick Start](#quick-start)
- [Why FastClipboard?](#why-fastclipboard)
- [Key Features](#key-features)
- [Real-World Use Cases](#real-world-use-cases)
- [Performance Benchmarks](#performance-benchmarks)
- [API Quick Reference](#api-quick-reference)
- [Clipboard Watcher & Fast Caching](#clipboard-watcher--fast-caching)
- [Technical Demos & Benchmarks](#technical-demos--benchmarks)
- [Installation](#installation)
- [Documentation](#documentation)
- [Platform Support](#platform-support)
- [Related Projects](#related-projects)
- [License](#license)

---

## Why FastClipboard?

Standard Java AWT clipboard handling (`Toolkit.getDefaultToolkit().getSystemClipboard()`) suffers from recurring issues in automated, real-time, and agentic workflows:

- ❌ **Ownership & Thread-Safety Stalls**: Intermittent `IllegalStateException` failures when other applications access the clipboard concurrently.
- ❌ **Slow Read Latency**: Java AWT round-trips through heavyweight flavor converters and peer threads.
- ❌ **Encoding & Formatting Glitches**: Inconsistent handling of multi-byte UTF-16 Unicode, DIB bitmap structures, and Explorer file drops.

**FastClipboard** solves this by directly executing clean Win32 API calls with retry loops, native format translation, and optional event-driven cache invalidation.

| Feature | java.awt.datatransfer (Clipboard) | PowerShell / Clip.exe | FastClipboard |
|:---|:---|:---|:---|
| **Access Backend** | Heavy AWT peer thread & flavors | External process spawn (~150 ms) | **Direct Win32 API (`OpenClipboard`)** |
| **Concurrency & Lock Guard**| Intermittent `IllegalStateException`| Blocking shell calls | **Atomic retry loops & mutex guards** |
| **Read Latency (Cached)** | ~3,000 µs (flavor overhead) | ~100,000–250,000 µs (CLI launch) | **~1 µs (Event watcher cache)** |
| **Native Formats** | Complex `DataFlavor` mapping | Text only | **`CF_UNICODETEXT`, `CF_DIB`, `CF_HDROP`** |

---

## Key Features

- ⚡ **Direct Win32 API Access** — Direct native `OpenClipboard`, `GetClipboardData`, and `SetClipboardData` calls.
- 📝 **Full Unicode Text Support** — True UTF-16 (`CF_UNICODETEXT`) and ANSI text handling.
- 🖼️ **DIB Bitmap Image Transfer** — Copy and retrieve raw RGBA pixel arrays via `CF_DIB`.
- 📁 **Shell File Drop Lists** — Native Windows Explorer file copy/paste via `CF_HDROP`.
- 👁️ **570× Faster Watcher Caching** — Optional OS-level event watcher providing ~1 µs read times with automatic cache invalidation.
- 📦 **Zero AWT Bloat** — Completely independent of Java AWT and Swing.

---

## Real-World Use Cases

- 🤖 **AI Agents & Automation Bots**: Frequent clipboard polling and telemetry without JNI or AWT lock contention.
- 🖼️ **Screen Capture & Image Editors**: Instant bitmap clipboard ingestion and export via direct packed integer buffers.
- 📁 **File Managers & Desktop Tools**: Seamless interaction with Windows File Explorer via native `CF_HDROP` lists.
- 📝 **Developer TUIs & FastTerminal**: Zero-latency text pasting for fast terminal emulators and command prompts.

---

## Performance Benchmarks

FastClipboard is rigorously profiled using **JMH** (Java Microbenchmark Harness) measuring throughput against standard Java AWT:

| Operation | Java AWT Clipboard | FastClipboard | Speedup |
|---|---|---|---|
| **Set Text** | ~5,000 µs | **~2,000 µs** | **2.5× faster** |
| **Get Text** | ~3,000 µs | **~1,500 µs** | **2.0× faster** |
| **Get Text (Watcher Enabled)** | ~3,000 µs | **~1 µs** | **570× faster** |
| **Full Set/Get Cycle** | ~8,000 µs | **~3,500 µs** | **2.3× faster** |

*Measured on Windows 11, Intel Core i5-1135G7 (Surface Pro 8), JDK 21.0.12.*

---

## API Quick Reference

| Method | Return Type | Description | Docs |
|---|---|---|---|
| `setClipboardText(String text)` | `boolean` | Copies text to clipboard via Win32 `CF_UNICODETEXT`. | [Reference](docs/REFERENCE.md#class-fastclipboardfastclipboard) |
| `getClipboardText()` | `String` | Retrieves text (direct query or ~1 µs cached if watcher enabled). | [Reference](docs/REFERENCE.md#class-fastclipboardfastclipboard) |
| `hasClipboardText()` | `boolean` | Checks if clipboard contains text data. | [Reference](docs/REFERENCE.md#class-fastclipboardfastclipboard) |
| `clearClipboard()` | `boolean` | Empties all contents from the OS clipboard. | [Reference](docs/REFERENCE.md#class-fastclipboardfastclipboard) |
| `setClipboardImage(w, h, px)` | `boolean` | Copies RGBA pixel data to clipboard in DIB format. | [Reference](docs/REFERENCE.md#class-fastclipboardfastclipboard) |
| `getClipboardImage()` | `int[]` | Retrieves image data `[width, height, pixels...]`. | [Reference](docs/REFERENCE.md#class-fastclipboardfastclipboard) |
| `hasClipboardImage()` | `boolean` | Checks if clipboard contains image data. | [Reference](docs/REFERENCE.md#class-fastclipboardfastclipboard) |
| `setClipboardFiles(String[] paths)` | `boolean` | Copies file paths for Windows Explorer (`CF_HDROP`). | [Reference](docs/REFERENCE.md#class-fastclipboardfastclipboard) |
| `getClipboardFiles()` | `String[]` | Retrieves file list paths from clipboard. | [Reference](docs/REFERENCE.md#class-fastclipboardfastclipboard) |
| `hasClipboardFiles()` | `boolean` | Checks if clipboard contains a file drop list. | [Reference](docs/REFERENCE.md#class-fastclipboardfastclipboard) |
| `enableWatcher()` | `boolean` | Attaches native listener for instant ~1 µs cached reads. | [Reference](docs/REFERENCE.md#class-fastclipboardfastclipboard) |
| `disableWatcher()` | `void` | Detaches native listener and disables caching. | [Reference](docs/REFERENCE.md#class-fastclipboardfastclipboard) |

---

## Clipboard Watcher & Fast Caching

Enable the watcher for **570× faster** repeated clipboard reads via automatic caching:

```java
FastClipboard clipboard = new FastClipboard();

// Enable watcher (opt-in, zero overhead when disabled)
clipboard.enableWatcher();

// First read fetches from Win32 clipboard (~500 µs)
String text = clipboard.getClipboardText();

// Subsequent reads use cache (~1 µs) - 570× faster!
for (int i = 0; i < 1000; i++) {
    String cached = clipboard.getClipboardText(); // No JNI / OS call!
}

// Automatically invalidates cache when clipboard changes externally
clipboard.disableWatcher();
```

---

## Technical Demos & Benchmarks

| Case | Java Example | Launcher | Description |
|---|---|---|---|
| **Unified Clipboard Demo** | [Demo.java](examples/Demo/src/main/java/fastclipboard/Demo.java) | `run-demo.bat` | Interactive demonstration of text copy/paste, DIB image transfer, and file drop list handling. |
| **JMH Microbenchmark Suite** | [Benchmark.java](examples/Benchmark/src/main/java/fastclipboard/benchmark/Benchmark.java) | `run-benchmark.bat` | High-frequency throughput benchmark comparing FastClipboard against standard Java AWT clipboard. |

---

## Installation

### Option 1: Maven (Recommended)

Add the JitPack repository and the dependency to your `pom.xml`:

```xml
<repositories>
    <repository>
        <id>jitpack.io</id>
        <url>https://jitpack.io</url>
    </repository>
</repositories>

<dependencies>
    <dependency>
        <groupId>com.github.andrestubbe</groupId>
        <artifactId>fastclipboard</artifactId>
        <version>0.1.0</version>
    </dependency>
</dependencies>
```

### Option 2: Gradle (via JitPack)

```groovy
repositories {
    maven { url 'https://jitpack.io' }
}

dependencies {
    implementation 'com.github.andrestubbe:fastclipboard:0.1.0'
}
```

### Option 3: Direct Download (No Build Tool)

Download the pre-built JAR with embedded native library:

1. 📦 **[fastclipboard-0.1.0.jar](https://github.com/andrestubbe/FastClipboard/releases/download/v0.1.0/fastclipboard-0.1.0.jar)** (The Core Library)

---

## Documentation

- **[COMPILE.md](COMPILE.md)**: Full compilation guide (MSVC C++17 build chain + JNI Setup).
- **[REFERENCE.md](docs/REFERENCE.md)**: Full API descriptions, method contracts, and format constants.
- **[CHANGELOG.md](docs/CHANGELOG.md)**: Version history and feature milestones.

---

## Platform Support

| Platform | Architecture | Status | Driver / Subsystem |
|:---|:---:|:---:|:---|
| **Windows 10 / 11** | x64 | ✅ Fully Supported | Native Win32 Clipboard Subsystem & Formats |
| **Linux** | x64 / AArch64 | 🚧 Planned | X11 Selection (`XFixes`) / Wayland Data Control |
| **macOS** | Apple Silicon / x64 | 🚧 Planned | Cocoa `NSPasteboard` Subsystem |

---

## Related Projects

- **[`FastHotkey`](https://github.com/andrestubbe/FastHotkey)** — Ultra-fast global hotkey library
- **[`FastWindow`](https://github.com/andrestubbe/FastWindow)** — High-performance native Win32 window engine
- **[`FastMouse`](https://github.com/andrestubbe/FastMouse)** — Low-latency native RawInput mouse engine
- **[`FastTerminal`](https://github.com/andrestubbe/FastTerminal)** — Native high-speed terminal and TUI engine

---

## License

MIT License — See [LICENSE](LICENSE) file for details.

---
**Part of the FastJava Ecosystem** — *Making the JVM faster.* 🚀
