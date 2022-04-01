package ExampleAppletViewer;

import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

class AdvertComponentListener implements ComponentListener {
    static private AdvertComponentListener m_instance;

    static AdvertComponentListener initialize() {
        if (m_instance == null) {
            m_instance = new AdvertComponentListener();
        }

        return m_instance;
    }

    private AdvertComponentListener() {
    }

    public void componentMoved(ComponentEvent event) {
        System.out.println("Received: Component moved event");
    }

    public void componentHidden(ComponentEvent event) {
        System.out.println("Received: Component hidden event");
    }

    public void componentResized(ComponentEvent event) {
        System.out.println("Received: Component resized event");
        AdvertFrame.setBounds();
    }

    public void componentShown(ComponentEvent event) {
        System.out.println("Received: Component shown event");
    }
}
