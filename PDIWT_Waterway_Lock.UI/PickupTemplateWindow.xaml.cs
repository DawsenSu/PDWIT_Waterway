using GalaSoft.MvvmLight.Messaging;
using System.Windows;

namespace PDIWT.Waterway.Lock.UI
{
    /// <summary>
    /// Interaction logic for PickupTemplateWindow.xaml
    /// </summary>
    public partial class PickupTemplateWindow : Window
    //, IDisposable
    {
        public PickupTemplateWindow()
        {
            InitializeComponent();

            Messenger.Default.Register<NotificationMessage>(this,
                notification =>
                {
                    if (notification.Notification == "CloseWindow")
                    {
                        Close();
                    }
                });
        }
    }
}