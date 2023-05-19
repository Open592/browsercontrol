package nativeadvert;

import java.awt.*;

public final class browsercontrol {
    private static boolean isCreated = false;
    private static boolean error = false;

    public static boolean isCreated() {
        return isCreated;
    }

    public static void destroy() {
        if (!isCreated()) {
            throw new IllegalStateException();
        }

        destroy0();
        isCreated = false;
    }

    public static void resize(int width, int height) {
        if (!isCreated()) {
            throw new IllegalStateException();
        }

        resize0(width, height);
    }

    public static void navigate(String URL) {
        if (!isCreated()) {
            throw new IllegalStateException();
        }

        navigate0(URL);
    }

    private browsercontrol() {}

    public static boolean create(Canvas advertContainer, String URL) {
        if (isCreated()) {
            throw new IllegalStateException();
        }

        if (error) {
            return false;
        }

        boolean result = browsercontrol0(advertContainer, URL);

        if (!result) {
            error = true;
        } else {
            isCreated = true;
        }

        return result;
    }

    private static native boolean browsercontrol0(Canvas var0, String var1);

    private static native void navigate0(String var0);

    private static native void resize0(int var0, int var1);

    private static native void destroy0();
}
