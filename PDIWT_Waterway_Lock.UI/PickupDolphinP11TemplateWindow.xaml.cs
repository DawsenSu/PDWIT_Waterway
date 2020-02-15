using GalaSoft.MvvmLight.Messaging;
using System.Windows;

namespace PDIWT.Waterway.Lock.UI
{
    /// <summary>
    /// Interaction logic for PickupDolphinP11TemplateWindow.xaml
    /// </summary>
    public partial class PickupDolphinP11TemplateWindow : Window
    //, IDisposable
    {
        public PickupDolphinP11TemplateWindow()
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