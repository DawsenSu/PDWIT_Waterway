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
using GalaSoft.MvvmLight.Messaging;

namespace PDIWT.Waterway.Lock.UI
{
    using ViewModel;
    using Model;
    /// <summary>
    /// Interaction logic for CreateElementByProxyObjsWindow.xaml
    /// </summary>
    public partial class CreateElementByProxyObjsWindow : Window
    {
        
		public CreateElementByProxyObjsWindow(BM.AddIn addIn, ICreateElementByProxyObjs nativeManagedInterface)
        {
            InitializeComponent();
            DataContext = new CreateElementByProxyObjsViewModel(nativeManagedInterface);
            m_wpfhelper = new BMWPF.WPFInteropHelper(this);
            m_wpfhelper.Attach(addIn, true, "CreateElementByProxyObjsWindow");
            Icon = new BitmapImage(new Uri("pack://application:,,,/PDIWT_Waterway_Lock.UI;component/Images/CreateByList.ico"));
            //Resources.MergedDictionaries.Add(new ResourceDictionary() { Source = new Uri("pack://application:,,,/MaterialDesignThemes.Wpf;component/Themes/MaterialDesignTheme.Defaults.xaml")});

        }

        readonly BM.MessageCenter _mc = BM.MessageCenter.Instance;
        static CreateElementByProxyObjsWindow m_CreateElementByProxyObjsWindow;
        BMWPF.WPFInteropHelper m_wpfhelper;

        public static void ShowWindow(BM.AddIn addIn, ICreateElementByProxyObjs nativeManagedInterface)
        {
            if (m_CreateElementByProxyObjsWindow != null)
            {
                m_CreateElementByProxyObjsWindow.Focus();
                return;
            };

            m_CreateElementByProxyObjsWindow = new CreateElementByProxyObjsWindow(addIn, nativeManagedInterface);
            m_CreateElementByProxyObjsWindow.Show();
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
            m_CreateElementByProxyObjsWindow = null;
        }
    }
}
