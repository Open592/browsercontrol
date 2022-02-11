package ExampleAppletViewer;

import java.io.File;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.Files;
import nativeadvert.browsercontrol;

public class ExampleAppletViewer {
    private static String findLibraryPath() {
        /*
         * On multi-configuration generators (For example Windows) we will
         * generate multiple library files. Loop through the possible
         * library-path-* files. When we successfully load the library return
         */
        Path[] libraryPathFileNames = new Path[] {
            Paths.get("library-path-Debug.txt"),
            Paths.get("library-path-Release.txt"),
            Paths.get("library-path-MinSizeRel.txt"),
            Paths.get("library-path-RelWithDebInfo.txt")
        };

        for (Path libraryPathFileName : libraryPathFileNames) {
            try {
                String libraryPath = new String(Files.readAllBytes(libraryPathFileName));

                System.out.println("Found library path: " + libraryPath);

                if (libraryPath != null && !libraryPath.isEmpty()) {
                    return libraryPath;
                }
            } catch (IOException ignored) {
                ignored.printStackTrace();
                continue;
            }
        }

        return "";
    }
    public static void main(String[] args) {
        String libraryPath = findLibraryPath();

        System.out.println("Found library path: " + libraryPath);

        System.load(libraryPath);

        browsercontrol.Test();
    }
}
