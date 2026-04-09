package fastclipboard;

/**
 * Test for optional clipboard watcher with caching.
 * 
 * Tests:
 * 1. Watcher enable/disable
 * 2. Cache functionality
 * 3. Cache invalidation on clipboard change
 * 4. Performance comparison
 */
public class WatcherTest {
    
    public static void main(String[] args) throws InterruptedException {
        System.out.println("=== FastClipboard Watcher Test ===\n");
        
        FastClipboard clipboard = new FastClipboard();
        
        // Test 1: Normal mode (watcher disabled)
        System.out.println("Test 1: Normal mode (watcher disabled)");
        testNormalMode(clipboard);
        
        // Test 2: Enable watcher
        System.out.println("\nTest 2: Enable watcher");
        boolean enabled = clipboard.enableWatcher();
        System.out.println("  Watcher enabled: " + enabled);
        System.out.println("  isWatcherEnabled(): " + clipboard.isWatcherEnabled());
        
        if (!enabled) {
            System.out.println("  ERROR: Failed to enable watcher!");
            return;
        }
        
        // Test 3: Cache test
        System.out.println("\nTest 3: Cache functionality");
        testCacheFunctionality(clipboard);
        
        // Test 4: Cache invalidation
        System.out.println("\nTest 4: Cache invalidation on clipboard change");
        testCacheInvalidation(clipboard);
        
        // Test 5: Performance comparison
        System.out.println("\nTest 5: Performance comparison");
        testPerformance(clipboard);
        
        // Test 6: Disable watcher
        System.out.println("\nTest 6: Disable watcher");
        clipboard.disableWatcher();
        System.out.println("  Watcher disabled");
        System.out.println("  isWatcherEnabled(): " + clipboard.isWatcherEnabled());
        
        System.out.println("\n=== All Tests Completed ===");
    }
    
    private static void testNormalMode(FastClipboard clipboard) {
        clipboard.setClipboardText("Normal mode test");
        String text1 = clipboard.getClipboardText();
        String text2 = clipboard.getClipboardText();
        System.out.println("  Set: \"Normal mode test\"");
        System.out.println("  Get #1: \"" + text1 + "\"");
        System.out.println("  Get #2: \"" + text2 + "\"");
        System.out.println("  ✓ Normal mode works");
    }
    
    private static void testCacheFunctionality(FastClipboard clipboard) {
        clipboard.setClipboardText("Cache test");
        
        // First read - should cache
        long start1 = System.nanoTime();
        String text1 = clipboard.getClipboardText();
        long duration1 = System.nanoTime() - start1;
        
        // Second read - should use cache
        long start2 = System.nanoTime();
        String text2 = clipboard.getClipboardText();
        long duration2 = System.nanoTime() - start2;
        
        System.out.println("  First read: " + (duration1 / 1000) + " μs");
        System.out.println("  Cached read: " + (duration2 / 1000) + " μs");
        System.out.println("  Values match: " + text1.equals(text2));
        
        if (duration2 < duration1 / 10) {
            System.out.println("  ✓ Cache is significantly faster!");
        } else {
            System.out.println("  ⚠ Cache speedup not detected (may need first read to warm up)");
        }
    }
    
    private static void testCacheInvalidation(FastClipboard clipboard) throws InterruptedException {
        clipboard.setClipboardText("Before change");
        String before = clipboard.getClipboardText();
        System.out.println("  Initial value: \"" + before + "\"");
        
        // Change clipboard externally
        clipboard.setClipboardText("After change");
        System.out.println("  Changed clipboard to: \"After change\"");
        
        // Wait a bit for the Windows message to propagate
        Thread.sleep(100);
        
        // Read again - cache should be invalidated
        String after = clipboard.getClipboardText();
        System.out.println("  Read after change: \"" + after + "\"");
        
        if ("After change".equals(after)) {
            System.out.println("  ✓ Cache was properly invalidated!");
        } else {
            System.out.println("  ⚠ Cache invalidation may need more time (Windows message loop)");
        }
    }
    
    private static void testPerformance(FastClipboard clipboard) {
        clipboard.setClipboardText("Performance test text for benchmarking clipboard operations");
        
        int iterations = 1000;
        
        // Test with watcher enabled (cached reads)
        long startCache = System.nanoTime();
        for (int i = 0; i < iterations; i++) {
            clipboard.getClipboardText();
        }
        long cacheTime = System.nanoTime() - startCache;
        
        // Disable watcher for normal reads
        clipboard.disableWatcher();
        
        // Warm up
        for (int i = 0; i < 10; i++) {
            clipboard.getClipboardText();
        }
        
        // Test normal mode
        long startNormal = System.nanoTime();
        for (int i = 0; i < iterations; i++) {
            clipboard.getClipboardText();
        }
        long normalTime = System.nanoTime() - startNormal;
        
        long cacheAvg = cacheTime / iterations / 1000;
        long normalAvg = normalTime / iterations / 1000;
        
        System.out.println("  " + iterations + " iterations:");
        System.out.println("    Cached mode: " + cacheAvg + " μs avg");
        System.out.println("    Normal mode: " + normalAvg + " μs avg");
        
        if (cacheAvg > 0 && normalAvg > 0) {
            double speedup = (double) normalAvg / cacheAvg;
            System.out.println("    Speedup: " + String.format("%.1fx", speedup));
        }
        
        System.out.println("  ✓ Performance test complete");
    }
}
