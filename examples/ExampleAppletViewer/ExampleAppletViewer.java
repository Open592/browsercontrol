package ExampleAppletViewer;

import java.io.File;
import java.io.IOException;
import java.lang.ProcessHandle;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.Files;
import java.util.Scanner;

import nativeadvert.browsercontrol;

public class ExampleAppletViewer {
    private static void waitForInput() {
        Scanner input = new Scanner(System.in);
        long pid = ProcessHandle.current().pid();

        System.out.println("PID: " + pid);
        System.out.println("Press any key after attaching debugger...");

        input.nextLine();
    }

    public static void main(String[] args) {
        // Allow for the attachment of a debugger to process
        waitForInput();

        System.loadLibrary("browsercontrol");

        AdvertFrame.initialize();
    }
}
