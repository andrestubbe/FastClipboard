package fastclipboard;

/**
 * High-performance native clipboard access via JNI.
 * 
 * Provides stable, fast copy/paste operations without Java clipboard bugs.
 * Uses Win32 API directly: OpenClipboard, GetClipboardData, SetClipboardData.
 * 
 * This is a minimal 1-evening module implementation.
 */
public class FastClipboard {
    
    static {
        try {
            // Try System.loadLibrary first (default)
            System.loadLibrary("fastclipboard");
        } catch (UnsatisfiedLinkError e1) {
            try {
                // Fallback to absolute path
                String dllPath = System.getProperty("user.dir") + "\\build\\fastclipboard.dll";
                System.load(dllPath);
            } catch (UnsatisfiedLinkError e2) {
                System.err.println("Failed to load fastclipboard.dll: " + e2.getMessage());
                throw e2;
            }
        }
    }
    
    // === Text operations ===
    
    /**
     * Copy text to clipboard.
     * 
     * @param text The text to copy
     * @return true if successful
     */
    public native boolean setClipboardText(String text);
    
    /**
     * Get text from clipboard.
     * 
     * @return The clipboard text, or null if empty/error
     */
    public native String getClipboardText();
    
    /**
     * Check if clipboard contains text.
     * 
     * @return true if clipboard has text data
     */
    public native boolean hasClipboardText();
    
    /**
     * Clear clipboard contents.
     * 
     * @return true if successful
     */
    public native boolean clearClipboard();
    
    // === Image operations ===
    
    /**
     * Copy image to clipboard (DIB format).
     * 
     * @param width Image width
     * @param height Image height
     * @param pixels RGBA pixel data (int array)
     * @return true if successful
     */
    public native boolean setClipboardImage(int width, int height, int[] pixels);
    
    /**
     * Get image from clipboard (DIB format).
     * 
     * @return int array containing [width, height, pixelData...] or null if no image
     */
    public native int[] getClipboardImage();
    
    /**
     * Check if clipboard contains image.
     * 
     * @return true if clipboard has image data
     */
    public native boolean hasClipboardImage();
    
    // === File list operations ===
    
    /**
     * Copy file list to clipboard (CF_HDROP format).
     * 
     * @param filePaths Array of file paths
     * @return true if successful
     */
    public native boolean setClipboardFiles(String[] filePaths);
    
    /**
     * Get file list from clipboard (CF_HDROP format).
     * 
     * @return Array of file paths, or null if no files
     */
    public native String[] getClipboardFiles();
    
    /**
     * Check if clipboard contains file list.
     * 
     * @return true if clipboard has file list
     */
    public native boolean hasClipboardFiles();
    
    // === Clipboard info ===
    
    /**
     * Get clipboard format availability.
     * 
     * @param format The format identifier (CF_TEXT = 1, CF_UNICODETEXT = 13)
     * @return true if format is available
     */
    public native boolean isFormatAvailable(int format);
    
    /**
     * Get number of clipboard formats available.
     * 
     * @return Number of formats, or -1 on error
     */
    public native int getFormatCount();
    
    // === Constants for clipboard formats ===
    public static final int CF_TEXT = 1;
    public static final int CF_BITMAP = 2;
    public static final int CF_METAFILEPICT = 3;
    public static final int CF_SYLK = 4;
    public static final int CF_DIF = 5;
    public static final int CF_TIFF = 6;
    public static final int CF_OEMTEXT = 7;
    public static final int CF_DIB = 8;
    public static final int CF_PALETTE = 9;
    public static final int CF_PENDATA = 10;
    public static final int CF_RIFF = 11;
    public static final int CF_WAVE = 12;
    public static final int CF_UNICODETEXT = 13;
    public static final int CF_ENHMETAFILE = 14;
    public static final int CF_HDROP = 15;
    public static final int CF_LOCALE = 16;
}
