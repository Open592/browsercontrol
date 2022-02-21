package nativeadvert;

import java.awt.Canvas;

public class browsercontrol {
    private static boolean isCreated = false;
    private static boolean error = false;

    private static native void resize0(int width, int height);
    private static native void navigate0(String destination);
    private static native boolean browsercontrol0(Canvas parentWindow, String initialDestination);
    private static native void destroy0();

    public static void Test() {
        System.out.println("Test");
    }

    public static boolean isCreated() {
        return isCreated;
    }

    public static boolean create(Canvas parentWindow, String initialDestination) throws IllegalStateException {
        if (isCreated) {
            throw new IllegalStateException();
        }

        if (error) {
            return false;
        }

        boolean isSuccessfullyInitialized = browsercontrol0(parentWindow, initialDestination);

        if (isSuccessfullyInitialized) {
            isCreated = true;
        } else {
            error = true;
        }

        return isSuccessfullyInitialized;
    }

    public static void destroy() throws IllegalStateException {
        if (!isCreated) {
            throw new IllegalStateException();
        }

        destroy0();

        isCreated = false;
    }

    public static void navigate(String destination) throws IllegalStateException {
        if (!isCreated) {
            throw new IllegalStateException();
        }

        navigate0(destination);
    }

    public static void resize(int width, int height) throws IllegalStateException {
        if (!isCreated) {
            throw new IllegalStateException();
        }

        resize0(width, height);
    }

    private browsercontrol() {
    }
}
