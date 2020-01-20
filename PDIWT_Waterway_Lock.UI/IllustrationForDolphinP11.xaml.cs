using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using BM = Bentley.MstnPlatformNET;
using BMWPF = Bentley.MstnPlatformNET.WPF;

namespace PDIWT.Waterway.Lock.UI
{
    /// <summary>
    /// Interaction logic for IllustrationForDolphinP11.xaml
    /// </summary>
    public partial class IllustrationForDolphinP11 : Window, IDisposable
    {
        public IllustrationForDolphinP11(BM.AddIn addIn)
        {
            InitializeComponent();

            m_wpfhelper = new BMWPF.WPFInteropHelper(this);
            m_wpfhelper.Attach(addIn, true, "SettingsWindow");
            Icon = new BitmapImage(new Uri("pack://application:,,,/PDIWT_Waterway_Lock.UI;component/Images/DolphinP11.ico", UriKind.RelativeOrAbsolute));
        }

        ~IllustrationForDolphinP11()
        {
            Dispose(false);
        }

        static IllustrationForDolphinP11 _illustrationForDolphinP11;
        BMWPF.WPFInteropHelper m_wpfhelper;

        public static void ShowWindow(BM.AddIn addIn)
        {
            if (_illustrationForDolphinP11 != null)
            {
                _illustrationForDolphinP11.Focus();
                return;
            };

            _illustrationForDolphinP11 = new IllustrationForDolphinP11(addIn);
            _illustrationForDolphinP11.Show();
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
            _illustrationForDolphinP11 = null;
        }
    }
}
