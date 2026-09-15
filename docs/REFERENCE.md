# FastClipboard Reference Manual

`FastClipboard` is the high-performance Win32 native clipboard access, format detection, and automated cache-invalidation substrate of the FastJava ecosystem.

---

## 1. Core Vocabulary

* **Win32 Clipboard Subsystem**: Bypasses AWT event thread synchronization and ownership bugs by directly interfacing with `OpenClipboard`, `GetClipboardData`, and `SetClipboardData`.
* **Zero-Allocation Text & Image Transfer**: Directly handles UTF-16 Unicode strings (`CF_UNICODETEXT`), bitmap DIB frames (`CF_DIB`), and OS file drag-and-drop lists (`CF_HDROP`).
* **Active Clipboard Watcher**: Optional OS-level event hook tracking clipboard sequence numbers to provide cached reads in ~1 µs with zero polling overhead.
* **Deterministic Native Lifecycle**: Automatically closes clipboard handles with reliable cleanup.

---

## 2. Java API Reference

### Class: `fastclipboard.FastClipboard`
Primary clipboard access engine.

#### Text Operations
- `public boolean setClipboardText(String text)`  
  Copies text directly to the OS clipboard in UTF-16 format (`CF_UNICODETEXT`). Returns `true` if successful.
- `public String getClipboardText()`  
  Retrieves text from the clipboard. If the watcher is active, returns cached text (~1 µs); otherwise queries Win32 directly.
- `public boolean hasClipboardText()`  
  Returns `true` if the system clipboard currently contains text data.
- `public boolean clearClipboard()`  
  Empties all clipboard contents via Win32 `EmptyClipboard()`.

#### Image Operations (DIB / Bitmaps)
- `public boolean setClipboardImage(int width, int height, int[] pixels)`  
  Converts RGBA integer pixel data into Device Independent Bitmap (`CF_DIB`) format and commits it to the clipboard.
- `public int[] getClipboardImage()`  
  Extracts bitmap data from the clipboard, returning a packed integer array `[width, height, pixel0, pixel1, ...]`, or `null` if no image is present.
- `public boolean hasClipboardImage()`  
  Returns `true` if bitmap/DIB image data is present on the clipboard.

#### File Transfer Operations
- `public boolean setClipboardFiles(String[] filePaths)`  
  Writes a list of absolute file paths to the clipboard using the Windows shell `CF_HDROP` format for native Explorer paste operations.
- `public String[] getClipboardFiles()`  
  Retrieves file path strings from a `CF_HDROP` structure present on the clipboard.
- `public boolean hasClipboardFiles()`  
  Returns `true` if a file list is available on the clipboard.

#### Format Inspection & Constants
- `public boolean isFormatAvailable(int format)`  
  Checks if a specific Win32 clipboard format identifier is currently populated.
- `public int getFormatCount()`  
  Returns the count of different clipboard formats currently available.

| Format Constant | Value | Description |
|---|---|---|
| `CF_TEXT` | `1` | Traditional ANSI text |
| `CF_BITMAP` | `2` | Standard GDI device-dependent bitmap |
| `CF_DIB` | `8` | Device Independent Bitmap (images) |
| `CF_UNICODETEXT` | `13` | UTF-16 Unicode text |
| `CF_HDROP` | `15` | Shell file drop list |

#### Clipboard Watcher & Fast Caching
- `public boolean enableWatcher()`  
  Attaches native Win32 clipboard listener (`AddClipboardFormatListener`). Enables 570x faster cached reads (`~1 µs`) that invalidate automatically when clipboard content changes.
- `public void disableWatcher()`  
  Detaches the native listener and falls back to direct Win32 reads.
- `public boolean isWatcherEnabled()`  
  Returns `true` if the caching watcher is currently active.

---

## 3. Platform & Memory Guarantees

* **Thread-Safety**: All native calls lock the system clipboard with retry loops to guarantee safe multi-threaded access.
* **No AWT Dependency**: Eliminates `java.awt.datatransfer.*` deadlocks and memory retention issues.
* **Platform**: Exclusively tuned for Windows 10/11 (x64) via Win32 API.

---

**Part of the FastJava Ecosystem** — *Making the JVM faster.* 🚀
