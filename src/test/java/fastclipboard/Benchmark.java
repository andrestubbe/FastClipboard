package fastclipboard;

import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.StringSelection;
import java.awt.datatransfer.DataFlavor;

/**
 * Benchmark comparing FastClipboard vs Java AWT Clipboard.
 * 
 * Tests:
 * - Set clipboard text (write)
 * - Get clipboard text (read)
 * - Combined read/write cycle
 * - Image operations
 * - File list operations
 */
public class Benchmark {
    
    private static final int WARMUP_ITERATIONS = 100;
    private static final int BENCHMARK_ITERATIONS = 1000;
    private static final String TEST_TEXT = "Hello, World! This is a benchmark test string for clipboard operations.";
    private static final int TEST_IMAGE_SIZE = 100; // 100x100 test image
    
    public static void main(String[] args) {
        System.out.println("=== FastClipboard vs Java AWT Clipboard Benchmark ===");
        System.out.println();
        System.out.println("Test iterations: " + BENCHMARK_ITERATIONS);
        System.out.println("Test text length: " + TEST_TEXT.length() + " characters");
        System.out.println();
        
        // Warmup
        System.out.println("Warming up...");
        warmupFastClipboard();
        warmupJavaClipboard();
        warmupFastClipboardImage();
        warmupFastClipboardFiles();
        System.out.println();
        
        // Benchmark FastClipboard
        System.out.println("=== FastClipboard Performance ===");
        long fastSetTime = benchmarkFastClipboardSet();
        long fastGetTime = benchmarkFastClipboardGet();
        long fastCycleTime = benchmarkFastClipboardCycle();
        long fastSetImageTime = benchmarkFastClipboardSetImage();
        long fastGetImageTime = benchmarkFastClipboardGetImage();
        long fastSetFilesTime = benchmarkFastClipboardSetFiles();
        long fastGetFilesTime = benchmarkFastClipboardGetFiles();
        System.out.println();
        
        // Benchmark Java AWT Clipboard
        System.out.println("=== Java AWT Clipboard Performance ===");
        long javaSetTime = benchmarkJavaClipboardSet();
        long javaGetTime = benchmarkJavaClipboardGet();
        long javaCycleTime = benchmarkJavaClipboardCycle();
        System.out.println();
        
        // Results
        System.out.println("=== Results ===");
        System.out.println("Text Operations:");
        System.out.printf("  Set Text:  FastClipboard=%dμs, Java=%dμs, Speedup=%.2fx%n", 
            fastSetTime, javaSetTime, (double) javaSetTime / fastSetTime);
        System.out.printf("  Get Text:  FastClipboard=%dμs, Java=%dμs, Speedup=%.2fx%n", 
            fastGetTime, javaGetTime, (double) javaGetTime / fastGetTime);
        System.out.printf("  Full Cycle: FastClipboard=%dμs, Java=%dμs, Speedup=%.2fx%n", 
            fastCycleTime, javaCycleTime, (double) javaCycleTime / fastCycleTime);
        System.out.println("Image Operations (FastClipboard only):");
        System.out.printf("  Set Image: FastClipboard=%dμs%n", fastSetImageTime);
        System.out.printf("  Get Image: FastClipboard=%dμs%n", fastGetImageTime);
        System.out.println("File Operations (FastClipboard only):");
        System.out.printf("  Set Files: FastClipboard=%dμs%n", fastSetFilesTime);
        System.out.printf("  Get Files: FastClipboard=%dμs%n", fastGetFilesTime);
    }
    
    private static void warmupFastClipboard() {
        FastClipboard clipboard = new FastClipboard();
        for (int i = 0; i < WARMUP_ITERATIONS; i++) {
            clipboard.setClipboardText(TEST_TEXT);
            clipboard.getClipboardText();
        }
    }
    
    private static void warmupJavaClipboard() {
        Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
        for (int i = 0; i < WARMUP_ITERATIONS; i++) {
            try {
                clipboard.setContents(new StringSelection(TEST_TEXT), null);
                clipboard.getData(DataFlavor.stringFlavor);
            } catch (Exception e) {
                // Ignore
            }
        }
    }
    
    private static void warmupFastClipboardImage() {
        FastClipboard clipboard = new FastClipboard();
        int[] pixels = new int[TEST_IMAGE_SIZE * TEST_IMAGE_SIZE];
        for (int i = 0; i < pixels.length; i++) {
            pixels[i] = 0xFF0000FF; // Blue pixels
        }
        for (int i = 0; i < WARMUP_ITERATIONS; i++) {
            clipboard.setClipboardImage(TEST_IMAGE_SIZE, TEST_IMAGE_SIZE, pixels);
            clipboard.getClipboardImage();
        }
    }
    
    private static void warmupFastClipboardFiles() {
        FastClipboard clipboard = new FastClipboard();
        String[] files = {"C:\\test1.txt", "C:\\test2.txt"};
        for (int i = 0; i < WARMUP_ITERATIONS; i++) {
            clipboard.setClipboardFiles(files);
            clipboard.getClipboardFiles();
        }
    }
    
    private static long benchmarkFastClipboardSet() {
        FastClipboard clipboard = new FastClipboard();
        long startTime = System.nanoTime();
        
        for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
            clipboard.setClipboardText(TEST_TEXT);
        }
        
        long endTime = System.nanoTime();
        return (endTime - startTime) / 1000; // Convert to microseconds
    }
    
    private static long benchmarkFastClipboardGet() {
        FastClipboard clipboard = new FastClipboard();
        clipboard.setClipboardText(TEST_TEXT);
        
        long startTime = System.nanoTime();
        
        for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
            clipboard.getClipboardText();
        }
        
        long endTime = System.nanoTime();
        return (endTime - startTime) / 1000; // Convert to microseconds
    }
    
    private static long benchmarkFastClipboardCycle() {
        FastClipboard clipboard = new FastClipboard();
        long startTime = System.nanoTime();
        
        for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
            clipboard.setClipboardText(TEST_TEXT);
            clipboard.getClipboardText();
        }
        
        long endTime = System.nanoTime();
        return (endTime - startTime) / 1000; // Convert to microseconds
    }
    
    private static long benchmarkJavaClipboardSet() {
        Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
        long startTime = System.nanoTime();
        
        for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
            try {
                clipboard.setContents(new StringSelection(TEST_TEXT), null);
            } catch (Exception e) {
                // Ignore
            }
        }
        
        long endTime = System.nanoTime();
        return (endTime - startTime) / 1000; // Convert to microseconds
    }
    
    private static long benchmarkJavaClipboardGet() {
        Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
        clipboard.setContents(new StringSelection(TEST_TEXT), null);
        
        long startTime = System.nanoTime();
        
        for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
            try {
                clipboard.getData(DataFlavor.stringFlavor);
            } catch (Exception e) {
                // Ignore
            }
        }
        
        long endTime = System.nanoTime();
        return (endTime - startTime) / 1000; // Convert to microseconds
    }
    
    private static long benchmarkJavaClipboardCycle() {
        Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
        long startTime = System.nanoTime();
        
        for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
            try {
                clipboard.setContents(new StringSelection(TEST_TEXT), null);
                clipboard.getData(DataFlavor.stringFlavor);
            } catch (Exception e) {
                // Ignore
            }
        }
        
        long endTime = System.nanoTime();
        return (endTime - startTime) / 1000; // Convert to microseconds
    }
    
    private static long benchmarkFastClipboardSetImage() {
        FastClipboard clipboard = new FastClipboard();
        int[] pixels = new int[TEST_IMAGE_SIZE * TEST_IMAGE_SIZE];
        for (int i = 0; i < pixels.length; i++) {
            pixels[i] = 0xFF0000FF; // Blue pixels
        }
        
        long startTime = System.nanoTime();
        
        for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
            clipboard.setClipboardImage(TEST_IMAGE_SIZE, TEST_IMAGE_SIZE, pixels);
        }
        
        long endTime = System.nanoTime();
        return (endTime - startTime) / 1000; // Convert to microseconds
    }
    
    private static long benchmarkFastClipboardGetImage() {
        FastClipboard clipboard = new FastClipboard();
        int[] pixels = new int[TEST_IMAGE_SIZE * TEST_IMAGE_SIZE];
        for (int i = 0; i < pixels.length; i++) {
            pixels[i] = 0xFF0000FF; // Blue pixels
        }
        clipboard.setClipboardImage(TEST_IMAGE_SIZE, TEST_IMAGE_SIZE, pixels);
        
        long startTime = System.nanoTime();
        
        for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
            clipboard.getClipboardImage();
        }
        
        long endTime = System.nanoTime();
        return (endTime - startTime) / 1000; // Convert to microseconds
    }
    
    private static long benchmarkFastClipboardSetFiles() {
        FastClipboard clipboard = new FastClipboard();
        String[] files = {"C:\\test1.txt", "C:\\test2.txt"};
        
        long startTime = System.nanoTime();
        
        for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
            clipboard.setClipboardFiles(files);
        }
        
        long endTime = System.nanoTime();
        return (endTime - startTime) / 1000; // Convert to microseconds
    }
    
    private static long benchmarkFastClipboardGetFiles() {
        FastClipboard clipboard = new FastClipboard();
        String[] files = {"C:\\test1.txt", "C:\\test2.txt"};
        clipboard.setClipboardFiles(files);
        
        long startTime = System.nanoTime();
        
        for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
            clipboard.getClipboardFiles();
        }
        
        long endTime = System.nanoTime();
        return (endTime - startTime) / 1000; // Convert to microseconds
    }
}
