package ExampleAppletViewer;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

final class TerminateHandler extends WindowAdapter implements ActionListener {
   private static TerminateHandler m_instance;

   private TerminateHandler() {
   }

   static TerminateHandler initialize() {
      if (m_instance == null) {
         m_instance = new TerminateHandler();
      }

      return m_instance;
   }

   public final void windowClosing(WindowEvent event) {
      AdvertFrame.quit();
   }

   public final void actionPerformed(ActionEvent event) {
      AdvertFrame.quit();
   }
}
