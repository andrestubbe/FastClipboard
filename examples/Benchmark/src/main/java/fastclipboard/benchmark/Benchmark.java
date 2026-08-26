package fastclipboard.benchmark;

import fastclipboard.FastClipboard;
import org.openjdk.jmh.annotations.*;

import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.StringSelection;
import java.util.concurrent.TimeUnit;

@BenchmarkMode(Mode.Throughput)
@OutputTimeUnit(TimeUnit.MILLISECONDS)
@State(Scope.Benchmark)
@Warmup(iterations = 2, time = 1, timeUnit = TimeUnit.SECONDS)
@Measurement(iterations = 3, time = 1, timeUnit = TimeUnit.SECONDS)
@Fork(1)
public class Benchmark {

    private FastClipboard fastClipboard;
    private Clipboard awtClipboard;
    private static final String PAYLOAD = "FastClipboard JMH Ultra-Fast Raw Win32 Test String";

    @Setup
    public void setup() {
        fastClipboard = new FastClipboard();
        awtClipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
    }

    @org.openjdk.jmh.annotations.Benchmark
    public boolean benchmarkFastClipboardSet() {
        return fastClipboard.setClipboardText(PAYLOAD);
    }

    @org.openjdk.jmh.annotations.Benchmark
    public String benchmarkFastClipboardGet() {
        return fastClipboard.getClipboardText();
    }

    @org.openjdk.jmh.annotations.Benchmark
    public void benchmarkAwtClipboardSet() {
        try {
            awtClipboard.setContents(new StringSelection(PAYLOAD), null);
        } catch (Exception ignored) {}
    }

    @org.openjdk.jmh.annotations.Benchmark
    public Object benchmarkAwtClipboardGet() throws Exception {
        if (awtClipboard.isDataFlavorAvailable(DataFlavor.stringFlavor)) {
            return awtClipboard.getData(DataFlavor.stringFlavor);
        }
        return null;
    }
}
