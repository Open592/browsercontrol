package ExampleAppletViewer;

import java.awt.BorderLayout;
import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.Insets;
import java.awt.Panel;

import nativeadvert.browsercontrol;

public final class AdvertFrame {
    private Frame frame;
    private Panel innerContainer;
    private Panel fakeApplet;
    private Canvas advertContainer;

    private static final int FRAME_WIDTH = 1024;
    private static final int FRAME_HEIGHT = 768;
    private static final int ADVERT_HEIGHT = 96;

    public AdvertFrame() {
        this.frame = new Frame();
        this.frame.setLayout(new BorderLayout());

        Insets frameInsets = frame.getInsets();
        frame.setTitle("Example Applet Viewer");
        frame.setSize(
            frameInsets.right + FRAME_WIDTH + frameInsets.left,
            frameInsets.bottom + FRAME_HEIGHT + (ADVERT_HEIGHT + frameInsets.top)
        );
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);

        this.innerContainer = new Panel();
        this.innerContainer.setBackground(Color.black);
        this.innerContainer.setLayout(null);

        this.fakeApplet = new Panel();
        this.fakeApplet.setBackground(Color.green);
        this.fakeApplet.setLayout(null);

        this.frame.add(this.innerContainer, "Center");

        this.advertContainer = new Canvas();
        this.innerContainer.add(this.advertContainer);
        this.innerContainer.add(fakeApplet);

        this.frame.doLayout();
        this.setBounds();
    }

    private void setBounds() {
        Dimension containerSize = this.innerContainer.getSize();
        Insets containerInsets = this.innerContainer.getInsets();
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

        this.fakeApplet.setBounds(
            (containerWidth - appletWidth) / 2,
            ADVERT_HEIGHT,
            appletWidth,
            appletHeight
        );

        if (this.advertContainer != null && browsercontrol.isCreated()) {
            Dimension advertSize = this.advertContainer.getSize();

            browsercontrol.resize(advertSize.width, advertSize.height);
        }

        this.frame.repaint();
    }


}
