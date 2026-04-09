#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

// Text operations
JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_setClipboardText(JNIEnv *env, jobject obj, jstring text);
JNIEXPORT jstring JNICALL Java_fastclipboard_FastClipboard_getClipboardText(JNIEnv *env, jobject obj);
JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_hasClipboardText(JNIEnv *env, jobject obj);
JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_clearClipboard(JNIEnv *env, jobject obj);

// Clipboard info
JNIEXPORT jboolean JNICALL Java_fastclipboard_FastClipboard_isFormatAvailable(JNIEnv *env, jobject obj, jint format);
JNIEXPORT jint JNICALL Java_fastclipboard_FastClipboard_getFormatCount(JNIEnv *env, jobject obj);

#ifdef __cplusplus
}
#endif
