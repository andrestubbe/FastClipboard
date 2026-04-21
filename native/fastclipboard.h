/**
 * @file fastclipboard.h
 * @brief FastClipboard JNI Header - System clipboard access for Java
 *
 * @details Provides comprehensive Windows clipboard operations including:
 * - Text (Unicode) get/set
 * - Image (DIB) get/set
 * - File list (HDROP) get/set
 * - Format enumeration and detection
 * - Clipboard change watcher
 *
 * @par Supported Formats
 * - CF_UNICODETEXT (UTF-16 text)
 * - CF_DIB / CF_DIBV5 (Device Independent Bitmap)
 * - CF_HDROP (File list)
 * - Custom formats via format enumeration
 *
 * @par Platform Requirements
 * - Windows 2000 or later
 * - User32.dll, Shell32.dll
 *
 * @author FastJava Team
 * @version 1.0.0
 * @copyright MIT License
 */

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup TextOperations Text Operations
 *  @brief Clipboard text (Unicode) operations
 *  @{ */

/**
 * @brief Set clipboard text
 * @param env JNI environment
 * @param obj FastClipboard instance
 * @param text Java string to copy to clipboard
 * @return JNI_TRUE if successful
 */
JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_setClipboardText(JNIEnv *env, jobject obj, jstring text);

/**
 * @brief Get clipboard text
 * @param env JNI environment
 * @param obj FastClipboard instance
 * @return Java string, or NULL if no text available
 */
JNIEXPORT jstring JNICALL Java_fastclipboard_FastClipboard_nativeGetClipboardText(JNIEnv *env, jobject obj);

/**
 * @brief Check if clipboard contains text
 * @param env JNI environment
 * @param obj FastClipboard instance
 * @return JNI_TRUE if CF_UNICODETEXT available
 */
JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_hasClipboardText(JNIEnv *env, jobject obj);

/**
 * @brief Clear clipboard contents
 * @param env JNI environment
 * @param obj FastClipboard instance
 * @return JNI_TRUE if successful
 */
JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_clearClipboard(JNIEnv *env, jobject obj);

/** @} */

/** @defgroup ImageOperations Image Operations
 *  @brief Clipboard image (DIB) operations
 *  @{ */

/**
 * @brief Set clipboard image
 * @param env JNI environment
 * @param obj FastClipboard instance
 * @param width Image width in pixels
 * @param height Image height in pixels
 * @param pixels RGBA pixel array
 * @return JNI_TRUE if successful
 */
JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_setClipboardImage(JNIEnv *env, jobject obj, jint width, jint height, jintArray pixels);

/**
 * @brief Get clipboard image
 * @param env JNI environment
 * @param obj FastClipboard instance
 * @return RGBA pixel array, or NULL if no image
 */
JNIEXPORT jintArray JNICALL Java_fastclipboard_FastClipboard_getClipboardImage(JNIEnv *env, jobject obj);

/**
 * @brief Check if clipboard contains image
 * @param env JNI environment
 * @param obj FastClipboard instance
 * @return JNI_TRUE if CF_DIB available
 */
JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_hasClipboardImage(JNIEnv *env, jobject obj);

/** @} */

/** @defgroup FileOperations File Operations
 *  @brief Clipboard file list (HDROP) operations
 *  @{ */

/**
 * @brief Set clipboard file list
 * @param env JNI environment
 * @param obj FastClipboard instance
 * @param filePaths Array of file path strings
 * @return JNI_TRUE if successful
 */
JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_setClipboardFiles(JNIEnv *env, jobject obj, jobjectArray filePaths);

/**
 * @brief Get clipboard file list
 * @param env JNI environment
 * @param obj FastClipboard instance
 * @return Array of file path strings, or NULL
 */
JNIEXPORT jobjectArray JNICALL Java_fastclipboard_FastClipboard_getClipboardFiles(JNIEnv *env, jobject obj);

/**
 * @brief Check if clipboard contains files
 * @param env JNI environment
 * @param obj FastClipboard instance
 * @return JNI_TRUE if CF_HDROP available
 */
JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_hasClipboardFiles(JNIEnv *env, jobject obj);

/** @} */

/** @defgroup Info Clipboard Information
 *  @brief Format enumeration and detection
 *  @{ */

/**
 * @brief Check if specific format is available
 * @param env JNI environment
 * @param obj FastClipboard instance
 * @param format Windows clipboard format ID
 * @return JNI_TRUE if format available
 */
JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_isFormatAvailable(JNIEnv *env, jobject obj, jint format);

/**
 * @brief Get number of available formats
 * @param env JNI environment
 * @param obj FastClipboard instance
 * @return Format count
 */
JNIEXPORT jint JNICALL Java_fastclipboard_FastClipboard_getFormatCount(JNIEnv *env, jobject obj);

/** @} */

/** @defgroup Watcher Clipboard Watcher
 *  @brief Monitor clipboard changes
 *  @{ */

/**
 * @brief Enable clipboard change watcher
 * @param env JNI environment
 * @param obj FastClipboard instance
 * @return JNI_TRUE if watcher started
 */
JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_nativeEnableWatcher(JNIEnv *env, jobject obj);

/**
 * @brief Disable clipboard change watcher
 * @param env JNI environment
 * @param obj FastClipboard instance
 */
JNIEXPORT void JNICALL Java_fastclipboard_FastClipboard_nativeDisableWatcher(JNIEnv *env, jobject obj);

/**
 * @brief Check if watcher is enabled
 * @param env JNI environment
 * @param obj FastClipboard instance
 * @return JNI_TRUE if watcher active
 */
JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_isWatcherEnabled(JNIEnv *env, jobject obj);

/** @} */

#ifdef __cplusplus
}
#endif
