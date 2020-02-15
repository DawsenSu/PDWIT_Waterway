using System;
using System.Windows;
using System.Windows.Media.Imaging;
using BM = Bentley.MstnPlatformNET;
using BMWPF = Bentley.MstnPlatformNET.WPF;

namespace PDIWT.Waterway.Lock.UI
{
    /// <summary>
    /// Interaction logic for IllustrationWindow.xaml
    /// </summary>
    public partial class IllustrationWindow : Window, IDisposable
    {
        public IllustrationWindow(BM.AddIn addIn)
        {
            InitializeComponent();

            m_wpfhelper = new BMWPF.WPFInteropHelper(this);
            m_wpfhelper.Attach(addIn, true, "SettingsWindow");
            Icon = new BitmapImage(new Uri("pack://application:,,,/PDIWT_Waterway_Lock.UI;component/Images/GuidewallP2.ico", UriKind.RelativeOrAbsolute));
        }

        ~IllustrationWindow()
        {
            Dispose(false);
        }

        private static IllustrationWindow _illustrationWindow;
        private BMWPF.WPFInteropHelper m_wpfhelper;

        public static void ShowWindow(BM.AddIn addIn)
        {
            if (_illustrationWindow != null)
            {
                _illustrationWindow.Focus();
                return;
            };

            _illustrationWindow = new IllustrationWindow(addIn);
            _illustrationWindow.Show();
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);
            Dispose();
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(true);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                //m_SettingsWindowhost.Dispose();
            }
            m_wpfhelper.Detach();
            m_wpfhelper.Dispose();
            _illustrationWindow = null;
        }
    }
}