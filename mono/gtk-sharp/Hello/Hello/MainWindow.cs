using System;
using Gtk;

public partial class MainWindow: Gtk.Window
{
	public MainWindow () : base (Gtk.WindowType.Toplevel)
	{
		Build ();
	}

	protected void OnDeleteEvent (object sender, DeleteEventArgs a)
	{
		Application.Quit ();
		a.RetVal = true;
	}

	protected void OnButton1Clicked (object sender, EventArgs e)
	{
		var msg_dlg = new MessageDialog (this, DialogFlags.Modal, MessageType.Info,
			              ButtonsType.Ok, "Hello, World!!", new object[]{ });
		msg_dlg.Run ();
		msg_dlg.Destroy ();
	}
}
