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
    /// Interaction logic for GuidewalP2View.xaml
    /// </summary>
    public partial class GuidewallP2View : UserControl
    {
        public GuidewallP2View(BM.AddIn addIn)
        {
            InitializeComponent();
            DataContext = new ViewModel.GuidewallP2ViewModel(addIn);
        }

        public ViewModel.GuidewallP2Data GetData()
        {
            return ((ViewModel.GuidewallP2ViewModel)DataContext).SelectedData;
        }
    }
}
