package fastclipboard;

/**
 * Demo application for FastClipboard.
 * 
 * Demonstrates basic clipboard operations:
 * - Setting text
 * - Getting text
 * - Checking for text availability
 * - Clearing clipboard
 * - Format detection
 */
public class Demo {
    
    public static void main(String[] args) {
        System.out.println("=== FastClipboard Demo ===");
        System.out.println();
        
        FastClipboard clipboard = new FastClipboard();
        
        // Test 1: Set text
        System.out.println("Test 1: Setting clipboard text...");
        boolean success = clipboard.setClipboardText("Hello from FastClipboard!");
        System.out.println("  Success: " + success);
        System.out.println();
        
        // Test 2: Check if text is available
        System.out.println("Test 2: Checking if clipboard has text...");
        boolean hasText = clipboard.hasClipboardText();
        System.out.println("  Has text: " + hasText);
        System.out.println();
        
        // Test 3: Get text
        System.out.println("Test 3: Getting clipboard text...");
        String text = clipboard.getClipboardText();
        System.out.println("  Text: " + text);
        System.out.println();
        
        // Test 4: Format detection
        System.out.println("Test 4: Checking format availability...");
        boolean hasUnicode = clipboard.isFormatAvailable(FastClipboard.CF_UNICODETEXT);
        boolean hasTextFormat = clipboard.isFormatAvailable(FastClipboard.CF_TEXT);
        int formatCount = clipboard.getFormatCount();
        System.out.println("  Has Unicode text: " + hasUnicode);
        System.out.println("  Has ANSI text: " + hasTextFormat);
        System.out.println("  Format count: " + formatCount);
        System.out.println();
        
        // Test 5: Clear clipboard
        System.out.println("Test 5: Clearing clipboard...");
        boolean cleared = clipboard.clearClipboard();
        System.out.println("  Cleared: " + cleared);
        System.out.println();
        
        // Test 6: Verify cleared
        System.out.println("Test 6: Verifying clipboard is empty...");
        String emptyText = clipboard.getClipboardText();
        System.out.println("  Text after clear: " + (emptyText == null ? "null" : emptyText));
        System.out.println();
        
        System.out.println("=== Demo Complete ===");
    }
}
