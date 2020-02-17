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
    /// Interaction logic for MountPierP1View.xaml
    /// </summary>
    public partial class MountPierP1View : UserControl
    {
        public MountPierP1View(BM.AddIn addIn)
        {
            
            InitializeComponent();
            DataContext = new ViewModel.MountPierP1ViewModel(addIn);
        }

        public ViewModel.MountPierP1Data GetData()
        {
            return ((ViewModel.MountPierP1ViewModel)DataContext).SelectedData;
        }
    }
}
