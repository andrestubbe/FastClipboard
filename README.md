# FastClipboard v0.1.0 [ALPHA] — Native Clipboard Access via JNI (2-3× Faster than Java AWT)

[![Status](https://img.shields.io/badge/status-v0.1.0-brightgreen.svg)](https://github.com/andrestubbe/FastClipboard/releases/tag/v0.1.0)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Java](https://img.shields.io/badge/Java-17+-blue.svg)](https://www.java.com)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010+-lightgrey.svg)]()
[![JitPack](https://img.shields.io/badge/JitPack-ready-green.svg)](https://jitpack.io/#andrestubbe)

---

**? Ultra-fast native clipboard access — stable copy/paste without Java clipboard bugs**

FastClipboard is a **high-performance Java clipboard library** that replaces
`java.awt.Toolkit.getDefaultToolkit().getSystemClipboard()` with a **native Windows backend** using Win32 API calls.
Built for **stable text operations**, **image handling**, **file transfer**, and **format detection**. Supports *
*CF_UNICODETEXT**, **CF_DIB** (images), and **CF_HDROP** (file lists) formats.

If you need **reliable clipboard operations**, **image transfer**, or **file drag-and-drop** without Java's clipboard
bugs, FastClipboard delivers native-level performance with Java simplicity.

---

[![FastFileIndex Showcase](docs/screenshot.png)](https://www.youtube.com/watch?v=BZsqQl7WqWk)

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

## Quick Start


```java
// Quick Start — Stable clipboard operations
FastClipboard clipboard = new FastClipboard();

// Copy text to clipboard
clipboard. setClipboardText("Hello, World!");

// Get text from clipboard
String text = clipboard.getClipboardText();

// Copy image to clipboard
int[] pixels = new int[100 * 100];
clipboard.setClipboardImage(100,100,pixels);

// Copy file list to clipboard
String[] files = {"C:\\file1.txt", "C:\\file2.txt"};
clipboard.setClipboardFiles(files);
```


### Basic Usage

```java
import fastclipboard.FastClipboard;

FastClipboard clipboard = new FastClipboard();

// Copy text to clipboard
boolean success = clipboard.setClipboardText("Hello, World!");

// Get text from clipboard
String text = clipboard.getClipboardText();
System.out.println("Clipboard: "+text);

// Check if clipboard has text
if(clipboard.hasClipboardText()){
        System.out.println("Clipboard contains text");
}

// Clear clipboard
clipboard.clearClipboard();
```

### Optional Clipboard Watcher (for instant reads)

Enable the watcher for **570× faster** repeated clipboard reads via automatic caching:

```java
FastClipboard clipboard = new FastClipboard();

// Enable watcher (opt-in, zero overhead when disabled)
clipboard.enableWatcher();

// First read fetches from clipboard (~500µs)
String text = clipboard.getClipboardText();

// Subsequent reads use cache (~1µs) - 570× faster!
for(int i = 0; i< 1000;i++){
    String cached = clipboard.getClipboardText(); // No JNI call!
}

// Cache auto-invalidates when clipboard changes externally
// Disable when done
        clipboard.disableWatcher();
```

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

| Operation                    | Java AWT Clipboard | FastClipboard | Speedup  |
|------------------------------|--------------------|---------------|----------|
| Set Text (1000 iterations)   | ~5000µs            | **~2000µs**   | **2.5×** |
| Get Text (1000 iterations)   | ~3000µs            | **~1500µs**   | **2.0×** |
| Full Cycle (1000 iterations) | ~8000µs            | **~3500µs**   | **2.3×** |

*Measured on Windows 11, Intel Core i7, Java 17*

**Run the benchmark:**

```bash
# Compile native DLL first
 [ALPHA] - v0.1.0
compile.bat

# Run benchmark
 [ALPHA] - v0.1.0
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

1. ?? *
   *[fastclipboard-v0.1.0.jar](https://github.com/andrestubbe/FastClipboard/releases/download/v0.1.0/fastclipboard-v0.1.0.jar)
   ** (The Core Library)



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
if(clipboard.isFormatAvailable(FastClipboard.CF_UNICODETEXT)){
        System.out.

println("Unicode text available");
}

// Get number of available formats
int formatCount = clipboard.getFormatCount();
System.out.

println("Available formats: "+formatCount);
```

### Image Operations

```java
// Copy image to clipboard (RGBA pixel data)
int width = 100;
int height = 100;
int[] pixels = new int[width * height];
// Fill with RGBA pixel data (0xAARRGGBB)
for(
int i = 0;
i<pixels.length;i++){
pixels[i]=0xFFFF0000; // Red pixels
        }
boolean success = clipboard.setClipboardImage(width, height, pixels);

// Get image from clipboard
int[] imageData = clipboard.getClipboardImage();
if(imageData !=null){
int imgWidth = imageData[0];
int imgHeight = imageData[1];
int[] imgPixels = new int[imageData.length - 2];
    System.

arraycopy(imageData, 2,imgPixels, 0,imgPixels.length);
}

// Check if clipboard has image
        if(clipboard.

hasClipboardImage()){
        System.out.

println("Clipboard contains image");
}
```

### File List Operations

```java
// Copy file list to clipboard
String[] files = {"C:\\path\\to\\file1.txt", "C:\\path\\to\\file2.txt"};
boolean success = clipboard.setClipboardFiles(files);

// Get file list from clipboard
String[] clipboardFiles = clipboard.getClipboardFiles();
if(clipboardFiles !=null){
        for(
String file :clipboardFiles){
        System.out.

println("File: "+file);
    }
            }

// Check if clipboard has file list
            if(clipboard.

hasClipboardFiles()){
        System.out.

println("Clipboard contains file list");
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

## Use Cases

- **Automation scripts** — reliable clipboard operations
- **Data processing pipelines** — fast text transfer
- **Bot development** — stable clipboard interaction
- **Test automation** — consistent clipboard behavior
- **Desktop applications** — bug-free clipboard access

---

## Documentation

* **[COMPILE.md](COMPILE.md)**: Full compilation guide (MSVC C++17 build chain + JNI Setup).
* **[REFERENCE.md](docs/REFERENCE.md)**: Full API descriptions, border configurations, and codepoint index.
* **[PHILOSOPHY.md](docs/PHILOSOPHY.md)**: The engineering rationale for zero-allocation performance.
* **[ROADMAP.md](docs/ROADMAP.md)**: Future milestones and planned features.

---

## Platform Support

| Platform      | Status            |
|---------------|-------------------|
| Windows 10/11 | ? Fully Supported |
| Linux         | ?? Planned        |
| macOS         | ?? Planned        |

---

## License

MIT License — See [LICENSE](LICENSE) file for details.

---

## Related Projects

- [FastFileIndex](https://github.com/andrestubbe/FastFileIndex) - Binary file indexing with mmap support
- [FastFileSearch](https://github.com/andrestubbe/FastFileSearch) - Prefix Trie, N-Gram index, and Ranking engine
- [FastFileWatch](https://github.com/andrestubbe/FastFileWatch) - USN Journal-based live file monitoring
- [FastCore](https://github.com/andrestubbe/FastCore) - Unified JNI loader and platform abstraction

---

**Part of the FastJava Ecosystem** — *Making the JVM faster. Small package. Maximum speed. Zero bloat. ????*

*Replace buggy Java clipboard with stable native performance!*


