package com.open592.appletviewer;

import nativeadvert.browsercontrol;

import java.awt.*;

public final class AdvertFrame {
    private static Frame frame;
    private static Panel innerContainer;
    private static Panel fakeApplet;
    private static Canvas advertContainer;

    private static final int FRAME_WIDTH = 1024;
    private static final int FRAME_HEIGHT = 768;
    private static final int ADVERT_HEIGHT = 96;

    public static void quit() {
        if (browsercontrol.isCreated()) {
            System.out.println("Attempting to destroy browsercontrol");

            browsercontrol.destroy();
            innerContainer.remove(fakeApplet);
        }

        System.exit(0);
    }

    public static void start() {
        frame = new Frame();
        frame.setLayout(new BorderLayout());

        Insets frameInsets = frame.getInsets();
        frame.setTitle("Example Applet Viewer");
        frame.setSize(FRAME_WIDTH, FRAME_HEIGHT + ADVERT_HEIGHT);
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);

        innerContainer = new Panel();
        innerContainer.setBackground(Color.black);
        innerContainer.setLayout(null);

        fakeApplet = new Panel();
        fakeApplet.setBackground(Color.red);
        fakeApplet.setLayout(null);

        frame.add(innerContainer, "Center");

        advertContainer = new Canvas();
        innerContainer.add(advertContainer);
        innerContainer.add(fakeApplet);

        frame.doLayout();
        setBounds();

        while (!advertContainer.isDisplayable() || !advertContainer.isShowing()) {
            try {
                System.out.println("Hello world");
                Thread.sleep(100L);
            } catch (Exception ignored) {
            }
        }

        try {
            browsercontrol.create(advertContainer, "https://oldschool.runescape.wiki/");
            browsercontrol.resize(advertContainer.getSize().width, advertContainer.getSize().height);
        } catch (Throwable err) {
            err.printStackTrace();

            return;
        }

        fakeApplet.setBackground(Color.green);
        frame.addWindowListener(TerminateHandler.initialize());
        innerContainer.addComponentListener(AdvertComponentListener.initialize());
    }

    public static void setBounds() {
        Dimension containerSize = innerContainer.getSize();
        Insets containerInsets = innerContainer.getInsets();
        int containerWidth = containerSize.width - (containerInsets.right + containerInsets.left);
        int containerHeight = containerSize.height - (containerInsets.top + containerInsets.bottom);
        int appletWidth = containerWidth;
        int appletHeight = containerHeight - ADVERT_HEIGHT;

        advertContainer.setBounds(
                0,
                0,
                appletWidth,
                ADVERT_HEIGHT
        );

        fakeApplet.setBounds(
                0,
                ADVERT_HEIGHT,
                appletWidth,
                appletHeight
        );

        if (advertContainer != null && browsercontrol.isCreated()) {
            Dimension advertSize = advertContainer.getSize();

            browsercontrol.resize(advertSize.width, advertSize.height);
        }

        frame.repaint();
    }
}
