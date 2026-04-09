import fastclipboard.FastClipboard;

public class TestApp {
    public static void main(String[] args) {
        System.out.println("=== Maven Dependency Test ===");
        System.out.println("Testing FastClipboard via Maven dependency...\n");
        
        try {
            FastClipboard clipboard = new FastClipboard();
            System.out.println("✓ FastClipboard instantiated successfully");
            
            // Test set
            boolean setResult = clipboard.setClipboardText("Maven test");
            System.out.println("✓ Set clipboard: " + setResult);
            
            // Test get
            String text = clipboard.getClipboardText();
            System.out.println("✓ Got clipboard: " + text);
            
            // Verify
            if ("Maven test".equals(text)) {
                System.out.println("\n=== SUCCESS ===");
                System.out.println("Maven dependency works correctly!");
            } else {
                System.out.println("\n=== FAILED ===");
                System.out.println("Clipboard content mismatch");
            }
        } catch (Exception e) {
            System.out.println("\n=== FAILED ===");
            System.out.println("Error: " + e.getMessage());
            e.printStackTrace();
        }
    }
}
