package com.open592.appletviewer;

public class AppletViewer {
    public static void main(String[] args) {
        long pid = ProcessHandle.current().pid();

        System.out.println("Starting AppletViewer... PID: " + pid);

        System.loadLibrary("browsercontrol");

        AdvertFrame.start();
    }
}
