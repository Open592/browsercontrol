package ExampleAppletViewer;

import java.io.File;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.Files;
import nativeadvert.browsercontrol;

public class ExampleAppletViewer {
    public static void main(String[] args) {
        System.load("./browsercontrol.dll");

        browsercontrol.Test();
    }
}
