/**
 * @file fastclipboard.cpp
 * @brief FastClipboard native implementation - Windows clipboard access
 *
 * @details Implements Windows clipboard operations for Java including:
 * - Text: UTF-8/UTF-16 conversion with proper memory management
 * - Images: DIB (Device Independent Bitmap) format conversion
 * - Files: HDROP (Drag and Drop) structure handling
 * - Watcher: WM_DRAWCLIPBOARD message monitoring
 *
 * @par Implementation Notes
 * - Uses GlobalAlloc/GlobalLock for clipboard-compatible memory
 * - UTF-16LE for Windows clipboard text (CF_UNICODETEXT)
 * - RGBA to BGRA conversion for image format compatibility
 * - Shell DragQueryFile for HDROP file enumeration
 *
 * @par Security Considerations
 * - Clipboard is shared system resource
 * - EmptyClipboard required before SetClipboardData
 * - Must CloseClipboard even on error paths
 *
 * @author FastJava Team
 * @version 1.0.0
 * @copyright MIT License
 */

#include <windows.h>
#include <shlobj.h>
#include <jni.h>
#include <iostream>
#include "fastclipboard.h"

/// @defgroup JNI_Implementation JNI Implementation
/// @brief JNI function implementations
/// @{

/**
 * @brief Set clipboard text
 * @details Converts UTF-8 to UTF-16, allocates global memory,
 *          and sets CF_UNICODETEXT format.
 * 
 * @param env JNI environment
 * @param obj FastClipboard instance
 * @param text Java string to copy
 * @return JNI_TRUE if successful
 * @note EmptyClipboard called before setting data
 */
// JNI method implementations

JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_setClipboardText(JNIEnv *env, jobject obj, jstring text) {
    if (text == NULL) {
        return JNI_FALSE;
    }
    
    const char* utfText = env->GetStringUTFChars(text, NULL);
    if (utfText == NULL) {
        return JNI_FALSE;
    }
    
    // Convert UTF-8 to wide string for Windows
    int len = MultiByteToWideChar(CP_UTF8, 0, utfText, -1, NULL, 0);
    if (len == 0) {
        env->ReleaseStringUTFChars(text, utfText);
        return JNI_FALSE;
    }
    
    wchar_t* wideText = new wchar_t[len];
    MultiByteToWideChar(CP_UTF8, 0, utfText, -1, wideText, len);
    
    // Open clipboard
    if (!OpenClipboard(NULL)) {
        delete[] wideText;
        env->ReleaseStringUTFChars(text, utfText);
        return JNI_FALSE;
    }
    
    // Empty clipboard
    EmptyClipboard();
    
    // Allocate global memory for the text
    HGLOBAL hClipboardData = GlobalAlloc(GMEM_MOVEABLE, (len + 1) * sizeof(wchar_t));
    if (hClipboardData == NULL) {
        CloseClipboard();
        delete[] wideText;
        env->ReleaseStringUTFChars(text, utfText);
        return JNI_FALSE;
    }
    
    // Lock the memory and copy the text
    wchar_t* pchData = (wchar_t*)GlobalLock(hClipboardData);
    if (pchData != NULL) {
        wcscpy_s(pchData, len + 1, wideText);
        GlobalUnlock(hClipboardData);
        
        // Set the clipboard data
        SetClipboardData(CF_UNICODETEXT, hClipboardData);
    }
    
    // Clean up
    CloseClipboard();
    delete[] wideText;
    env->ReleaseStringUTFChars(text, utfText);
    
    return JNI_TRUE;
}

JNIEXPORT jstring JNICALL Java_fastclipboard_FastClipboard_nativeGetClipboardText(JNIEnv *env, jobject obj) {
    if (!OpenClipboard(NULL)) {
        return NULL;
    }
    
    // Check if text is available
    if (!IsClipboardFormatAvailable(CF_UNICODETEXT)) {
        CloseClipboard();
        return NULL;
    }
    
    // Get clipboard data
    HANDLE hClipboardData = GetClipboardData(CF_UNICODETEXT);
    if (hClipboardData == NULL) {
        CloseClipboard();
        return NULL;
    }
    
    // Lock the memory
    wchar_t* pchData = (wchar_t*)GlobalLock(hClipboardData);
    if (pchData == NULL) {
        CloseClipboard();
        return NULL;
    }
    
    // Convert wide string to UTF-8
    int len = WideCharToMultiByte(CP_UTF8, 0, pchData, -1, NULL, 0, NULL, NULL);
    if (len == 0) {
        GlobalUnlock(hClipboardData);
        CloseClipboard();
        return NULL;
    }
    
    char* utfText = new char[len];
    WideCharToMultiByte(CP_UTF8, 0, pchData, -1, utfText, len, NULL, NULL);
    
    // Create Java string
    jstring result = env->NewStringUTF(utfText);
    
    // Clean up
    delete[] utfText;
    GlobalUnlock(hClipboardData);
    CloseClipboard();
    
    return result;
}

JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_hasClipboardText(JNIEnv *env, jobject obj) {
    if (!OpenClipboard(NULL)) {
        return JNI_FALSE;
    }
    
    jboolean result = IsClipboardFormatAvailable(CF_UNICODETEXT) ? JNI_TRUE : JNI_FALSE;
    CloseClipboard();
    
    return result;
}

JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_clearClipboard(JNIEnv *env, jobject obj) {
    if (!OpenClipboard(NULL)) {
        return JNI_FALSE;
    }
    
    EmptyClipboard();
    CloseClipboard();
    
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_isFormatAvailable(JNIEnv *env, jobject obj, jint format) {
    if (!OpenClipboard(NULL)) {
        return JNI_FALSE;
    }
    
    jboolean result = IsClipboardFormatAvailable(format) ? JNI_TRUE : JNI_FALSE;
    CloseClipboard();
    
    return result;
}

JNIEXPORT jint JNICALL Java_fastclipboard_FastClipboard_getFormatCount(JNIEnv *env, jobject obj) {
    if (!OpenClipboard(NULL)) {
        return -1;
    }
    
    UINT count = CountClipboardFormats();
    CloseClipboard();
    
    return (jint)count;
}

// Image operations

JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_setClipboardImage(JNIEnv *env, jobject obj, jint width, jint height, jintArray pixels) {
    if (pixels == NULL || width <= 0 || height <= 0) {
        return JNI_FALSE;
    }
    
    jint* pixelData = env->GetIntArrayElements(pixels, NULL);
    if (pixelData == NULL) {
        return JNI_FALSE;
    }
    
    // Create BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height; // Negative for top-down DIB
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = width * height * 4;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;
    
    // Calculate total size
    DWORD dwSize = sizeof(BITMAPINFOHEADER) + bi.biSizeImage;
    
    // Allocate global memory
    HGLOBAL hClipboardData = GlobalAlloc(GMEM_MOVEABLE, dwSize);
    if (hClipboardData == NULL) {
        env->ReleaseIntArrayElements(pixels, pixelData, JNI_ABORT);
        return JNI_FALSE;
    }
    
    // Lock memory and copy data
    BYTE* pData = (BYTE*)GlobalLock(hClipboardData);
    if (pData == NULL) {
        GlobalFree(hClipboardData);
        env->ReleaseIntArrayElements(pixels, pixelData, JNI_ABORT);
        return JNI_FALSE;
    }
    
    // Copy BITMAPINFOHEADER
    memcpy(pData, &bi, sizeof(BITMAPINFOHEADER));
    
    // Convert RGBA to BGRA and copy pixel data
    BYTE* pixelPtr = pData + sizeof(BITMAPINFOHEADER);
    for (int i = 0; i < width * height; i++) {
        jint pixel = pixelData[i];
        pixelPtr[i * 4 + 0] = (BYTE)(pixel & 0xFF);         // B
        pixelPtr[i * 4 + 1] = (BYTE)((pixel >> 8) & 0xFF);  // G
        pixelPtr[i * 4 + 2] = (BYTE)((pixel >> 16) & 0xFF); // R
        pixelPtr[i * 4 + 3] = (BYTE)((pixel >> 24) & 0xFF); // A
    }
    
    GlobalUnlock(hClipboardData);
    env->ReleaseIntArrayElements(pixels, pixelData, JNI_ABORT);
    
    // Set clipboard data
    if (!OpenClipboard(NULL)) {
        GlobalFree(hClipboardData);
        return JNI_FALSE;
    }
    
    EmptyClipboard();
    SetClipboardData(CF_DIB, hClipboardData);
    CloseClipboard();
    
    return JNI_TRUE;
}

JNIEXPORT jintArray JNICALL Java_fastclipboard_FastClipboard_getClipboardImage(JNIEnv *env, jobject obj) {
    if (!OpenClipboard(NULL)) {
        return NULL;
    }
    
    if (!IsClipboardFormatAvailable(CF_DIB)) {
        CloseClipboard();
        return NULL;
    }
    
    HANDLE hClipboardData = GetClipboardData(CF_DIB);
    if (hClipboardData == NULL) {
        CloseClipboard();
        return NULL;
    }
    
    BYTE* pData = (BYTE*)GlobalLock(hClipboardData);
    if (pData == NULL) {
        CloseClipboard();
        return NULL;
    }
    
    // Get BITMAPINFOHEADER
    BITMAPINFOHEADER* bi = (BITMAPINFOHEADER*)pData;
    int width = bi->biWidth;
    int height = abs(bi->biHeight);
    
    // Create Java array: [width, height, pixelData...]
    int totalPixels = width * height;
    jintArray result = env->NewIntArray(2 + totalPixels);
    if (result == NULL) {
        GlobalUnlock(hClipboardData);
        CloseClipboard();
        return NULL;
    }
    
    jint* resultData = env->GetIntArrayElements(result, NULL);
    if (resultData == NULL) {
        GlobalUnlock(hClipboardData);
        CloseClipboard();
        return NULL;
    }
    
    // Set width and height
    resultData[0] = width;
    resultData[1] = height;
    
    // Copy pixel data (BGRA to RGBA)
    BYTE* pixelPtr = pData + sizeof(BITMAPINFOHEADER);
    for (int i = 0; i < totalPixels; i++) {
        BYTE b = pixelPtr[i * 4 + 0];
        BYTE g = pixelPtr[i * 4 + 1];
        BYTE r = pixelPtr[i * 4 + 2];
        BYTE a = pixelPtr[i * 4 + 3];
        resultData[2 + i] = (a << 24) | (r << 16) | (g << 8) | b;
    }
    
    env->ReleaseIntArrayElements(result, resultData, 0);
    GlobalUnlock(hClipboardData);
    CloseClipboard();
    
    return result;
}

JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_hasClipboardImage(JNIEnv *env, jobject obj) {
    if (!OpenClipboard(NULL)) {
        return JNI_FALSE;
    }
    
    jboolean result = IsClipboardFormatAvailable(CF_DIB) ? JNI_TRUE : JNI_FALSE;
    CloseClipboard();
    
    return result;
}

// File list operations

JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_setClipboardFiles(JNIEnv *env, jobject obj, jobjectArray filePaths) {
    if (filePaths == NULL) {
        return JNI_FALSE;
    }
    
    int fileCount = env->GetArrayLength(filePaths);
    if (fileCount == 0) {
        return JNI_FALSE;
    }
    
    // Calculate total buffer size
    int totalSize = sizeof(DROPFILES);
    for (int i = 0; i < fileCount; i++) {
        jstring filePath = (jstring)env->GetObjectArrayElement(filePaths, i);
        const char* utfPath = env->GetStringUTFChars(filePath, NULL);
        int len = MultiByteToWideChar(CP_UTF8, 0, utfPath, -1, NULL, 0);
        totalSize += len * sizeof(wchar_t);
        env->ReleaseStringUTFChars(filePath, utfPath);
        env->DeleteLocalRef(filePath);
    }
    totalSize += sizeof(wchar_t); // Double null terminator
    
    // Allocate global memory
    HGLOBAL hClipboardData = GlobalAlloc(GMEM_MOVEABLE, totalSize);
    if (hClipboardData == NULL) {
        return JNI_FALSE;
    }
    
    // Lock memory
    DROPFILES* dropFiles = (DROPFILES*)GlobalLock(hClipboardData);
    if (dropFiles == NULL) {
        GlobalFree(hClipboardData);
        return JNI_FALSE;
    }
    
    // Initialize DROPFILES structure
    dropFiles->pFiles = sizeof(DROPFILES);
    dropFiles->pt = { 0, 0 };
    dropFiles->fNC = FALSE;
    dropFiles->fWide = TRUE; // Use wide strings
    
    // Copy file paths
    wchar_t* filePtr = (wchar_t*)((BYTE*)dropFiles + sizeof(DROPFILES));
    for (int i = 0; i < fileCount; i++) {
        jstring filePath = (jstring)env->GetObjectArrayElement(filePaths, i);
        const char* utfPath = env->GetStringUTFChars(filePath, NULL);
        int len = MultiByteToWideChar(CP_UTF8, 0, utfPath, -1, filePtr, totalSize - ((BYTE*)filePtr - (BYTE*)dropFiles));
        filePtr += len;
        env->ReleaseStringUTFChars(filePath, utfPath);
        env->DeleteLocalRef(filePath);
    }
    *filePtr = L'\0'; // Double null terminator
    
    GlobalUnlock(hClipboardData);
    
    // Set clipboard data
    if (!OpenClipboard(NULL)) {
        GlobalFree(hClipboardData);
        return JNI_FALSE;
    }
    
    EmptyClipboard();
    SetClipboardData(CF_HDROP, hClipboardData);
    CloseClipboard();
    
    return JNI_TRUE;
}

JNIEXPORT jobjectArray JNICALL Java_fastclipboard_FastClipboard_getClipboardFiles(JNIEnv *env, jobject obj) {
    if (!OpenClipboard(NULL)) {
        return NULL;
    }
    
    if (!IsClipboardFormatAvailable(CF_HDROP)) {
        CloseClipboard();
        return NULL;
    }
    
    HANDLE hClipboardData = GetClipboardData(CF_HDROP);
    if (hClipboardData == NULL) {
        CloseClipboard();
        return NULL;
    }
    
    DROPFILES* dropFiles = (DROPFILES*)GlobalLock(hClipboardData);
    if (dropFiles == NULL) {
        CloseClipboard();
        return NULL;
    }
    
    // Count files
    int fileCount = 0;
    wchar_t* filePtr = (wchar_t*)((BYTE*)dropFiles + dropFiles->pFiles);
    while (*filePtr != L'\0') {
        fileCount++;
        filePtr += wcslen(filePtr) + 1;
    }
    
    // Create Java string array
    jclass stringClass = env->FindClass("java/lang/String");
    jobjectArray result = env->NewObjectArray(fileCount, stringClass, NULL);
    if (result == NULL) {
        GlobalUnlock(hClipboardData);
        CloseClipboard();
        return NULL;
    }
    
    // Copy file paths
    filePtr = (wchar_t*)((BYTE*)dropFiles + dropFiles->pFiles);
    for (int i = 0; i < fileCount; i++) {
        int len = WideCharToMultiByte(CP_UTF8, 0, filePtr, -1, NULL, 0, NULL, NULL);
        char* utfPath = new char[len];
        WideCharToMultiByte(CP_UTF8, 0, filePtr, -1, utfPath, len, NULL, NULL);
        
        jstring filePath = env->NewStringUTF(utfPath);
        env->SetObjectArrayElement(result, i, filePath);
        
        delete[] utfPath;
        filePtr += wcslen(filePtr) + 1;
    }
    
    GlobalUnlock(hClipboardData);
    CloseClipboard();
    
    return result;
}

JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_hasClipboardFiles(JNIEnv *env, jobject obj) {
    if (!OpenClipboard(NULL)) {
        return JNI_FALSE;
    }
    
    jboolean result = IsClipboardFormatAvailable(CF_HDROP) ? JNI_TRUE : JNI_FALSE;
    CloseClipboard();
    
    return result;
}

// Clipboard Watcher Implementation
static HWND g_watcherHwnd = NULL;
static HWND g_nextViewer = NULL;
static JavaVM* g_vm = NULL;
static jobject g_javaObject = NULL;
static jmethodID g_onClipboardChangedMethod = NULL;
static volatile bool g_watcherEnabled = false;

LRESULT CALLBACK WatcherWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            return 0;
            
        case WM_CHANGECBCHAIN:
            // Clipboard viewer chain changed
            if ((HWND)wParam == g_nextViewer) {
                g_nextViewer = (HWND)lParam;
            } else if (g_nextViewer != NULL) {
                SendMessage(g_nextViewer, msg, wParam, lParam);
            }
            return 0;
            
        case WM_DRAWCLIPBOARD:
            // Clipboard content changed
            if (g_watcherEnabled && g_vm != NULL && g_javaObject != NULL && g_onClipboardChangedMethod != NULL) {
                JNIEnv* env;
                jint attachResult = g_vm->AttachCurrentThread((void**)&env, NULL);
                if (attachResult == JNI_OK || attachResult == JNI_EDETACHED) {
                    env->CallVoidMethod(g_javaObject, g_onClipboardChangedMethod);
                    if (attachResult == JNI_EDETACHED) {
                        g_vm->DetachCurrentThread();
                    }
                }
            }
            // Pass message to next viewer
            if (g_nextViewer != NULL) {
                SendMessage(g_nextViewer, msg, wParam, lParam);
            }
            return 0;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_nativeEnableWatcher(JNIEnv *env, jobject obj) {
    if (g_watcherEnabled) {
        return JNI_TRUE;  // Already enabled
    }
    
    // Cache JVM reference
    if (g_vm == NULL) {
        env->GetJavaVM(&g_vm);
    }
    
    // Cache Java object reference
    if (g_javaObject != NULL) {
        env->DeleteGlobalRef(g_javaObject);
    }
    g_javaObject = env->NewGlobalRef(obj);
    
    // Cache method ID for onClipboardChanged
    jclass clazz = env->GetObjectClass(obj);
    g_onClipboardChangedMethod = env->GetMethodID(clazz, "onClipboardChanged", "()V");
    
    if (g_onClipboardChangedMethod == NULL) {
        return JNI_FALSE;
    }
    
    // Create window class if not already registered
    static bool classRegistered = false;
    if (!classRegistered) {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = WatcherWindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = "FastClipboardWatcher";
        
        if (!RegisterClassEx(&wc)) {
            return JNI_FALSE;
        }
        classRegistered = true;
    }
    
    // Create hidden window for clipboard watching
    g_watcherHwnd = CreateWindowEx(
        0,
        "FastClipboardWatcher",
        "FastClipboard Watcher",
        0,
        0, 0, 0, 0,
        HWND_MESSAGE,  // Message-only window (no UI)
        NULL,
        GetModuleHandle(NULL),
        NULL
    );
    
    if (g_watcherHwnd == NULL) {
        return JNI_FALSE;
    }
    
    // Add to clipboard viewer chain
    g_nextViewer = SetClipboardViewer(g_watcherHwnd);
    g_watcherEnabled = true;
    
    return JNI_TRUE;
}

JNIEXPORT void JNICALL Java_fastclipboard_FastClipboard_nativeDisableWatcher(JNIEnv *env, jobject obj) {
    if (!g_watcherEnabled) {
        return;
    }
    
    g_watcherEnabled = false;
    
    // Remove from clipboard viewer chain
    if (g_watcherHwnd != NULL) {
        ChangeClipboardChain(g_watcherHwnd, g_nextViewer);
        DestroyWindow(g_watcherHwnd);
        g_watcherHwnd = NULL;
        g_nextViewer = NULL;
    }
    
    // Clean up Java references
    if (g_javaObject != NULL) {
        env->DeleteGlobalRef(g_javaObject);
        g_javaObject = NULL;
    }
    g_onClipboardChangedMethod = NULL;
}

JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_isWatcherEnabled(JNIEnv *env, jobject obj) {
    return g_watcherEnabled ? JNI_TRUE : JNI_FALSE;
}
