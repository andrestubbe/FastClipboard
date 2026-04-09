#include <windows.h>
#include <jni.h>
#include <iostream>
#include "fastclipboard.h"

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

JNIEXPORT jstring JNICALL Java_fastclipboard_FastClipboard_getClipboardText(JNIEnv *env, jobject obj) {
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
