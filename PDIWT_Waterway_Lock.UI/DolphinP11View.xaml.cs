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
using System.Windows.Navigation;
using System.Windows.Shapes;
using BM = Bentley.MstnPlatformNET;

namespace PDIWT.Waterway.Lock.UI
{
    /// <summary>
    /// Interaction logic for DolphinP11View.xaml
    /// </summary>
    public partial class DolphinP11View : UserControl
    {
        public DolphinP11View(BM.AddIn addIn)
        {
            InitializeComponent();
            DataContext = new ViewModel.DolphinP11ViewModel(addIn);
        }

        public ViewModel.DolphinP11Data GetData()
        {
            return ((ViewModel.DolphinP11ViewModel)DataContext).SelectedData;
        }
    }
}
