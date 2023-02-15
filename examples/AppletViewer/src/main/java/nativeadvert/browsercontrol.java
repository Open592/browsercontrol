package nativeadvert;

import java.awt.*;

public class browsercontrol {
    private static boolean iscreated = false;
    private static boolean error = false;

    private static native void resize0(int var0, int var1);

    public static boolean isCreated() {
        return iscreated;
    }

    private static native void navigate0(String var0);

    private static native boolean browsercontrol0(Canvas var0, String var1);

    public static void destroy() {
        if (iscreated) {
            destroy0();
            iscreated = false;
        } else {
            throw new IllegalStateException();
        }
    }

    public static void resize(int var0, int var1) {
        if (!iscreated) {
            throw new IllegalStateException();
        } else {
            resize0(var0, var1);
        }
    }

    public static void navigate(String var0) {
        if (iscreated) {
            navigate0(var0);
        } else {
            throw new IllegalStateException();
        }
    }

    private browsercontrol() {
    }

    public static boolean create(Canvas var0, String var1) {
        if (!iscreated) {
            if (error) {
                return false;
            } else {
                boolean var2 = browsercontrol0(var0, var1);
                if (!var2) {
                    error = true;
                } else {
                    iscreated = true;
                }

                return var2;
            }
        } else {
            throw new IllegalStateException();
        }
    }

    private static native void destroy0();
}
