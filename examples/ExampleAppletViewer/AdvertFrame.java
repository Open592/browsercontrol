package ExampleAppletViewer;

import java.awt.BorderLayout;
import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.Frame;
import java.awt.Insets;
import java.awt.Panel;

import nativeadvert.browsercontrol;

public final class AdvertFrame implements ComponentListener {
    private static Frame frame;
    private static Panel innerContainer;
    private static Panel fakeApplet;
    private static Canvas advertContainer;

    private static final int FRAME_WIDTH = 1024;
    private static final int FRAME_HEIGHT = 768;
    private static final int ADVERT_HEIGHT = 96;

    public static void initialize() {
        frame = new Frame();
        frame.setLayout(new BorderLayout());

        Insets frameInsets = frame.getInsets();
        frame.setTitle("Example Applet Viewer");
        frame.setSize(
            frameInsets.right + FRAME_WIDTH + frameInsets.left,
            frameInsets.bottom + FRAME_HEIGHT + (ADVERT_HEIGHT + frameInsets.top)
        );
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);

        innerContainer = new Panel();
        innerContainer.setBackground(Color.black);
        innerContainer.setLayout(null);

        fakeApplet = new Panel();
        fakeApplet.setBackground(Color.green);
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
            browsercontrol.create(advertContainer, "https://example.com");
        } catch (Throwable err) {
            err.printStackTrace();

            return;
        }

        innerContainer.addComponentListener(new AdvertFrame());
    }

    private static void setBounds() {
        Dimension containerSize = innerContainer.getSize();
        Insets containerInsets = innerContainer.getInsets();
        int containerWidth = containerSize.width - (containerInsets.right + containerInsets.left);
        int containerHeight = containerSize.height - (containerInsets.top + containerInsets.bottom);
        int appletWidth = containerWidth;
        int appletHeight = containerHeight - ADVERT_HEIGHT;

        advertContainer.setBounds(
            (containerWidth - appletWidth) / 2,
            0,
            appletWidth,
            ADVERT_HEIGHT
        );

        fakeApplet.setBounds(
            (containerWidth - appletWidth) / 2,
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

    public void componentMoved(ComponentEvent event) {
    }

    public void componentHidden(ComponentEvent event) {
    }

    public void componentResized(ComponentEvent event) {
        setBounds();
    }

    public void componentShown(ComponentEvent event) {
    }
}
